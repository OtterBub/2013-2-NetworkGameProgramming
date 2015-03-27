#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUFSIZE 512

#define START_PORT 0
#define END_PORT 20000

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
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main()
{
	// 2013�� 10�� 5�� ����� ���� 6�� 30�п� �ּ��� �ޱ� �����Ͽ����ϴ�.

	int retval; // �Լ����� ���� ���� �ޱ� ���� ���Դϴ�.

	int portOpen[100] = {0,}; // ��Ʈ�� ���� �Ǿ� �ִ� ���� �޾��ֱ� ���� �迭�Դϴ�.
	int countOpenPort = 0; // �迭�� ���� ���� ���µ� ��Ʈ�� ���ؼ� ī��Ʈ�� ���ݴϴ�.

	// ��Ʈ ��ĵ ������ �������ֱ� ���� �����Դϴ�.
	int startPort = 0;
	int endPort = 0;

	// ������ ������ �ޱ����� ���� �迭�Դϴ�.
	char hostname[100] = {0,};

	// ������ ������ �ʱ�ȭ ���ݴϴ�.
	WSADATA wsa;	
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		// ���� WSAStartup�� 0�� �ƴ� �ٸ� ���� �����ϸ� ������ ���� �ʱ�ȭ �������� ������ �� ������ �����Ͽ�
		// -1�� �����Ͽ� �ü���� ���α׷��� �������� ���� �Ǿ����� �˸��ϴ�.
		return -1;

	// ��Ʈ ��ĵ ������ �ּ�ġ�� �������ݴϴ�.
	printf("set start Port : ");
	scanf("%d", &startPort);
	fflush(stdin);

	// ��Ʈ ��ĵ ������ �ִ�ġ�� �������ݴϴ�.
	printf("set end Port : ");
	scanf("%d", &endPort);
	fflush(stdin);

	// ������ ������ �޾��ݴϴ�.
	printf("domain : ");
	gets(hostname);
	fflush(stdin);

	
	// ip ��巹���� domain name �Ѵ� �޾� �ַ��� �Ͽ��ٰ� �˻� �ð��� �ٰ��� �ּ� ó�� �� �κ��Դϴ�.

	//if(isdigit(hostname[0])){
	//	//serveraddr.sin_addr.s_addr = inet_addr(hostname);
	//	SOCKADDR_IN addr;
	//	ZeroMemory(&addr, sizeof(addr));
	//	
	//	inet_pton(AF_INET, hostname, &addr);
	//	host = gethostbyaddr((char*) &addr, 4, AF_INET);
	//	
	//	if(host == NULL){
	//		err_display("err : gethostbyname()");
	//		exit(1);
	//	}
	//	printf("inet_addr()\n");

	//} else {
	//	host = gethostbyname(hostname);
	//	printf("gethostbyname()\n");
	//	if(host == NULL){
	//		err_display("err : gethostbyname()");
	//		exit(1);
	//	}
	//	
	//}	

	// gethostbyname �Լ����� �������ִ� HOSTENT ������ ����ü�� �޾��ֱ� ���� ������ ����ü �����Դϴ�.
	HOSTENT *host;

	// connect�� ���ڷ� �� ������ ������� �ֱ� ���� ipv4 ������ ������ �ִ� �����Դϴ�.
	// ���������δ� sockaddr�̶�� ����ü�� union���ν� �޸𸮸� �����ϰ� �ֽ��ϴ�.
	// �Ͽ� char sa_data[14] ��� ������ ipv4 �������� ���� ������ ��ȯ���ִ� ������ ���ְ� �ִ� ����ü �Դϴ�.
	SOCKADDR_IN serveraddr;

	// inet_ntop �Լ��� ���ڷ� ���� ��巹�� ��Ʈ���� �޾��ֱ� ���� ���� �迭�Դϴ�.
	char addrstring[100];

	// ��Ʈ��ũ�� ���۵� ����Ʈ �̱� ������ ����ϰ� �޸� ������ ���ݴϴ�.
	ZeroMemory(&serveraddr, sizeof(serveraddr));

	// ��� �޾Ҵ� ������ ������ ���Ͽ� HOSTENT ����ü�� �޾� ȣ��Ʈ�� ���� ������ ����ϴ�.
	host = gethostbyname(hostname);

	// gethostbyname() �Լ��� ���� �Ͽ����� ��Ÿ���ݴϴ�.
	printf("gethostbyname()\n");

	// host������ NULL �Ͻÿ� gethostbyname���� ȣ��Ʈ�� ���� ������ ���� ���� ���̹Ƿ� ������ ó���Ͽ� ���α׷��� �������ݴϴ�.
	if(host == NULL){
		// gethostbyname() �Լ����� ������ ���ٰ� ����Ʈ ���ݴϴ�.
		err_display("err : gethostbyname()");
		// ���α׷��� �����ŵ�ϴ�.
		exit(1);
	}

	// ������ �ּҸ� �߸� ���� ���� �ڵ��Դϴ�.
	//serveraddr.sin_addr.s_addr = inet_addr(host->h_addr_list[0]);
	
	// ��Ʈ��ũ ����Ʈ�� ���ĵǾ��ִ� host->h_addr_list[0]�� ȣ��Ʈ ����Ʈ�� ������ �Ͽ� addrstring�� �־� �ݴϴ�.
	// inet_ntop�� ���� ���� ���� ip ������ ����, ��Ʈ��ũ ����Ʈ ���� �Ǿ��ִ� ��巹��, ���ڿ��� �޾��� ���ڹ迭, ���ڹ迭 ������ �ִ� ũ�� �Դϴ�.
	inet_ntop(host->h_addrtype, host->h_addr_list[0], addrstring, 100);

	// �������� �ְ� ���� ip������ �����ݴϴ�.
	serveraddr.sin_family = AF_INET;

	// ��� inet_ntop���� �޾��� addrstring�� ��Ʈ��ũ ����Ʈ ������ ���־� �����ݴϴ�.
	serveraddr.sin_addr.s_addr = inet_addr(addrstring);

	// AF_INET = ipv4 �������� �����ݴϴ�.
	// SOCK_STREAM TCP ��������, �� ������ �սǾ��� ���� ������ �������� ���ϴ�.
	// 3��° ���ڷ� ���������� �������ټ� ������ �ι�° ���ڷ� ���Ͽ� �̹� ���������� ������ �� �ֱ� ������ 0���� �ξ ������ �����˴ϴ�.
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	// INVALID_SOCKET�� ������ �߻��Ұ�쿡 socket()�Լ��� �������ִ� ���Դϴ�.
	// � ���������� ���ؼ��� WSAGetLastError �� ���Ͽ� �� �� �ֽ��ϴ�.
	if(sock == INVALID_SOCKET) err_quit("socket()");

	// �������� ��Ʈ ��ĳ�� �����Դϴ�.
	// i�� �ε��� ������ ��� �� ���ٿ��� �޾��� startPort ���� �����Ͽ� endPort��� ���� ���ų� ���������� ���� �մϴ�.
	for(int i = startPort; i <= endPort; i++){
		
		// �̿� ���ؼ��� ���ٿ� �����Ͽ����ϴ�.
		// �� ���� ������ ������ �ΰ� �����Ǿ� �̻��� ����� ���Ծ��� �˾� ���� ���Ͽ� �־������ϴ�.
		// �׷��� ����� ���ҽ��ϴ�.
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if(sock == INVALID_SOCKET) err_quit("socket()");

		// serveraddr���� connect�� �� �� ������ �ϱ� ���� �ٸ� ��� �������� ������ �������־��� ������
		// �̰������� ��Ʈ��ũ ����Ʈ ���ķ� ��ĵ�� ��Ʈ�� �ٽ� ������ �Ͽ� connect() �Լ��� ������ �غ� �մϴ�.
		serveraddr.sin_port = htons(i);

		// �� �� ��Ʈ�� ���� ��ĵ�ǰ� �ִ� ������ �˷��ݴϴ�.
		printf("%d port scanning... \n", i);

		// connet�Լ��� �Ʊ� �������� serveraddr�̶� ������ ���Ͽ� �����ϰ� ���� ������ ������ �õ��մϴ�.
		retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

		// retval�� 0�̶�� ���� connect �Լ��� 0�� �����Ͽ��ٴ� ���Դϴ�.
		// connect�Լ��� 0�� �����ϸ� ������ ������ ����Ǿ��ٴ� ���Դϴ�.
		if(retval == 0) {
			// ������ ����� ��Ʈ�� ��ȣ�� portOpen[]�̶�� �迭 ������ ���Ͽ� ���� countOpenPort������ �ε��� ��� 0�� �迭���� ��������
			// ���� �ֽ��ϴ�.
			portOpen[countOpenPort++] = i;

			// retval�� 0�̸� ��Ʈ�� ���ȴ� �����Ͽ� is open �̶�� �޼����� �Ʒ� ���ϴ�.
			printf("is open\n");
		}
		// retval�� 0�� �ƴϸ� connect �Լ��� 0�� �ƴ� �ٸ� ���� ���� ���־����Ƿ� ���� ���з� �����մϴ�.
		else {
			// �� ���ῡ �����ߴ��� ���� �ڵ带 �����ݴϴ�.
			printf("Error Code : %d\n" , WSAGetLastError());
			// �� ���ῡ �����޴��� ���� �ڵ带 ������� ������ ���� ���� �޼����� ����ݴϴ�.
			// ��Ƽ ����Ʈ�� �������� �ؾ߸� �ѱ۷� ��Ȯ�� ���Դϴ�.
			err_display("err : connect()");
			//err_quit("err : connect()");
		}
		// for�� �ȿ� �ִ� ������ �ݾ��ݴϴ�.
		// ������ �����ٶ����� �ڿ��� �Ҹ��ϱ� ������ �ݰ� �� �ڿ� ������� �Ͽ� ��Ʈ�� ��ĵ���ݴϴ�.
		closesocket(sock);
	}

	// ��Ʈ ��ĵ�� �Ϸ��� � ������ ���ӿ� ���Ͽ� ��ĵ�� �ߴ��� �����ְ�
	// �������� ��Ʈ ��ĵ ����� �����ְڴ�. ��� �˷��ݴϴ�.
	printf("\n\n%s port scanning result\n\n", hostname);


	// portOpen[]��� �迭 ������ ����Ǿ��ִ� ���� countOpenPort�� ����� Ƚ�� ��ŭ �ٽ� ����� �ݴϴ�.
	for(int i = 0; i < countOpenPort; i++){
		// � ��Ʈ�� ������ �ִ��� portOpen[] �迭 ������ ����� ���� print�Ͽ� �����ݴϴ�.
		printf("%d port is open ", portOpen[i]);
		// ����ġ ���� ���Ͽ� �˾Ƴ� ��Ʈ�� � ��Ʈ������ ���ؼ� �����ݴϴ�.
		switch(portOpen[i])
		{
		case 1:
			printf("TCPMUX \n");
			break;
		case 7:
			printf("ECHO \n");
			break;
		case 9:
			printf("DISCARD \n");
			break;
		case 13:
			printf("DAYTIME \n");
			break;
		case 17:
			printf("QOTD \n");
			break;
		case 19:
			printf("CHARGEN \n");
			break;
		case 20:
			printf("FTP \n");
			break;
		case 21:
			printf("FTP - command port \n");
			break;
		case 22:
			printf("SSH \n");
			break;
		case 23:
			printf("telnet \n");
			break;
		case 25:
			printf("SMTP \n");
			break;
		case 37:
			printf("TIME \n");
			break;
		case 49:
			printf("TACACS \n");
			break;
		case 53:
			printf("DNS \n");
			break;
		case 67:
			printf("BOOTP or DHCP \n");
			break;
		case 68:
			printf("BOOTP or DHCP \n");
			break;
		case 69:
			printf("TFTP \n");
			break;
		case 79:
			printf("Finger \n");
			break;
		case 80:
			printf("HTTP \n");
			break;
		case 81:
			printf("Torpark - Onion routing (unofficial) \n");
			break;
		case 82:
			printf("Torpard - Control (unofficial) \n");
			break;
		case 88:
			printf("Kerberos \n");
			break;
		case 110:
			printf("POP3 (Post Office Protocol v3) \n");
			break;
		case 119:
			printf("NNTP (Network News Transfer Protocol) \n");
			break;
		case 143:
			printf("IMAP (Internet Message Access Protocol) \n");
			break;
		case 465:
			printf("URL Rendezvous Directory for SSM (Cisco protocol) \n");
			break;
		case 563:
			printf("NNTPS (over TLS/SSL) \n");
			break;
		case 587:
			printf("SMPT (e-mail message submission) \n");
			break;
		case 993:
			printf("IMAPS (over TLS/SSL) \n");
			break;
		case 995:
			printf("POP3S (over TLS/SSL) \n");
			break;
		case 8008:
			printf("HTTP Alternate \n");
			break;
		case 8080:
			printf("HTTP Alternate \n");
			break;
		default:
			// �ڵ��߿� ���� ���� ��Ʈ�̰ų� �˷����� ���� ��Ʈ�� �˻� �Ǿ��� ���
			// �˷����� �ʰų� ��� �Ǿ����� ���� ��Ʈ��� �˷��ݴϴ�.
			printf("unknown or dont register port \n");
			break;
		}
		// 1�� ��Ʈ���� ��� ��Ʈ�� �� ��� �Ϸ��� ������ ������ �˷��� ��Ʈ�鸸 ����ҽ��ϴ�.
	}
	
	// �� ������ �ݾ��ݴϴ�.
	WSACleanup();

	// ���α׷��� �� ���� �Ǿ��ٰ� �ü���� �˷��ֱ� ���Ͽ� return 0;�� ���Ͽ� main�Լ��� �������ְ� �մϴ�.
	return 0;

	// 2013�� 10�� 5�� ����� ���� 8�� 09�п� �ּ� �ۼ� �Ϸ��Ͽ����ϴ�.	


	// --- 2013�� 10�� 5�� ����� ���� 8�� 12�� PortScanner.exe �׽�Ʈ ��� ---
	// �׽�Ʈ ��Ʈ ��ȣ 80�� ~ 83�� ��Ʈ
	//
	// ������ �ǵ��ƿ� ����Ʈž���� �׽�Ʈ ��� 80�� ��Ʈ ���� �˻����� �ʾҽ��ϴ�.
	// ���� �ð����� �ٽ� �׽�Ʈ �غ��Ƶ� 80�� ��Ʈ���� �ȵǾ����ϴ�.
	//
	// ��Ʈ��ũ ȯ���� ���� �б��� �޶� �׷��� ��Ʈ���� �Ѽ� �׽�Ʈ �غ�������
	// �ǽ��˻綧�� ���������� 80�� ��Ʈ���� 83�� ��Ʈ���� ��� �����ִٰ� ����� ���Խ��ϴ�.
	//
	// ���̾� ��ũ�� ����ִ� ���� �����ΰ� �;� ����ũž���� ���̾��ũ�� ��ƺ����� �غ���
	// �̹� ������ִ� �����Դϴ�.
	// 
	// ����� �� ��Ʈ���� ��Ʈ��ũ ȯ���� �ſ� �̻��� �� �����ϴ�.
	// ���̷����� �ɷ� ���� ��Ʈ�� �̻��ϰ� ���� �Ͱ� ���� �̻������� �� �����ϴ�.
	// ���� �׷��� ���̷����� ���� �ϴ����� ��Ȯ�� �𸣰ڽ��ϴ�.
	// ������ ������ ��ƮŶ ���̷����� ��� ���Ʈ ��ſ� �ɷ��� ġ���� ������ ����
	// ���̷��� ������ ũ�ٰ� ���� �˴ϴ�.
	// ������ ��Ȯ�� ������ �𸣰ڽ��ϴ�.

	// http://www.yougetsignal.com/tools/open-ports/ ��� ����Ʈ�� ��Ʈ ���� �׽�Ʈ ����Ʈ�Դϴ�.
	// 2013�� 10�� 5�� ����� ����
	// ���̹��� Ping�� ���� ���� ������ �ּ��� 202.131.30.12�� �׽�Ʈ ���
	// 81�� ��Ʈ�� ���� �������� �ʾҽ��ϴ�.
	//
	// http://blog.naver.com/PostView.nhn?blogId=ezmirk&logNo=130083432129
	// ��� ��α׿��� ���� �������� 81�� ��Ʈ�� ���񽺸� �ϰ�
	// http���� ���� ���� �߿� squid�� 80�� ��Ʈ�� ���񽺸� �Ѵٰ� �Ǿ��־
	// ����ũ ž���� �׽�Ʈ �ϱ� ������ �̷��� ���� 81�� ��Ʈ�� �����ٷ� �˾ҽ��ϴ�.
	// �׷��� �ٸ� �ܺ� ���� �׽�Ʈ ��� 
	// 80~83�� ��Ʈ�߿� 80�� ��Ʈ ���� �������� �ʾҽ��ϴ�.
	
}