//Копирование файлов с использованием функции полуфабриката Win32

#include <windows.h>
#include <stdio.h>

int main (int argc, LPTSTR argv [])
{
	if (argc != 3)
	{
		printf ("Использование: cpCF file1 file2\n");
		return 1;
	}
	if (!CopyFile (argv [1], argv [2], FALSE))// Функция-полуфабрикат, которая не использует буфер, как в прошлых программах и значительно повышат быстродейстие
	{
		printf ("Ошибка CopyFile: % x\n", GetLastError ());
		return 2;
	}
	return 0;
}