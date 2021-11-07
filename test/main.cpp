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
void test_win()
{
	// 获取带标题栏和菜单栏即全屏像素大小
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);


	printf("width = %d, height = %d\n", width, height);
	// 睡眠5s，准备时间
	Sleep(5000);
	// 死循环
	while (1) 
	{
		// 移动到绝对位置右击
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_MOVE, 1170 * 65535 / width, 362 * 65535 / height, 0, 0 );
		Sleep(3500);

		// 按下'q'键
		keybd_event(81, 0, 0, 0);
		keybd_event(81, 0, KEYEVENTF_KEYUP, 0);
		Sleep(500);

		// 移动到绝对位置右击
		printf("-----\n");
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN  | MOUSEEVENTF_RIGHTUP , 679 * 65535 / width, 760 * 65535 / height, 0, 0 );
		Sleep(3500);

	}
	
}

#include <Windows.h>


#include<iostream>
#include<Windows.h>
#include<TlHelp32.h>
using namespace std;
int testmain(int argc, char* argv[])
{
	TCHAR c[]={L"cmd.exe"};	//定义字符串并初始化,c为8长度,最后结尾有'\0',定义一个字符为'x',  
	HANDLE handle;	 //定义CreateToolhelp32Snapshot系统快照句柄 
	HANDLE handle1;	 //定义要结束进程句柄 
	handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//获得系统快照句柄 
	PROCESSENTRY32 *info;	 //定义PROCESSENTRY32结构字指  

							 //PROCESSENTRY32  结构的 dwSize 成员设置成 sizeof(PROCESSENTRY32) 

	info=new PROCESSENTRY32;             

	info->dwSize=sizeof(PROCESSENTRY32); 

	//调用一次 Process32First 函数，从快照中获取进程列表 

	Process32First(handle,info); 

	//重复调用 Process32Next，直到函数返回 FALSE 为止 

	while(Process32Next(handle,info)!=FALSE) 
	{ 

		info->szExeFile;     //指向进程名字 

							 //比较字符串是否相同 
		if( wcscmp(c,info->szExeFile) == 0 ) 
		{ 
			//根据进程ID打开进程
			handle1=OpenProcess(PROCESS_TERMINATE,FALSE,info->th32ProcessID); 
			//结束进程 
			TerminateProcess(handle1,0); 
		} 
	}  
	//关闭句柄
	CloseHandle(handle); 
	CloseHandle(handle1);
	return 0;
}



//定义进程ID和窗口句柄关联结构体 
struct ProcessWindow 
{ 
	DWORD dwProcessId; 
	HWND hwndWindow; 
}; 

//定义回调函数
BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam) 
{ 
	ProcessWindow *pProcessWindow = (ProcessWindow *)lParam; 
	DWORD dwProcessId; 
	GetWindowThreadProcessId(hWnd, &dwProcessId); 
	// 判断是否是指定进程的主窗口
	if (pProcessWindow->dwProcessId == dwProcessId && IsWindowVisible(hWnd) && GetParent(hWnd) == NULL) 
	{ 
		pProcessWindow->hwndWindow = hWnd; 
		return FALSE; 
	} 
	return TRUE; 
}  

//... 
//根据进程ID找到窗口句柄并操作
//ProcessWindow procwin; 
//procwin.dwProcessId = info->th32ProcessID; //上一步遍历得到的进程ID
//procwin.hwndWindow = NULL;  
//
//// 查找主窗口
//EnumWindows(EnumWindowCallBack, (LPARAM)&procwin); 
////根据找到的窗口句柄显示窗口
//ShowWindow(procwin.hwndWindow,SW_SHOWNORMAL); 
//
////窗口置顶 
//SetForegroundWindow(procwin.hwndWindow);


//int mousemove(int x, int y) {
//	     ::SetCursorPos(x, y);
//	     mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
//	     return 0;
//	 }
//bool mouse_down(string titlename, int position_x, int position_y) {
//     HWND hd_desk = GetDesktopWindow();
// 
//     HWND hd = GetWindow(hd_desk, GW_CHILD);        //得到屏幕上第一个子窗口
//     char s[200] = { 0 };
//     std::cout << "enter in" << std::endl;
//     while (hd != NULL)                    //循环得到所有的子窗口
//     {
//         memset(s, 0, 200);
//         GetWindowText(hd, s, 200);
//         //GetClassName(hd, s, 200);
//         string b(&s[0], &s[strlen(s)]);
//         if (b ==titlename) {       //Notepad
//             RECT rect;
//             GetWindowRect(hd, &rect);
//             int w = rect.right - rect.left, h = rect.bottom - rect.top;
//             std::cout << "宽:" << w << " " << "高:" << h << std::endl;
//             std::cout << "rect.left：" << rect.left << " " << "rect.top：" << rect.top << std::endl;
//             //SetWindowPos(hd, HWND_TOPMOST, rect.left, rect.top,  w, h, NULL);
//             //int bool_break = TRUE;
//             int num = 0;
//             while (TRUE) {
//                 int mouse_x = rect.left + position_x;
//                 int mouse_y = rect.top + position_y;
//                 mousemove(mouse_x, mouse_y);
//                 num++;
//                 if (NULL == FindWindow(NULL, s)||num==10) {
//                     break;
//                 }
//             }
//         std::cout << "find it" << std::endl;
// 
//             
// 
// 
//             //模拟点击事件
// 
//             //mousemove(rect.left + 180, rect.top + 210 + 240);
//             //::SetCursorPos(lpPoint.x, lpPoint.y);
//             //SetWindowPos(hd, HWND_NOTOPMOST, rect.left, rect.top, w, h, NULL);
//             break;
//         }
//         hd = GetNextWindow(hd, GW_HWNDNEXT);
//     }
// }

//点击实现
//void test_message()
//{
//	
//
//
//}
int main(int argc, char* argv[])
{
	//test_win();
	// Register signal SIGINT and signal handler.
	signal(SIGINT, signalHandler);

	// Retrieve configuration from environment variables.
	//const char* envServerUrl    = std::getenv("SERVER_URL");
	//const char* envRoomId       = std::getenv("ROOM_ID");
	//const char* envEnableAudio  = std::getenv("ENABLE_AUDIO");
	const char* envUseSimulcast = std::getenv("USE_SIMULCAST");
	const char* envWebrtcDebug  = std::getenv("WEBRTC_DEBUG");
	//SERVER_URL=https://my.mediasoup-demo.org:4443 ROOM_ID=broadcaster build/broadcaster
	const char * envServerUrl = "http://169.254.119.31:8888";
	const char * envRoomId = "chensong";
	const char * envEnableAudio = "false";
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
