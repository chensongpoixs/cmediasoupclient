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
#include <WinUser.h>
#include <Windows.h>



using json = nlohmann::json;
Broadcaster broadcaster;
bool stoped = false;
void signalHandler(int signum)
{
	RTC_LOG(LS_INFO) << "[INFO] interrupt signal (" << signum << ") received";
	webrtc::g_websocket_mgr.destroy();
	// Remove broadcaster from the server.
	broadcaster.Stop();
	RTC_LOG(LS_INFO) << "[INFO] leaving!" ;
	stoped = true;
	//std::exit(signum);
}


void test_win()
{
	// 获取带标题栏和菜单栏即全屏像素大小
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);


	printf("width = %d, height = %d\n", width, height);
	// 睡眠5s，准备时间
	//Sleep(5000);
	// 死循环
	int w = 0;
	while (1) 
	{
		w += 10;
		if (width < w)
		{
			w = 0;
		}
		printf("-%d----\n", w);
		// 移动到绝对位置右击
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, w, 0);
		//mouse_event(MOUSEEVENTF_ABSOLUTE  | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP/* | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP*/ | MOUSEEVENTF_MOVE, w * 65535 / width, 362 * 65535 / height, 0, 0 );
		std::this_thread::sleep_for(std::chrono::seconds(3));

		// 按下'q'键
		//keybd_event(81, 0, 0, 0);
		//keybd_event(81, 0, KEYEVENTF_KEYUP, 0);
		//Sleep(500);
		w += 10;
		if (width < w)
		{
			w = 0;
		}
		// 移动到绝对位置右击
		printf("-%d----\n", w);
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, w, 0);
		//mouse_event(MOUSEEVENTF_ABSOLUTE  | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP/*| MOUSEEVENTF_RIGHTDOWN  | MOUSEEVENTF_RIGHTUP*/ , w * 65535 / width, 760 * 65535 / height, 0, 0 );
		std::this_thread::sleep_for(std::chrono::seconds(3));
		
	}

}


void test_scroll()
{
	//1.先获得桌面窗口
	HWND windows = ::GetDesktopWindow();
	SCROLLINFO scrollinfo;
	//SCROLLINFO info;
	//scrollinfo.nPos = 50;
	scrollinfo.cbSize = sizeof(SCROLLINFO);
	bool ret = 	GetScrollInfo(windows, SB_VERT, &scrollinfo/*SIF_ALL*/);


	/*cbSize = sizeof (SCROLLINFO) ;
	fMask = SIF_RANGE | SIF_PAGE ;
	nMin = 0 ;
	nMax = NUMLINES - 1 ;
	nPage = cyClient / cyChar ;
	SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;*/
	/*switch (nSBCode)
	{
	case SB_LINERIGHT:*/
		//if (scrollinfo.nPos < scrollinfo.nMax)
		while(true)
		{
			scrollinfo.nPos += 10;
			SetScrollInfo(SB_HORZ, 0, &scrollinfo, 0);
			//ScrollWindow(-10, 0);
			printf("pos = %d ----\n", scrollinfo.nPos);
			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
		

		//break;
	//}
}

int main(int argc, char* argv[])
{
	// Register signal SIGINT and signal handler.
	//test_win();
	//return 0;
	/*test_scroll();
	return 0;*/

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
	webrtc::g_cfg.show();
	std::string ws_url = "ws://" + webrtc::g_cfg.get_string(webrtc::ECI_MediaSoup_Host)+ ":" + std::to_string(webrtc::g_cfg.get_int32(webrtc::ECI_MediaSoup_Http_Port)) +"/?roomId="+webrtc::g_cfg.get_string(webrtc::ECI_Room_Name) +"&peerId=" + webrtc::g_cfg.get_string(webrtc::ECI_Client_Name);//ws://127.0.0.1:8888/?roomId=chensong&peerId=xiqhlyrn", "http://127.0.0.1:8888")
	std::string origin = "http://" + webrtc::g_cfg.get_string(webrtc::ECI_MediaSoup_Host)+ ":" + std::to_string(webrtc::g_cfg.get_int32(webrtc::ECI_MediaSoup_Http_Port)) ;
	if (!webrtc::g_websocket_mgr.init(ws_url, origin))
	{
		RTC_LOG(LS_ERROR) << "weboscket connect failed !!! url = " << ws_url;
		return -1;
	}
	webrtc::g_websocket_mgr.start();
	uint64_t id = 34;
	bool send = false;
	while (webrtc::g_websocket_mgr.get_status() != webrtc::CWEBSOCKET_MESSAGE)
	{
		//RTC_LOG(LS_INFO) << "websocket connect .... 100 ";
		/*{
		request : true,
			id      : 12345678,
			method  : 'chatmessage',
			data    :
		{
		type  : 'text',
			value : 'Hi there!'
		}
		}*/

		json body_data =
		{
			{
				"type", "text"
			},
				{
					"value", "Hi there@@"
				}
		};
		json body =
		{
			{ "request" , true },
			{ "id", ++id},
			{ "method", "chatmeessage"},
			{ "data", body_data } 
			
		};
		//RTC_LOG(LS_INFO) << body.dump().c_str();
		/*if (!send)
		{
			webrtc::g_websocket_mgr.send(body.dump().c_str());
			send = true;
		}*/
		
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

	

	broadcaster.Start(baseUrl, enableAudio, useSimulcast, response, verifySsl, name);

	std::cout << "[INFO] press Ctrl+C or Cmd+C to leave..." << std::endl;
	std::string new_url  =  url + "/chensong";
	std::set<std::string> dataProduceIds;
	while (!stoped)
	{
		//broadcaster.createDataConsumer();

		
		res = cli.Get(new_url.c_str());
		if (!res)
		{
			RTC_LOG(LS_ERROR) << "[ERROR]Stop";

			//	promise.set_exception(std::make_exception_ptr(res->body));
			//return -1;// promise.get_future();
		}
		if (res->status != 200)
		{
			RTC_LOG(LS_ERROR)  << "[ERROR] Stop"
				<< " [status code:" << res->status << ", body:\"" << res->body << "\"]" ;

			//promise.set_exception(std::make_exception_ptr(res->body));
			//return -1;// promise.get_future();
		}
		else
		{
			//RTC_LOG(INFO)  << __FUNCTION__ << __LINE__ <<"[" << res->body << "]" ;
			auto response = nlohmann::json::parse(res->body);

			/*
			{
    "peers":[
        {
            "id":"vqsz1a1g",
            "displayName":"Doublade",
            "device":{
                "flag":"chrome",
                "name":"Chrome",
                "version":"95.0.4638.69"
            },
            "transports":[
                {
                    "id":"586d48c7-d370-4fc1-9d3b-b5d209b2b933"
                },
                {
                    "id":"026b77b1-a0f0-4377-b69e-4ad9470448a6"
                }
            ],
            "dataProducers":[
                {
                    "id":"d3221396-02da-400c-b2f8-ba8883bf690b",
                    "label":"chat",
                    "protocol":""
                },
                {
                    "id":"60a0d2d5-04f2-4726-b80b-40d21217c473",
                    "label":"bot",
                    "protocol":""
                }
            ],
            "dataConsumers":[
                {
                    "id":"c92efaa6-c6b9-4965-9ed8-79912e2aaefc",
                    "sctpStreamParameters":{
                        "maxRetransmits":1,
                        "ordered":false,
                        "streamId":1
                    },
                    "label":"chat",
                    "protocol":"",
                    "dataProducerId":"c28d81fb-3d00-49c5-a434-baa5ad2a655a",
                    "displayName":"Toxapex"
                }
            ]
        },
        {
            "id":"vznfk7u6",
            "displayName":"Toxapex",
            "device":{
                "flag":"chrome",
                "name":"Chrome",
                "version":"95.0.4638.69"
            },
            "transports":[
                {
                    "id":"1ae9c259-314b-444f-837c-896783ffedbd"
                },
                {
                    "id":"43682bb4-6329-4aca-bad1-2e4ba7c1d550"
                }
            ],
            "dataProducers":[
                {
                    "id":"c28d81fb-3d00-49c5-a434-baa5ad2a655a",
                    "label":"chat",
                    "protocol":""
                },
                {
                    "id":"a47ad3c7-8837-4519-aaea-18495d3bcbbc",
                    "label":"bot",
                    "protocol":""
                }
            ],
            "dataConsumers":[
                {
                    "id":"6daf12dc-871b-4cf9-85b4-842f46c2ca91",
                    "sctpStreamParameters":{
                        "maxRetransmits":1,
                        "ordered":false,
                        "streamId":1
                    },
                    "label":"chat",
                    "protocol":"",
                    "dataProducerId":"d3221396-02da-400c-b2f8-ba8883bf690b",
                    "displayName":"Doublade"
                }
            ]
        }
    ]
}


			*/

			if (response["peers"].is_array())
			{
				for (int i = 0; i < response["peers"].size(); ++i)
				{
					if (response["peers"][i]["displayName"] != "test" )
					{
						auto iter = dataProduceIds.find(response["peers"][i]["displayName"]);
						if (iter != dataProduceIds.end())
						{
							continue;
						}
						std::string displayName = response["peers"][i]["displayName"];
						
						for (int j = 0; j < response["peers"][i]["dataProducers"].size(); ++j)
						{
							if ("chat" == response["peers"][i]["dataProducers"][j]["label"])
							{
								//dataProduceIds;
								//std::string id = response["peers"][i]["dataProducers"][j]["id"];
								std::string dataProducerId = response["peers"][i]["dataProducers"][j]["id"];
								//uint32_t streamId = response["peers"][i]["dataConsumers"][j]["sctpStreamParameters"]["streamId"];
								//json AppData = response["peers"][i]["dataConsumers"][j]["sctpStreamParameters"]["AppData"];;
								json body =
								{
									{ "dataProducerId", dataProducerId }
								};
								broadcaster.CreateDataConsumer(body);
								RTC_LOG(LS_INFO) << "id = " << id << ", dataProducerId = " << dataProducerId;
								dataProduceIds.insert(displayName);
							}
						}
					}
				}
			}
		}
		

		std::this_thread::sleep_for(std::chrono::seconds(1));
		//std::cin.get();
	}

	return 0;
}
