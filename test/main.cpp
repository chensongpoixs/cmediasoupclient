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
	std::exit(signum);
}


void test_win()
{

	//SetWindowPos(nullptr, -1, 0, 0, 0, 0, 3);
	int width =  GetSystemMetrics(SM_CXVIRTUALSCREEN);//虚拟桌面宽度

	  width =  GetSystemMetrics(SM_CYVIRTUALSCREEN);//虚拟桌面高度

	  width =  GetSystemMetrics(SM_XVIRTUALSCREEN );//虚拟桌面左上角X坐标

	  width =  GetSystemMetrics(SM_YVIRTUALSCREEN );//虚拟桌面左上角Y坐标
	// 获取带标题栏和菜单栏即全屏像素大小
	  width = GetSystemMetrics(SM_CXSCREEN) ;
	int height = GetSystemMetrics(SM_CYSCREEN);


	printf("width = %d, height = %d\n", width, height);
	// 睡眠5s，准备时间
	//Sleep(5000);
	// 死循环
	int w = 1920;
	while (1) 
	{
		w += 32;
		if (width < w)
		{
		//	w = 0;
		}
		printf("-%d----\n", w);
		// 移动到绝对位置右击
		//mouse_event(MOUSEEVENTF_WHEEL, 0, 0, w, 0);
		mouse_event(MOUSEEVENTF_ABSOLUTE  | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP/* | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP*/ /*| MOUSEEVENTF_MOVE*/, (w * 65536) / width , 362 * 65535 / height, 0, 0 );
		std::this_thread::sleep_for(std::chrono::seconds(3));

		// 按下'q'键
		//keybd_event(81, 0, 0, 0);
		//keybd_event(81, 0, KEYEVENTF_KEYUP, 0);
		//Sleep(500);
		w += 40;
		if (width < w)
		{
			//w = 0;
		}
		// 移动到绝对位置右击
		printf("-%d----\n", w);
		//mouse_event(MOUSEEVENTF_WHEEL, 0, 0, w, 0);
		mouse_event(MOUSEEVENTF_ABSOLUTE  /*| MOUSEEVENTF_MOVE*/ | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP/*| MOUSEEVENTF_RIGHTDOWN  | MOUSEEVENTF_RIGHTUP*/ , (w * 65536) / width  , 760 * 65535 / height, 0, 0 );
		
		std::this_thread::sleep_for(std::chrono::seconds(3));
		
	}

}


void test_scroll()
{
	//1.先获得桌面窗口
	//HWND windows = ::GetDesktopWindow();
	//SCROLLINFO scrollinfo;
	////SCROLLINFO info;
	////scrollinfo.nPos = 50;
	//scrollinfo.cbSize = sizeof(SCROLLINFO);
	//scrollinfo.fMask = SIF_ALL;
	//scrollinfo.nPos = 10;
	//bool ret = 	GetScrollInfo(windows, SB_VERT, &scrollinfo/*SIF_ALL*/);


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
	bool x = true;
	int delta = 0;
		while(true)
		{
			//scrollinfo.nPos += 10;
			//SetScrollInfo(SB_HORZ, 0, &scrollinfo, 0);
			////ScrollWindow(-10, 0);
			//printf("pos = %d ----\n", scrollinfo.nPos);
			//std::this_thread::sleep_for(std::chrono::seconds(3));
			delta = 10;
			mouse_event(MOUSEEVENTF_ABSOLUTE |/*MOUSEEVENTF_HWHEEL*/  MOUSEEVENTF_WHEEL , 0, 0, delta, 0);
			printf("delta = %d\n", delta);
			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
		

		//break;
	//}
}





#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>

void accumulate(std::vector<int>::iterator first,
	std::vector<int>::iterator last,
	std::promise<int> accumulate_promise)
{
	int sum = std::accumulate(first, last, 0);
	accumulate_promise.set_value(sum);  // 提醒 future
}

void do_work(std::promise<void> barrier)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	barrier.set_value();
}

int test_promise()
{
	// 演示用 promise<int> 在线程间传递结果。
	std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
	std::promise<int> accumulate_promise;
	std::future<int> accumulate_future = accumulate_promise.get_future();
	std::thread work_thread(accumulate, numbers.begin(), numbers.end(),
		std::move(accumulate_promise));

	// future::get() 将等待直至该 future 拥有合法结果并取得它
	// 无需在 get() 前调用 wait()
	//accumulate_future.wait();  // 等待结果
	std::cout << "result=" << accumulate_future.get() << '\n';
	work_thread.join();  // wait for thread completion

						 // 演示用 promise<void> 在线程间对状态发信号
	std::promise<void> barrier;
	std::future<void> barrier_future = barrier.get_future();
	std::thread new_work_thread(do_work, std::move(barrier));
	barrier_future.wait();
	new_work_thread.join();
	return 0;
}


void get_display()
{
	//枚举所有屏幕 
	DISPLAY_DEVICE ddDisplay;
	ZeroMemory(&ddDisplay, sizeof(ddDisplay));
	ddDisplay.cb = sizeof(ddDisplay);
	DEVMODE dm;
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);
	//获取屏幕数量
	int screenNUm = GetSystemMetrics(SM_CMONITORS);
	if (screenNUm < 2)
	{
		//L"未发现第二个屏幕，请检查投影是否正常分屏"
	}
	//因为屏幕不是连续的所以需要逐个遍历 具体应该遍历多个才合适我不请
	//期望专业人士给予指点 默认屏幕在0位 但是接入的屏幕不在1位
	for (char i = 0; i < screenNUm +10; i++)
	{
		//下一步活为了获得每个显示设备的名字
		int flag = EnumDisplayDevices(NULL, i, &ddDisplay, NULL);
		flag = flag&&EnumDisplaySettings(ddDisplay.DeviceName, ENUM_CURRENT_SETTINGS, &dm);
		if (!flag)
		{
			continue;
		}
		printf("screenNUm = %d, devicename = %s\n", screenNUm, ddDisplay.DeviceName);
		HDC desktopDc = GetDC(NULL);
		int DPI = GetDeviceCaps(desktopDc, LOGPIXELSX);
		DeleteDC(desktopDc);
		float Bili;//屏幕的放大比例 
		switch (DPI)
		{
		case 96:
			Bili = 1.0;
			break;
		case 120:
			Bili = 1.25;
			break;
		case 144:
			Bili = 1.50;
			break;
		case 192:
			Bili = 2.00;
			break;

		default:
			break;
		}
		////这里拿到了第二屏幕的尺寸信息
		//CRect Sc2Rect = CRect(dm.dmPosition.x*Bili,
		//	dm.dmPosition.y*Bili, 
		//	(dm.dmPosition.x + dm.dmPelsWidth)*Bili, 
		//	(dm.dmPosition.y + dm.dmPelsHeight)*Bili);
	}


}






struct ALLMONITORINFO 

{ 

	HMONITOR hMonitor; 

	RECT     rect; 

	bool     isPrimary; 

}; 

BOOL CALLBACK MonitorEnumProc(__in  HMONITOR hMonitor, __in  HDC hdcMonitor, __in  LPRECT lprcMonitor, __in  LPARAM dwData) 
{ 

	std::vector<ALLMONITORINFO>& infoArray = *reinterpret_cast<std::vector<ALLMONITORINFO>* >(dwData); 

	ALLMONITORINFO monitorInfo; 

	monitorInfo.hMonitor = hMonitor; 

	//下面这句代码已经获取到了屏幕的分辨率，不管你有多少个屏幕都可以获取到，但是该分辨率是受缩放影响的。 

	monitorInfo.rect = *lprcMonitor; 

	infoArray.push_back(monitorInfo); 



	//这里是另一种获取屏幕分辨率的办法。 

	MONITORINFO monInfo; 

	monInfo.cbSize = sizeof(MONITORINFO); 

	//这个方法也是会受缩放影响，shit. 

	BOOL isGet = GetMonitorInfo(hMonitor, &monInfo); 

	if (isGet == TRUE) { 

		printf("rect wdith:%d,rect height:%d.\n", monInfo.rcMonitor.right - monInfo.rcMonitor.left, monInfo.rcMonitor.bottom - monInfo.rcMonitor.top);; 

	} 



	return TRUE; 

} 



int maintest() { 

	std::vector<ALLMONITORINFO> mInfo; 

	mInfo.clear(); 

	//get number of monitors 

	mInfo.reserve(GetSystemMetrics(SM_CMONITORS)); 

	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&mInfo)); 



	//通过枚举之后，显示器的信息就存储到了mInfo里了。 

	return 1; 

} 

#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>


void wchar2strstring(std::string & szDst, WCHAR * wchart)
{
	wchar_t * wtext = wchart;
	DWORD dwNmu = WideCharToMultiByte(CP_OEMCP, NULL, wtext, -1, NULL, 0, NULL, FALSE);
	char * psTest;
	psTest = new char[dwNmu];
	WideCharToMultiByte(CP_OEMCP, NULL, wtext, -1, psTest, dwNmu, NULL, FALSE);
	szDst = psTest;
	delete[]psTest;
}
void killProcessByName(const char *filename)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof (pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		std::string exefile;
		wchar2strstring(exefile, pEntry.szExeFile);
		printf("exe_name = %s\n", exefile.c_str());
		if (strcmp(exefile.c_str(), filename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD) pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}


void start_kill()
{
	//killProcessByName("peerconnection_desktop.exe");
	STARTUPINFO si = { sizeof(si) };
	//si.lpDesktop = (LPWSTR)"1234567890";
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWMAXIMIZED;
	PROCESS_INFORMATION pi = { 0 };
	//// C:/Users/Public/Nwt/cache/recv/DESKTOP-QL/JJ202111121500/WindowsNoEditor/Prj_ChengDu.exe
	////TCHAR szApp[MAX_PATH] = { _T("D:/Work/cmedia_server/webrtc_google/libmediasoupclient/build/test/Debug/test_mediasoupclient.exe  D:/Work/cmedia_server/webrtc_google/libmediasoupclient/build/test/Debug/client.cfg") };
	////char * szApp = "C:/Users/Public/Nwt/cache/recv/DESKTOP-QL/JJ202111121500/WindowsNoEditor/Prj_ChengDu.exe";
	TCHAR  szApp[MAX_PATH] = { L"D:/Work/cmedia_server/webrtc_google/src/out/test_vs2017_debug/peerconnection_desktop.exe" };
	//TCHAR szCmdLine[MAX_PATH] = {
	//	L"C:\\Windows\\System32\\rundll32.exe"
	//	L" D:\\Test.dll,TestFunc" // 注意前面的空格
	//};//CREATE_NO_WINDOW
	if (::CreateProcess(NULL, szApp, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}


	std::this_thread::sleep_for(std::chrono::seconds(3));
	killProcessByName("peerconnection_desktop.exe");
}
//int main()
//{
//	killProcessByName("notepad++.exe");
//	return 0;
//}
#include <Windows.h>
int main(int argc, char* argv[])
{

	
	//system("D:/Work/cmedia_server/webrtc_google/src/out/test_vs2017_debug/peerconnection_desktop.exe");
//	test_win();
	return 0;
	/*for (int i = 0; i < 10; ++i)
	{
		HDC desktopDc = GetDC(NULL);
		printf("hdc = %p\n", desktopDc);
	}
	get_display();
	maintest();
	system("pause");
	return 0;*/
	//get_display();
	/*test_promise();

	return 0;*/


	//ACCESS_MASK desired_access = DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW |
	//	DESKTOP_ENUMERATE | DESKTOP_HOOKCONTROL |
	//	DESKTOP_WRITEOBJECTS | DESKTOP_READOBJECTS |
	//	DESKTOP_SWITCHDESKTOP | GENERIC_WRITE;
	//HDESK hDesk = ::OpenDesktopW((LPWSTR)"1234567890", 0, FALSE, desired_access);
	////HDESK hDesk =  ::CreateDesktop(_T("123456789"), NULL, NULL, DF_ALLOWOTHERACCOUNTHOOK, GENERIC_ALL, NULL);
	//assert(hDesk);
	// Register signal SIGINT and signal handler.
	//test_win();
	//return 0;
	/*test_scroll();
	return 0;*/
	{}
	//HDESK hDesk1 = ::CreateDesktop((LPWSTR)"1234567890", NULL, NULL, DF_ALLOWOTHERACCOUNTHOOK, GENERIC_ALL, NULL);
	//assert(hDesk1);
	//HDESK hDesktopToSwitch = ::OpenDesktop((LPWSTR)"1234567890", DF_ALLOWOTHERACCOUNTHOOK, TRUE, GENERIC_ALL);
	//assert(hDesktopToSwitch);
	STARTUPINFO si = { sizeof(si) };
	//si.lpDesktop = (LPWSTR)"1234567890";
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWMAXIMIZED;
	//PROCESS_INFORMATION pi = { 0 };
	//// C:/Users/Public/Nwt/cache/recv/DESKTOP-QL/JJ202111121500/WindowsNoEditor/Prj_ChengDu.exe
	////TCHAR szApp[MAX_PATH] = { _T("D:/Work/cmedia_server/webrtc_google/libmediasoupclient/build/test/Debug/test_mediasoupclient.exe  D:/Work/cmedia_server/webrtc_google/libmediasoupclient/build/test/Debug/client.cfg") };
	////char * szApp = "C:/Users/Public/Nwt/cache/recv/DESKTOP-QL/JJ202111121500/WindowsNoEditor/Prj_ChengDu.exe";
	//TCHAR  szApp[MAX_PATH] = { L"D:/Work/cmedia_server/webrtc_google/src/out/test_vs_debug/peerconnection_desktop.exe" };
	//TCHAR szCmdLine[MAX_PATH] = {
	//	L"C:\\Windows\\System32\\rundll32.exe"
	//	L" D:\\Test.dll,TestFunc" // 注意前面的空格
	//};//CREATE_NO_WINDOW
	//if (::CreateProcess(NULL, szApp, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	//{
	//	CloseHandle(pi.hThread);
	//	CloseHandle(pi.hProcess);
	//}
	//Sleep(10);
	//HWND hWinHandle = ::FindWindow(NULL, L"WebRTC_DESKTOP");
//	system("D:/Work/cmedia_server/webrtc_google/src/out/test_vs_debug/peerconnection_desktop.exe");
	/*PROCESS_INFORMATION pi = { 0 };
	TCHAR  szApp[MAX_PATH] = { L"C:/Program Files/VideoLAN/VLC/vlc.exe  rtmp://127.0.0.1/live/test" };
	if (::CreateProcess(NULL, szApp, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{

	}*/
	/*int sum = 0;
	while (sum < 2)
	{
		printf("sleep 100 ... \n");
		++sum;
		::Sleep(100);
	}*/
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
	const char * envEnableAudio = "true";
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
