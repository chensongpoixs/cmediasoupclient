/***********************************************************************************************
						created: 		2019-05-10

						author:			chensong

						purpose:		robot_client
************************************************************************************************/

#include "crobot_client.h"
#include "clog.h"
#include "crobot_cfg.h"
//#include "clogin_mgr.h"
#include "cnet_msg_header.h"
//#include "cmatch_game_mgr.h"

namespace  chen {
	crobot_client	g_lan_client;



	crobot_client::crobot_client()
		:m_net_ptr(NULL)
//		, m_stoped(false)
		, m_max_client_session_num(0)
		, m_session_ptr(NULL)
//		, m_update_timer(0)
	{
	}
	crobot_client::~crobot_client()
	{
	}
	bool crobot_client::init(uint32 max_session)
	{
		m_net_ptr = cnet_mgr::construct();
		if (!m_net_ptr)
		{
			ERROR_EX_LOG("new cnet_mgr fail");
			return false;
		}
		m_max_client_session_num = max_session; 

		const uint32 recv_buf_size = g_cfg.get_uint32(ECI_WanInputBufSize);
		const uint32 send_buf_size = g_cfg.get_uint32(ECI_WanOutputBufSize);
		if (!m_net_ptr->init("client_device", m_max_client_session_num , 0, send_buf_size, recv_buf_size))
		{
			return false;
		}

		m_net_ptr->set_connect_callback(
			std::bind(&crobot_client::on_connect, this, std::placeholders::_1, std::placeholders::_2
				, std::placeholders::_3)
		);

		m_net_ptr->set_msg_callback(
			std::bind(&crobot_client::on_msg_receive, this, std::placeholders::_1)
		);

		m_net_ptr->set_disconnect_callback(
			std::bind(&crobot_client::on_disconnect, this, std::placeholders::_1)
		);

		m_session_ptr = new csession[m_max_client_session_num];
		if (!m_session_ptr)
		{
			ERROR_EX_LOG("new faild session max num =%lu", m_max_client_session_num);
			return false;
		}

		for (uint32 i = 0; i < m_max_client_session_num; ++i)
		{
			m_session_ptr[i].init(i);
		}
		return true;
	}
	void crobot_client::destroy()
	{
		if (m_net_ptr)
		{
			m_net_ptr->destroy();
			cnet_mgr::destroy(m_net_ptr);
		}

		if (m_session_ptr)
		{
			for (uint32_t i = 0; i < m_max_client_session_num; ++i)
			{
				m_session_ptr[i].destroy();
			}
			delete[]	m_session_ptr;
			m_session_ptr = NULL;
		}
	}
	bool crobot_client::startup()
	{
		if (!m_net_ptr->startup(1, "", 0))
		{
			return false;
		}
		
		return true;

	}
	void crobot_client::update(uint32 elapse)
	{
		m_net_ptr->process_msg();
		//g_login_mgr.update(elapse);
		// update session

		uint32_t max_login_num = g_cfg.get_uint32(ECI_RobotMaxConnectTick);

		uint32_t login_num = 0;

		for (uint32_t i = 0; i < m_max_client_session_num; ++i)
		{
			csession* session_ptr = &m_session_ptr[i];
			ELanSessionStatus status = session_ptr->get_status();
			//SYSTEM_LOG("status = %d", status);
			switch (status)
			{
			case ELSS_None:
				/*if (!g_login_mgr.is_full())
				{
					session_ptr->sub_reconnect_timer(elapse);
					if (session_ptr->get_reconnect_timer() < 0)
					{
						if (g_login_mgr.request_login("", "", i + g_cfg.get_uint32(ECI_RobotStartIndex)))
						{
							session_ptr->set_status(ELSS_None, ELSS_Logining);
						}
					}
				}*/
				session_ptr->set_status(ELSS_None, ELSS_Logined);
				break;
			case ELSS_Logining:
				break;
			case ELSS_Logined:
				if (login_num < max_login_num)
				{
					connect_to_gateway(i);
					//DEBUG_LOG("connect = %d", i);
					++login_num;
				//	session_ptr->set_status(ELSS_Gameing);
				}
				break;
			case ELSS_Connecting:
				break;
			case ELSS_Conencted:
			case ELSS_Loading:
				session_ptr->update(elapse);
				break;
			case ELSS_Gameing:
				session_ptr->update(elapse);
				break;
			case ELSS_Closed:
				session_ptr->set_status(ELSS_Closed, ELSS_None);
				//session_ptr->get_player().reset();
				session_ptr->set_reconnect_timer(10000);
				break;
			default:
				break;
			}
		}
	}
	void crobot_client::shutdown()
	{
		if (m_net_ptr)
		{
			m_net_ptr->shutdown();
		}
	}
	void crobot_client::on_connect(uint32 session_id, uint32 para, const char * buf)
	{
        NORMAL_LOG("session = %lu", session_id);
		const uint32 index = get_session_index(session_id);
		if (!index_valid(index))
		{
			ERROR_LOG("[%s] invalid session index, index = %u, session id = %u", __FUNCTION__, index, session_id);
			m_session_ptr[index].on_disconnect();
			return;
		}

		/*if (error_code != 0)
		{
			ERROR_LOG( "connect error, error code = %d,session_id=%d", error_code , session_id);
			m_session_ptr[index].on_disconnect();
			return;
		}*/

		ELanSessionStatus status = m_session_ptr[index].get_status();

		if (status == ELSS_Connecting)
		{
			if (!m_session_ptr[index].set_status(ELSS_Connecting, ELSS_Conencted))
			{
				ERROR_LOG("%s, set status to ELSS_LoginConencted failed. index=%d", __FUNCTION__, index);
				close(session_id);
				return;
			}
		}
		else
		{
			ERROR_LOG("[%s] invalid session status, index = %u, status=%d", __FUNCTION__, index, status);
			close(session_id);
			m_session_ptr[index].set_status(ELSS_Connecting, ELSS_Closed);
			return;
		}

		m_session_ptr[index].on_connect(session_id);
//		cmatch_game_mgr * match_mgr_ptr = m_session_ptr[index].get_player().get_match_game_mgr();
//		if (match_mgr_ptr)
//		{
////			match_mgr_ptr->request_player_join_match();
//		}
	}
	void crobot_client::on_msg_receive(cnet_msg &msg_ptr)
	{
	//	ERROR_EX_LOG("session = %lu,msg_id = %lu, size = %lu", session_id, msg_id, size);
		const uint32_t index = get_session_index(msg_ptr.get_session_id());
		csession* session_ptr = get_session(index, msg_ptr.get_session_id());

		if (!session_ptr)
		{
			ERROR_LOG("[%s] get session error, session_id = %u", __FUNCTION__, msg_ptr.get_session_id());
			return;
		}

		if (!session_ptr->is_connected())
		{
			ERROR_LOG("[%s] this session not connected!, index = %u, session_id = %u", __FUNCTION__, index, msg_ptr.get_session_id());
			return;
		}

		session_ptr->handle_msg(msg_ptr.get_msg_id(), msg_ptr.get_buf(), msg_ptr.get_size());
	}
	void crobot_client::on_disconnect(uint32 session_id)
	{
		ERROR_EX_LOG("session = %lu", session_id);
		const uint32_t index = get_session_index(session_id);
		// 注销连接
		if (!index_valid(index))
		{
			ERROR_LOG("[%s] invalid session index, index = %u, session id = %u", __FUNCTION__, index, session_id);
			return;
		}

		csession* session_ptr = &m_session_ptr[index];
		ELanSessionStatus status = session_ptr->get_status();
		switch (status)
		{
		case ELSS_Connecting:
			SYSTEM_LOG("connecting failed, index = %d", index);
			break;
		case ELSS_Conencted:
			SYSTEM_LOG("disconnect with login after connected, index = %d", index);
			break;
		case ELSS_Gameing:
			SYSTEM_LOG("disconnect with game when gaming, index = %d", index);
			break;
		default:
			ERROR_LOG("[%s] invalid session status %d, index = %u", __FUNCTION__, status, index);
			return;
		}
		session_ptr->on_disconnect();
	}
	bool crobot_client::connect_to_gateway(uint32 index)
	{
		const std::string& ip = g_cfg.get_string(ECI_WanIp);
		uint16 port = g_cfg.get_uint32(ECI_WanPort);
		if (!m_session_ptr[index].set_status(ELSS_Logined, ELSS_Connecting))
		{
			ERROR_LOG("%s, set status to ELSS_LoginConnecting failed. index=%d", __FUNCTION__, index + g_cfg.get_uint32(ECI_RobotStartIndex));
			return false;
		}

		if (!m_net_ptr->connect_to(index, ip.c_str(), port))
		{
			ERROR_LOG("[%s] connect error!, ip = %s, port = %d", __FUNCTION__, ip.c_str(), port);
			m_session_ptr[index].set_status(ELSS_Connecting, ELSS_Closed);
			return false;
		}
        SYSTEM_LOG("connect %s:%d, index=%d", ip.c_str(), port, index + g_cfg.get_uint32(ECI_RobotStartIndex));
        //m_net_ptr->connect_to(index, ip.c_str(), port);
		return true;
	}
	bool crobot_client::send_msg(uint32 session_id, uint16 msg_id, const void * p, uint32 size)
	{
		if (!m_net_ptr->send_msg(session_id, msg_id, p, size))
		{
			WARNING_LOG("send session_id = %lu", session_id);
		}
		return true;
	}

	bool crobot_client::send_msg(uint32 session_id, uint16 msg_id, const void * p, uint32 size, int32 extra)
	{
		if (!m_net_ptr->send_msg(session_id, msg_id, p, size, extra))
		{
			WARNING_LOG("send session_id = %lu", session_id);
		}
		return true;
	}

	csession* crobot_client::get_session(uint32 index)
	{
		if (!index_valid(index))
		{
			ERROR_LOG("[%s] invalid session index, index = %u", __FUNCTION__, index);
			return NULL;
		}

		return &(m_session_ptr[index]);
	}

	csession* crobot_client::get_session(uint32_t index, uint32_t session_id)
	{
		if (!index_valid(index))
		{
			ERROR_LOG("[%s] invalid session index, index = %u, session id = %u", __FUNCTION__, index, session_id);
			return NULL;
		}

		if (m_session_ptr[index].get_session_id() != session_id)
		{
			return NULL;
		}


		return &(m_session_ptr[index]);
	}

	void crobot_client::close(uint32 session_id)
	{
		m_net_ptr->close(session_id);
	}

	csession * crobot_client::get_used_session(int index)
	{
		if (index < 0)
		{
			for (uint32_t i = 0; i < m_max_client_session_num; ++i)
			{
				if (m_session_ptr[i].is_connected())
				{
					return &(m_session_ptr[i]);
				}
			}
		}
		else if ((uint32_t)index < m_max_client_session_num && m_session_ptr[index].is_connected())
		{
			return &(m_session_ptr[index]);
		}
		return NULL;
	}
} // namespace  chen