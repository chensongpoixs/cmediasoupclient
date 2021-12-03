/***********************************************************************************************
					created: 		2019-05-13

					author:			chensong

					purpose:		gm_command
************************************************************************************************/

#include "cgm_command.h"
#include "clog.h"
//#include "csend_msgc2s.h"
//#include "cfriend_circle_table_mgr.h"
namespace chen {
	cgm_command::cgm_command()
	{
	}
	cgm_command::~cgm_command()
	{
	}
	bool cgm_command::init()
	{
		//using namespace csend_msgc2s;
		//_register_command("help", &help, "[cmd]");

		return true;
	}
	void cgm_command::destroy()
	{
		m_gm_commands.clear();
	}
	void cgm_command::parse_commands(const char * text, csession * session_ptr)
	{
		if (text == NULL || text[0] == 0)
		{
			return;
		}
		SYSTEM_LOG("[commands]cmd = %s", text);
		ccmd_param params;
		params.parse_cmd(text);
		
		if (_execute_command(params.get_cmd(), params, session_ptr))
		{
			NORMAL_LOG("gm command: %s" , text);
		}
	}
	void cgm_command::print_command(const std::string & cmd) const
	{
		for (NCOMMAND_MAP::const_iterator cit = m_gm_commands.begin(); cit != m_gm_commands.end(); ++cit)
		{
			if (cmd.empty() || cit->second.name.find(cmd) != std::string::npos)
			{
				NORMAL_LOG("%s, %s", cit->second.name.c_str() , cit->second.desc.c_str());
			}
		}
	}
	void cgm_command::_register_command(const std::string & cmd_name, gm_cmd_handler_type handler, const std::string & desc)
	{
		ncommand cmd(cmd_name, handler, desc);

		if (false == m_gm_commands.insert(std::make_pair(cmd_name, cmd)).second)
		{
			ERROR_LOG("%s, insert command error");
			return;
		}
	}
	bool cgm_command::_execute_command(const std::string & cmd_name, const ccmd_param & param, csession * session_ptr)
	{
		//SYSTEM_LOG("find name = %s", cmd_name.c_str());
		NCOMMAND_MAP::const_iterator ncmd = m_gm_commands.find(cmd_name);

		if (ncmd == m_gm_commands.end())
		{
			ERROR_LOG(  "unknown command ");
			return false;
		}

		if (!(ncmd->second.handler(param, session_ptr)))
		{
			ERROR_LOG( "execute failed " );
			return false;
		}

		return true;
	}
}//namespace chen 