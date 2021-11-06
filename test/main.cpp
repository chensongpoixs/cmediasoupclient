#include "Broadcaster.hpp"
#include "mediasoupclient.hpp"
//#include <cpr/cpr.h>
#include <csignal> // sigsuspend()
#include <cstdlib>
#include <iostream>
#include <string>
#include "httplib.h"
using json = nlohmann::json;

static Broadcaster broadcaster;

void signalHandler(int signum)
{
	std::cout << "[INFO] interrupt signal (" << signum << ") received" << std::endl;

	// Remove broadcaster from the server.
	broadcaster.Stop();

	std::cout << "[INFO] leaving!" << std::endl;

	std::exit(signum);
}

int main(int argc, char* argv[])
{
	// Register signal SIGINT and signal handler.
	signal(SIGINT, signalHandler);

	// Retrieve configuration from environment variables.
	//const char* envServerUrl    = std::getenv("SERVER_URL");
	//const char* envRoomId       = std::getenv("ROOM_ID");
	const char* envEnableAudio  = std::getenv("ENABLE_AUDIO");
	const char* envUseSimulcast = std::getenv("USE_SIMULCAST");
	const char* envWebrtcDebug  = std::getenv("WEBRTC_DEBUG");
	//SERVER_URL=https://my.mediasoup-demo.org:4443 ROOM_ID=broadcaster build/broadcaster
	const char * envServerUrl = "http://169.254.119.31:8888";
	const char * envRoomId = "chensong";
	if (envServerUrl == nullptr)
	{
		RTC_LOG(INFO)  << "[ERROR] missing 'SERVER_URL' environment variable"  ;

		return 1;
	}

	if (envRoomId == nullptr)
	{
		RTC_LOG(INFO)  << "[ERROR] missing 'ROOM_ID' environment variable"  ;

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

	// Set RTC logging severity.
	if (envWebrtcDebug && std::string(envWebrtcDebug) == "info")
		rtc::LogMessage::LogToDebug(rtc::LoggingSeverity::LS_INFO);
	else if (envWebrtcDebug && std::string(envWebrtcDebug) == "warn")
		rtc::LogMessage::LogToDebug(rtc::LoggingSeverity::LS_WARNING);
	else if (envWebrtcDebug && std::string(envWebrtcDebug) == "error")
		rtc::LogMessage::LogToDebug(rtc::LoggingSeverity::LS_ERROR);

	auto logLevel = mediasoupclient::Logger::LogLevel::LOG_DEBUG;
	mediasoupclient::Logger::SetLogLevel(logLevel);
	mediasoupclient::Logger::SetDefaultHandler();

	// Initilize mediasoupclient.
	mediasoupclient::Initialize();

	RTC_LOG(INFO)  << "[INFO] welcome to mediasoup broadcaster app!\n"  ;

	RTC_LOG(INFO)  << "[INFO] verifying that room '" << envRoomId << "' exists..."  ;
	/*auto r = cpr::GetAsync(cpr::Url{ baseUrl }).get();

	if (r.status_code != 200)
	{
		std::cerr << "[ERROR] unable to retrieve room info"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		return 1;
	}
*/

	httplib::Client cli("127.0.0.1", 8888);
	//std::string url = "/rooms/" + std::string(envRoomId);
	auto res = cli.Get(baseUrl.c_str());
	{
		RTC_LOG(INFO)  << res->status  ;
		RTC_LOG(INFO)  << res->get_header_value("Content-Type")  ;
		RTC_LOG(INFO)  << res->body  ;
	} 
	if (res == nullptr)
	{
		RTC_LOG(INFO)  << "error code: " << res->status  ;
		return -1;

	}
	if (res->status != 200)
	{
		RTC_LOG(INFO)  << "error code: " << res->status  ;
		return -1;
	}
	RTC_LOG(INFO)  <<  __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;
	RTC_LOG(INFO)  << "[" << res->reason << "]"  ;
	//httplib::Client client();
	auto response = nlohmann::json::parse(res->body);

	broadcaster.Start(baseUrl, enableAudio, useSimulcast, response);

	RTC_LOG(INFO)  << "[INFO] press Ctrl+C or Cmd+C to leave...";

	//(void)sigsuspend(nullptr);
	//signalHandler(15);
	
	while (true)
	{
		RTC_LOG(INFO) << "milliseconds 10000 ...";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}
	return 0;
}
