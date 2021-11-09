#include "Broadcaster.hpp"
#include "mediasoupclient.hpp"
#include "peerConnectionUtils.hpp"
#include "json.hpp"
//#include <cpr/cpr.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include "httplib.h"
#include "desktop_capture.h"
using json = nlohmann::json;

void Broadcaster::OnTransportClose(mediasoupclient::Producer* /*producer*/)
{
	RTC_LOG(INFO)  << "[INFO] Broadcaster::OnTransportClose()" ;
}

/* Transport::Listener::OnConnect
 *
 * Fired for the first Transport::Consume() or Transport::Produce().
 * Update the already created remote transport with the local DTLS parameters.
 */
std::future<void> Broadcaster::OnConnect(
  mediasoupclient::Transport* /*transport*/, const json& dtlsParameters)
{
	RTC_LOG(INFO)  << "[INFO] Broadcaster::OnConnect()" ;
	// std::cout << "[INFO] dtlsParameters: " << dtlsParameters.dump(4) << std::endl;

	std::promise<void> promise;

	/* clang-format off */
	json body =
	{
		{ "dtlsParameters", dtlsParameters }
	};
	/* clang-format on */

	/*auto r = cpr::PostAsync(
	           cpr::Url{ this->baseUrl + "/broadcasters/" + this->id + "/transports/" +
	                     this->transportId + "/connect" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } })
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



	httplib::Client cli("localhost", 8888);
	std::string url = baseUrl + "/broadcasters/" + this->id + "/transports/" +
		this->transportId + "/connect";
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
  mediasoupclient::Transport* transport, const std::string& connectionState)
{
	RTC_LOG(INFO)  << "[INFO] Broadcaster::OnConnectionStateChange() [connectionState:" << connectionState
	          << "]" ;

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
	RTC_LOG(INFO)  << "[INFO] Broadcaster::OnProduce()";
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
	                     this->transportId + "/producers" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } })
	           .get();

	if (r.status_code == 200)
	{
		auto response = json::parse(r.text);

		auto it = response.find("id");
		if (it == response.end() || !it->is_string())
			promise.set_exception(std::make_exception_ptr("'id' missing in response"));

		promise.set_value((*it).get<std::string>());
	}
	else
	{
		std::cerr << "[ERROR] unable to create producer"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		promise.set_exception(std::make_exception_ptr(r.text));
	}*/


	httplib::Client cli("localhost", 8888);
	std::string url = baseUrl + "/broadcasters/" + this->id + "/transports/" +
		this->transportId + "/producers";
	auto res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (!res)
	{
		RTC_LOG(LS_ERROR)  <<"[error] !!!!\n";
		return promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] unable to create producer"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		promise.set_exception(std::make_exception_ptr(res->body));
		return promise.get_future();
	}
	auto response = json::parse(res->body);
	RTC_LOG(INFO) <<  __FUNCTION__ << __LINE__ <<"[" << res->body << "]";
	auto it = response.find("id");
	if (it == response.end() || !it->is_string())
		promise.set_exception(std::make_exception_ptr("'id' missing in response"));

	promise.set_value((*it).get<std::string>());
	return promise.get_future();
}

void Broadcaster::Start(
  const std::string& baseUrl, bool enableAudio, bool useSimulcast, const json& routerRtpCapabilities, std::string name)
{
	RTC_LOG(INFO) << "[INFO] Broadcaster::Start()"  ;

	this->baseUrl = baseUrl;

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
				{ "name",    "libmediasoupclient"       },
				{ "version", mediasoupclient::Version() }
			}
		}
	};
	/* clang-format on */

	/*auto r = cpr::PostAsync(
	           cpr::Url{ this->baseUrl + "/broadcasters" },
	           cpr::Body{ body.dump() },
	           cpr::Header{ { "Content-Type", "application/json" } })
	           .get();

	if (r.status_code != 200)
	{
		std::cerr << "[ERROR] unable to create Broadcaster"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		return;
	}*/

	httplib::Client cli("localhost", 8888);
	std::string url = this->baseUrl + "/broadcasters";
	auto res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (!res)
	{
		RTC_LOG(WARNING) << "[error] !!!!\n";
		return;
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR) << "[ERROR] unable to create Broadcaster"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]"  ;

		//promise.set_exception(std::make_exception_ptr(r.text));
		return;
	}


	RTC_LOG(INFO) << "[INFO] creating mediasoup WebRtcTransport..."  ;

	/* clang-format off */
	body =
	{
		{ "type",    "webrtc" },
		{ "rtcpMux", true     }
	};
	/* clang-format on */

	/*r = cpr::PostAsync(
	      cpr::Url{ this->baseUrl + "/broadcasters/" + this->id + "/transports" },
	      cpr::Body{ body.dump() },
	      cpr::Header{ { "Content-Type", "application/json" } })
	      .get();

	if (r.status_code != 200)
	{
		std::cerr << "[ERROR] unable to create mediasoup WebRtcTransport"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		return;
	}*/
	 url = baseUrl + "/broadcasters/" + this->id + "/transports";
	 res = cli.Post(url.c_str(), body.dump(), "application/json");
	if (res == nullptr)
	{
		RTC_LOG(WARNING) << "[error] !!!!\n";
		return;
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR) << "[ERROR] unable to create mediasoup WebRtcTransport"
			<< " [status code:" << std::to_string(res->status) << ", body:\"" << res->body.c_str() << "\"]" ;

		//promise.set_exception(std::make_exception_ptr(r.text));
		return;
	}
	/*
	
	{
    "id":"2266b942-1ef3-4527-b282-dd3a3deb7f92",
    "iceParameters":{
        "iceLite":true,
        "password":"yzgnrby4ruc1fp3irgt0kcvl8vde0q4n",
        "usernameFragment":"0pi95tgqtcq3p754"
    },
    "iceCandidates":[
        {
            "foundation":"udpcandidate",
            "ip":"192.168.0.78",
            "port":44854,
            "priority":1076302079,
            "protocol":"udp",
            "type":"host"
        }
    ],
    "dtlsParameters":{
        "fingerprints":[
            {
                "algorithm":"sha-1",
                "value":"CB:03:0A:85:E2:90:B0:F4:2C:F5:4D:10:CE:DA:EB:D6:F0:E6:B7:AB"
            },
            {
                "algorithm":"sha-224",
                "value":"D5:CB:D3:4E:69:06:CA:B6:10:EA:57:73:40:49:71:6A:30:A2:EB:AE:A2:44:89:3B:02:15:C9:1B"
            },
            {
                "algorithm":"sha-256",
                "value":"CB:3E:B4:6B:5E:B2:88:58:63:CC:52:0E:6C:13:A7:9F:5D:33:21:76:E2:D1:41:BD:97:A7:8E:3D:C9:0E:7A:7D"
            },
            {
                "algorithm":"sha-384",
                "value":"FD:AC:84:04:12:36:66:55:60:1A:4A:A5:9C:45:E6:21:FE:32:73:7C:75:62:A7:0B:CD:12:55:EF:DE:77:FB:3D:CA:67:2F:EE:83:39:CF:40:8C:B0:2A:72:1E:C6:67:79"
            },
            {
                "algorithm":"sha-512",
                "value":"01:7D:7B:4A:68:33:A4:E2:26:DB:FB:25:18:D0:AC:48:A2:83:0C:CA:E8:6C:35:1A:28:24:3A:F3:D5:5B:DA:74:1E:41:E3:E2:57:24:42:0C:39:9C:7B:2A:8C:8A:B9:D3:07:9E:56:91:14:D4:C8:60:C3:89:AE:49:E1:EE:D2:79"
            }
        ],
        "role":"auto"
    }
}
	*/
	auto response = json::parse(res->body);
	RTC_LOG(INFO)  <<  __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;
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

	RTC_LOG(INFO)  <<"[INFO] creating SendTransport..." ;

	this->transportId = response["id"].get<std::string>();

	auto sendTransport = this->device.CreateSendTransport(
	  this,
	  this->transportId,
	  response["iceParameters"],
	  response["iceCandidates"],
	  response["dtlsParameters"]);

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
}

void Broadcaster::Stop()
{
	RTC_LOG(INFO)  << "[INFO] Broadcaster::Stop()" ;
	httplib::Client cli("localhost", 8888);
	std::string url = this->baseUrl + "/broadcasters/"+id;
	auto res = cli.Post(url.c_str());
	//cpr::DeleteAsync(cpr::Url{ this->baseUrl + "/broadcasters/" + this->id }).get();
}
