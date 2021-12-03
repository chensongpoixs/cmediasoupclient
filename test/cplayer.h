/***********************************************************************************************
				created: 		2019-05-12

				author:			chensong

				purpose:		palyer
************************************************************************************************/


#ifndef _C_PLAYER_H_
#define _C_PLAYER_H_
#include "cnoncopyable.h"
#include "cnet_types.h"
#include "ccmd_param.h"
//#include "csession.h"
//#include "cmgr_base.h"
#include <google/protobuf/message_lite.h>
//#include "cfriend_circle_table_mgr.h" //头文件在包含顺序上不要成闭合的环状，他们结构顺序最好应该是树。
namespace chen {
	class csession;   //告诉编译编译器 要先编译clan_session文件再编译cplayer
	//class cmgr_base;
	class cfriend_circle_table_mgr; // C++ 里面编译是比较奇葩的事情
	class ctest_case_mgr;
	class cmatch_game_mgr;
	class cplayer : private cnoncopyable
	{
	public:
		explicit cplayer();
		~cplayer();
	public:
		bool init(csession *lan_session_ptr, uint32 index);
		void destroy();
		void update(uint32 msleep);
	public:
		void reset();

		void on_logined(const std::string &user_name, int32 score, const std::string& token , int32 uid, uint32 login_session_id);
	
	public:
		uint32 get_player_id() const { return m_web_user_id; }
		std::string get_player_name() {
			return m_user_name;
		};
        const std::string get_player_name() const {
            return m_user_name;
        };
	public:
		//得到玩家id
		const uint32 get_session_id() const { return m_session_index; }
		const csession * get_session() const {return m_session_ptr;}
        csession * get_session()  {return m_session_ptr;}
	public:
	 
		
	public:
		 
	public:
		bool send_msg(uint16_t msg_id, const ::google::protobuf::MessageLite& msg) const;
		bool send_msg(uint16 msg_id, const ::google::protobuf::MessageLite& msg, int32 match_id);
		bool send_gm(const char* buf) const;
		void on_msg(uint16_t msg_id, int32 result);
	public:
		void 	handle_code(const void* packet, uint32 msg_size);
	

	private:
		csession *				m_session_ptr;
		//cmgr_base*				m_mgrs[MGR_MAX];
		uint32					m_session_index;
		std::string				m_user_name;
		int32					m_score;
		std::string				m_token;
		int32					m_web_user_id;
		uint32					m_login_session_id; //http 登录时回话id
	};

}// namespace chen

#endif // !#define _C_PLAYER_H_