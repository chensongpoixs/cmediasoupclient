/***********************************************************************************************
created: 		2019-09-12

author:			chensong

purpose:		cmsg_parse
************************************************************************************************/
#ifndef _C_MSG_PARSE_H_
#define _C_MSG_PARSE_H_

#include <google/protobuf/message_lite.h>

#include "clog.h"
namespace chen {
#define PRASE_SERVER_MSG(MSG_PTR, MSG_ID, MSG_SIZE) \
	M##MSG_ID msg; \
	if (false == msg.ParsePartialFromArray(MSG_PTR, MSG_SIZE)) \
	{ ERROR_EX_LOG("ParsePartialFromArray error, msg id = %s" ,  #MSG_ID); return; }

#define PRASE_CLIENT_MSG(MSG_PTR, MSG_ID, MSG_SIZE) \
	M##MSG_ID msg; \
	if (false == msg.ParsePartialFromArray(MSG_PTR, MSG_SIZE)) \
	{ ERROR_EX_LOG ( "ParsePartialFromArray error, msg id = %s" , #MSG_ID); return; }

#define PRASE_OLD_CLIENT_MSG(MSG_PTR, MSG_DATA, MSG_SIZE) \
	MSG_DATA msg; \
	if (false == msg.ParseFromArray(MSG_PTR, MSG_SIZE)) \
	{ ERROR_EX_LOG ( "ParseFromArray error, msg id = %s" , #MSG_DATA); return; }

} //namespace chen


#endif // !#define _C_MSG_PARSE_H_