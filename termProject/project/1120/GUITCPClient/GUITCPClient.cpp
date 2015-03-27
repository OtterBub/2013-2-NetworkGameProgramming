#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource.h"
#include "PacketStruct.h"

#define BUFSIZE 512

// ��ȭ���� �޽��� ó�� �Լ�
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
// ����Ʈ ��Ʈ�� ��� �Լ�
void DisplayText(HWND handle, char *fmt, ...);
// ���� ��� �Լ�
DWORD WINAPI ClientMain(LPVOID arg);
// UDP ��� �Լ�
DWORD WINAPI UDPClientMain(LPVOID arg);
// ���� ��� �Լ�
void err_quit(char *msg);
void err_display(char *msg);
// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags);

SOCKET sock; // ����
char buf[BUFSIZE+1]; // ������ �ۼ��� ����
HWND hEdit1, hEdit2, hEdit3, hEdit4, hEdit5, hEdit6, hOKButton; // ����Ʈ ��Ʈ�Ѱ� OK ��ư
HANDLE hReadEvent, hWriteEvent; // �̺�Ʈ

int g_count = 0;
PlayerDate g_PlayerDate;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE,
	LPSTR lpCmdLine, int nCmdShow)
{

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// �̺�Ʈ ����
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(hReadEvent == NULL) return -1;
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(hWriteEvent == NULL) return -1;

	// ���� ����� ����ϴ� ������ ����
	DWORD ThreadId;
	CreateThread(NULL, 0, ClientMain, NULL, 0, &ThreadId);

	CreateThread(NULL, 0, UDPClientMain, NULL, 0, &ThreadId);
	
	// ��ȭ ���� ����
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	
	// �̺�Ʈ ����
	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();	

	return 0;
}

// ��ȭ���� �޽��� ó��
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
			// OK ��ư ��Ȱ��ȭ
			EnableWindow(hOKButton, FALSE);
			// �б� �ϷḦ ��ٸ�
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

			// ���� �ϷḦ �˸�
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

// TCP Ŭ���̾�Ʈ ���� �κ�
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	// ���� �ʱ�ȭ

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
		
	// ������ ������ ���
	while(1){
		// ���� �ϷḦ ��ٸ�
		WaitForSingleObject(hWriteEvent, INFINITE);

		// ���ڿ� ���̰� 0�̸� ������ ����
		if(strlen(buf) == 0){
			// OK ��ư Ȱ��ȭ
			EnableWindow(hOKButton, TRUE);
			// �б� �ϷḦ �˸�
			SetEvent(hReadEvent);
			continue;
		}

		// ������ ������
		/*retval = send(sock, buf, strlen(buf), 0);
		if(retval == SOCKET_ERROR){
			err_display("send()");
			break;
		}*/
		DisplayText(hEdit2, "[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\r\n", retval);

		retval = send(sock, (char*)&g_PlayerDate, sizeof(g_PlayerDate), 0);
		if(retval == SOCKET_ERROR){
			err_display("send()");
			break;
		}

		// ������ �ޱ�
		//retval = recvn(sock, buf, retval, 0);
		//if(retval == SOCKET_ERROR){
		//	err_display("recv()");
		//	break;
		//}
		//else if(retval == 0){
		//	break;
		//}
		//
		//// ���� ������ ���
		//buf[retval] = '\0';
		//DisplayText("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\r\n", retval);
		//DisplayText("[���� ������] %s\r\n", buf);

		// OK ��ư Ȱ��ȭ
		EnableWindow(hOKButton, TRUE);
		// �б� �ϷḦ �˸�
		SetEvent(hReadEvent);
	}

	return 0;
}

DWORD WINAPI UDPClientMain(LPVOID arg)
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");	
	
	// ���� �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// ������ ��ſ� ����� ����
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[BUFSIZE+1];
	int len;

	// ������ ������ ���
	while(1){
		// ������ �Է�
		
		strcpy(buf, " ");

		// '\n' ���� ����
		len = strlen(buf);
		if(buf[len-1] == '\n')
			buf[len-1] = '\0';
		if(strlen(buf) == 0)
			break;

		// ������ ������
		retval = sendto(sock, buf, strlen(buf), 0, 
			(SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if(retval == SOCKET_ERROR){
			err_display("sendto()");
			continue;
		}
		DisplayText(hEdit6, "[UDP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		// ������ �ޱ�
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, 
			(SOCKADDR *)&peeraddr, &addrlen);
		if(retval == SOCKET_ERROR){
			err_display("recvfrom()");
			continue;
		}

		// �۽����� IP �ּ� üũ
		if(memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))){
			printf("[����] �߸��� �������Դϴ�!\n");
			continue;
		}

		// ���� ������ ���
		buf[retval] = '\0';
		DisplayText(hEdit6, "[UDP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		DisplayText(hEdit6, "[���� ������] %s\n", buf);

		
		_sleep(300);
	}

	// closesocket()
	closesocket(sock);

	return 0;
}

// ����Ʈ ��Ʈ�ѿ� ���ڿ� ���
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

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// ����� ���� ������ ���� �Լ�
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