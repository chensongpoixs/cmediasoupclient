/***********************************************************************************************
				created: 		2019-05-13

				author:			chensong

				purpose:		gm_command
************************************************************************************************/

#ifndef _C_GM_COMMAND_H_
#define _C_GM_COMMAND_H_
#include <unordered_map>
#include "cnoncopyable.h"
#include "csession.h"
#include "ccmd_param.h"
#include "csingleton.h"
namespace chen {
	class cgm_command :private cnoncopyable
	{
	
	public:
		typedef bool(*gm_cmd_handler_type)(const ccmd_param&, csession*);

		struct ncommand
		{
			std::string					name;			// 命令名
			gm_cmd_handler_type			handler;		// 函数指针
			std::string					desc;			// 注释
			ncommand(const std::string& cmd_name, gm_cmd_handler_type MsgHandler, const std::string& _desc) :
				name(cmd_name), handler(MsgHandler), desc(_desc) {}
		};
	private:
		typedef std::unordered_map<std::string, ncommand>	NCOMMAND_MAP;
	public:
		explicit cgm_command();
		~cgm_command();
	public:
		bool init();
		void destroy();
	public:
		void parse_commands(const char* text, csession* session_ptr);

		void print_command(const std::string& cmd) const;

	private:
		void _register_command(const std::string& cmd_name, gm_cmd_handler_type handler, const std::string& desc);
		bool _execute_command(const std::string& cmd_name, const ccmd_param& param, csession* session_ptr);

	private:
		NCOMMAND_MAP									m_gm_commands;
	};

#define s_gm_command   csingleton<cgm_command>::get_instance()
}//namespace chen 

#endif // !#define _C_GM_COMMAND_H_