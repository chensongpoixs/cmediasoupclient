/***********************************************************************************************
			created: 		2019-05-13

			author:			chensong

			purpose:		msg_dipatch
************************************************************************************************/

#ifndef _C_MSG_DIPATCH_H_
#define _C_MSG_DIPATCH_H_
#include "cplayer.h"
#include "cnet_types.h"
#include <atomic>
#include "cmsg_base_id.h"
namespace chen {
	typedef void (cplayer::*handler_type)(const void* msg_ptr, uint32 msg_size);

#pragma pack(push, 4)

	struct cmsg_handler
	{
		std::string			msg_name;
		std::atomic<long>	handle_count;
		handler_type		handler;

		cmsg_handler() : msg_name(""), handle_count(0), handler(NULL) {}
	};
	class cmsg_dispatch :private cnoncopyable
	{
	public:
		explicit cmsg_dispatch();
		~cmsg_dispatch();
	public:

		bool init();
		void destroy();

		cmsg_handler* get_msg_handler(uint16 msg_id);

	private:
		void _register_msg_handler(uint16 msg_id, const std::string& msg_name, handler_type handler);

	private:
		cmsg_handler		m_msg_handler[Msg_Client_Max];
	};
 
#pragma pack(pop)
	extern cmsg_dispatch g_msg_dispatch;
} //namespace chen

#endif //!#define _C_MSG_DIPATCH_H_