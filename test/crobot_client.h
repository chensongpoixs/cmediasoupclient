/***********************************************************************************************
				created: 		2019-05-10

				author:			chensong

				purpose:		robot_client
************************************************************************************************/


#ifndef _C_ROBOT_CLINET_H_
#define _C_ROBOT_CLINET_H_
#include "cnoncopyable.h"
#include "cnet_mgr.h"
#include "csession.h"
namespace chen {
	class crobot_client :private cnoncopyable
	{
	public:
		explicit crobot_client();
		~crobot_client();
	public:
		bool init(uint32 max_session);
		void destroy();
	public:
		bool startup();
	public:
		void update(uint32 elapse);
		void shutdown();
	public:
		void on_connect(uint32 session_id, uint32 para, const char* buf);
		void on_msg_receive(cnet_msg&msg_ptr);
		void on_disconnect(uint32 session_id);
	private:
		bool connect_to_gateway(uint32 index);
	public:
		bool send_msg(uint32 session_id, uint16 msg_id, const void *p, uint32 size);
		/************************************************************************/
		/* 指定服务器发送 extra                                                   */
		/************************************************************************/
		bool send_msg(uint32 session_id, uint16 msg_id, const void *p, uint32 size, int32 extra);
	public:
		csession* get_session(uint32 index);
		csession*  get_session(uint32_t index, uint32_t session_id);
	public:
		/**
		* 关闭指定连接
		* @param session_id	连接id
		*/
		void close(uint32 session_id);
	public:

		csession* get_used_session(int index);
	private:
		bool index_valid(uint32_t index) { return index < m_max_client_session_num; }
	private:
		cnet_mgr	*				m_net_ptr;
//		bool						m_stoped;
		uint32						m_max_client_session_num;
		csession*					m_session_ptr;
//		uint32						m_update_timer;


	};

	extern crobot_client	g_lan_client;
} // namespace chen

#endif // !#define _C_ROBOT_CLINET_H_