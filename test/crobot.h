/***********************************************************************************************
					created: 		2019-05-10

					author:			chensong

					purpose:		robot
************************************************************************************************/

#ifndef _C_ROBOT_H_
#define _C_ROBOT_H_

#include "cnoncopyable.h"
#include <thread>
#include <vector>
namespace chen {
	class crobot : private cnoncopyable
	{
	public:
		explicit crobot();
		~crobot();
	public:
		bool init(const char * log_path, const char *config_file);
		bool Loop();
		void destroy();
		static void stop();
	private:
		static void _on_signal(int s);
		void _hook_signals();
		void _unhook_signals();
	private:
		void _over_write_config();
		void _update_thread();
	private:
		std::vector<std::thread>							m_update_thread_ptr;
		static volatile bool								m_stoped;
	};

	extern crobot g_rotbot;
} //namespace chen 

#endif // !#define _C_ROBOT_H_