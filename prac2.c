#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
// 4 траектория: y1 = 10, y2 = 300 , x1

#define TIMER_ID 1234
#define MS_DOWN 0 // движение вниз
#define MS_TOPRIGHT 1 // движение вправо-вверх
#define MS_TOPLEFT 2 // движение влево-вверх

BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
void ClientDraw(HWND , WPARAM , LPARAM );


HINSTANCE hlnst;
char szClassName[] = "WindowAppClass";
char szChild[] = "ChildWindowAppClass";
WORD xPos, yPos, nSize;
HWND hwnd1;
POINT Base = {50, 0},
      Step = {4, 3};
int MoveState = MS_DOWN;

void NextMoveStep(HWND hwnd) {
// Проверяем текущее состояние
	switch (MoveState) {
	case MS_DOWN: // движение вниз
		// Если координата y достигла значения 300, переходим в состояние «движение вправо-вверх» или «движение влево-вверх»
		if (Base.y > 300) {
			if (Base.x <= 50) MoveState = MS_TOPRIGHT;
			else MoveState = MS_TOPLEFT;
		}
		// а иначе продолжаем движение вниз
		else Base.y += Step.y;
		break;
	case MS_TOPRIGHT: // движение вправо-вверх
		// Если координата y достигла значения 0, переходим в состояние «движение вниз»
		if (Base.y <= 0) MoveState = MS_DOWN;
		// а иначе продолжаем движение вправо
		else {
			Base.x += Step.x;
			Base.y -= Step.y  ;
		}
		break;
	case MS_TOPLEFT: // движение влево-вверх
// Если координата y стала меньше 0, переходим в исходное состояние «движение вниз»
		if (Base.y <= 0) MoveState = MS_DOWN;
		// а иначе продолжаем движение влево-вверх
		else {
			Base.x -= Step.x;
			Base.y -= Step.y;
		}
		break;
// Если по какой-то причине переменная состояния имеет иное
// значение, возвращаем её в исходное состояние
	default: MoveState = MS_DOWN;
	}
// помечаем всю клиентскую область окна, как требующую перерисовки
	InvalidateRect(hwnd, NULL, 1);
}

int WINAPI WinMain(HINSTANCE hlnstance, HINSTANCE hPrevlnstance, LPSTR lpszCmdLine, int nCmdShow) {
	MSG msg;
	HWND hwnd;
	hlnst = hlnstance;

	if (!RegClass(WndProc, szClassName, COLOR_WINDOW)) return FALSE;

	hwnd = CreateWindow(szClassName, "PracticTask1",
	                    WS_OVERLAPPEDWINDOW | WS_VISIBLE , CW_USEDEFAULT, CW_USEDEFAULT,
	                    CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hlnstance, NULL);

	if (!hwnd) return FALSE;
	SetTimer(hwnd, TIMER_ID, 30, NULL);
	while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
// По окончанию работы программы уничтожаем таймер
	KillTimer(hwnd, TIMER_ID);
	return msg.wParam;
}

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground) {

	WNDCLASS wc;
	wc.style = wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.lpfnWndProc = Proc;
	wc.hInstance = hlnst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = (LPCTSTR)NULL;
	wc.lpszClassName = szName;
	return (RegisterClass(&wc) != 0);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
                         WPARAM wParam, LPARAM lParam) {


	HDC hdc1;
	PAINTSTRUCT ps;
	RECT Rect;
	HMENU hmenu;

	switch (msg) {
	case WM_TIMER: {
		if (wParam == TIMER_ID) // если сообщение от «нужного» таймера,
			NextMoveStep(hwnd); // то выполняем очередной шаг перемещения
		break;
	}
	case WM_PAINT: {
		ClientDraw(hwnd, wParam, lParam);
		break;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	default:
		DefWindowProc(hwnd, msg, wParam, lParam);
	}

}



void ClientDraw(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	// char str[] = '0' + Base.x;
	char buff[20];
  int a = Base.x;
  char *p;

  p = itoa(a, buff, 10); 

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	TextOut(hdc, 50, 20,p, strlen(p));
//
	LOGBRUSH lb; // структура, описывающая заливку линии
	lb.lbStyle = BS_SOLID; // стиль заливки - сплошной
	lb.lbColor = RGB(255, 255, 0); // цвет - красный
	lb.lbHatch = 0; // штриховки нет

	int	dx = 100,
	    dy = 90;
	int TopY = Base.y,
	    DownY = Base.y + dy,
	    MinX = Base.x,
	    MaxX = Base.x + dx;

	HGDIOBJ hPen1 = ExtCreatePen(PS_GEOMETRIC | PS_DASH, 5, &lb, 0, NULL);// создаём перо1: пунктирная линия (PS_SOLID), ширина 5
	SelectObject(hdc, hPen1);// выбираем новое перо (1)
	MoveToEx(hdc, MinX, TopY, NULL);
	LineTo(hdc, MaxX, DownY );
	MoveToEx(hdc, MaxX, TopY,  NULL);
	LineTo(hdc, MinX, DownY);

	lb.lbColor = RGB(0, 0, 0); // цвет - черный
	HGDIOBJ hPen2 = ExtCreatePen(PS_GEOMETRIC | PS_DOT, 1, &lb, 0, NULL);
	SelectObject(hdc, hPen2);
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	// MoveToEx(hdc, 200, 20, NULL);
	Rectangle(hdc, MinX, TopY, MaxX, DownY);

	// Уничтожаем созданные нами перья (освобождаем ресурсы)
	DeleteObject(hPen1);
	DeleteObject(hPen2);
	EndPaint(hwnd, &ps);
}



