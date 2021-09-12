//��������� lab7.exe ������������ �������������� ����� ASCII �
//Unicode � �������������� ������������ ����� - ������ � ������� ��� -
//������ ����������.������ ���������� ������� �����������, ��� ����
//����� ��� ���� �������� ��� �������� ����������.������ ���������
//������������ �� ��������� ������ � ���� : lab7.exe filel file2.
#include "pch.h"
#include "EvryThng.h"
#define MAX_OVRLP 4
#define REC_SIZE 64
#define UREC_SIZE 2 * REC_SIZE
static VOID WINAPI ReadDone(DWORD, DWORD, LPOVERLAPPED);
static VOID WINAPI WriteDone(DWORD, DWORD, LPOVERLAPPED);
/* ������ ��������� ���������� ������������� ��� ������, � ������ ���
������. ��������� � ������ ���������� ��� ������ �������������
��������. */
OVERLAPPED OverLapIn[MAX_OVRLP], OverLapOut[MAX_OVRLP];
CHAR AsRec[MAX_OVRLP][REC_SIZE];
WCHAR UnRec[MAX_OVRLP][REC_SIZE];
HANDLE hInputFile, hOutputFile;
LONGLONG nRecord, nDone;
LARGE_INTEGER FileSize;
LARGE_INTEGER CurPosIn, CurPosOut;
DWORD ic;
int _tmain(int argc, LPTSTR argv[])
{
	hInputFile = CreateFile(argv[1], GENERIC_READ,
		0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	hOutputFile = CreateFile(argv[2], GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	FileSize.LowPart = GetFileSize(hInputFile,
		(LPDWORD)FileSize.HighPart);
	nRecord = FileSize.QuadPart / REC_SIZE;
	printf("nR=%d\n", (int)nRecord);
	if ((FileSize.QuadPart % REC_SIZE) != 0) nRecord++;
	CurPosIn.QuadPart = 0;
	printf("nR2=%d\n", (int)nRecord);
	for (ic = 0; ic < MAX_OVRLP; ic++)
	{
		printf("ic=%d\n", (int)ic);
		OverLapIn[ic].hEvent = (HANDLE)ic; /* ������������ �������. */
		OverLapOut[ic].hEvent = (HANDLE)ic;
		OverLapIn[ic].Offset = CurPosIn.LowPart;
		OverLapIn[ic].OffsetHigh = CurPosIn.HighPart;
		if (CurPosIn.QuadPart < FileSize.QuadPart)

			ReadFileEx(hInputFile, AsRec[ic], REC_SIZE, &OverLapIn[ic], ReadDone);
		CurPosIn.QuadPart += (LONGLONG)REC_SIZE;
	}
	/* ��� �������� ������ �����������. ������ ��������� ��������
	���������� � ����������, ���� �� ����� ���������� ��� ������. */
	nDone = 0;
	while (nDone < 2 * nRecord)
		SleepEx(INFINITE, TRUE);
	CloseHandle(hInputFile);
	CloseHandle(hOutputFile);
	_tprintf(_T("ASCII in Unicode is completed\n"));
	return 0;
}


static VOID WINAPI ReadDone(DWORD Code, DWORD nBytes,
	LPOVERLAPPED pOv)
{
	/* ������ ���������. ����������� ������ � �������� ������. */
	DWORD i;
	nDone++;
	printf("nD1=%d\n", (int)nDone);
	/* ��������� ������ � ������ �������� ������. */
	ic = (DWORD)(pOv->hEvent);
	CurPosIn.LowPart = OverLapIn[ic].Offset;
	CurPosIn.HighPart = OverLapIn[ic].OffsetHigh;
	CurPosOut.QuadPart = (CurPosIn.QuadPart / REC_SIZE) * UREC_SIZE;
	OverLapOut[ic].Offset = CurPosOut.LowPart;
	OverLapOut[ic].OffsetHigh = CurPosOut.HighPart;
	/* �������������� ������ ASCII � Unicode. */
	for (i = 0; i < nBytes; i++)
		UnRec[ic][i] = AsRec[ic][i];
	WriteFileEx(hOutputFile, UnRec[ic], nBytes * 2,
		&OverLapOut[ic], WriteDone);
	/* ���������� ��������� ���������� � ���������� ������. */
	CurPosIn.QuadPart += REC_SIZE * (LONGLONG)(MAX_OVRLP);
	OverLapIn[ic].Offset = CurPosIn.LowPart;
	OverLapIn[ic].OffsetHigh = CurPosIn.HighPart;
	return;
}

static VOID WINAPI WriteDone(DWORD Code, DWORD nBytes,LPOVERLAPPED pOv){
		/* ������ ���������. �������� ��������� ������. */
		/*LARGE_INTEGER CurPosIn;*/
		/*DWORD ic;*/
	nDone++;
	printf("nD2=%d\n", (int)nDone);
	ic = (DWORD)(pOv->hEvent);
	CurPosIn.LowPart = OverLapIn[ic].Offset;
	CurPosIn.HighPart = OverLapIn[ic].OffsetHigh;
	if (CurPosIn.QuadPart < FileSize.QuadPart)
	{
		ReadFileEx(hInputFile, AsRec[ic], REC_SIZE, &OverLapIn[ic], ReadDone);
	}
	return;
}