/***********************************************************************************************
			created: 		2019-05-25

			author:			chensong

			purpose:		player_headler
************************************************************************************************/

#include "cplayer.h"
#include "clog.h"
//#include "GameClient.pb.h"
//#include "csend_msgc2s.h"
//#include "LS2GC.pb.h"
//#include "CS2GC.pb.h"
//#include "MS2GC.pb.h"
//#include "cshow.h"
#include "crobot_cfg.h"
//#include "cfriend_circle_table_mgr.h"
//#include "cpoker_util.h"
//#include "cmatch_game_mgr.h"
#include "ctime_const.h"
#include "ctime_api.h"
#include "cmsg_parse.h"
namespace chen {

	void cplayer::handle_code(const void * packet, uint32 msg_size)
	{
	}
//	void cplayer::handle_announce(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, GameClient::Announce, msg_size);
////		GameClient::Announce msg;
////		if (msg.ParseFromArray(packet, msg_size))
//		{
//			//matchController_.modify_friend_circle_tableinfo(msg);
//			DEBUG_LOG("ios test version = %s, android  version = %lu, ios update url = %s, android update url = %s", msg.ioslastestversion().c_str(), msg.androidlastestversion(), msg.iosupdateurl().c_str(), msg.androidupdateurl().c_str());
//			if(g_cfg.get_int32(ECI_ScanLogin) == 0)
//            {
//                ccmd_param cmd_param;
//                cmd_param.set_cmd("login");
//                /************************************************************************/
//                /* msg.set_userid(23) ;
//                msg.set_username("visitor_5cdfe8f7713e9");
//                msg.set_mobile("15850774503");
//                msg.set_token("245b8328aa0afe91c321c714bd22daa3b81f6934");
//                msg.set_clientversion(2);
//                msg.set_language(1);
//                msg.set_sessionid(0);
//                msg.set_device_id("FA5434C0-6643-47BF-9DC7-0");                                                                     */
//                /************************************************************************/
//                cmd_param.push_param(std::to_string(m_web_user_id));
//                cmd_param.push_param(g_cfg.get_string(ECI_RobotNamePre) + std::to_string(m_login_session_id));
//                cmd_param.push_param("15850774503");
//                cmd_param.push_param(m_token);
//                cmd_param.push_param(std::to_string(g_cfg.get_uint32(ECI_Clinet_Verion)));  //新的版本 大等于10
//                cmd_param.push_param("1");
//                cmd_param.push_param("555");
//                cmd_param.push_param(g_cfg.get_string(ECI_Device_Id)+ std::to_string(m_login_session_id));
//                csend_msgc2s::send_login(cmd_param, m_session_ptr);
//            }
//		}
//	}
//	void cplayer::handle_breatheart(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, GameClient::BeatHeart, msg_size);
//		{
//			//matchController_.modify_friend_circle_tableinfo(msg);
//			//DEBUG_LOG("S2C -> handle_breatheart");
//			//ccmd_param cmd_param;
//			//cmd_param.set_cmd("breatheart");
//			//csend_msgc2s::send_beatheart(cmd_param, m_session_ptr);
//		}
//	}
//	void cplayer::handle_login(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, LS2GC::LoginRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("login error result = %d", msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("S2C -> Login ok, user_id = %d, btimeout = %d, sessionid = %d", msg.userid(), msg.btimeout(), msg.sessionid());
//        }
//	}
//	void cplayer::handle_match_list(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, CS2GC::MatchListRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("match error result = %d", msg.result());
//        }
//        else
//        {
//            get_match_game_mgr()->add_match(msg);
//            DEBUG_LOG("[CS2GC::MatchListRS][size = %d][msg_size = %d]match info gameuserid = %d, requestreason = %d", msg.matchlist_size(), msg_size, msg.gameuserid(), msg.requestreason());
//            for (int32 i = 0; i < msg.matchlist_size(); ++i)
//            {
//                cshow::show(msg.matchlist(i));
//            }
//        }
//	}
//	void cplayer::handle_player_joinlist(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, CS2GC::MatchPlayerJoinedListRS, msg_size);
//        get_match_game_mgr()->player_join_list_match(msg);
//        DEBUG_LOG("CS2GC::MatchPlayerJoinedListRS player_id = %u, matchlistsize = %d", msg.gameuserid(), msg.matchlist_size());
//
//        for (int32 i = 0; i < msg.matchlist_size(); ++i)
//        {
//            cshow::show(msg.matchlist(i));
//        }
//	}
//	void cplayer::handle_match_run_list(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, CS2GC::MTTMatchRuleList, msg_size);
//        get_match_game_mgr()->add_match_rule_list(msg);
//        DEBUG_LOG("[CS2GC::MTTMatchRuleList] size = %d", msg.rulelist_size());
//        for (int32 i = 0; i < msg.rulelist_size(); ++i)
//        {
//            cshow::show(msg.rulelist(i));
//        }
//	}
//	void cplayer::handle_create_table(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, CS2GC::CreateFriendTableRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("palyer_id = %d, create friend circle table error code = %d", msg.uid(), msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("palyer_id = %d, create friend circle table ok", msg.uid());
//            cshow::show(msg.friendtable());
//        }
//	}
//	void cplayer::handle_delete_table(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, CS2GC::DeleteFriendTableRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("delete friend circle table error code = %d， table_id = %d", msg.result(), msg.table_id());
//        }
//        else
//        {
//            DEBUG_LOG("delete friend circle table ok");
//        }
//	}
//	void cplayer::handle_modify_table(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::ModifyFriendTableRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("modify friend circle table error code = %d", msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("modify friend circle table ok table_id = %d", msg.tableid());
//        }
//	}
//
//
//    void    cplayer::handle_create_scan(const void *packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet, LS2GC::CreateScanRS, msg_size);
//        if (0 != msg.result() )
//        {
//            ERROR_EX_LOG("create scan failed %d, des = %s", msg.result(), msg.des_result().c_str());
//        }
//        else
//        {
//            DEBUG_LOG("create scan ok scan = [%s]time_stamp = [%u]", msg.scan_uuid().c_str(), msg.time_stamp());
//        }
//    }
//    void    cplayer::handle_scan_login(const void *packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet, LS2GC::ScanLoginRS, msg_size);
//        if (0 != msg.result() )
//        {
//            ERROR_EX_LOG(" scan login failed %d, des = %s", msg.result(), msg.des_result().c_str());
//        }
//        else
//        {
//            m_session_ptr->set_status(ELSS_Conencted , ELSS_Scan);
//            //设置新的状态不登录就可以了
//            DEBUG_LOG(" scan  login ok  player_id = %u, player_name = %s", m_web_user_id, m_user_name.c_str());
//        }
//    }
//    void    cplayer::handle_scan_login_notify(const void *packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet, LS2GC::ScanLoginNotify, msg_size);
//        DEBUG_LOG("scan login ok notify player_id = %d, player_name = %s", msg.userid(), msg.username().c_str());
//    }
//	void cplayer::handle_join_table(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::JoinMatchRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("join  table error code = %d, describe = %s", msg.result(), msg.des_result().c_str());
//        }
//        else
//        {
//            get_match_game_mgr()->join_match(msg);
//            DEBUG_LOG("join  table ok user_id = %d", msg.userid());
//            DEBUG_LOG("table info game_id = %d, game_name = %s, game_type = %d, matchentercode = %d, game_status = %d", msg.matchid(), msg.matchname().c_str(), msg.matchtype(), msg.matchentercode(), msg.matchstatus());
//            DEBUG_LOG("game_type = %d", msg.game_type());
//        }
//	}
//
//
//	void cplayer::handle_leave_table(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::LeaveMatchRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("leave  table error code = %d", msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("leave  table ok user_id = %d", msg.userid());
//            DEBUG_LOG("table info game_id = %d, game_type = %d", msg.matchid(), msg.matchtype());
//            DEBUG_LOG("game_type = %d", msg.game_type());
//        }
//	}
//
//	void cplayer::handle_game_info(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchGameInfoRS, msg_size);
//        DEBUG_LOG("[MS2GC::MatchGameInfoRS]game info user_id = %d, game_id = %d, game_type = %d, game_status =%d, match_status =%d, chair_id = %d", msg.userid(), msg.matchid(), msg.matchtype(), msg.tablestatus(), msg.matchstate(), msg.chairid());
//        DEBUG_LOG("currentuserchairid = %d, currentante = %d, currentsmallblind = %d, currentbigblind = %d, maxplayercount = %d, operatortime= %d", msg.currentuserchairid(), msg.currentante(), msg.currentsmallblind(), msg.currentbigblind(), msg.maxplayercount(), msg.operatestatus());
//        //DEBUG_LOG("table handcard :");
//        for (int32 i = 0; i < msg.handcard_size(); ++i)
//        {
//            DEBUG_LOG("handcard %lu:%d", i, msg.handcard(i));
//        }
//        DEBUG_LOG("publiccardcount = %d", msg.publiccardcount());
//        for (int32 i = 0; i < msg.publiccard_size(); ++i)
//        {
//            DEBUG_LOG("publiccard %lu:%d", i, msg.publiccard(i));
//        }
//        DEBUG_LOG("wealerchairid = %d, smallblindchairid = %d, bigblindchairid = %d, currentuserlefttime = %d, highestset = %d, newhighestset = %d,minimumraise = %d ", msg.wealerchairid(), msg.smallblindchairid(), msg.bigblindchairid(), msg.currentuserlefttime(), msg.highestset(), msg.newhighestset(), msg.minimumraise());
//        DEBUG_LOG("operatestatus = %d,  usercoin = %d, buyin = %d, roomname = %s, commgameroomid = %d, tableid = %d", msg.operatestatus(), msg.usercoin(), msg.buyin(), msg.roomname().c_str(), msg.commgameroomid(), msg.tableid());
//        for (int32 i = 0; i < msg.tablepot_size(); ++i)
//        {
//            DEBUG_LOG("tablepot %lu:%d", i, msg.tablepot(i));
//        }
//        for (int32 i = 0; i < msg.chairinfo_size(); ++i)
//        {
//            cshow::show(msg.chairinfo(i));
//        }
//        DEBUG_LOG("buyin_times = %d", msg.buyin_times());
//        DEBUG_LOG("add_operation_times = %d, support_operation = %d, AddOperationInfo add_operatortime = %d, add_currentuserlefttime = %d", msg.add_operation_times(), msg.support_operation(), msg.add_operation_info().add_operatortime(), msg.add_operation_info().add_currentuserlefttime());
//        DEBUG_LOG("reentry signup reentry_num %d", msg.reentry_num());
//        for (size_t i = 0; i < msg.card_index_size(); ++i)
//        {
//            DEBUG_LOG("card_index = %d", msg.card_index(i));
//        }
//        DEBUG_LOG("add_chip_num = %d, server_add_chip_time = %d, cur_server_add_chip = %d, cur_server_push_msg = %d", msg.add_chip_num(), msg.push_add_match_chip().server_add_chip_time(), msg.push_add_match_chip().cur_server_add_chip(), msg.push_add_match_chip().cur_server_push_msg());
//        DEBUG_LOG("current_match_level = %d", msg.current_match_level());
//	}
//
//	void cplayer::handle_game_timer_info(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchTimerInfoRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("game timer info error_code = %d", msg.result());
//        }
//        else
//        {
//            cshow::show(msg);
//        }
//	}
//
//	void cplayer::handle_game_bonus(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchBonusRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("game bonus info error_code = %d", msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("game bonus user_id = %d, game_id = %d, game_type = %d", msg.userid(), msg.matchid(), msg.matchtype());
//            for (int32 i = 0; i < msg.useridlist_size(); ++i)
//            {
//                DEBUG_LOG("userlist %lu:%lld", i, msg.useridlist(i));
//            }
//            for (int32 i = 0; i < msg.matchbonuslist_size(); ++i)
//            {
//                cshow::show(msg.matchbonuslist(i));
//            }
//        }
//	}
//	void cplayer::handle_game_action(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameUserActionRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("game action info error_code = %d", msg.result());
//        }
//        else
//        {
//
//            DEBUG_LOG(" comm--> game_type = %d",  msg.comm_game_type());
//            DEBUG_LOG("game action info user_id = %d, game_id = %d, chair_id = %d, action_type = %d, highestset = %d, minimumraise = %d", msg.userid(), msg.matchid(), msg.chairid(), msg.actiontype(), msg.highestset(), msg.minimumraise());
//        }
//	}
//	void cplayer::handle_game_player_ranking(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchPlayerRankingRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("game player ranking info error_code = %d", msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("game  player ranking info user_id = %d, game_id = %d, rewardperson = %d", msg.userid(), msg.matchid(), msg.rewardperson());
//            return;
////				for (int32 i = 0; i < msg.rankinglist_size(); ++i)
////				{
////					cshow::show(msg.rankinglist(i));
////				}
//        }
//	}
//	void cplayer::handle_game_show_hand_card(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameShowHandCardRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("game show handCard info error_code = %d", msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("game  show handCard info user_id = %d, game_id = %d", msg.userid(), msg.matchid());
//            for (int32 i = 0; i < msg.indexs_size(); ++i)
//            {
//                DEBUG_LOG("index %lu:%d", i, msg.indexs(i));
//            }
//        }
//	}
//
//	void cplayer::handle_match_add_chip(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchAddChipRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("match_add_chip player_id = %d,  match_id = %d, error_code = %d", msg.userid(), msg.matchid(), msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("match_add_chip  player_id = %d, match_id = %d, match_type = %d ", msg.userid(), msg.matchid(), msg.matchtype());
//        }
//	}
//
////	void cplayer::handle_match_free_signup_num(const void * packet, uint32 msg_size)
////	{
////		MS2GC::Match_Free_Signup_NumRS msg;
////		if (msg.ParseFromArray(packet, msg_size))
////		{
////			if (msg.result())
////			{
////				ERROR_LOG("match_add_chip player_id = %d, error_code = %d", msg.userid(), msg.result());
////			}
////			else
////			{
////				DEBUG_LOG("Match_Free_Signup_NumRS player_id = %llu", msg.userid());
////				for (int32 i = 0; i < msg.match_signup_size(); ++i)
////				{
////					cshow::show(msg.match_signup(i));
////				}
////			}
////		}
////	}
//
//	void cplayer::handle_game_lookon(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchLookonRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("game lookon info error_code = %d", msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("game  show handCard info user_id = %d, game_id = %d, table_count = %d", msg.userid(), msg.matchid(), msg.table_count());
//        }
//	}
//
//	void cplayer::handle_game_player_back_chair(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameBackChairRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("game player back chair  error_code = %d", msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("game  player back chair  user_id = %d, game_id = %d, game_type = %d, player_cash = %d, chair_id = %d, buyin_times = %d", msg.userid(), msg.matchid(), msg.matchtype(), msg.playercash(), msg.chairid(), msg.buyin_times());
//        }
//	}
//	void cplayer::handle_game_add_score(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameManualAddScoreRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("game player back chair  error_code = %d", msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("game  player back chair  user_id = %d, game_id = %d, game_type = %d, player_cash = %d , buyin_times = %d", msg.userid(), msg.matchid(), msg.matchtype(), msg.playercash() , msg.buyin_times());
//        }
//	}
//    void    cplayer::handle_game_add_operationtimes(const void *packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchPlayerAddOperationtimeRS, msg_size);
//        if (msg.result())
//        {
//            ERROR_LOG("game player add operationtimes  error_code = %d", msg.result());
//        }
//        else
//        {
//            DEBUG_LOG("game  player add operationtimes ok user_id = %d, game_id = %d, game_type = %d, add_operationtimer = %d , operationtimes = %d", msg.userid(), msg.matchid(), msg.matchtype(), msg.add_operationtimer(), msg.operationtimes());
//        }
//    }
//    void    cplayer::handle_game_lookon_players(const void *packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::Match_Lookon_PlayersRS, msg_size);
//        DEBUG_LOG("game look player data match_id = %d, match_type = %d, table_id = %d, lookon_size = %u", msg.match_id(), msg.match_type(), msg.table_id(), msg.player_datas_size());
//        for (size_t i= 0; i < msg.player_datas_size(); ++i)
//        {
//            DEBUG_LOG("player_id = %d, player_name = %s, gender = %d, nationality = %d", msg.player_datas(i).player_id(), msg.player_datas(i).player_name().c_str(), msg.player_datas(i).gender(), msg.player_datas(i).nationality());
//        }
//    }
//    void    cplayer::handle_match_game_device(const void *packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::Match_DeviceRS, msg_size);
//        DEBUG_LOG("[Match_DeviceRS] match_id = %d, reseult = %d, player_id = %d, des_result= %s", msg.match_id(), msg.result(), msg.player_id(), msg.des_result().c_str());
//
//    }
//
////    void    cplayer::handle_game_player_operationtimes(const void *packet, uint32 msg_size)
////    {
////        MS2GC::MatchPlayerOperationtimesRS msg;
////        if (msg.ParseFromArray(packet, msg_size))
////        {
////            if (msg.result())
////            {
////                ERROR_LOG("game player operationtimes  error_code = %d", msg.result());
////            }
////            else
////            {
////                DEBUG_LOG("game  player operationtimes info user_id = %d, game_id = %d, game_type = %d, operationtimes = %d", msg.userid(), msg.matchid(), msg.matchtype(), msg.operationtimes());
////            }
////        }
////    }
//
//	void	cplayer::handle_match_info_update_notify(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, CS2GC::MatchInfoUpdateNotify, msg_size);
//        DEBUG_LOG("match info update notify :");
//        DEBUG_LOG("game_id = %d, game_type = %d, state = %d, totalplayeramount = %d, currentplayeramount = %d", msg.matchid(), msg.matchtype(), msg.state(), msg.totalplayeramount(), msg.currentplayeramount());
//
//    }
//
//	void cplayer::handle_ante_blind_change_notify(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::AnteBlindChangeNotify, msg_size);
//        DEBUG_LOG("ante blind change notify :");
//        DEBUG_LOG("game_id = %d, game_type = %d, currentlevel = %d, currentante = %d, currentsmallblind = %d", msg.matchid(), msg.matchtype(), msg.currentlevel(), msg.currentante(), msg.currentsmallblind());
//        DEBUG_LOG("currentbigblind = %d, nextante = %d, nextsmallblind = %d, nextbigblind= %d, increaseblindtime = %d, matchstate = %d", msg.currentbigblind(), msg.nextante(), msg.nextsmallblind(), msg.nextbigblind(), msg.increaseblindtime(), msg.matchstate());
//
//    }
//
//	void cplayer::handle_match_start_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchStartNotify, msg_size);
//        DEBUG_LOG("match start notify :");
//        DEBUG_LOG("game_id = %d", msg.matchid());
//	}
//	void cplayer::handle_match_finish_notify(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchFinishNotify, msg_size);
//        DEBUG_LOG("match finish notify :");
//        if (msg.result())
//        {
//            ERROR_LOG("user_id = %d, game_id = %d, ", msg.userid(), msg.matchid());
//        }
//        else
//        {
//            DEBUG_LOG("user_id = %d, game_id = %d, masteruserid =%lld, masterreward= %d, totalreward = %d, totalrewardperson = %d, matchname = %s, ranking = %d, matchbonus = %s", msg.userid(), msg.matchid(), msg.masteruserid(), msg.masterreward(), msg.totalreward(), msg.totalrewardperson(), msg.matchname().c_str(), msg.ranking(), msg.matchbonus().c_str());
//        }
//	}
//
//	void cplayer::handle_match_wait_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchWaitTableNotify, msg_size);
//        DEBUG_LOG("match wait notify :");
//        DEBUG_LOG("user_id = %d, game_id = %d, game_type = %d, game_status = %d, userchipcount = %d", msg.userid(), msg.matchid(), msg.matchtype(), msg.matchstatus(), msg.userchipcount());
//
//    }
//
//	void cplayer::handle_match_result_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchResultNotify, msg_size);
//        DEBUG_LOG("match result notify :");
//        DEBUG_LOG("user_id = %d, game_id = %d, game_type = %d, matchrank = %d, matchbonus = %s, matchremainperson = %d, bnotifyreward = %d", msg.userid(), msg.matchid(), msg.matchtype(), msg.matchrank(), msg.matchbonus().c_str(), msg.matchremainperson(), msg.bnotifyreward());
//
//    }
//
//	void cplayer::handle_match_cancle_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchCancelNotify, msg_size);
//        DEBUG_LOG("match cancel notify :");
//        DEBUG_LOG("game_id = %d,reason = %d",  msg.matchid(), msg.reason());
//	}
//
//	void cplayer::handle_match_status_change_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchStatusChangedNotify, msg_size);
//        DEBUG_LOG("match status change notify :");
//        DEBUG_LOG("game_id = %d,matchtype = %d, matchstatus = %d", msg.matchid(), msg.matchtype(), msg.matchstatus());
//    }
//
//	void cplayer::handle_match_add_chip_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::Match_Add_Chip_Notify, msg_size);
//        DEBUG_LOG("match add chip notify :");
//        char buf[ASCII_DATETIME_LEN];
//        chen::ctime_base_api::time64_datetime_format(msg.time_stamp(), buf, '-', ' ', ':');
//        DEBUG_LOG("match_id = %d,matchtype = %d, player_score = %d, match_chip = %d, time_stamp = %s", msg.match_id(), msg.match_type(), msg.player_score(), msg.match_chip(), buf);
//
//    }
//
//	void cplayer::handle_game_ready_start_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameReadyStartNotify, msg_size);
//        DEBUG_LOG("game_id = %d, tableid = %d, currentante = %d, nextante = %d", msg.matchid(), msg.tableid(), msg.currentante(), msg.nextante());
//        for (int32 i = 0; i < msg.tablepot_size(); ++i)
//        {
//            DEBUG_LOG("tablepot %lu:%d", i, msg.tablepot(i));
//        }
//        for (int32 i = 0; i < msg.userchipsinfo_size(); ++i)
//        {
//            cshow::show(msg.userchipsinfo(i));
//        }
//	}
//
//	void cplayer::handle_game_start_notify(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameStartNotify, msg_size);
//        DEBUG_LOG("game start notify :");
//        DEBUG_LOG("user_id = %d, cur_user_id = %d, game_id = %d, tableid = %d， currentuserchairid = %d, dealerchairid = %d, smallblindchairid = %d", msg.userid(), msg.currentuserid(), msg.matchid(), msg.tableid(), msg.currentuserchairid(), msg.dealerchairid(), msg.smallblindchairid());
//        DEBUG_LOG("bigblindchairid = %d, smallblind = %d, bigblind = %d, nextsmallblind = %d, nextbigblind = %d, operatetime = %d, highestset = %d, minimumraise = %d", msg.bigblindchairid(), msg.smallblind(), msg.bigblind(), msg.nextsmallblind(), msg.nextbigblind(), msg.operatetime(), msg.highestset(), msg.minimumraise());
//        for (int32 i = 0; i < msg.handcard_size(); ++i)
//        {
//
//            std::string card = cpoker_util::get_color(msg.handcard(i));
//            card += cpoker_util::get_value(msg.handcard(i));
//            DEBUG_LOG("handcard %lu:%s", i, card.c_str());
//        }
//        for (int32 i = 0; i < msg.userchipsinfo_size(); ++i)
//        {
//            cshow::show(msg.userchipsinfo(i));
//        }
//        get_match_game_mgr()->match_game_start(msg);
//	}
//	void cplayer::handle_game_flop_cards_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GamePublicFlopCardsNotify, msg_size);
//        DEBUG_LOG("game public flop cards notify :");
//        DEBUG_LOG("game_id = %d, tableid = %d,currentuserid = %d, currentuserchairid = %d",  msg.matchid(), msg.tableid(), msg.currentuserid(), msg.currentuserchairid());
//        std::string card1 = cpoker_util::get_color(msg.flopcard1()) + cpoker_util::get_value(msg.flopcard1());
//        std::string card2 = cpoker_util::get_color(msg.flopcard2()) + cpoker_util::get_value(msg.flopcard2());
//        std::string card3 = cpoker_util::get_color(msg.flopcard3()) + cpoker_util::get_value(msg.flopcard3());
//        DEBUG_LOG("flopcard1 = %s, flopcard2 = %s, flopcard3 = %s, minimumraise = %d", card1.c_str(), card2.c_str(), card3.c_str());
//
//    }
//	void cplayer::handle_game_turn_cards_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GamePublicTurnCardsNotify, msg_size);
//        DEBUG_LOG("game public turn cards notify :");
//        std::string card3 = cpoker_util::get_color(msg.turncard()) + cpoker_util::get_value(msg.turncard());
//        DEBUG_LOG("game_id = %d, tableid = %d, currentuserid = %d, currentuserchairid = %d, turncard = %s, minimumraise = %d", msg.matchid(), msg.tableid(), msg.currentuserid(), msg.currentuserchairid(), card3.c_str(), msg.minimumraise());
//
//    }
//
//	void cplayer::handle_game_river_cards_notify(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GamePublicRiverCardsNotify, msg_size);
//        DEBUG_LOG("game public turn cards notify :");
//        std::string card = cpoker_util::get_color(msg.rivercard()) + cpoker_util::get_value(msg.rivercard());
//        DEBUG_LOG("game_id = %d, tableid = %d, currentuserid = %d, currentuserchairid = %d, rivercard = %s, minimumraise = %d", msg.matchid(), msg.tableid(), msg.currentuserid(), msg.currentuserchairid(), card.c_str(), msg.minimumraise());
//
//    }
//	void cplayer::handle_game_user_action_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameUserActionNotify, msg_size);
//        DEBUG_LOG("game user action notify :");
//        DEBUG_LOG("user_id = %d, chair_id = %d, action_type = %d, game_id = %d,  currentuserid = %d, currentuserchairid = %d, minimumraise = %d", msg.userid(), msg.chairid(), msg.actiontype(), msg.matchid(), msg.currentuserid(), msg.currentuserchairid(), msg.minimumraise());
//        DEBUG_LOG("coincount = %d, cashcount = %d, highestset = %d, truststatus = %d, absoluteset = %d, newhighestset = %d", msg.coincount(), msg.cashcount(), msg.highestset(), msg.truststatus(), msg.absoluteset(), msg.newhighestset());
//        for (int32 i = 0; i < msg.tablepot_size(); ++i)
//        {
//            DEBUG_LOG("tablepot %lu:%d", i, msg.tablepot(i));
//        }
//        get_match_game_mgr()->match_game_action_notify(msg);
//	}
//
//	void cplayer::handle_game_user_handcard_notify(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameUserHandcardNotify, msg_size);
//        DEBUG_LOG("game user hand card notify :");
//        DEBUG_LOG("game_id = %d", msg.matchid());
//        for (int32 i = 0; i < msg.useridlist_size(); ++i)
//        {
//            DEBUG_LOG("useridlist %lu:%d", i, msg.useridlist(i));
//        }
//        for (int32 i = 0; i < msg.userhandcard_size(); ++i)
//        {
//            cshow::show(msg.userhandcard(i));
//        }
//	}
//
//	void cplayer::handle_game_end_info_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameEndInfoNotify, msg_size);
//        DEBUG_LOG("[GameEndInfoNotify]game end info notify :");
//        DEBUG_LOG("game_id = %d", msg.matchid());
//        for (int32 i = 0; i < msg.gameendinfo_size(); ++i)
//        {
//            cshow::show(msg.gameendinfo(i));
//        }
//        for (int32 i = 0; i < msg.poolinfo_size(); ++i)
//        {
//            cshow::show(msg.poolinfo(i));
//        }
//	}
//
//	void cplayer::handle_game_show_hand_card_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameShowHandCardNotify, msg_size);
//        DEBUG_LOG("game show hand card notify :");
//        DEBUG_LOG("game_id = %d", msg.matchid());
//        for (int32 i = 0; i < msg.userhandcards_size(); ++i)
//        {
//            cshow::show(msg.userhandcards(i));
//        }
//	}
//	void cplayer::handle_game_user_sitedown_notify(const void *packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameUserSiteDownNotify, msg_size);
//        DEBUG_LOG("game user sitedown notify :");
//        DEBUG_LOG("user_id = %d, chair_id = %d, user_name = %s, game_id = %d, game_type = %d, playercash = %d, sitdowntype = %d",msg.userid(), msg.chairid(), msg.username().c_str(), msg.matchid(), msg.matchtype(), msg.playercash(), msg.sitdowntype());
//
//        for (int32 i = 0; i < msg.useridlist_size(); ++i)
//        {
//            DEBUG_LOG("useridlist %lu:%d", i, msg.useridlist(i));
//        }
//	}
//	void cplayer::handle_game_user_leaveseat_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameUserLeaveSeatNotify, msg_size);
//        DEBUG_LOG("game user leaveseat notify :");
//        DEBUG_LOG("user_id = %d, chair_id = %d,  game_id = %d, game_type = %d, playercash = %d, sitdowntype = %d", msg.userid(), msg.chairid(), msg.matchid(), msg.matchtype());
//
//    }
//	void cplayer::handle_game_user_reservestandup_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::GameUserstateNotify, msg_size);
//        DEBUG_LOG("game user reservestandup notify :");
//        DEBUG_LOG("user_id = %d, chair_id = %d,  game_id = %d, game_type = %d, playercash = %d, sitdowntype = %d, state = %d", msg.userid(), msg.chairid(), msg.matchid(), msg.matchtype(), msg.state());
//
//    }
//	void cplayer::handle_match_player_cash_chage_notify(const void * packet, uint32 msg_size)
//	{
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchPlayerCashChangedNotify, msg_size);
//        DEBUG_LOG("match player cash change notify :");
//        DEBUG_LOG("game_id = %d, game_type = %d, playercash = %d, sitdowntype = %d", msg.matchid(), msg.matchtype());
//        for (int32 i = 0; i < msg.userlist_size(); ++i)
//        {
//            cshow::show(msg.userlist(i));
//        }
//	}
//    void    cplayer::handle_game_user_add_operationtimes_notify(const void * packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchAddOperationTimerNotify, msg_size);
//        DEBUG_LOG("match player add operationtimes  notify :");
//        DEBUG_LOG("match_id = %d, match_type = %d, player_id = %d, chairid = %d,  operationtimes = %d, currentuserlefttime = %d", msg.matchid(),  msg.matchtype(), msg.userid(), msg.chairid(), msg.operationtimer(), msg.cur_user_lefttime());
//
//    }
//    void    cplayer::handle_match_operationtimes_status_notify(const void * packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::MatchOperationStatusNotify, msg_size);
//        DEBUG_LOG("MatchOperationStatusNotify  notify :");
//        DEBUG_LOG("match_id = %d, match_type = %d, operationstatus = %d", msg.matchid(),  msg.matchtype(), msg.operationstatus());
//
//    }
//    void cplayer::handle_match_multianguage_notify(const void * packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet, CS2GC::MultilanguageNotify, msg_size);
//        DEBUG_LOG("MultilanguageNotify");
//    }
//    void    cplayer::handle_game_lookon_players_num_notify(const void * packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet, MS2GC::Game_Lookon_Players_NumNotify, msg_size);
//        DEBUG_LOG("[Game_Lookon_Players_numNotify]game look player data num match_id = %d, match_type = %d, table_id = %d, lookon_size = %d", msg.match_id(), msg.match_type(), msg.table_id(), msg.lookon_players());
//
//    }
//    void    cplayer::handle_match_table_count_notify(const void * packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet,  MS2GC::NoticeTableCount, msg_size);
//        {
//            DEBUG_LOG("[NoticeTableCount] match_id = %d, table_size = %d", msg.matchid(), msg.tablecount());
//        }
//
//    }
//    void    cplayer::handle_game_end_add_chip_notice(const void *packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet,  MS2GC::Game_End_Add_Chip_Notice, msg_size);
//        {
//           DEBUG_LOG("[Game_End_Add_Chip_Notice] match_id = %d, match_type = %d, player_id = %d, add_match_chip_type = %d, player_score = %d, match_chip = %d, add_chip_num = %d, player_match_chip = %d", msg.match_id(), msg.match_type(), msg.user_id(), msg.add_match_chip_type(), msg.player_score(), msg.match_chip(), msg.add_chip_num(), msg.player_match_chip());
//        }
//
//    }
//
//    void   cplayer::handle_game_device_notify(const void * packet, uint32 msg_size)
//    {
//        PRASE_OLD_CLIENT_MSG(packet,  MS2GC::Game_Device_Notify, msg_size);
//        DEBUG_LOG("[Game_Device_Notify] match_id = %d,  device_type = %d", msg.match_id(), msg.device_type());
//
//
//    }
} // namespace chen