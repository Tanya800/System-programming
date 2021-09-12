#include "windows.h"
// линии : цвет - желтый, тип - пунктир, толщина - 5
// квадрат : цвет - черный , тип - точки, толщина - 4

#define CM_HIDE 1
#define CM_MOVE 2


BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
void ClientDraw(HWND , WPARAM , LPARAM );


HINSTANCE hlnst;
char szClassName[] = "WindowAppClass";
char szChild[] = "ChildWindowAppClass";
WORD xPos, yPos, nSize;
HWND hwnd1;



int WINAPI WinMain(HINSTANCE hlnstance, HINSTANCE hPrevlnstance, LPSTR lpszCmdLine, int nCmdShow) {
	MSG msg;
	HWND hwnd;
	hlnst = hlnstance;

	if (!RegClass(WndProc, szClassName, COLOR_WINDOW)) return FALSE;

	hwnd = CreateWindow(szClassName, "PracticTask1",
	                    WS_OVERLAPPEDWINDOW | WS_VISIBLE , CW_USEDEFAULT, CW_USEDEFAULT,
	                    CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hlnstance, NULL);

	if (!hwnd) return FALSE;
	while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
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
	case WM_PAINT: {
		ClientDraw(hwnd, wParam, lParam);
		break;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);

}



void ClientDraw(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	char str[] = "HELLO!!";
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	TextOut(hdc, 50, 20, str, strlen(str));
	LOGBRUSH lb; // структура, описывающая заливку линии
	lb.lbStyle = BS_SOLID; // стиль заливки - сплошной
	lb.lbColor = RGB(255, 255, 0); // цвет - красный
	lb.lbHatch = 0; // штриховки нет
	int TopY = 400,
			DownY= 700,
			MinX=30,
			MaxX=80;
	HGDIOBJ hPen1 = ExtCreatePen(PS_GEOMETRIC | PS_DASH, 5, &lb, 0, NULL);// создаём перо1: пунктирная линия (PS_SOLID), ширина 5
	SelectObject(hdc, hPen1);// выбираем новое перо (1)
	MoveToEx(hdc, TopY, MinX, NULL);
	LineTo(hdc, DownY, MaxX);
	MoveToEx(hdc, TopY, MaxX, NULL);
	LineTo(hdc, DownY, MinX);

	lb.lbColor = RGB(0, 0, 0); // цвет - черный
	HGDIOBJ hPen2 = ExtCreatePen(PS_GEOMETRIC | PS_DOT, 1, &lb, 0, NULL);
	SelectObject(hdc, hPen2); 
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	// MoveToEx(hdc, 200, 20, NULL);
	Rectangle(hdc, TopY, MinX, DownY, MaxX);
	
	// Уничтожаем созданные нами перья (освобождаем ресурсы)
	DeleteObject(hPen1);
	DeleteObject(hPen2);
	EndPaint(hwnd, &ps);
}



