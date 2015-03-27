#include <Windows.h>
#include <stdio.h>

#define MAXCNT 100000
int g_count = 0;

CRITICAL_SECTION cs;


DWORD WINAPI MyThread1(LPVOID arg)
{
	do{
		for(int i = 0; i < MAXCNT; i++){
			//EnterCriticalSection(&cs);
			g_count += 2;
			//LeaveCriticalSection(&cs);
		}
		printf("Thread1::g_count = %d\n", g_count);
		_sleep(0);
	}while(0);
	return 0;
}

DWORD WINAPI MyThread2(LPVOID arg)
{
	do{
		for(int i = 0; i < MAXCNT; i++){
			//EnterCriticalSection(&cs);
			g_count -= 2;
			//LeaveCriticalSection(&cs);
		}
		printf("Thread2::g_count = %d\n", g_count);
		_sleep(0);
	}while(0);
	return 0;
}

int main(int argc, char** argv)
{
	HANDLE hThread[2];

	InitializeCriticalSection(&cs);
	hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	for(int i = 0; i < 2; i++)
		CloseHandle(hThread[i]);
	DeleteCriticalSection(&cs);
	//_sleep(500);

	printf("g_count = %d\n", g_count);
	return 0;
}