#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource.h"
#include "PacketStruct.h"

#define BUFSIZE 512

// 대화상자 메시지 처리 함수
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
// 에디트 컨트롤 출력 함수
void DisplayText(HWND handle, char *fmt, ...);
// 소켓 통신 함수
DWORD WINAPI ClientMain(LPVOID arg);
// UDP 통신 함수
DWORD WINAPI UDPClientMain(LPVOID arg);
// 오류 출력 함수
void err_quit(char *msg);
void err_display(char *msg);
// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);

SOCKET sock; // 소켓
char buf[BUFSIZE+1]; // 데이터 송수신 버퍼
HWND hEdit1, hEdit2, hEdit3, hEdit4, hEdit5, hEdit6, hOKButton; // 에디트 컨트롤과 OK 버튼
HANDLE hReadEvent, hWriteEvent; // 이벤트

int g_count = 0;
PlayerDate g_PlayerDate;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE,
	LPSTR lpCmdLine, int nCmdShow)
{

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// 이벤트 생성
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(hReadEvent == NULL) return -1;
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(hWriteEvent == NULL) return -1;

	// 소켓 통신을 담당하는 스레드 생성
	DWORD ThreadId;
	CreateThread(NULL, 0, ClientMain, NULL, 0, &ThreadId);

	CreateThread(NULL, 0, UDPClientMain, NULL, 0, &ThreadId);
	
	// 대화 상자 생성
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	
	// 이벤트 제거
	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();	

	return 0;
}

// 대화상자 메시지 처리
BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, 
	WPARAM wParam, LPARAM lParam)
{
	switch(uMsg){
	case WM_INITDIALOG:
		hEdit1 = GetDlgItem(hDlg, IDC_EDIT1); // PlayerDataSend (player position x)
		hEdit2 = GetDlgItem(hDlg, IDC_EDIT2); // PlayerDataSend Message
		hEdit3 = GetDlgItem(hDlg, IDC_EDIT3); // PlayerDataSend (player position y)
		hEdit4 = GetDlgItem(hDlg, IDC_EDIT4); // PlayerDataSend (player position z)
		hEdit5 = GetDlgItem(hDlg, IDC_EDIT5); // PlayerDataSend (Attack)
		hEdit6 = GetDlgItem(hDlg, IDC_EDIT6); // WorldDataRecv
		hOKButton = GetDlgItem(hDlg, IDOK);
		SendMessage(hEdit1, EM_SETLIMITTEXT, BUFSIZE, 0);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDOK:
			// OK 버튼 비활성화
			EnableWindow(hOKButton, FALSE);
			// 읽기 완료를 기다림
			WaitForSingleObject(hReadEvent, INFINITE);

			char temp [100];
			GetDlgItemText(hDlg, IDC_EDIT1, temp, BUFSIZE+1);
			strcpy(buf, temp);
			g_PlayerDate.UserPosition.x = atof(temp);

			GetDlgItemText(hDlg, IDC_EDIT3, temp, BUFSIZE+1);
			strcpy(&buf[strlen(buf)], ", ");
			strcpy(&buf[strlen(buf)], temp);
			g_PlayerDate.UserPosition.y = atof(temp);

			GetDlgItemText(hDlg, IDC_EDIT4, temp, BUFSIZE+1);
			strcpy(&buf[strlen(buf)], ", ");
			strcpy(&buf[strlen(buf)], temp);
			g_PlayerDate.UserPosition.z = atof(temp);

			GetDlgItemText(hDlg, IDC_EDIT5, temp, BUFSIZE+1);
			g_PlayerDate.attackDir = atoi(temp);

			// 쓰기 완료를 알림
			SetEvent(hWriteEvent);

			SetFocus(hEdit1);
			SendMessage(hEdit1, EM_SETSEL, 0, -1);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

// TCP 클라이언트 시작 부분
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	// 윈속 초기화

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");	
	
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("connect()");
		
	// 서버와 데이터 통신
	while(1){
		// 쓰기 완료를 기다림
		WaitForSingleObject(hWriteEvent, INFINITE);

		// 문자열 길이가 0이면 보내지 않음
		if(strlen(buf) == 0){
			// OK 버튼 활성화
			EnableWindow(hOKButton, TRUE);
			// 읽기 완료를 알림
			SetEvent(hReadEvent);
			continue;
		}

		// 데이터 보내기
		/*retval = send(sock, buf, strlen(buf), 0);
		if(retval == SOCKET_ERROR){
			err_display("send()");
			break;
		}*/
		DisplayText(hEdit2, "[TCP 클라이언트] %d바이트를 보냈습니다.\r\n", retval);

		retval = send(sock, (char*)&g_PlayerDate, sizeof(g_PlayerDate), 0);
		if(retval == SOCKET_ERROR){
			err_display("send()");
			break;
		}

		// 데이터 받기
		//retval = recvn(sock, buf, retval, 0);
		//if(retval == SOCKET_ERROR){
		//	err_display("recv()");
		//	break;
		//}
		//else if(retval == 0){
		//	break;
		//}
		//
		//// 받은 데이터 출력
		//buf[retval] = '\0';
		//DisplayText("[TCP 클라이언트] %d바이트를 받았습니다.\r\n", retval);
		//DisplayText("[받은 데이터] %s\r\n", buf);

		// OK 버튼 활성화
		EnableWindow(hOKButton, TRUE);
		// 읽기 완료를 알림
		SetEvent(hReadEvent);
	}

	return 0;
}

DWORD WINAPI UDPClientMain(LPVOID arg)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");	
	
	// 소켓 주소 구조체 초기화
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[BUFSIZE+1];
	int len;

	// 서버와 데이터 통신
	while(1){
		// 데이터 입력
		
		strcpy(buf, " ");

		// '\n' 문자 제거
		len = strlen(buf);
		if(buf[len-1] == '\n')
			buf[len-1] = '\0';
		if(strlen(buf) == 0)
			break;

		// 데이터 보내기
		retval = sendto(sock, buf, strlen(buf), 0, 
			(SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if(retval == SOCKET_ERROR){
			err_display("sendto()");
			continue;
		}
		DisplayText(hEdit6, "[UDP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, 
			(SOCKADDR *)&peeraddr, &addrlen);
		if(retval == SOCKET_ERROR){
			err_display("recvfrom()");
			continue;
		}

		// 송신자의 IP 주소 체크
		if(memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))){
			printf("[오류] 잘못된 데이터입니다!\n");
			continue;
		}

		// 받은 데이터 출력
		buf[retval] = '\0';
		DisplayText(hEdit6, "[UDP 클라이언트] %d바이트를 받았습니다.\n", retval);
		DisplayText(hEdit6, "[받은 데이터] %s\n", buf);

		
		_sleep(300);
	}

	// closesocket()
	closesocket(sock);

	return 0;
}

// 에디트 컨트롤에 문자열 출력
void DisplayText(HWND handle, char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	
	char cbuf[BUFSIZE+256];
	vsprintf(cbuf, fmt, arg);
	
	int nLength = GetWindowTextLength(handle);
	SendMessage(handle, EM_SETSEL, nLength, nLength);
	SendMessage(handle, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
	
	va_end(arg);
}

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	DisplayText(hEdit2, "[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR) 
			return SOCKET_ERROR;
		else if(received == 0) 
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}