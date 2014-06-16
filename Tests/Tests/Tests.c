#include "stdio.h"
#include "stdlib.h"
#include "windows.h"
#include <strsafe.h>
#include <float.h>

#define BUF_SIZE 1024

int RunProgramm(char *cmdLine, char *buf, int bufSize)
{
	DWORD readBytes;
	DWORD avail;
	DWORD error;
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	HANDLE read_stdout,write_stdout;
	SECURITY_ATTRIBUTES sa;
	PROCESS_INFORMATION pi;
	STARTUPINFO info;

	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = 1;

	CreatePipe(&read_stdout,&write_stdout,&sa,0);

	memset(&info, 0, sizeof(STARTUPINFO));
	info.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
	info.wShowWindow = SW_SHOWNORMAL;
	info.hStdOutput = write_stdout;
	info.hStdError = write_stdout;   

	if( CreateProcess(NULL,cmdLine,NULL,NULL,TRUE,NULL,NULL,NULL,&info,&pi) == 0)
	{
		error = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		// Display the error message and exit the process

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)"create process error") + 40) * sizeof(TCHAR)); 
		StringCchPrintf((LPTSTR)lpDisplayBuf, 
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"), 
			"create process error", error, lpMsgBuf); 
		printf("create process error %s", (LPCTSTR)lpDisplayBuf);
		getchar();
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	PeekNamedPipe(read_stdout,buf,bufSize-1,&readBytes,&avail,NULL);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(write_stdout);
	CloseHandle(read_stdout);

	return readBytes;
}

int main(int argc, char* argv[])
{
	long count, i;
	int k;
	float time = 0, element = 0;
	double minTime = DBL_MAX;
	char *app;
	char cmdLine[100];
	char buf[BUF_SIZE];
	int readBytes;
	long startSize, finishSize, dSize, seed;
	int sortNum;
	FILE *fileTimes, *fileVals; 
	char fname[100];
	char wBuf[100];
	
	if(argc < 5)
	{
		printf("Error!\n");
		printf("Usage: Tests.exe <arg1> <arg2> <arg3> <arg4> <arg5>\n");
		printf("  <arg1> - start size\n");
		printf("  <arg2> - finish size\n");
		printf("  <arg3> - dsize\n");
		printf("  <arg4> - random seed\n");
		printf("  <arg5> - sort number:\n");
		printf("		1 - insertion sort\n");
		printf("		2 - merge sort\n");
		printf("		3 - std quick sort\n");
		getchar();
		return 0;
	}

	app = "Sorts.exe";
	startSize = atoi(argv[1]);
	finishSize = atoi(argv[2]);
	dSize = atoi(argv[3]);
	seed = atoi(argv[4]);
	sortNum = atoi(argv[5]);
	
	/*
	app = "Sorts.exe";
	startSize = 10000;
	finishSize = 10000;
	dSize = 10000;
	seed = 145;
	sortNum = 1;
	*/
	sprintf(fname, "outputTimes%d.txt", sortNum);
	fileTimes = fopen(fname,"w+");
	if(fileTimes == NULL)
	{
		printf("cannot open file");
		getchar();
	}
	sprintf(fname, "outputVals%d.txt", sortNum);
	fileVals = fopen(fname,"w+");

	for(i=0; i<=finishSize/dSize-startSize/dSize; i++)
	{
		minTime = DBL_MAX;
		count = startSize + i*dSize;
		sprintf(cmdLine, "%s %d %d %d", app, count, seed, sortNum);
		for(k=0; k<3; k++)
		{
			memset(buf,0,sizeof(buf));
			readBytes = RunProgramm(cmdLine, buf, 1024);
			sscanf(buf, "%e %e", &time, &element);
			if(time < minTime)
				minTime = time;
			printf(buf);
			printf( "%d; %g\n", count, time );
		}

		sprintf(wBuf, "%d; %g\n", count, minTime*1000);
		fwrite(wBuf, sizeof(wBuf[0]), strlen(wBuf), fileTimes );

		sprintf(wBuf, "%d; %g\n", count, element);
		fwrite(wBuf, sizeof(wBuf[0]), strlen(wBuf), fileVals );
	}
	
	fclose(fileTimes);
	fclose(fileVals);
	
	getchar();
	return 0;
}