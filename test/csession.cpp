/***********************************************************************************************
					created: 		2019-05-10

					author:			chensong

					purpose:		网络层
************************************************************************************************/


#include "csession.h"
#include "crobot_client.h"
#include "cmsg_dispatch.h"
#include "clog.h"
//#include "csend_msgc2s.h"
#include "crobot_cfg.h"
#include "ctime_mgr.h"
namespace chen {
	csession::csession()
		: m_session_id(0)
		, m_status(ELSS_None)
		/*, m_player(NULL)*/
		, m_reconnect_timer(0)
		, m_timestamp_second(0)
		/*, m_count_beart_head(0)*/
	{
	}
	csession::~csession()
	{
	}
	bool csession::init(uint32 index)
	{
		m_session_id = INVALID_SESSION_ID;
		/*m_player = new cplayer();
		if (!m_player)
		{
			ERROR_EX_LOG(" new player failed ");
			return false;
		}*/
		return true; //m_player.init(this, index);
		
		//return true;
	}
	void csession::destroy()
	{
	}
	bool csession::send_msg(uint16_t msg_id, const void * pMsgPtr, uint32 size)
	{
		return g_lan_client.send_msg(m_session_id, msg_id, pMsgPtr, size);
	}

	bool csession::send_msg(uint16_t msg_id, const ::google::protobuf::MessageLite& msg)
	{
		return g_lan_client.send_msg(m_session_id, msg_id, msg.SerializeAsString().c_str(), msg.ByteSize());
	}

	bool csession::send_msg(uint16 msg_id, const void * pMsgPtr, uint32 size, int32 extra)
	{
		return g_lan_client.send_msg(m_session_id, msg_id, pMsgPtr, size, extra);
	}

	bool csession::send_msg(uint16_t msg_id, const ::google::protobuf::MessageLite& msg, int32 extra)
	{
		return g_lan_client.send_msg(m_session_id, msg_id, msg.SerializeAsString().c_str(), msg.ByteSize(), extra);
	}

	void csession::update(uint32 elapse)
	{
		m_timestamp_second += elapse;
		if (m_timestamp_second > (g_cfg.get_uint32(ECI_Beart_Heart) * 150))
		{
			//++m_count_beart_head;
			//ERROR_EX_LOG("m_count_beart_head = %llu", m_count_beart_head);
			//ERROR_EX_LOG("timstamp = %d, beart_heart = %lu, timer = %d", m_timestamp_second, g_cfg.get_uint32(ECI_Beart_Heart), g_timer_mgr.get_second_timestamp());
			//ccmd_param param;
			//csend_msgc2s::send_beatheart(param, this);
			uint16_t msg_id = 101;
			char  pMsgPtr[1024] = "hello world !!!";
			
			send_msg(msg_id, (const void *)pMsgPtr, sizeof(pMsgPtr));
			NORMAL_LOG("send msgid = %u, ^_^", msg_id);
			m_timestamp_second = m_timestamp_second - (g_cfg.get_uint32(ECI_Beart_Heart) * 150);
		}
		//m_player.update(elapse);
	}
	bool csession::is_used()
	{
		return true;
	}

	void csession::on_disconnect()
	{
		m_session_id = INVALID_SESSION_ID;
		m_status = ELSS_Closed;
		//m_crypto_flag = false;
	}

	void csession::handle_msg(uint16_t msg_id, const void * msg_ptr, uint32_t msg_size)
	{
		cmsg_handler* handler_ptr = g_msg_dispatch.get_msg_handler(msg_id);

		if (!handler_ptr)
		{
			ERROR_LOG("[%s] get msg handler_ptr error, msg_id = %u", __FUNCTION__, msg_id);
		}
		else if (!handler_ptr->handler)
		{
			ERROR_LOG("[%s] get msg handler_ptr->handler error, msg_id = %u", __FUNCTION__, msg_id);
		}
		else
		{
			++handler_ptr->handle_count;

			/*if (msg_id != S2C_UpdatePlayer && msg_id != S2C_HeartBeat)
			{
				DEBUG_LOG("recv %s, size = %u", handler_ptr->msg_name.c_str(), msg_size);
			}

			decrypt(msg_ptr, const_cast<void*>(msg_ptr), msg_size);*/
			//DEBUG_LOG("msg_id=%u", pMsgPtr->msg_id);
			//DEBUG_LOG("src_msg=%s", wutil::get_hex_str(pMsgPtr->buffer, pMsgPtr->size).c_str());
			//DEBUG_LOG("dst_msg=%s", wutil::get_hex_str(g_robot_msg_helper.get_crypto_buf(), pMsgPtr->size).c_str());
			//(m_player.*(handler_ptr->handler))(msg_ptr, msg_size);
		}
	}

	//void csession::set_status(ELanSessionStatus status)
	//{
	//	m_status = status;
	//}

}//namespace chen