/***********************************************************************************************
						created: 		2019-05-10

						author:			chensong

						purpose:		robot_cfg
************************************************************************************************/


#ifndef _C_CFG_ROBOT_H_
#define _C_CFG_ROBOT_H_
#include "cnoncopyable.h"
#include "cconfig.h"
namespace chen {

	enum EConfigIndex
	{
		ECI_TimeZone = 0,
		ECI_TimeAdjust,
		//ECI_WanClientMaxNum,
		ECI_WanInputBufSize,
		ECI_WanOutputBufSize,
		ECI_WanIp,
		ECI_WanPort,
		ECI_RobotMax,						// 机械人的数量
		ECI_RobotStartIndex,			    //机器人起始值 
		ECI_RobotNamePre,					//机械人的名称
		ECI_RobotPassWord,                  //机器人的密码
		ECI_RobotRegion,                    //机器人区域
		ECI_MachineType,					// 机型
		ECI_RobotMaxConnectTick,			// 一次tick时间内登陆几个玩家
		ECI_LoginAddress,							//webIP地址
		ECI_MaxLoginRequest,
		ECI_Device_Id,
		ECI_Clinet_Verion,
		ECI_Beart_Heart, //心跳包的秒数

		ECI_TestCase,				// 测试用例
		ECI_OpenTestCase,			// 开启测试用例

		//friend_cricle
		ECI_Cricle_Friend_Address,    //friend_cricle 地址
		ECI_MaxCircle_Friend_Request,
		ECI_Match_Id,
		ECI_Player_Action_Timeout, //玩家操作延迟时间 0-14s
		ECI_Max,
	};

	class crobot_cfg :public cconfig
	{
	public:
		explicit	crobot_cfg();
		virtual	~crobot_cfg();
	public:
	    bool init(const char * file_name);
		void destroy();
	public:
		void show_cfg_to_title();
	};
	extern crobot_cfg	g_cfg;
} //namespace chen 

#endif // !#define _C_CFG_ROBOT_H_