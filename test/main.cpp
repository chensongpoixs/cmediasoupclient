#include "Broadcaster.hpp"
#include "mediasoupclient.hpp"
//#include <cpr/cpr.h>
#include <csignal> // sigsuspend()
#include <cstdlib>
#include <iostream>
#include <string>
#include "httplib.h"
#include "cwebsocket_mgr.h"
#include "ccfg.h"

using json = nlohmann::json;

void signalHandler(int signum)
{
	RTC_LOG(LS_INFO) << "[INFO] interrupt signal (" << signum << ") received";

	RTC_LOG(LS_INFO) << "[INFO] leaving!" ;

	std::exit(signum);
}

int main(int argc, char* argv[])
{
	// Register signal SIGINT and signal handler.
	
	

	signal(SIGINT, signalHandler);
	const char* config_filename = "client.cfg";
	if (argc > 1)
	{
		config_filename = argv[1];
	}
	bool init = webrtc::g_cfg.init(config_filename);
	if (!init)
	{
		RTC_LOG(LS_ERROR) << "config init failed !!!" << config_filename;
		return -1;
	}
	webrtc::g_cfg.show();
	std::string ws_url = "ws://" + webrtc::g_cfg.get_string(webrtc::ECI_MediaSoup_Host)+ ":" + std::to_string(webrtc::g_cfg.get_int32(webrtc::ECI_MediaSoup_Http_Port)) +"/?roomId="+webrtc::g_cfg.get_string(webrtc::ECI_Room_Name) +"&peerId=" + webrtc::g_cfg.get_string(webrtc::ECI_Client_Name);//ws://127.0.0.1:8888/?roomId=chensong&peerId=xiqhlyrn", "http://127.0.0.1:8888")
	std::string origin = "http://" + webrtc::g_cfg.get_string(webrtc::ECI_MediaSoup_Host)+ ":" + std::to_string(webrtc::g_cfg.get_int32(webrtc::ECI_MediaSoup_Http_Port)) ;
	if (!webrtc::g_websocket_mgr.init(ws_url, origin))
	{
		RTC_LOG(LS_ERROR) << "weboscket connect failed !!! url = " << ws_url;
		return -1;
	}
	webrtc::g_websocket_mgr.start();

	while (webrtc::g_websocket_mgr.get_status() != webrtc::CWEBSOCKET_MESSAGE)
	{
		RTC_LOG(LS_INFO) << "websocket connect .... 100 ";
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}





	// Retrieve configuration from environment variables.
	//const char* envServerUrl    = std::getenv("SERVER_URL");
	//const char* envRoomId       = std::getenv("ROOM_ID");
	//const char* envEnableAudio  = std::getenv("ENABLE_AUDIO");
	//const char* envUseSimulcast = std::getenv("USE_SIMULCAST");
	const char* envWebrtcDebug  = std::getenv("WEBRTC_DEBUG");
	const char* envVerifySsl    = std::getenv("VERIFY_SSL");
	const char * envServerUrl = origin.c_str();
	std::string room_name =  webrtc::g_cfg.get_string(webrtc::ECI_Room_Name);
	const char * envRoomId = room_name.c_str();
	const char * envEnableAudio = "false";
	std::string client_name = webrtc::g_cfg.get_string(webrtc::ECI_Client_Name);
	const char * name =  client_name.c_str();
	const char* envUseSimulcast = "false";
	if (envServerUrl == nullptr)
	{
		std::cerr << "[ERROR] missing 'SERVER_URL' environment variable" << std::endl;

		return 1;
	}

	if (envRoomId == nullptr)
	{
		std::cerr << "[ERROR] missing 'ROOM_ID' environment variable" << std::endl;

		return 1;
	}

	std::string baseUrl = envServerUrl;
	baseUrl.append("/rooms/").append(envRoomId);

	bool enableAudio = true;

	if (envEnableAudio && std::string(envEnableAudio) == "false")
		enableAudio = false;

	bool useSimulcast = true;

	if (envUseSimulcast && std::string(envUseSimulcast) == "false")
		useSimulcast = false;

	bool verifySsl = true;
	if (envVerifySsl && std::string(envVerifySsl) == "false")
		verifySsl = false;

	// Set RTC logging severity.
	if (envWebrtcDebug)
	{
		if (std::string(envWebrtcDebug) == "info")
			rtc::LogMessage::LogToDebug(rtc::LoggingSeverity::LS_INFO);
		else if (std::string(envWebrtcDebug) == "warn")
			rtc::LogMessage::LogToDebug(rtc::LoggingSeverity::LS_WARNING);
		else if (std::string(envWebrtcDebug) == "error")
			rtc::LogMessage::LogToDebug(rtc::LoggingSeverity::LS_ERROR);
	}

	auto logLevel = mediasoupclient::Logger::LogLevel::LOG_DEBUG;
	mediasoupclient::Logger::SetLogLevel(logLevel);
	mediasoupclient::Logger::SetDefaultHandler();

	// Initilize mediasoupclient.
	mediasoupclient::Initialize();

	std::cout << "[INFO] welcome to mediasoup broadcaster app!\n" << std::endl;

	std::cout << "[INFO] verifying that room '" << envRoomId << "' exists..." << std::endl;
	/*auto r = cpr::GetAsync(cpr::Url{ baseUrl }, cpr::VerifySsl{ verifySsl }).get();

	if (r.status_code != 200)
	{
		std::cerr << "[ERROR] unable to retrieve room info"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		return 1;
	}
	else
	{
		std::cout << "[INFO] found room" << envRoomId << std::endl;
	}*/
	std::string host =webrtc::g_cfg.get_string(webrtc::ECI_MediaSoup_Host) ;
	httplib::Client cli(host, webrtc::g_cfg.get_uint32(webrtc::ECI_MediaSoup_Http_Port));
	std::string url = baseUrl;
	auto res = cli.Get(url.c_str());
	if (!res)
	{
		RTC_LOG(LS_ERROR) << "[ERROR]Stop";

		//	promise.set_exception(std::make_exception_ptr(res->body));
		return -1;// promise.get_future();
	}
	if (res->status != 200)
	{
		RTC_LOG(LS_ERROR)  << "[ERROR] Stop"
			<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

		//promise.set_exception(std::make_exception_ptr(res->body));
		return -1;// promise.get_future();
	}

	RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;
	auto response = nlohmann::json::parse(res->body);

	Broadcaster broadcaster;

	broadcaster.Start(baseUrl, enableAudio, useSimulcast, response, verifySsl, name);

	std::cout << "[INFO] press Ctrl+C or Cmd+C to leave..." << std::endl;

	while (true)
	{
		std::cin.get();
	}

	return 0;
}
