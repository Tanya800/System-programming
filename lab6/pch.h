#pragma once
#pragma once
#pragma once
//��������� �������������

#include "EvryThng.h"
//����� ���������� ����� ���������� �EvryThng.h�

/* Envirmnt.h - ������������ UNICODE _MT. */
/* ����� � ����� ���������� UNICODE � �������. */
/* �������� ������� Project... Settings ... C/C++ � � ����
Project Options ����� �������� ����� /D "UNICODE". */
/* �������� �� �� ��� _�� � _STATIC_LIB. */
//#define UNICODE
#undef UNICODE
#ifdef UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#undef _UNICODE
#endif
//#define _MT
#define _STATICLIB
/* ����������� _STATICLIB ��� ������������ �����������
���������� ��� ���������� � ���. */
//#undef_STATICLIB
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>
/* "Support.h" */

/* ����������� ���� ���������� �������� � ��������� ������� ����-
�� ���������� ��� ���� �������� ��������. */

/* ����� ���������� ������� UTILITY_EXPORTS �
_STATICLIB HE �����, � � �������, ������ �� ��������
���������� �����. */

/* ������ "UTILITY_EXPORTS" ������������ � Dev Studio ��� �����-
��� ������� DLL � ������ "Utility" � ������������ � ��������� ������

�. */
//#define UTILITY_EXPORTS ����������������; �����������
// ���� ������ � �������.
#define UTILITY_EXPORTS
#ifdef UTILITY_EXPORTS
#define LIBSPEC _declspec (dllexport)
#else
#define LIBSPEC _declspec (dllimport)
#endif
#define EMPTY _T ("")
#define YES _T ("y")
#define NO _T ("n")
#define CR OxOD
#define LF OxOA
#define TSIZE sizeof (TCHAR)
/* ������� � ���������. */
#define TYPE_FILE 1 /* ������������ � Is, rm � IsFP. */
#define TYPE_DIR 2
#define TYPE_DOT 3
#define MAX_OPTIONS 20
/* ������������ ���-�� ����� ��������� ������. */
#define MAX_ARG 1000
/* ������������ ���-�� ���������� ��������� ������. */
#define MAX_COMMAND_LINE MAX_PATH+50
/* ������������ ������ ��������� ������. */
/* ������� ������ ����������. */
LIBSPEC BOOL ConsolePrompt(LPCTSTR, LPTSTR, DWORD, BOOL);
LIBSPEC BOOL PrintStrings(HANDLE, ...);
LIBSPEC BOOL PrintMsg(HANDLE, LPCTSTR);
LIBSPEC VOID ReportError(LPCTSTR, DWORD, BOOL);