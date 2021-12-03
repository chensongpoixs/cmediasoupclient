/***********************************************************************************************
					created: 		2019-05-13

					author:			chensong

					purpose:		msg_dipatch
************************************************************************************************/

#include "cmsg_dispatch.h"
#include "clog.h"
//#include "GameClient.pb.h"
//#include "LS2GC.pb.h"
//#include "CS2GC.pb.h"
//#include "MS2GC.pb.h"
namespace chen {
	cmsg_dispatch::cmsg_dispatch()
	{
	}
	cmsg_dispatch::~cmsg_dispatch()
	{
	}
	bool cmsg_dispatch::init()
	{
		//_register_msg_handler(0, "S2C_Code", &cplayer::handle_code);
        return true;
	}
	void cmsg_dispatch::destroy()
	{
	}
	cmsg_handler * cmsg_dispatch::get_msg_handler(uint16 msg_id)
	{
		if (static_cast<int> (msg_id) >= Msg_Client_Max)
		{
			return NULL;
		}

		return &(m_msg_handler[msg_id]);
	}
	void cmsg_dispatch::_register_msg_handler(uint16 msg_id, const std::string & msg_name, handler_type handler)
	{
		if (static_cast<int> (msg_id) >= Msg_Client_Max || m_msg_handler[msg_id].handler)
		{
			ERROR_LOG("[%s] register msg error, msg_id = %u, msg_name = %s", __FUNCTION__, msg_id, msg_name.c_str());
			return;
		}

		m_msg_handler[msg_id].msg_name = msg_name;//   数据统计
		m_msg_handler[msg_id].handle_count = 0;
		m_msg_handler[msg_id].handler = handler;
	}


	cmsg_dispatch g_msg_dispatch;
} //namespace chen