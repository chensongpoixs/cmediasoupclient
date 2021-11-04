﻿#include "Broadcaster.hpp"
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
	const char* envServerUrl    = std::getenv("SERVER_URL");
	const char* envRoomId       = std::getenv("ROOM_ID");
	const char* envEnableAudio  = std::getenv("ENABLE_AUDIO");
	const char* envUseSimulcast = std::getenv("USE_SIMULCAST");
	const char* envWebrtcDebug  = std::getenv("WEBRTC_DEBUG");

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
	baseUrl.append("/rooms/").append("chensong");

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

	std::cout << "[INFO] welcome to mediasoup broadcaster app!\n" << std::endl;

	std::cout << "[INFO] verifying that room '" << envRoomId << "' exists..." << std::endl;
	/*auto r = cpr::GetAsync(cpr::Url{ baseUrl }).get();

	if (r.status_code != 200)
	{
		std::cerr << "[ERROR] unable to retrieve room info"
		          << " [status code:" << r.status_code << ", body:\"" << r.text << "\"]" << std::endl;

		return 1;
	}
*/

	httplib::Client cli("127.0.0.1", 8888);

	auto res = cli.Get("/rooms/chensong");
	{
		std::cout << res->status << std::endl;
		std::cout << res->get_header_value("Content-Type") << std::endl;
		std::cout << res->body << std::endl;
	} 
	if (!res)
	{
		std::cout << "error code: " << res->status << std::endl;
		return -1;

	}
	if (res->status != 200)
	{
		std::cout << "error code: " << res->status << std::endl;
		return -1;
	}
	//httplib::Client client();
	auto response = nlohmann::json::parse(res->reason);

	broadcaster.Start(baseUrl, enableAudio, useSimulcast, response);

	std::cout << "[INFO] press Ctrl+C or Cmd+C to leave...";

	//(void)sigsuspend(nullptr);

	return 0;
}
