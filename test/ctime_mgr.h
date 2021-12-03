/***********************************************************************************************
			created: 		2019-05-06

			author:			chensong

			purpose:		ctime_mgr
************************************************************************************************/


#ifndef _C_TIME_MGR_H_
#define _C_TIME_MGR_H_
#include "cnoncopyable.h"
#include "ctime_api.h"
#include "cnet_types.h"
namespace chen {
	class ctime_mgr :private cnoncopyable
	{
	public:
		explicit ctime_mgr();
		~ctime_mgr();
	public:
		bool init();
		void destroy();
	public:
		int get_second_timestamp() const { return m_timestamp_second; }
	public:
		void update(uint32 elapse);
	private:
		int	m_timestamp_second;							// 当前时刻
	};
	extern ctime_mgr g_timer_mgr;
}//namespace chen 

#endif //!#define _C_TIME_MGR_H_