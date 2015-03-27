#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <stdio.h>

struct 
{
	union
	{
		int num;
		char c;
	};

}typedef test;

bool IsLittleEndian()
{
	u_short x = 0x1234;
	u_short x2 = htons(x);

	if(x2 == 0x3412)
		return true;
	else
		return false;
}
bool IsBigEndian()
{
	u_short x = 0x1234;
	u_short x2 = htons(x);

	if(x2 == 0x1234)
		return true;
	else
		return false;
}

int main(int argc, char* argv[])
{
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return -1;

	// IsLittleEndian or IsBigEndian?
	if(IsBigEndian())
		printf("빅 엔디안입니다.\n");

	if(IsLittleEndian())
		printf("리틀 엔디안입니다.\n");

	int x = 1;
	// Little
	// 0001 / 0000
	// Big
	// 0000 / 0001

	// char
	// 0000


	if (*(char *)&x == 1) {
		/* Little-Endian */
		 printf("Little-Endian\n"); 
	}
	else {
		/* Big-Endian  */
		 printf("Big-Endian\n"); 
	}

	test a;
	a.num = 1;

	if( a.c == 1 )
	{
		printf( "LIttle-Endian\n" );
	}
	else
	{
		printf( "Big-Endian\n" );
	}

    WSACleanup();
    return 0;
}