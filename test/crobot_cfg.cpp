/***********************************************************************************************
							created: 		2019-05-10

							author:			chensong

							purpose:		robot_cfg
************************************************************************************************/
#include "crobot_cfg.h"
//#include "build_version.h"

namespace chen {
	crobot_cfg	g_cfg;
	crobot_cfg::crobot_cfg()
	{
	}

	crobot_cfg::~crobot_cfg()
	{
	}

	bool crobot_cfg::init( const char * file_name)
	{
		if (!cconfig::init(ECI_Max, file_name))
		{
			return false;
		}
		// 默认值
		set_int32(ECI_TimeZone, "time_zone", 8);
		set_int32(ECI_TimeAdjust, "time_adjust", 0);
		//Wan
	//	set_uint32(ECI_WanClientMaxNum, "wan_client_max_num", 1);
		//单位 MB
		set_uint32(ECI_WanInputBufSize, "wan_input_buf_size", 128 * 1024);//5242880
		//单位 MB
		set_uint32(ECI_WanOutputBufSize, "wan_output_buf_size", 70 * 1024);

		set_string(ECI_WanIp, "wan_ip", "127.0.0.1");
		set_uint32(ECI_WanPort, "wan_port", 8001);
		set_uint32(ECI_RobotMax, "robot_max", 1);
		set_uint32(ECI_RobotStartIndex, "robot_start_index", 0);
		set_string(ECI_RobotNamePre, "robot_name_prefix", "robot");
		set_string(ECI_RobotPassWord, "robot_password", "123456789");
		set_uint32(ECI_RobotRegion, "robot_region", 1);
		set_uint32(ECI_RobotMaxConnectTick, "robot_connect_tick", 1);
		set_string(ECI_MachineType, "machine_type", "pc");
		set_string(ECI_LoginAddress, "login_address", "dev.cpglive.com/index.php/user/");
		set_uint32(ECI_MaxLoginRequest, "max_login_request", 100);
		set_string(ECI_Device_Id, "device_id", "FA5434C0-6643-47BF-9DC7-");
		set_uint32(ECI_Clinet_Verion, "client_version", 10);

		// 计时器 单位秒
		set_uint32(ECI_Beart_Heart, "beart_heart", 30);

		// 测试案例
		set_string(ECI_TestCase, "test_case", "../../test_case.tab");
		set_uint32(ECI_OpenTestCase, "open_test_case", 0);

		set_string(ECI_Cricle_Friend_Address, "cricle_friend_address", "pydev.cpg.cn/v3/");//http://pydev.cpg.cn/v3/friends/store
		
		set_uint32(ECI_MaxCircle_Friend_Request, "max_circle_friend_request", 100);
		set_uint32(ECI_Match_Id, "match_id", 232323);
		set_uint32(ECI_Player_Action_Timeout, "player_action_timeout", 5);
		//配置表的值
		load_cfg_file();

		return true;
	}

	void crobot_cfg::destroy()
	{
		cconfig::destroy();
	}

	void crobot_cfg::show_cfg_to_title()
	{
#if defined(_MSC_VER)
		//show config
		/*char buf[2048];

		sprintf(buf, "git branch_name:%s version:%d, robot:[%u, %u]", BUILD_GIT_BRANCH_NAME, BUILD_GIT_REVERSION
			, g_cfg.get_uint32(ECI_RobotStartIndex), g_cfg.get_uint32(ECI_RobotMax));
		SetConsoleTitleA(buf);*/
#endif
	}

}//namespace chen