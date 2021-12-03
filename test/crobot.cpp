/***********************************************************************************************
created: 		2019-05-10

author:			chensong

purpose:		robot
************************************************************************************************/

#include "crobot.h"
#include "ctime_mgr.h"
#include "clog.h"
#include "crobot_cfg.h"
#include "cconfig.h"
#include "crobot_client.h"
#include <signal.h>
#include "cmsg_dispatch.h"
#include "cgm_command.h"
#include <chrono>
#include "cnet_msg_header.h"
#include "cstr2digit.h"
#include "crandom.h"

namespace chen {
	crobot g_rotbot;
	const uint32_t TICK_TIME = 200;
	volatile bool crobot::m_stoped = false;
	crobot::crobot()
	{
	}
	crobot::~crobot()
	{
	}
	bool crobot::init(const char * log_path, const char * config_file)
	{
		printf("Log init ...\n");
		if (!LOG::init(log_path, "client_device", ".log", ELogName_DateTime, false, true))
		{
			return false;
		}
		//LOG::set_level(ELogLevel_Debug);
		if (!g_cfg.init(config_file))
		{
			return false;
		}

		ctime_base_api::set_time_zone(g_cfg.get_int32(ECI_TimeZone));
		ctime_base_api::set_time_adjust(g_cfg.get_int32(ECI_TimeAdjust));

		c_rand.init();
		_over_write_config();

		// 消息注册
		if (false == g_msg_dispatch.init())
		{
			ERROR_LOG("[%s] nmsg_dispatch init error!", __FUNCTION__);
			return false;
		}

		SYSTEM_LOG("message dispatch init success!");

		uint32 value = g_cfg.get_uint32(ECI_RobotMax);
		// nwan_server 初始化,注意先初始化再startup
		if (false == g_lan_client.init(value))
		{
			ERROR_LOG("[%s] client init error!", __FUNCTION__);
			return false;
		}
		SYSTEM_LOG("client init success!");

		// WAN启动 
		if (false == g_lan_client.startup())
		{
			ERROR_LOG("[%s] client start error!", __FUNCTION__);
			return false;
		}

		SYSTEM_LOG("client startup success!");

		// 初始化本类相关信息
		s_gm_command.init();

		// 启动update线程
		m_stoped = false;
		 
		m_update_thread_ptr.emplace_back(&crobot::_update_thread, this);
		SYSTEM_LOG("update thread init success!");

		return true;
	}
	bool crobot::Loop()
	{
		_hook_signals();
		char cmd[4096];

		while (!m_stoped)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			cmd[0] = 0;
			std::cin.getline(cmd, sizeof(cmd));
			//SYSTEM_LOG("cmd = %s", cmd);
			if (cmd[0] != 0 && cmd[0] != '\r' && cmd[0] != '\n')
			{
				//看第一个字段是否是数字
				int index = -1;
				int len = 0;
				{
					for (int i = 0; i < static_cast<int>(sizeof(cmd)); ++i)
					{
						if (cmd[i] >= '0' && cmd[i] <= '9')
						{
							++len;
						}
						else
						{
							break;
						}
					}
					if (len > 0 && len < 11)
					{
						char index_str[24] = {0};
						memcpy(index_str, cmd, len);
						ERROR_LOG("%s", index_str);
						index_str[len] = 0;
						index = atoi(index_str);
					}
				}
				//ERROR_LOG("%d, cmd = %s", index, cmd + len);
				csession* session_ptr = g_lan_client.get_used_session(index);
				if (session_ptr)
				{
					s_gm_command.parse_commands(cmd + len, session_ptr);
				}
				else
				{
					ERROR_LOG("session 0 no connected");
				}
			}
		}

		_hook_signals();
		return true;
	}
	void crobot::destroy()
	{
		for (std::thread &thread : m_update_thread_ptr)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
		m_update_thread_ptr.clear();
		SYSTEM_LOG("update thread close!");
		 


		// WAN关闭
		g_lan_client.shutdown();
		SYSTEM_LOG("client shutdown!");
		 

		// wan session 销毁
		g_lan_client.destroy();
		SYSTEM_LOG("client destroy!");

		// 消息派发
		g_msg_dispatch.destroy();
		SYSTEM_LOG("message dispatch destroy!");

		 

		// 配置文件退出
		g_cfg.destroy();
		SYSTEM_LOG("config destroy!");
		// log退出

		LOG::destroy();
	}
	void crobot::stop()
	{
		m_stoped = true;
	}

	void crobot::_on_signal(int s)
	{
		switch (s)
		{
		case SIGINT:
		case SIGTERM:
#ifdef _WINDOWS
		case SIGBREAK:
#endif
		{
			stop();
		}
		break;
		}

		signal(s, _on_signal);
	}
	void crobot::_hook_signals()
	{
		signal(SIGINT, _on_signal);
		signal(SIGTERM, _on_signal);
#ifdef _WINDOWS
		signal(SIGBREAK, _on_signal);
#endif
	}

	void crobot::_unhook_signals()
	{
		signal(SIGINT, 0);
		signal(SIGTERM, 0);
#ifdef _WINDOWS
		signal(SIGBREAK, 0);
#endif
	}
	void crobot::_over_write_config()
	{
		g_cfg.show_cfg_to_title();
	}
	void crobot::_update_thread()
	{
		using namespace std::chrono;

		uint32_t elapse = 0;
		steady_clock::time_point cur_time;
		steady_clock::time_point pre_time = steady_clock::now();
		steady_clock::duration dur;
		milliseconds ms;
       /* struct rusage self_ru, c_ru;*/
        long long show_cpu = 0;
		while (!m_stoped)
		{
		     
			cur_time = steady_clock::now();
			dur = cur_time - pre_time;
			ms = duration_cast<milliseconds>(dur);
			elapse = static_cast<uint32_t>(ms.count());
			pre_time = cur_time;
            g_timer_mgr.update(elapse);
			g_lan_client.update(elapse);

			 

			cur_time = steady_clock::now();

			dur = cur_time - pre_time;
			ms = duration_cast<milliseconds>(dur);
			elapse = static_cast<uint32_t>(ms.count());
           // ERROR_EX_LOG("elapse = %d, timer = %d", elapse , g_timer_mgr.get_second_timestamp());
			if (elapse < TICK_TIME)
			{
				std::this_thread::sleep_for(milliseconds(TICK_TIME - elapse));
			}
		}
	}
}//namespace chen 
