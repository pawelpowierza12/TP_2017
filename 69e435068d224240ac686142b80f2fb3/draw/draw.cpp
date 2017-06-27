#include "stdafx.h"
#include "draw.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>

#define MAX_LOADSTRING 100
#define TMR_1 1

struct Form
{
	int figure;
	int osX;
	int osY;
	bool orcaught;
};

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
//INT value;
RECT drawArea1 = {100, 100, 900, 750}; 
HWND hwndButton;

int length;
int tabheight[8];
int position;
int caught;
int location;
Form tab[8];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void Initialization()
{
	srand(time(NULL));
	for (int i = 0; i < 8; i++) {
		tab[i].figure = rand() % 3;
		tab[i].osY = 0;
		tab[i].osX = 0;
		tab[i].orcaught = false;
		tabheight[i] = 300;
	}
}

void MyOnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));
	graphics.DrawLine(&pen, 130 + position, 100, 130 + position, 300 + length);								// PODNOŒNIK
	graphics.DrawRectangle(&pen, 80, 100, 1025, 40);														// DWIG
	graphics.DrawRectangle(&pen, 1000, 50, 50, 600);
	graphics.DrawRectangle(&pen, 900, 550, 250, 100);

	for (int i = 0; i < 8; i++) {
		switch (tab[i].figure) {
			case 0:
				graphics.DrawRectangle(&pen, 105 + i * 100 + tab[i].osX, 600 + tab[i].osY, 50, 50);			//KWADRAT
				break;
			case 1:
				graphics.DrawRectangle(&pen, 80 + i * 100 + tab[i].osX, 600 + tab[i].osY, 100, 50);			//PROSTOK¥T
				break;
			case 2:
				graphics.DrawLine(&pen, 105 + i * 100 + tab[i].osX, 650 + tab[i].osY, 155 + i * 100, 650);	// TRÓJK¥T
				graphics.DrawLine(&pen, 155 + i * 100 + tab[i].osX, 650 + tab[i].osY, 130 + i * 100, 600);
				graphics.DrawLine(&pen, 130 + i * 100 + tab[i].osX, 600 + tab[i].osY, 105 + i * 100, 650);
				break;
		}
	}
}

int OnCreate(HWND window)
{
	SetTimer(window, TMR_1, 25, 0);
	Initialization();
	return 0;
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	InvalidateRect(hWnd, &drawArea1, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc);
	EndPaint(hWnd, &ps);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)){
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Move right ->"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1325, 350, 125, 80, hWnd, (HMENU)ID_RIGHT, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("<- Move left"),	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1075, 350, 125, 80, hWnd, (HMENU)ID_LEFT, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Move down"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1200, 350, 125, 80, hWnd, (HMENU)ID_MOVEDOWN, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Move up"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1200, 270, 125, 80, hWnd, (HMENU)ID_MOVEUP, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Drop"),	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1175, 450, 75, 75, hWnd, (HMENU)ID_DROP, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Catch"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1290, 450, 75, 75, hWnd, (HMENU)ID_CATCH, GetModuleHandle(NULL), NULL);

	OnCreate(hWnd);
	if (!hWnd){
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message){
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Parse the menu selections:
			switch (wmId){
				case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case ID_LEFT:
				if (location > 0){
						location--;
						for (int i = 0; i < 10; i++){
							if (tab[caught].orcaught == true) 
								tab[caught].osX -= 10;
							position -= 10;
							repaintWindow(hWnd, hdc, ps, &drawArea1);
					}
				}
				break;
			case ID_RIGHT:
				if (location < 7) {
					location++;
					for (int i = 0; i < 10; i++) {
						if (tab[caught].orcaught == true)
							tab[caught].osX += 10;
						position += 10;
						repaintWindow(hWnd, hdc, ps, &drawArea1);
					}
				}
				break;
			case ID_MOVEUP:
				for (int i = 0; i < 10; i++){
					if (length > -150){
						if (tab[caught].orcaught == true) 
							tab[caught].osY -= 10;
						length	-= 10;
					}
					repaintWindow(hWnd, hdc, ps, &drawArea1);
				}
				break;
			case ID_MOVEDOWN:
				for (int i = 0; i < 10; i++){
					if (length < tabheight[location]){
						if (tab[caught].orcaught == true) 
							tab[caught].osY += 10;
						length += 10;
					}
					repaintWindow(hWnd, hdc, ps, &drawArea1);
				}
				break;
			case ID_CATCH:
				if (tab[location].figure == 0){
					if (length == tabheight[location]){
						tab[location].orcaught = true;
						caught = location;
						for (int i = 0; i < 8; i++){
							if (i != location) 
								tabheight[i] -= 100;
						}
					}
				}
				break;
			case ID_DROP:
				if (length == tabheight[location]){
					if (tab[location].figure == 0){
						switch (tabheight[location]){
							case 300:
								tab[caught].orcaught = false;
								break;
							case 200:
								tab[caught].orcaught = false;
								tab[caught].osY += 50;
								break;
							case 100:
								tab[caught].orcaught = false;
								tab[caught].osY += 100;
							default:
								break;
						}
						for (int i = 0; i < 8; i++){
							if (i != location) tabheight[i] += 100;
						}
					}
				}
				break;
			default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			MyOnPaint(hdc);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_TIMER:
			switch (wParam){
				case TMR_1:
					repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message){
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}