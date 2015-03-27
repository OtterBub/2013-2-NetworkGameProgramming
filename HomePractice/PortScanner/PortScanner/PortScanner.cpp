#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUFSIZE 512

#define START_PORT 0
#define END_PORT 20000

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
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main()
{
	// 2013년 10월 5일 토요일 오후 6시 30분에 주석을 달기 시작하였습니다.

	int retval; // 함수에서 리턴 값을 받기 위한 값입니다.

	int portOpen[100] = {0,}; // 포트가 오픈 되어 있는 값을 받아주기 위한 배열입니다.
	int countOpenPort = 0; // 배열을 쓰기 위해 오픈된 포트에 대해서 카운트를 해줍니다.

	// 포트 스캔 범위를 지정해주기 위한 변수입니다.
	int startPort = 0;
	int endPort = 0;

	// 도메인 네임을 받기위한 문자 배열입니다.
	char hostname[100] = {0,};

	// 윈도우 소켓을 초기화 해줍니다.
	WSADATA wsa;	
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		// 만일 WSAStartup이 0이 아닌 다른 값을 리턴하면 윈도우 소켓 초기화 과정에서 에러가 난 것으로 간주하여
		// -1을 리턴하여 운영체제로 프로그램이 비정상적 종료 되었음을 알립니다.
		return -1;

	// 포트 스캔 범위의 최소치를 설정해줍니다.
	printf("set start Port : ");
	scanf("%d", &startPort);
	fflush(stdin);

	// 포트 스캔 범위의 최대치를 설정해줍니다.
	printf("set end Port : ");
	scanf("%d", &endPort);
	fflush(stdin);

	// 도메인 네임을 받아줍니다.
	printf("domain : ");
	gets(hostname);
	fflush(stdin);

	
	// ip 어드레스와 domain name 둘다 받아 주려고 하였다가 검사 시간이 다가와 주석 처리 한 부분입니다.

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

	// gethostbyname 함수에서 리턴해주는 HOSTENT 포인터 구조체를 받아주기 위한 포인터 구조체 변수입니다.
	HOSTENT *host;

	// connect의 인자로 들어가 서버로 연결시켜 주기 위한 ipv4 구조를 가지고 있는 변수입니다.
	// 내부적으로는 sockaddr이라는 구조체와 union으로써 메모리를 공유하고 있습니다.
	// 하여 char sa_data[14] 라는 변수를 ipv4 구조에서 쉽게 쓰도록 변환해주는 역할을 해주고 있는 구조체 입니다.
	SOCKADDR_IN serveraddr;

	// inet_ntop 함수의 인자로 들어가서 어드레스 스트링을 받아주기 위한 문자 배열입니다.
	char addrstring[100];

	// 네트워크로 전송될 바이트 이기 때문에 깔끔하게 메모리 정리를 해줍니다.
	ZeroMemory(&serveraddr, sizeof(serveraddr));

	// 방금 받았던 도메인 네임을 통하여 HOSTENT 구조체를 받아 호스트에 대한 정보를 얻습니다.
	host = gethostbyname(hostname);

	// gethostbyname() 함수가 동작 하였음을 나타내줍니다.
	printf("gethostbyname()\n");

	// host변수가 NULL 일시에 gethostbyname에서 호스트에 대한 정보를 얻지 못한 것이므로 에러로 처리하여 프로그램을 종료해줍니다.
	if(host == NULL){
		// gethostbyname() 함수에서 에러가 났다고 프린트 해줍니다.
		err_display("err : gethostbyname()");
		// 프로그램을 종료시킵니다.
		exit(1);
	}

	// 기존에 주소를 잘못 전달 받은 코드입니다.
	//serveraddr.sin_addr.s_addr = inet_addr(host->h_addr_list[0]);
	
	// 네트워크 바이트로 정렬되어있던 host->h_addr_list[0]를 호스트 바이트로 재정렬 하여 addrstring에 넣어 줍니다.
	// inet_ntop의 인자 값을 각각 ip 버전의 정보, 네트워크 바이트 정렬 되어있는 어드레스, 문자열을 받아줄 문자배열, 문자배열 변수의 최대 크기 입니다.
	inet_ntop(host->h_addrtype, host->h_addr_list[0], addrstring, 100);

	// 서버에서 주고 받을 ip버전을 정해줍니다.
	serveraddr.sin_family = AF_INET;

	// 방금 inet_ntop에서 받아준 addrstring을 네트워크 바이트 정렬을 해주어 보내줍니다.
	serveraddr.sin_addr.s_addr = inet_addr(addrstring);

	// AF_INET = ipv4 버전으로 열어줍니다.
	// SOCK_STREAM TCP 프로토콜, 즉 데이터 손실없이 전달 가능한 소켓으로 엽니다.
	// 3번째 인자로 프로토콜을 지정해줄수 있지만 두번째 인자로 인하여 이미 프로토콜이 지정될 수 있기 때문에 0으로 두어도 소켓이 생성됩니다.
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	// INVALID_SOCKET은 에러가 발생할경우에 socket()함수가 리턴해주는 값입니다.
	// 어떤 에러인지에 대해서는 WSAGetLastError 를 통하여 알 수 있습니다.
	if(sock == INVALID_SOCKET) err_quit("socket()");

	// 본격적인 포트 스캐닝 루프입니다.
	// i를 인덱스 변수로 삼아 맨 윗줄에서 받아준 startPort 부터 시작하여 endPort라는 값과 같거나 작을때까지 실행 합니다.
	for(int i = startPort; i <= endPort; i++){
		
		// 이에 대해서는 윗줄에 설명하였습니다.
		// 또 넣은 이유는 소켓이 두개 생성되어 이상한 결과가 나왔었나 알아 보기 위하여 넣었었습니다.
		// 그러나 결과는 같았습니다.
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if(sock == INVALID_SOCKET) err_quit("socket()");

		// serveraddr에서 connect에 들어가 제 역할을 하기 위해 다른 멤버 변수들은 위에서 설정해주었기 때문에
		// 이곳에서는 네트워크 바이트 정렬로 스캔할 포트만 다시 재지정 하여 connect() 함수에 전달할 준비를 합니다.
		serveraddr.sin_port = htons(i);

		// 몇 번 포트가 현재 스캔되고 있는 것인지 알려줍니다.
		printf("%d port scanning... \n", i);

		// connet함수로 아까 지정해준 serveraddr이란 변수를 통하여 연결하고 싶은 서버에 연결을 시도합니다.
		retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

		// retval이 0이라는 것은 connect 함수가 0을 리턴하였다는 뜻입니다.
		// connect함수가 0을 리턴하면 무사히 서버와 연결되었다는 뜻입니다.
		if(retval == 0) {
			// 무사히 연결된 포트의 번호를 portOpen[]이라는 배열 변수를 통하여 현재 countOpenPort변수를 인덱스 삼아 0번 배열부터 차근차근
			// 집어 넣습니다.
			portOpen[countOpenPort++] = i;

			// retval이 0이면 포트가 열렸다 간주하여 is open 이라는 메세지를 아래 띄웁니다.
			printf("is open\n");
		}
		// retval이 0이 아니면 connect 함수가 0이 아닌 다른 값을 리턴 해주었으므로 연결 실패로 간주합니다.
		else {
			// 왜 연결에 실패했는지 에러 코드를 보여줍니다.
			printf("Error Code : %d\n" , WSAGetLastError());
			// 왜 연결에 실패햇는지 에러 코드를 기반으로 설정한 언어로 에러 메세지를 띄워줍니다.
			// 멀티 바이트로 컴파일을 해야만 한글로 정확히 보입니다.
			err_display("err : connect()");
			//err_quit("err : connect()");
		}
		// for문 안에 있던 소켓을 닫아줍니다.
		// 소켓을 열어줄때마다 자원을 소모하기 때문에 닫고 난 뒤에 재생성을 하여 포트를 스캔해줍니다.
		closesocket(sock);
	}

	// 포트 스캔을 완료후 어떤 도메인 네임에 대하여 스캔을 했는지 보여주고
	// 이제부터 포트 스캔 결과를 보여주겠다. 라고 알려줍니다.
	printf("\n\n%s port scanning result\n\n", hostname);


	// portOpen[]라는 배열 변수에 저장되어있던 값을 countOpenPort에 저장된 횟수 만큼 다시 출력해 줍니다.
	for(int i = 0; i < countOpenPort; i++){
		// 어떤 포트가 열려져 있는지 portOpen[] 배열 변수에 저장된 값을 print하여 보여줍니다.
		printf("%d port is open ", portOpen[i]);
		// 스윗치 문을 통하여 알아낸 포트가 어떤 포트인지에 대해서 보여줍니다.
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
			// 코드중에 넣지 않은 포트이거나 알려지지 않은 포트가 검색 되었을 경우
			// 알려지지 않거나 등록 되어있지 않은 포트라고 알려줍니다.
			printf("unknown or dont register port \n");
			break;
		}
		// 1번 포트부터 모든 포트를 다 등록 하려고 했으나 적당히 알려진 포트들만 써놓았습니다.
	}
	
	// 윈 소켓을 닫아줍니다.
	WSACleanup();

	// 프로그램이 잘 종료 되었다고 운영체제에 알려주기 위하여 return 0;를 통하여 main함수가 리턴해주게 합니다.
	return 0;

	// 2013년 10월 5일 토요일 오후 8시 09분에 주석 작성 완료하였습니다.	


	// --- 2013년 10월 5일 토요일 오후 8시 12분 PortScanner.exe 테스트 결과 ---
	// 테스트 포트 번호 80번 ~ 83번 포트
	//
	// 집으로 되돌아와 데스트탑으로 테스트 결과 80번 포트 뿐이 검색되지 않았습니다.
	// 현재 시각으로 다시 테스트 해보아도 80번 포트뿐이 안되었습니다.
	//
	// 네트워크 환경이 집과 학교가 달라 그런지 노트북을 켜서 테스트 해보았으나
	// 실습검사때와 마찬가지로 80번 포트부터 83번 포트까지 모두 열려있다고 결과가 나왔습니다.
	//
	// 와이어 샤크가 깔려있는 것이 문제인가 싶어 데스크탑에도 와이어샤크를 깔아보려고 해보니
	// 이미 깔려져있는 상태입니다.
	// 
	// 결론은 제 노트북의 네트워크 환경이 매우 이상한 것 같습니다.
	// 바이러스가 걸려 오픈 포트가 이상하게 보이 것과 같은 이상현상인 것 같습니다.
	// 정말 그러한 바이러스가 존재 하는지는 정확히 모르겠습니다.
	// 하지만 이전에 루트킷 바이러스가 계속 어베스트 백신에 걸려서 치료한 것으로 보면
	// 바이러스 영향이 크다고 생각 됩니다.
	// 하지만 정확한 원인은 모르겠습니다.

	// http://www.yougetsignal.com/tools/open-ports/ 라는 사이트는 포트 오픈 테스트 사이트입니다.
	// 2013년 10월 5일 토요일 현재
	// 네이버로 Ping을 보내 얻은 아이피 주소인 202.131.30.12로 테스트 결과
	// 81번 포트는 전혀 열려있지 않았습니다.
	//
	// http://blog.naver.com/PostView.nhn?blogId=ezmirk&logNo=130083432129
	// 라는 블로그에서 실제 웹서버는 81번 포트로 서비스를 하고
	// http가속 모드로 동작 중에 squid를 80번 포트로 서비스를 한다고 되어있어서
	// 데스크 탑으로 테스트 하기 전에는 이러기 위해 81번 포트를 열은줄로 알았습니다.
	// 그러나 다른 외부 툴로 테스트 결과 
	// 80~83번 포트중에 80번 포트 뿐이 열려있지 않았습니다.
	
}