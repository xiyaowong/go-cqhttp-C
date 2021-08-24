#pragma once
#include"gocqhttp_err.h"

/*
	初始化
*/
cqhttp_err init_gocqhttpAPI(const int port, const char* ip);

/*
	退出
*/
void exit_gocqhttpAPI(void);

/////////////////////////////////
/*send_private_msg 发送私密消息*/
/////////////////////////////////

#define API_SEND_PRIVATE_MSG_FORM	"GET /send_private_msg?user_id=%u&group_id=%u&message=%s&auto_escape=%d HTTP/1.1\r\nHost: 127.0.0.1:5700\r\nConnection: keep-alive\r\n\r\n"

typedef struct
{
	unsigned long user_id;	//对方 QQ 号
	unsigned long group_id;	//主动发起临时会话群号(机器人本身必须是管理员/群主)
	char message[300];		//要发送的内容
	int auto_escape;		//消息内容是否作为纯文本发送，只在 message 字段是字符串时有效(默认值 false）
}send_private_msg_s;	//发送消息包

typedef struct
{
	int message_id;		//消息 ID
}send_private_msg_r;	//接收消息包

typedef union
{
	send_private_msg_s send_msg;	//发包
	send_private_msg_r recv_mag;	//收包
}send_private_msg_data;//组合包

cqhttp_err send_private_msg(
	send_private_msg_data* data				//发包
);

send_private_msg_data* New_send_private_msg(
	unsigned long user_id,					//用户ID
	unsigned long group_id,					//群号
	char message[300],						//消息
	int auto_escape							//是否纯文本
);

send_private_msg_data Get_send_private_msg(
	char* data								//接收消息
);

#define API_SEND_PRIVATE_MSG_SIZE	sizeof(send_private_msg_data)

/////////////////////////////
/*send_group_msg 发送群消息*/
/////////////////////////////

#define API_SEND_GROUP_MSG_FORM		"GET /send_group_msg?group_id=%ld&message=%s&auto_escape=%d HTTP/1.1\r\nHost: 127.0.0.1:5700\r\nConnection: keep-alive\r\n\r\n"

typedef struct
{
	unsigned long group_id;	//群号
	char message[300];		//要发送的内容
	int auto_escape;		//消息内容是否作为纯文本发送，只在 message 字段是字符串时有效(默认值 false）
}send_group_msg_s;  //发送消息包

typedef struct
{
	int message_id;		//消息 ID
}send_group_msg_r;  //接收消息包

typedef union
{
	send_group_msg_s send_msg;		//发包
	send_group_msg_r recv_msg;		//收包
}send_group_msg_data;//组合包

cqhttp_err send_group_msg(
	send_group_msg_data* data				//发包
);
send_group_msg_data* New_send_group_msg(
	long group_id,							//群号
	char message[300],						//消息
	int auto_escape							//是否纯文本
);
send_group_msg_data Get_send_group_msg(
	char* data								//接收消息
);

#define API_SEND_GROUP_MSG_SIZE		sizeof(send_group_msg_data)

///////////////////////
/*delete_msg 撤回消息*/
///////////////////////

#define API_DELETE_MSG_FORM			"GET /delete_msg?message_id=%d HTTP/1.1\r\nHost: 127.0.0.1:5700\r\nConnection: keep-alive\r\n\r\n"

typedef struct
{
	int message_id;	//消息 ID
}delete_msg_s;		//发送消息包

typedef union
{
	delete_msg_s send_msg;			//发包
}delete_msg_data;		//组合包

cqhttp_err delete_msg(
	delete_msg_data* data					//发包
);

delete_msg_data* New_delete_msg(
	int message_id							//消息ID
);

#define API_DELETE_MSG_SIZE			sizeof(delete_msg_data)