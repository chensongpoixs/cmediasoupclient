#include "Broadcaster.hpp"
#include "MediaStreamTrackFactory.hpp"
#include "mediasoupclient.hpp"
#include "json.hpp"
#include <chrono>
//#include <cpr/cpr.h>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include "httplib.h"
#include "ccfg.h"
#include "peerConnectionUtils.hpp"
using json = nlohmann::json;
Broadcaster broadcaster;

enum EACTION_MOUSE_TYPE
{
	EACTION_MOUSE_MOVE = 0,
	EACTION_MOUSE_DOWNUP,
	EACTION_MOUSE_DOWN,
	EACTION_MOUSE_UP,
	EACTION_MOUSE_BIG,
	EACTION_MOUSE_SMALL
};


Broadcaster::~Broadcaster()
{
	this->Stop();
}

void Broadcaster::OnTransportClose(mediasoupclient::Producer* /*producer*/)
{
	std::cout << "[INFO] Broadcaster::OnTransportClose()" << std::endl;
}

void Broadcaster::OnTransportClose(mediasoupclient::DataProducer* /*dataProducer*/)
{
	std::cout << "[INFO] Broadcaster::OnTransportClose()" << std::endl;
}

/* Transport::Listener::OnConnect
 *
 * Fired for the first Transport::Consume() or Transport::Produce().
 * Update the already created remote transport with the local DTLS parameters.
 */
std::future<void> Broadcaster::OnConnect(mediasoupclient::Transport* transport, const json& dtlsParameters)
{
	std::cout << "[INFO] Broadcaster::OnConnect()" << std::endl;
	// std::cout << "[INFO] dtlsParameters: " << dtlsParameters.dump(4) << std::endl;

	if (transport->GetId() == this->sendTransport->GetId())
	{
		return this->OnConnectSendTransport(dtlsParameters);
	}
	else if (transport->GetId() == this->recvTransport->GetId())
	{
		return this->OnConnectRecvTransport(dtlsParameters);
	}
	else
	{
		std::promise<void> promise;

		promise.set_exception(std::make_exception_ptr("Unknown transport requested to connect"));

		return promise.get_future();
	}
}

std::future<void> Broadcaster::OnConnectSendTransport(const json& dtlsParameters)
{
	std::promise<void> promise;

	/* clang-format off */
	json body =
	{
		{ "dtlsParameters", dtlsParameters }
	};
	/* clang-format on */

	/*auto r = cpr::PostAsync(
	           cpr::Url{ this->baseUrl + "/broadcasters/" + this->id + "/transports/" +
	                     this->sendTransport->GetId() + "/connect" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } },
	           cpr::VerifySsl{ verifySsl })
	           .get();

	if (r.status_code == 200)
	{
		promise.set_value();
	}
	else
	{
		std::cerr << "[ERROR] unable to connect transport"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		promise.set_exception(std::make_exception_ptr(r.text));
	}*/


	std::string host =chen::g_cfg.get_string(chen::ECI_MediaSoup_Host) ;
	httplib::Client cli(host, chen::g_cfg.get_uint32(chen::ECI_MediaSoup_Http_Port));
	std::string url = baseUrl + "/broadcasters/" + this->id + "/transports/" +
		this->sendTransport->GetId() + "/connect";
	auto res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (!res)
	{
		RTC_LOG(LS_ERROR)  <<"[error] !!!!\n";
		return promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] unable to connect transport"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		promise.set_exception(std::make_exception_ptr(res->body));
		return promise.get_future();
	}

	RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;
	promise.set_value();
	return promise.get_future();
}

std::future<void> Broadcaster::OnConnectRecvTransport(const json& dtlsParameters)
{
	std::promise<void> promise;

	/* clang-format off */
	json body =
	{
		{ "dtlsParameters", dtlsParameters }
	};
	/* clang-format on */

	/*auto r = cpr::PostAsync(
	           cpr::Url{ this->baseUrl + "/broadcasters/" + this->id + "/transports/" +
	                     this->recvTransport->GetId() + "/connect" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } },
	           cpr::VerifySsl{ verifySsl })
	           .get();

	if (r.status_code == 200)
	{
		promise.set_value();
	}
	else
	{
		std::cerr << "[ERROR] unable to connect transport"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		promise.set_exception(std::make_exception_ptr(r.text));
	}
*/

	std::string host =chen::g_cfg.get_string(chen::ECI_MediaSoup_Host) ;
	httplib::Client cli(host, chen::g_cfg.get_uint32(chen::ECI_MediaSoup_Http_Port));
	std::string url = baseUrl +"/broadcasters/" + this->id + "/transports/" +
		this->recvTransport->GetId() + "/connect" ;
	auto res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (!res)
	{
		RTC_LOG(LS_ERROR)  <<"[error] !!!!\n";
		return promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] unable to connect transport"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		promise.set_exception(std::make_exception_ptr(res->body));
		return promise.get_future();
	}

	RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;

	promise.set_value();
	return promise.get_future();
}

/*
 * Transport::Listener::OnConnectionStateChange.
 */
void Broadcaster::OnConnectionStateChange(
  mediasoupclient::Transport* /*transport*/, const std::string& connectionState)
{
	std::cout << "[INFO] Broadcaster::OnConnectionStateChange() [connectionState:" << connectionState
	          << "]" << std::endl;

	if (connectionState == "failed")
	{
		Stop();
		std::exit(0);
	}
}

/* Producer::Listener::OnProduce
 *
 * Fired when a producer needs to be created in mediasoup.
 * Retrieve the remote producer ID and feed the caller with it.
 */
std::future<std::string> Broadcaster::OnProduce(
  mediasoupclient::SendTransport* /*transport*/,
  const std::string& kind,
  json rtpParameters,
  const json& /*appData*/)
{
	std::cout << "[INFO] Broadcaster::OnProduce()" << std::endl;
	// std::cout << "[INFO] rtpParameters: " << rtpParameters.dump(4) << std::endl;

	std::promise<std::string> promise;

	/* clang-format off */
	json body =
	{
		{ "kind",          kind          },
		{ "rtpParameters", rtpParameters }
	};
	/* clang-format on */

	/*auto r = cpr::PostAsync(
	           cpr::Url{ this->baseUrl + "/broadcasters/" + this->id + "/transports/" +
	                     this->sendTransport->GetId() + "/producers" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } },
	           cpr::VerifySsl{ verifySsl })
	           .get();

	if (r.status_code == 200)
	{
		auto response = json::parse(r.text);

		auto it = response.find("id");
		if (it == response.end() || !it->is_string())
		{
			promise.set_exception(std::make_exception_ptr("'id' missing in response"));
		}

		promise.set_value((*it).get<std::string>());
	}
	else
	{
		std::cerr << "[ERROR] unable to create producer"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		promise.set_exception(std::make_exception_ptr(r.text));
	}
*/


	std::string host =chen::g_cfg.get_string(chen::ECI_MediaSoup_Host) ;
	httplib::Client cli(host, chen::g_cfg.get_uint32(chen::ECI_MediaSoup_Http_Port));
	std::string url = baseUrl + "/broadcasters/" + this->id + "/transports/" +
		this->sendTransport->GetId() + "/producers"  ;
	auto res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (!res)
	{
		RTC_LOG(LS_ERROR)  <<"[error] !!!!\n";
		return promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] unable to connect transport"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		promise.set_exception(std::make_exception_ptr(res->body));
		return promise.get_future();
	}

	RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;


	auto response = json::parse(res->body);

	auto it = response.find("id");
	if (it == response.end() || !it->is_string())
	{
		promise.set_exception(std::make_exception_ptr("'id' missing in response"));
	}

	promise.set_value((*it).get<std::string>());

	return promise.get_future();
}

/* Producer::Listener::OnProduceData
 *
 * Fired when a data producer needs to be created in mediasoup.
 * Retrieve the remote producer ID and feed the caller with it.
 */
std::future<std::string> Broadcaster::OnProduceData(
  mediasoupclient::SendTransport* /*transport*/,
  const json& sctpStreamParameters,
  const std::string& label,
  const std::string& protocol,
  const json& /*appData*/)
{
	std::cout << "[INFO] Broadcaster::OnProduceData()" << std::endl;
	// std::cout << "[INFO] rtpParameters: " << rtpParameters.dump(4) << std::endl;

	std::promise<std::string> promise;

	/* clang-format off */
	json body =
    {
        { "label"                , label },
        { "protocol"             , protocol },
        { "sctpStreamParameters" , sctpStreamParameters }
		// { "appData"				 , "someAppData" }
	};
	/* clang-format on */

	/*auto r = cpr::PostAsync(
	           cpr::Url{ this->baseUrl + "/broadcasters/" + this->id + "/transports/" +
	                     this->sendTransport->GetId() + "/produce/data" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } },
	           cpr::VerifySsl{ verifySsl })
	           .get();

	if (r.status_code == 200)
	{
		auto response = json::parse(r.text);

		auto it = response.find("id");
		if (it == response.end() || !it->is_string())
		{
			promise.set_exception(std::make_exception_ptr("'id' missing in response"));
		}
		else
		{
			auto dataProducerId = (*it).get<std::string>();
			promise.set_value(dataProducerId);
		}
	}
	else
	{
		std::cerr << "[ERROR] unable to create data producer"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		promise.set_exception(std::make_exception_ptr(r.text));
	}*/
	std::string host =chen::g_cfg.get_string(chen::ECI_MediaSoup_Host) ;
	httplib::Client cli(host, chen::g_cfg.get_uint32(chen::ECI_MediaSoup_Http_Port));
	std::string url = baseUrl + "/broadcasters/" + this->id + "/transports/" +
		this->sendTransport->GetId() + "/produce/data" ;
	auto res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (!res)
	{
		RTC_LOG(LS_ERROR)  <<"[error] !!!!\n";
		//promise.set_exception(std::make_exception_ptr(res->body));
		return promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] unable to connect transport"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		//promise.set_exception(std::make_exception_ptr(res->body));
		return promise.get_future();
	}

	RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;
	auto response = json::parse(res->body);

	auto it = response.find("id");
	if (it == response.end() || !it->is_string())
	{
		promise.set_exception(std::make_exception_ptr("'id' missing in response"));
	}
	else
	{
		auto dataProducerId = (*it).get<std::string>();
		promise.set_value(dataProducerId);
	}
	return promise.get_future();
}

void Broadcaster::Start(
  const std::string& baseUrl,
  bool enableAudio,
  bool useSimulcast,
  const json& routerRtpCapabilities,
  bool verifySsl, std::string name)
{
	std::cout << "[INFO] Broadcaster::Start()" << std::endl;

	m_wight = GetSystemMetrics(SM_CXSCREEN);
	m_height = GetSystemMetrics(SM_CYSCREEN);

	this->baseUrl   = baseUrl;
	this->verifySsl = verifySsl;

	// Load the device.
	this->device.Load(routerRtpCapabilities);

	std::cout << "[INFO] creating Broadcaster..." << std::endl;

	/* clang-format off */
	json body =
	{
		{ "id",          this->id          },
		{ "displayName", name     },
		{ "device",
			{
				{ "name",    "chensong"       },
				{ "version", mediasoupclient::Version() }
			}
		},
		{ "rtpCapabilities", this->device.GetRtpCapabilities() }
	};
	/* clang-format on */

	/*auto r = cpr::PostAsync(
	           cpr::Url{ this->baseUrl + "/broadcasters" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } },
	           cpr::VerifySsl{ verifySsl })
	           .get();

	if (r.status_code != 200)
	{
		std::cerr << "[ERROR] unable to create Broadcaster"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		return;
	}*/
	std::string host =chen::g_cfg.get_string(chen::ECI_MediaSoup_Host) ;
	httplib::Client cli(host, chen::g_cfg.get_uint32(chen::ECI_MediaSoup_Http_Port));
	std::string url = baseUrl +  "/broadcasters"  ;
	auto res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (!res)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] unable to create Broadcaster"
			<< " [status code:" << 0 << ", body:\""  << "\"]";
	//	promise.set_exception(std::make_exception_ptr(res->body));
		return;// promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] unable to create Broadcaster"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		//promise.set_exception(std::make_exception_ptr(res->body));
		return;// promise.get_future();
	}

	RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;
	this->CreateSendTransport(enableAudio, useSimulcast);
	this->CreateRecvTransport();
}

void Broadcaster::CreateDataConsumer(const json& body)
{
	//const std::string& dataProducerId = this->dataProducer->GetId();

	/* clang-format off */
	/*json body =
	{
		{ "dataProducerId", dataProducerId }
	};*/
	/* clang-format on */
	// create server data consumer
	/*auto r = cpr::PostAsync(
	           cpr::Url{ this->baseUrl + "/broadcasters/" + this->id + "/transports/" +
	                     this->recvTransport->GetId() + "/consume/data" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } },
	           cpr::VerifySsl{ verifySsl })
	           .get();
	if (r.status_code != 200)
	{
		std::cerr << "[ERROR] server unable to consume mediasoup recv WebRtcTransport"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;
		return;
	}*/
	std::string host =chen::g_cfg.get_string(chen::ECI_MediaSoup_Host) ;
	httplib::Client cli(host, chen::g_cfg.get_uint32(chen::ECI_MediaSoup_Http_Port));
	std::string url = baseUrl +"/broadcasters/" + this->id + "/transports/" +
		this->recvTransport->GetId() + "/consume/data";
	auto res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (!res)
	{
		RTC_LOG(LS_ERROR) << "[ERROR] server unable to consume mediasoup recv WebRtcTransport";
			
		//	promise.set_exception(std::make_exception_ptr(res->body));
		return;// promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] server unable to consume mediasoup recv WebRtcTransport"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		//promise.set_exception(std::make_exception_ptr(res->body));
		return;// promise.get_future();
	}

	RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;
	auto response = json::parse(res->body);
	if (response.find("id") == response.end())
	{
		std::cerr << "[ERROR] 'id' missing in response" << std::endl;
		return;
	}
	auto dataConsumerId = response["id"].get<std::string>();

	if (response.find("streamId") == response.end())
	{
		std::cerr << "[ERROR] 'streamId' missing in response" << std::endl;
		return;
	}
	RTC_LOG(LS_INFO) << " consume mediasoup recv  url = " << url << ", body = " << body.dump();
	uint16_t streamId = response["streamId"].get<uint16_t>();
	auto json_value = nlohmann::json();
	// Create client consumer.
	this->dataConsumer = this->recvTransport->ConsumeData(
	  this, dataConsumerId, body["dataProducerId"]/*dataProducerId*/, std::to_string(streamId), "chat", "stcp");
}
//void Broadcaster::createDataConsumer(std::string dataConsumerId, std::string dataProducerId, std::string streamId, const nlohmann::json& appData)
//{
//	this->recvTransport->ConsumeData(
//		this, dataConsumerId, dataProducerId, "chat", "stcp", appData);
//}
void Broadcaster::CreateSendTransport(bool enableAudio, bool useSimulcast)
{
	std::cout << "[INFO] creating mediasoup send WebRtcTransport..." << std::endl;

	json sctpCapabilities = this->device.GetSctpCapabilities();
	/* clang-format off */
	json body =
	{
		{ "type",    "webrtc" },
		{ "rtcpMux", true     },
		{ "sctpCapabilities", sctpCapabilities }
	};
	/* clang-format on */

	/*auto r = cpr::PostAsync(
	           cpr::Url{ this->baseUrl + "/broadcasters/" + this->id + "/transports" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } },
	           cpr::VerifySsl{ verifySsl })
	           .get();

	if (r.status_code != 200)
	{
		std::cerr << "[ERROR] unable to create send mediasoup WebRtcTransport"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		return;
	}*/
	std::string host =chen::g_cfg.get_string(chen::ECI_MediaSoup_Host) ;
	httplib::Client cli(host, chen::g_cfg.get_uint32(chen::ECI_MediaSoup_Http_Port));
	std::string url = baseUrl + "/broadcasters/" + this->id + "/transports" ;
	auto res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (!res)
	{
		RTC_LOG(LS_ERROR) << "[ERROR] unable to create send mediasoup WebRtcTransport";

		//	promise.set_exception(std::make_exception_ptr(res->body));
		return;// promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] unable to create send mediasoup WebRtcTransport"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		//promise.set_exception(std::make_exception_ptr(res->body));
		return;// promise.get_future();
	}

	RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;
	auto response = json::parse(res->body);

	if (response.find("id") == response.end())
	{
		std::cerr << "[ERROR] 'id' missing in response" << std::endl;

		return;
	}
	else if (response.find("iceParameters") == response.end())
	{
		std::cerr << "[ERROR] 'iceParametersd' missing in response" << std::endl;

		return;
	}
	else if (response.find("iceCandidates") == response.end())
	{
		std::cerr << "[ERROR] 'iceCandidates' missing in response" << std::endl;

		return;
	}
	else if (response.find("dtlsParameters") == response.end())
	{
		std::cerr << "[ERROR] 'dtlsParameters' missing in response" << std::endl;

		return;
	}
	else if (response.find("sctpParameters") == response.end())
	{
		std::cerr << "[ERROR] 'sctpParameters' missing in response" << std::endl;

		return;
	}

	std::cout << "[INFO] creating SendTransport..." << std::endl;

	auto sendTransportId = response["id"].get<std::string>();
	// 创建offer 的流程 回调 onConnect函数
	this->sendTransport = this->device.CreateSendTransport(
	  this,
	  sendTransportId,
	  response["iceParameters"],
	  response["iceCandidates"],
	  response["dtlsParameters"],
	  response["sctpParameters"]);


	///////////////////////// Create Audio Producer //////////////////////////

	if (enableAudio && this->device.CanProduce("audio"))
	{
		auto audioTrack = createAudioTrack(std::to_string(rtc::CreateRandomId()));

		/* clang-format off */
		json codecOptions = {
			{ "opusStereo", true },
		{ "opusDtx",		true }
		};
		/* clang-format on */

		sendTransport->Produce(this, audioTrack, nullptr, &codecOptions);
	}
	else
	{
		RTC_LOG(WARNING)  <<"[WARN] cannot produce audio" ;
	}

	///////////////////////// Create Video Producer //////////////////////////

	if (this->device.CanProduce("video"))
	{
		auto videoTrack = createVideoTrack(std::to_string(rtc::CreateRandomId()));

		if (useSimulcast)
		{
			std::vector<webrtc::RtpEncodingParameters> encodings;
			encodings.emplace_back(webrtc::RtpEncodingParameters());
			encodings.emplace_back(webrtc::RtpEncodingParameters());
			encodings.emplace_back(webrtc::RtpEncodingParameters());

			sendTransport->Produce(this, videoTrack, &encodings, nullptr);
		}
		else
		{
			sendTransport->Produce(this, videoTrack, nullptr, nullptr);
		}
	}
	else
	{
		RTC_LOG(WARNING)  << "[WARN] cannot produce video" ;
	}

	/////////////////////////// Create Audio Producer //////////////////////////

	//if (enableAudio && this->device.CanProduce("audio"))
	//{
	//	auto audioTrack = createAudioTrack(std::to_string(rtc::CreateRandomId()));

	//	/* clang-format off */
	//	json codecOptions = {
	//		{ "opusStereo", true },
	//		{ "opusDtx",		true }
	//	};
	//	/* clang-format on */

	//	this->sendTransport->Produce(this, audioTrack, nullptr, &codecOptions);
	//}
	//else
	//{
	//	std::cerr << "[WARN] cannot produce audio" << std::endl;
	//}

	/////////////////////////// Create Video Producer //////////////////////////

	//if (this->device.CanProduce("video"))
	//{
	//	auto videoTrack = createSquaresVideoTrack(std::to_string(rtc::CreateRandomId()));

	//	if (useSimulcast)
	//	{
	//		std::vector<webrtc::RtpEncodingParameters> encodings;
	//		encodings.emplace_back(webrtc::RtpEncodingParameters());
	//		encodings.emplace_back(webrtc::RtpEncodingParameters());
	//		encodings.emplace_back(webrtc::RtpEncodingParameters());

	//		this->sendTransport->Produce(this, videoTrack, &encodings, nullptr);
	//	}
	//	else
	//	{
	//		this->sendTransport->Produce(this, videoTrack, nullptr, nullptr);
	//	}
	//}
	//else
	//{
	//	std::cerr << "[WARN] cannot produce video" << std::endl;

	//	return;
	//}

	///////////////////////// Create Data Producer //////////////////////////

	this->dataProducer = sendTransport->ProduceData(this, "chat", "stcp");

	uint32_t intervalSeconds = 100;
	std::thread([this, intervalSeconds]() {
		bool run = true;
		while (run)
		{
			std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
			std::time_t t                           = std::chrono::system_clock::to_time_t(p);
			std::string s                           = std::ctime(&t);
			auto dataBuffer                         = webrtc::DataBuffer(s);
			std::cout << "[INFO] sending chat data: " << s << std::endl;
			this->dataProducer->Send(dataBuffer);
			run = timerKiller.WaitFor(std::chrono::seconds(intervalSeconds));
		}
	})
	  .detach();
}

void Broadcaster::CreateRecvTransport()
{
	std::cout << "[INFO] creating mediasoup recv WebRtcTransport..." << std::endl;

	json sctpCapabilities = this->device.GetSctpCapabilities();
	/* clang-format off */
	json body =
	{
		{ "type",    "webrtc" },
		{ "rtcpMux", true     },
		{ "sctpCapabilities", sctpCapabilities }
	};
	/* clang-format on */

	// create server transport
	/*auto r = cpr::PostAsync(
	           cpr::Url{ this->baseUrl + "/broadcasters/" + this->id + "/transports" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } },
	           cpr::VerifySsl{ verifySsl })
	           .get();

	if (r.status_code != 200)
	{
		std::cerr << "[ERROR] unable to create mediasoup recv WebRtcTransport"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		return;
	}*/


	std::string host =chen::g_cfg.get_string(chen::ECI_MediaSoup_Host) ;
	httplib::Client cli(host, chen::g_cfg.get_uint32(chen::ECI_MediaSoup_Http_Port));
	std::string url = baseUrl + "/broadcasters/" + this->id + "/transports" ;
	auto res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (!res)
	{
		RTC_LOG(LS_ERROR) << "[ERROR] unable to create mediasoup recv WebRtcTransport";

		//	promise.set_exception(std::make_exception_ptr(res->body));
		return;// promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] unable to create mediasoup recv WebRtcTransport"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		//promise.set_exception(std::make_exception_ptr(res->body));
		return;// promise.get_future();
	}

	RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;

	auto response = json::parse(res->body);

	if (response.find("id") == response.end())
	{
		std::cerr << "[ERROR] 'id' missing in response" << std::endl;

		return;
	}
	else if (response.find("iceParameters") == response.end())
	{
		std::cerr << "[ERROR] 'iceParameters' missing in response" << std::endl;

		return;
	}
	else if (response.find("iceCandidates") == response.end())
	{
		std::cerr << "[ERROR] 'iceCandidates' missing in response" << std::endl;

		return;
	}
	else if (response.find("dtlsParameters") == response.end())
	{
		std::cerr << "[ERROR] 'dtlsParameters' missing in response" << std::endl;

		return;
	}
	else if (response.find("sctpParameters") == response.end())
	{
		std::cerr << "[ERROR] 'sctpParameters' missing in response" << std::endl;

		return;
	}

	auto recvTransportId = response["id"].get<std::string>();

	std::cout << "[INFO] creating RecvTransport..." << std::endl;

	auto sctpParameters = response["sctpParameters"];

	this->recvTransport = this->device.CreateRecvTransport(
	  this,
	  recvTransportId,
	  response["iceParameters"],
	  response["iceCandidates"],
	  response["dtlsParameters"],
	  sctpParameters);
	const std::string& dataProducerId = this->dataProducer->GetId();

	/* clang-format off */
 	json body_json =
	{
	{ "dataProducerId", dataProducerId }
	};
	this->CreateDataConsumer(body_json);
}



struct handle_data {
	unsigned long process_id;
	HWND best_handle;
};

BOOL IsMainWindow(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}
BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !IsMainWindow(handle)) {
		return TRUE;
	}
	data.best_handle = handle;
	return FALSE;
}

HWND FindMainWindow(unsigned long process_id)
{
	handle_data data;
	data.process_id = process_id;
	data.best_handle = 0;
	EnumWindows(EnumWindowsCallback, (LPARAM)&data);
	return data.best_handle;
}

void Broadcaster::test()
{
	FILE *out_file_ptr = fopen("processid.log", "wb+");
	
 std::thread([=]() {
	 int32_t windows = 0;
		while (true)
		{
			HWND wnd;
			//DWORD main_word = CoGetCurrentProcess();
			//DWORD cur_word =  GetCurrentProcessId();
			std::this_thread::sleep_for(std::chrono::seconds(2));
			//wnd = GetActiveWindow();GetForegroundWindow
			DWORD processid = GetCurrentProcessId();

			printf("=================processid = %s\n", std::to_string(processid).c_str());
			wnd =  FindMainWindow(processid); // GetActiveWindow();;
			//IsWindowVisible()
			if (IsWindowVisible(wnd))
			{
				fprintf(out_file_ptr, "windows !!!\n");
				HWND active = GetActiveWindow();
				if (IsWindowVisible(active))
				{
					fprintf(out_file_ptr, " active main window !!!\n");
				}
				else 
				{
					fprintf(out_file_ptr, "not active -> main window !!!\n");
				}
				if (active == wnd)
				{
					fprintf(out_file_ptr, "windows == main window !!!\n");
				}
				else
				{
					fprintf(out_file_ptr, "windows != main window !!!\n");
				}
			}
			else
			{
				fprintf(out_file_ptr, "not find windows !!!\n");
			}
			fflush(out_file_ptr);
											 //GetForegroundWindow();//获得当前激活的窗口句柄
											 //DWORD SelfThreadId = GetCurrentThreadId(); // GetCurrentThreadId();//获取本身的线程ID
											 //DWORD ForeThreadId = GetWindowThreadProcessId(wnd, NULL);//根据窗口句柄获取线程ID
											 //AttachThreadInput(ForeThreadId, SelfThreadId, true);//附加线程
											 //HWND new_wnd = GetFocus();//获取具有输入焦点的窗口句柄
											 //AttachThreadInput(ForeThreadId, SelfThreadId, false);//取消附加的线程
											 //::PostMessage(wnd, WM_MOUSEWHEEL, WPARAM( 0xFF), 0);//发送一个字消息
			if (windows == 0)
			{
				::PostMessage(wnd, WM_MOUSEWHEEL, MAKEWPARAM(0, 120), MAKELPARAM(600, 300));
				++windows;
				
			}
			else if (windows == 1)
			{
				::PostMessage(wnd, WM_LBUTTONDOWN, NULL, MAKELONG(5, 5));	
				++windows;
			}
			else if (windows == 2)
			{
				::PostMessage(wnd, WM_LBUTTONDOWN, NULL, MAKELONG(300, 300));
				::PostMessage(wnd, WM_MOUSEMOVE, NULL, MAKELONG(900, 900));
				//Sleep(10);//

				::PostMessage(wnd, WM_LBUTTONUP, NULL, MAKELONG(300, 300));
				++windows;
			}
			else if (windows == 3)
			{
				::PostMessage(wnd, WM_MOUSEWHEEL, MAKEWPARAM(0, -120), MAKELPARAM(600, 300));
				++windows;
			}
			else
			{
				windows = 0;
			}
			
			//			
			//
			//std::this_thread::sleep_for()
		}
	}).detach();
}

void Broadcaster::OnMessage(mediasoupclient::DataConsumer* dataConsumer, const webrtc::DataBuffer& buffer)
{
	std::string s = std::string(buffer.data.data<char>(), buffer.data.size());


	RTC_LOG(LS_INFO)<< "[INFO] Broadcaster::OnMessage()"  << "dataConsumer->GetLabel() = " << dataConsumer->GetLabel()<< "[s = " << s << "]";
	if (dataConsumer->GetLabel() == "chat")
	{

		std::cout << "[INFO] received chat data: " + s << std::endl;
	}
	
	return;
	

	return;
	json response;
	try
	{
		response = json::parse(s);
	}
	catch (const std::exception&)
	{
		RTC_LOG(LS_ERROR) << "json parse error !!!!";
		return;
	}
	if (response.find("event") == response.end())
	{
		RTC_LOG(LS_ERROR) << "[ERROR] 'event' missing in response" ;

		return;
	}
	if (response.find("wight") == response.end())
	{
		RTC_LOG(LS_ERROR) << "[ERROR] 'wight' missing in response" ;

		return;
	}
	if (response.find("height") == response.end())
	{
		RTC_LOG(LS_ERROR) << "[ERROR] 'height' missing in response";

		return;
	}
	if (response.find("windowwidth") == response.end())
	{
		RTC_LOG(LS_ERROR) << "[ERROR] 'windowwidth' missing in response";

		return;
	}
	if (response.find("windowheight") == response.end())
	{
		RTC_LOG(LS_ERROR) << "[ERROR] 'windowheight' missing in response";

		return;
	}
	EACTION_MOUSE_TYPE event = static_cast<EACTION_MOUSE_TYPE>(response["event"]);
	double wight = response["wight"] ;
	double height = response["height"]  ;
	double windowwidth =  response["windowwidth"];
	double windowheight =  response["windowheight"];
	if (wight < 0 || height < 0 || windowwidth < 0 || windowheight < 0)
	{
		RTC_LOG(LS_ERROR) << "tail small  wight = " << wight << ", hieght = " << height << ", windowwidth = " << windowwidth << ", windowheight = " << windowheight << " failed !!!";
		return;
	}
	static int wheel = 0;
	DWORD action = 0;


	if (event == EACTION_MOUSE_MOVE)
	{
		action = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	}
	else if (event == EACTION_MOUSE_DOWNUP)
	{
		return;
		action = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	}
	else if (event == EACTION_MOUSE_DOWN)
	{
		action =MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN ;
	}
	else if (event == EACTION_MOUSE_UP)
	{
		action =MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE  | MOUSEEVENTF_LEFTUP;
	}
	else if (event == EACTION_MOUSE_BIG)
	{
		action =MOUSEEVENTF_ABSOLUTE |MOUSEEVENTF_WHEEL;
		if (wheel < 0)
		{
			wheel = 0;
		}
		wheel += 10;
		mouse_event(action,0, 0, wheel, 0 );
		return;
	}
	else if (event == EACTION_MOUSE_SMALL)
	{
		action =MOUSEEVENTF_ABSOLUTE |MOUSEEVENTF_WHEEL;
		if (wheel > 0)
		{
			wheel = 0;
		}
		wheel -= 10;
		mouse_event(action,0, 0, wheel, 0 );
		return;
	}
	else
	{
		RTC_LOG(LS_ERROR) << " event = " << event << " failed !!!";
		return;
	}
	wheel = 0;
	static uint64_t HT = 100000;
	double wx = (wight / windowwidth) * HT ;
	double hy = height / windowheight * HT;
	double  x = ((wx * m_wight)/HT) + 1920;
	double y = ((hy * m_height)/HT) + 1080;
	RTC_LOG(LS_INFO) << "wight = " << wight << ", height = " << height << ", windowwidth = " << windowwidth << ", windowheight = " << windowheight;
	RTC_LOG(LS_INFO) << "wx = " << wx << ", hy = " << hy <<"x = " << x << ", y = " << y;
	mouse_event(action, x * 65535 / m_wight, y * 65535 / m_height, 0, 0 );



}

void Broadcaster::Stop()
{
	std::cout << "[INFO] Broadcaster::Stop()" << std::endl;

	this->timerKiller.Kill();

	stopTrack();
	if (this->recvTransport)
	{
		recvTransport->Close();
	}

	if (this->sendTransport)
	{
		sendTransport->Close();
	}

	/*cpr::DeleteAsync(
	  cpr::Url{ this->baseUrl + "/broadcasters/" + this->id }, cpr::VerifySsl{ verifySsl })
	  .get();*/

	std::string host =chen::g_cfg.get_string(chen::ECI_MediaSoup_Host) ;
	httplib::Client cli(host, chen::g_cfg.get_uint32(chen::ECI_MediaSoup_Http_Port));
	std::string url = baseUrl +"/broadcasters/" + this->id ;
	auto res = cli.Get(url.c_str());
	if (!res)
	{
		RTC_LOG(LS_ERROR) << "[ERROR]Stop";

		//	promise.set_exception(std::make_exception_ptr(res->body));
		return;// promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] Stop"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		//promise.set_exception(std::make_exception_ptr(res->body));
		return;// promise.get_future();
	}

	RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;
}

void Broadcaster::OnOpen(mediasoupclient::DataProducer* /*dataProducer*/)
{
	std::cout << "[INFO] Broadcaster::OnOpen()" << std::endl;
}
void Broadcaster::OnClose(mediasoupclient::DataProducer* /*dataProducer*/)
{
	std::cout << "[INFO] Broadcaster::OnClose()" << std::endl;
}
void Broadcaster::OnBufferedAmountChange(mediasoupclient::DataProducer* /*dataProducer*/, uint64_t /*size*/)
{
	std::cout << "[INFO] Broadcaster::OnBufferedAmountChange()" << std::endl;
}
