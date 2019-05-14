#include"network.h"
#include"main.h"
#include"data.h"

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#include<WinSock2.h>
#include<thread>
#include<fstream>
#include<string>

#define VERSION			(MAKEWORD(2,2))			//Winsockのバージョン
#define HOSTNAME_FILE	("Data/hostname.neu")	//ホスト名を読み込むファイル名
#define PORT			(10000)					//ポート番号
#define RECV_SIZE		(128)					//受信サイズ

struct UDP
{
	WSAData wsa_data;
	SOCKET sock;
	std::thread thread;
	std::string host_name;
	hostent *host;
	in_addr internet_addr;
	sockaddr_in send_addr;
	sockaddr_in recv_addr;
	sockaddr_in sender_addr;
	bool connecting;
};

static UDP g_udp;

void InitUDP()
{
	WSAStartup(VERSION, &g_udp.wsa_data);

	std::ifstream load_file(HOSTNAME_FILE);
	if (std::getline(load_file, g_udp.host_name)) g_udp.host_name;
	g_udp.host = gethostbyname(g_udp.host_name.c_str());
	if (g_udp.host != NULL)
	{
		memcpy(&g_udp.internet_addr, g_udp.host->h_addr_list[0], g_udp.host->h_length);

		g_udp.send_addr.sin_family = AF_INET;
		g_udp.send_addr.sin_port = htons(PORT);
		g_udp.send_addr.sin_addr = g_udp.internet_addr;

		g_udp.recv_addr.sin_family = AF_INET;
		g_udp.recv_addr.sin_port = htons(PORT);
		g_udp.recv_addr.sin_addr.s_addr = INADDR_ANY;

		g_udp.sock = socket(AF_INET, SOCK_DGRAM, 0);
		bind(g_udp.sock, (struct sockaddr *)&g_udp.recv_addr, sizeof(g_udp.recv_addr));

		g_udp.connecting = true;
		g_udp.thread = std::thread([&] {while (g_udp.connecting) { RecvUDP(); }});
	}
	else
	{
		UninitUDP();
		MessageBox(NULL, "マッチングできる相手がいないため、オフラインモードに切り替えます。", "エラー", MB_OK);
	}
}

void UninitUDP()
{
	ResetData();
	g_udp.connecting = false;
	closesocket(g_udp.sock);
	WSACleanup();
	if (g_udp.thread.joinable()) g_udp.thread.join();
}

void SendUDP(void *data)
{
	if (g_udp.connecting) sendto(g_udp.sock, (char*)data, (int)sizeof(Player), 0, (struct sockaddr *)&g_udp.send_addr, sizeof(g_udp.send_addr));
}

void RecvUDP()
{
	char buf[RECV_SIZE] = {};
	int addrlen = sizeof(g_udp.sender_addr);
	recvfrom(g_udp.sock, buf, RECV_SIZE - 1, 0, (struct sockaddr *)&g_udp.sender_addr, &addrlen);
	memcpy(&Data::g_player, buf, sizeof(Player));
}