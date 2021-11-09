#include "cwebsocket_mgr.h"

namespace webrtc
{


	class WebSocketCallback :public wsclient::WebSocketCallback
	{
	public:
		WebSocketCallback(wsclient::WebSocket*_ws)
			:ws(_ws)
		{
		}
		void OnMessage(const std::string& message) {
			printf("RX: %s\n",message.c_str());
			if (message == "world") 
				ws->close();
		}

		void OnMessage(const std::vector<uint8_t>& message) {
		}

		wsclient::WebSocket* ws;
	};


	cwebsocket_mgr g_websocket_mgr;
	cwebsocket_mgr::cwebsocket_mgr()
		:m_stoped(true)
		, m_status(CWEBSOCKET_INIT)
		, m_ws(nullptr)
	{

	}
	cwebsocket_mgr::~cwebsocket_mgr()
	{
	}


	bool cwebsocket_mgr::init(std::string ws_url, std::string origin)
	{
		if (!m_stoped)
		{
			return false;
		}
		m_stoped.store(false);
		m_status.store(CWEBSOCKET_CONNECTING);
#ifdef _WIN32
		INT rc;
		WSADATA wsaData;

		rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (rc) 
		{
			printf("WSAStartup Failed.\n");
			return false;
		}
#endif

		m_ws = wsclient::WebSocket::from_url(ws_url, origin);
		//WebSocketCallback callback(m_ws);

		assert(m_ws);
		if (m_ws)
		{
			m_status.store(CWEBSOCKET_CONNECTED);
		}

	
		//ws->send("goodbye");
		//ws->send("hello");
		


		return true;
	}
	void cwebsocket_mgr::start()
	{
		if (m_status != CWEBSOCKET_CONNECTED || m_stoped || !m_ws)
		{
			assert(-1);
		}

		m_thread = std::thread(&cwebsocket_mgr::_work_thread, this);
		m_status.store(CWEBSOCKET_MESSAGE);
	}
	void cwebsocket_mgr::destroy()
	{
		m_stoped.store(true);
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}
	void cwebsocket_mgr::_work_thread()
	{
		WebSocketCallback callback(m_ws);
		while (!m_stoped && m_ws->getReadyState() != wsclient::WebSocket::CLOSED) 
		{
			m_ws->poll();
			m_ws->dispatch(callback);
		}
		if (m_ws)
		{
			delete m_ws;
			m_ws = nullptr;
		}

#ifdef _WIN32
		WSACleanup();
#endif
	}
}