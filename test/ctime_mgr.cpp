/***********************************************************************************************
				created: 		2019-05-06

				author:			chensong

				purpose:		ctime_mgr
************************************************************************************************/

#include "ctime_mgr.h"
#include "ctime_api.h"

namespace chen {
	ctime_mgr g_timer_mgr;

	ctime_mgr::ctime_mgr()
		:m_timestamp_second(0)
	{
	}
	ctime_mgr::~ctime_mgr()
	{
	}
	bool ctime_mgr::init()
	{
		m_timestamp_second = static_cast<int>( ctime_base_api::get_gmt());
		return true;
	}
	void ctime_mgr::destroy()
	{
	}
	void ctime_mgr::update(uint32 elapse)
	{
		m_timestamp_second = static_cast<int>(ctime_base_api::get_gmt());
	}
} //namespace chen
