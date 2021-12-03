/***********************************************************************************************
					created: 		2019-05-10

					author:			chensong

					purpose:		网络层
************************************************************************************************/


#ifndef _C_SESSION_H_
#define _C_SESSION_H_
#include "cnoncopyable.h"
#include "cnet_types.h"
#include "cplayer.h"
#include <atomic>

namespace chen {
	enum ELanSessionStatus
	{
		ELSS_None = 0,
		ELSS_Logining,		//登录中
		ELSS_Logined,		//已登录
		ELSS_Connecting,	//正在链接游戏服务器
		ELSS_Conencted,		//已链接游戏服务器
		ELSS_Loading,		//从游戏服务器加载数据
		ELSS_Gameing,		//开始游戏
		ELSS_Closed,
		ELSS_Max,
	};
	class csession : private cnoncopyable
	{
	private:
		static const uint32 INVALID_SESSION_ID = 0XFFFFFFFF;
	public:
		explicit csession();
		~csession();
	public:
		bool init(uint32 index);
		void update(uint32 elapse);
		void destroy();
		
	public:
		bool send_msg(uint16_t msg_id, const void* pMsgPtr, uint32 size);
		bool send_msg(uint16_t msg_id, const ::google::protobuf::MessageLite& msg);
		bool send_msg(uint16 msg_id, const void* pMsgPtr, uint32 size, int32 extra);
		bool send_msg(uint16_t msg_id, const ::google::protobuf::MessageLite& msg, int32 extra);
	public:
		bool is_used();
		bool set_status(ELanSessionStatus old_stauts, ELanSessionStatus new_stauts) { return m_status.compare_exchange_strong(old_stauts, new_stauts); }

		//void set_status(ELanSessionStatus status);
		uint32_t	get_session_id() const { return m_session_id; }
		ELanSessionStatus get_status() const { return m_status.load(); }
		void on_connect(uint32 session_id) { m_session_id = session_id;	}
		bool   is_connected() const { return m_session_id != INVALID_SESSION_ID; }
		void  on_disconnect();
	public:
//		cplayer &get_player() { return m_player; }

		//cplayer &get_player() { return m_player; }
       // const cplayer &get_player() const { return m_player; }
		void set_reconnect_timer(int value) { m_reconnect_timer = value; }
		void sub_reconnect_timer(int value) { m_reconnect_timer -= value; }
		int get_reconnect_timer() const { return m_reconnect_timer; }

	public:
		void handle_msg(uint16_t msg_id, const void* msg_ptr, uint32_t msg_size);

	private:
	     volatile	uint32						m_session_id;
		std::atomic<ELanSessionStatus>			m_status; // status
		//cplayer									m_player; // player
		int32									m_reconnect_timer;
		int32									m_timestamp_second;
		//uint64									m_count_beart_head;
	};

}//namespace chen


#endif // !#define _C_SESSION_H_