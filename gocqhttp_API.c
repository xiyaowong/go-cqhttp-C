#include"gocqhttp_API.h"
#include<process.h>
#include<winsock2.h>
#include<windows.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<string.h>
#pragma comment(lib, "ws2_32.lib")

/* socket */
static SOCKADDR_IN	server_addr;//服务端
static SOCKET		server;		//新建套接字

/*包*/
static int len_max = 130;

/*send_private_msg 发送私密消息*/
send_private_msg_data* New_send_private_msg(unsigned long user_id, unsigned long group_id, char message[300], int auto_escape)
{
	send_private_msg_data* data = (send_private_msg_data*)malloc(sizeof(send_private_msg_data));
	if (!data)
		return NULL;
	data->send_msg.user_id = user_id;
	data->send_msg.group_id = group_id;
	strcpy_s(data->send_msg.message, sizeof(data->send_msg.message), message);
	data->send_msg.auto_escape = auto_escape;
	return data;
}

send_private_msg_data Get_send_private_msg(char* data)
{
	//code
}

cqhttp_err send_private_msg(send_private_msg_data* data)
{
	char func[20] = "send_private_msg";

	char rmsg[500] = { '\0' };							//收包
	const int len = strlen(data->send_msg.message) + 1;	//消息长度
	char* smsg = (char*)malloc(len + len_max);			//发包
	if (!smsg)
		return set_cqhttp_err(NULLError, func);
	memset(smsg, 0, len + len_max);

	//构建发包（字符串）
	sprintf_s(smsg, len + len_max, API_SEND_PRIVATE_MSG_FORM,
		data->send_msg.user_id,
		data->send_msg.group_id,
		data->send_msg.message,
		data->send_msg.auto_escape);
	int isend = send(server, smsg, strlen(smsg), 0);	//发送
	if (isend < 0)
		return set_cqhttp_err(NetworkIOError, func);

	//////////////
	while (recv(server, rmsg, sizeof(rmsg), 0) < 0);	//接收
	puts(rmsg);
	//code
	//////////////

	free(smsg);
	return set_cqhttp_err(None, func);
}

/*send_group_msg 发送群消息*/
send_group_msg_data* New_send_group_msg(long group_id, char message[226], int auto_escape)
{
	send_group_msg_data* data = (send_group_msg_data*)malloc(sizeof(send_group_msg_data));
	if (!data)
		return NULL;
	data->send_msg.group_id = group_id;
	strcpy_s(data->send_msg.message, sizeof(data->send_msg.message), message);
	data->send_msg.auto_escape = auto_escape;
	return data;
}

send_group_msg_data Get_send_group_msg(char* data)
{
	send_group_msg_data p;
	p.send_msg.group_id = 123;
	return p;
}

cqhttp_err send_group_msg(send_group_msg_data* data)
{
	char func[20] = "send_group_msg";

	char rmsg[500] = { '\0' };							//收包
	const int len = strlen(data->send_msg.message) + 1;	//消息长度
	char* smsg = (char*)malloc(len + len_max);			//发包
	if (!smsg)
		return set_cqhttp_err(NULLError, func);
	memset(smsg, 0, len + len_max);

	//构建发包
	sprintf_s(smsg, len + len_max, API_SEND_GROUP_MSG_FORM,
		data->send_msg.group_id,
		data->send_msg.message,
		data->send_msg.auto_escape);
	int isend = send(server, smsg, strlen(smsg), 0);	//发送
	if (isend < 0)
		return set_cqhttp_err(NetworkIOError, func);

	//////////////
	while (recv(server, rmsg, sizeof(rmsg), 0) < 0);	//接收
	puts(rmsg);
	//code
	//////////////

	free(smsg);
	return set_cqhttp_err(None, func);
}

/*delete_msg 撤回消息*/
delete_msg_data* New_delete_msg(int message_id)
{
	delete_msg_data* data = (delete_msg_data*)malloc(sizeof(delete_msg_data));
	if (!data)
		return NULL;
	data->send_msg.message_id = message_id;
	return data;
}

cqhttp_err delete_msg(delete_msg_data* data)
{
	char func[20] = "delete_msg";

	char rmsg[500] = { '\0' };							//收包
	char* smsg = (char*)malloc(len_max);				//发包
	if (!smsg)
		return set_cqhttp_err(NULLError, func);
	memset(smsg, 0, len_max);

	//构建发包
	sprintf_s(smsg, len_max, API_DELETE_MSG_FORM,
		data->send_msg.message_id);
	int isend = send(server, smsg, strlen(smsg), 0);	//发送
	if (isend < 0)
		return set_cqhttp_err(NetworkIOError, func);
	//////////////

	while (recv(server, rmsg, sizeof(rmsg), 0) < 0);
	puts(rmsg);
	//code
	//////////////

	free(smsg);
	return set_cqhttp_err(None, func);
}

/*
	初始化
*/
cqhttp_err init_gocqhttpAPI(const int port, const char* ip)
{
	const char func[20] = "init_gocqhttpAPI";

	memset((void*)&server_addr, 0, sizeof(SOCKADDR_IN));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, (void*)&server_addr.sin_addr);

	WSADATA wsaData;
	int e = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (e)
		return set_cqhttp_err(WSAStartupError, func);
	
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return set_cqhttp_err(SocketInitError, func);
	
	if (connect(server, (SOCKADDR*)&server_addr, sizeof(server_addr)) < 0)
		return set_cqhttp_err(ConnectionError, func);
	else
		return set_cqhttp_err(None, func);
}

/*
	退出
*/
void exit_gocqhttpAPI(void)
{
	closesocket(server);  
	memset((void*)&server_addr, 0, sizeof(server_addr));
}