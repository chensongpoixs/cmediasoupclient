/***********************************************************************************************
				created: 		2019-05-15

				author:			chensong

				purpose:		msg_header
************************************************************************************************/


#ifndef _C_MSG_HEADER_H_
#define _C_MSG_HEADER_H_
#include "cnoncopyable.h"

namespace chen {
	class cmsg_header : private cnoncopyable
	{
	public:
		explicit cmsg_header();
		~cmsg_header();
	private:
	};
}//namespace chen

#endif // !#define _C_MSG_HEADER_H_