//Копирование файлов с использованием API Win32

#include <windows.h> // функции и типы данных для Win32
#include <stdio.h>
#define BUF_SIZE 256
int main (int argc, LPTSTR argv [])
{
	HANDLE hIn, hOut; //Всеобъекты Win32 идентифицируютсяпеременнымитипа HANDLE
	DWORD nIn, nOut;
	CHAR Buffer[BUF_SIZE];
	if (argc != 3)
	{
		printf ("Использование: CpW file1 File2\n");
		return 1;
	}
	hIn = CreateFile(argv [1], GENERIC_READ, 0, NULL,OPEN_EXISTING, 0, NULL);
	if (hIn == INVALID_HANDLE_VALUE) //В Win32 определены многочисленные символические константы и флаги
	{
		printf ("Нельзя открыть входной файл. Ошибка: %х\n", GetLastError()); // GetLastError позволяет получить системные коды ошибок в DWORD
		return 2;
	}
	hOut = CreateFile(argv [2], GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);
	if (hOut ==INVALID_HANDLE_VALUE)
	{
		printf ("Нельзя открыть выходной файл. Ошибка: %х\n", GetLastError());
		return 3;
	}
	while (ReadFile(hIn, Buffer, BUF_SIZE, &nIn, NULL) && nIn > 0) //ReadFile возвращает логическое значение, а не количество байт
	{
		WriteFile (hOut, Buffer, nIn, &nOut, NULL);//WriteFile возвращает логическое значение, а не количество байт
		if (nIn != nOut)
		{
			printf ("Неисправимая ошибка записи: %x\n", GetLastError());
			return 4;
		}
	}
	CloseHandle (hIn);
	CloseHandle (hOut);
	return 0;
}