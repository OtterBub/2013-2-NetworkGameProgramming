#pragma comment(lib, "ws2_32")
#include <iostream>
#include <winsock2.h>
#include <stdio.h>

void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

int main(int argc, char *argv[])
{
	// 扩加 檬扁拳
	WSADATA wsa;
	WSADATA wsa2;

	if(WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
		return 1;
	std::cout << "------- 扩加1.1 檬扁拳 己傍-------\n" << std::endl;

	std::cout << " wVersion : " << wsa.wVersion << std::endl;
	std::cout << " wVersion : " << ((wsa.wVersion >> 8 ) & 0x0f) << ". " << (wsa.wVersion & 0x0f) << std::endl;
	std::cout << " wHighVersion : " << ((wsa.wHighVersion >> 8 ) & 0x0f) << ". " << (wsa.wHighVersion & 0x0f) << std::endl;
	std::cout << " szDescription : " << wsa.szDescription << std::endl;
	std::cout << " szSystemStatus : " << wsa.szSystemStatus << std::endl;
	std::cout << " iMaxUdpDg : " << wsa.iMaxUdpDg << std::endl;
	//std::cout << " lpVendorInfo : " << wsa.lpVendorInfo << std::endl;
	std::cout << " iMaxSockets : " << wsa.iMaxSockets << std::endl << std::endl;

	if(WSAStartup(MAKEWORD(2, 2), &wsa2) != 0)
		return 1;
	std::cout << "------- 扩加2.2 檬扁拳 己傍-------\n" << std::endl;

	std::cout << " wVersion : " << ((wsa2.wVersion >> 8 ) & 0x0f) << ". " << (wsa2.wVersion & 0x0f) << std::endl;
	std::cout << " wHighVersion : " << ((wsa2.wHighVersion >> 8 ) & 0x0f) << ". " << (wsa2.wHighVersion & 0x0f) << std::endl;
	std::cout << " szDescription : " << wsa2.szDescription << std::endl;
	std::cout << " szSystemStatus : " << wsa2.szSystemStatus << std::endl;
	std::cout << " iMaxUdpDg : " << wsa2.iMaxUdpDg << std::endl;
	//std::cout << " lpVendorInfo : " << wsa.lpVendorInfo << std::endl;
	std::cout << " iMaxSockets : " << wsa2.iMaxSockets << std::endl << std::endl;

	// socket()
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_sock == INVALID_SOCKET) err_quit("socket()");
	std::cout << "TCP 家南 积己 己傍" << std::endl;

	SOCKET tcp_sock_v6 = socket(AF_INET6, SOCK_STREAM, 0);
	if(tcp_sock_v6 == INVALID_SOCKET) err_quit("socket()");
	std::cout << "TCPv6 家南 积己 己傍\n" << std::endl;

	SOCKET udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(udp_sock == INVALID_SOCKET) err_quit("socket()");
	std::cout << "UDP 家南 积己 己傍" << std::endl;

	SOCKET udp_sock_v6 = socket(AF_INET6, SOCK_DGRAM, 0);
	if(udp_sock_v6 == INVALID_SOCKET) err_quit("socket()");
	std::cout << "UDPv6 家南 积己 己傍" << std::endl;

	// closesocket()
	closesocket(tcp_sock);
	closesocket(tcp_sock_v6);
	closesocket(udp_sock);
	closesocket(udp_sock_v6);

	// 扩加 辆丰
	WSACleanup();
	WSACleanup();
	return 0;
}