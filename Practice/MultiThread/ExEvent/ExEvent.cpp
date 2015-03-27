#include <Windows.h>
#include <stdio.h>

#define BUFSIZE 10

HANDLE hReadEvent;
HANDLE hWriteEvent;
int buf[BUFSIZE];

DWORD WINAPI WriteThread(LPVOID arg)
{
	DWORD retval;
	for(int k = 1; k <= 500; k++){
		retval = WaitForSingleObject(hReadEvent, INFINITE);
		if(retval != WAIT_OBJECT_0) break;

		for(int i = 0; i < BUFSIZE; i++){
			buf[i] = k;
		}

		SetEvent(hWriteEvent);
	}
	return 0;
}

DWORD WINAPI ReadThread(LPVOID arg)
{
	DWORD retval;

	while(1){
		retval = WaitForSingleObject(hWriteEvent, INFINITE);
		if(retval != WAIT_OBJECT_0) break;

		printf("Thread %4d: ", GetCurrentThreadId());
		for(int i = 0; i < BUFSIZE; i++)
			printf("%3d ", buf[i]);
		printf("\n");

		ZeroMemory(buf, sizeof(buf));

		SetEvent(hReadEvent);
	}

	return 0;
}

int main()
{
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(hReadEvent == NULL) return -1;
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(hWriteEvent == NULL) return -1;

	HANDLE hThread[3];
	DWORD ThreadId[3];
	hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, &ThreadId[0]);
	hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, &ThreadId[1]);
	hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, &ThreadId[2]);

	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	CloseHandle(hReadEvent);

	printf("작업 완료\n");
	return 0;
}