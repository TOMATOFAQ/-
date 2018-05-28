#include<stdio.h>
#include<windows.h>

HANDLE hMutex;
int tickets=100;
DWORD WINAPI Fun1Proc(LPVOID pPararneter)
{
	while(tickets>0)
	{
		WaitForSingleObject(hMutex,INFINITE);
		if(tickets>0)
		{   printf("thread1 sell ticket : %d\n",tickets);  tickets--;  }
		ReleaseMutex(hMutex);
	}
	return 0;
}

DWORD WINAPI Fun2Proc(LPVOID pPararneter)
{
	while(tickets>0)
	{
		WaitForSingleObject(hMutex,INFINITE);
		if(tickets>0)
		{ printf("thread2 sell ticket : %d\n",tickets);   tickets--; }
		ReleaseMutex(hMutex);
	}
	return 0;
}

int main()
{
	HANDLE hThread1,hThread2;
	hMutex=CreateMutex(NULL,FALSE,NULL);
	hThread1=CreateThread(NULL,0,Fun1Proc,NULL ,0,NULL);
	hThread2=CreateThread(NULL,0,Fun2Proc,NULL ,0,NULL);
	CloseHandle(hThread1);
	CloseHandle(hThread2);
	Sleep(4000);
	return 0;
}

