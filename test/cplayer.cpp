/***********************************************************************************************
		created: 		2019-05-12

		author:			chensong

		purpose:		palyer
************************************************************************************************/

#include "cplayer.h"
#include "clog.h"
//#include "GameClient.pb.h"
//#include "csend_msgc2s.h"
//#include "LS2GC.pb.h"
//#include "CS2GC.pb.h"
//#include "cshow.h"
#include "crobot_cfg.h"
//#include "cfriend_circle_table_mgr.h"
//#include "ctest_case_global_mgr.h"
//#include "cmatch_game_mgr.h"
#include "cgm_command.h"
namespace chen {
	cplayer::cplayer()
		:m_session_ptr(NULL)
		,m_session_index(0)
		,m_user_name("")
		, m_score(0)
		, m_token("")
		, m_web_user_id(0)
		, m_login_session_id(0)
	{
		//memset(m_mgrs, 0, sizeof(m_mgrs));
	}
	cplayer::~cplayer()
	{
	}
	bool cplayer::init(csession *lan_session_ptr, uint32 index)
	{
		m_session_ptr = lan_session_ptr;

	/*	m_mgrs[MGR_FIRENDCIRCLE] = new cfriend_circle_table_mgr(this);
		m_mgrs[MGR_MATCH_GAME] = new cmatch_game_mgr(this);
		for (int i = MGR_None; i < MGR_MAX; ++i)
		{
			if (m_mgrs[i])
			{
				m_mgrs[i]->init();
			}
		}
		*/
		m_session_index = index;
		{
			std::stringstream ss;
			if (g_cfg.get_int32(ECI_RobotMax) == 1 && g_cfg.get_int32(ECI_RobotStartIndex) < 0)
			{
				ss << g_cfg.get_string(ECI_RobotNamePre);
			}
			else
			{

				ss << g_cfg.get_string(ECI_RobotNamePre);
				ss << g_cfg.get_uint32(ECI_RobotStartIndex) + m_session_index;
			}
		//	m_account_name = ss.str();
		}
		return true;

	//	m_session_index = index;
		return true;
	}
	void cplayer::destroy()
	{
		/*for (int i = MGR_None; i < MGR_MAX; ++i)
		{
			if (m_mgrs[i])
			{
				m_mgrs[i]->destroy();
				delete m_mgrs[i];
				m_mgrs[i] = NULL;
			}
		}*/
		m_session_index = 0;
		//m_guid = 0;
		//m_index = 0;
		//m_account_name.clear();
		//m_remain_ms = 0;
	}

	void cplayer::update(uint32 msleep)
	{
		/*ELanSessionStatus status = m_lan_session_ptr->get_status();
		if (status == ELSS_Conencted || status == ELSS_Loading)
		{
			if (m_remain_ms == 0)
			{
				m_remain_ms = sncfg.get_uint32(ECI_LoginTime);

				MC2S_EnterGame request;
				request.set_id(m_account_id);
				request.set_key(m_token);
				request.set_world_id(sncfg.get_int32(ECI_WorldId));
				request.set_channel(sncfg.get_int32(ECI_Channel));
				request.set_publisher(sncfg.get_int32(ECI_Publisher));
				request.set_machine_type(sncfg.get_string(ECI_MachineType));
				request.set_uuid(sncfg.get_string(ECI_UUID));
				if (!send_msg(C2S_EnterGame, request))
				{
					ERROR_LOG("%s, send msg MC2S_EnterGame failed, index=%u", m_index);
					disconnect();
				}
				else
				{
					DEBUG_LOG("send MC2S_EnterGame, index=%u", m_index);
				}
			}
			m_remain_ms -= elapse;
			if (m_remain_ms <= 0)
			{
				m_remain_ms = -1;
				disconnect();
			}
		}
		else if (status == ELSS_Gameing)*/
		{
			
			/*for (int32 i = MGR_None; i < MGR_MAX; ++i)
			{
				if (m_mgrs[i])
				{
					m_mgrs[i]->update(msleep);
				}
			}*/
		}
	}

	void cplayer::reset()
	{
		/*for (int i = MGR_None; i < MGR_MAX; ++i)
		{
			if (m_mgrs[i])
			{
				m_mgrs[i]->reset();
			}
		}*/
		m_session_index = 0;
		//m_guid = 0;
		//m_account_id = 0;
		//m_remain_ms = 0;
		//m_heart_beat = 0;
		//m_token.clear();
	}

	void cplayer::on_logined(const std::string &user_name, int32 score, const std::string& token, int32 uid, uint32 login_session_id)
	{
		m_user_name = user_name;
		m_score = score;
		m_token = token;
		m_web_user_id = uid;
		m_login_session_id = login_session_id;
		//g_test_case_mgr.insert_player_id(m_web_user_id);
		DEBUG_LOG("user_name =%s, score = %lu, token = %s, uid = %d", user_name.c_str(), score, token.c_str(), uid);
	}

	bool cplayer::send_msg(uint16_t msg_id, const ::google::protobuf::MessageLite& msg) const
	{
		return m_session_ptr->send_msg(msg_id, msg);
	}
    bool cplayer::send_msg(uint16 msg_id, const ::google::protobuf::MessageLite& msg, int32 match_id)
    {
	    return m_session_ptr->send_msg(msg_id, msg, match_id);
    }

	bool cplayer::send_gm(const char * buf) const
	{
		s_gm_command.parse_commands(buf, m_session_ptr);
		return true;
	}

	void cplayer::on_msg(uint16_t msg_id, int32 result)
	{
		/*if (m_mgrs[MGR_TestCase])
		{
			get_test_case_mgr()->on_msg(msg_id, result);
		}*/
	}


}//namespace chen