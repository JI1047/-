#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "resource.h"
#include <cmath>
#include <stdio.h>


#define MAX_POINTS 50 // 최대 그릴 수 있는 점의 개수
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam);

int count = 0;
LPCTSTR lpszClass = TEXT("New Title Hong Gil Dong");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //WINAPI : 윈도우 프로그램이라는 의미 os에서 호출
	LPSTR lpszCmdLine, int nCmdShow)						 //hInstance : 운영체제의 커널이 응용 프로그램에 부여한 ID
{																 //szCmdLine : 커멘트라인 상에서 프로그램 구동 시 전달된 문자열
	HWND	hwnd;												 //iCmdShow : 윈도우가 화면에 출력될 형태
	MSG		msg;
	WNDCLASS WndClass;											 //WndClass 라는 구조체 정의									 
	WndClass.style = CS_HREDRAW | CS_VREDRAW;			 //출력스타일 : 수직/수평의 변화시 다시 그림
	WndClass.lpfnWndProc = WndProc;							 //프로시저 함수명
	WndClass.cbClsExtra = 0;								 //O/S 사용 여분 메모리 (Class)
	WndClass.cbWndExtra = 0;								 //O/s 사용 여분 메모리 (Window)
	WndClass.hInstance = hInstance;						 //응용 프로그램 ID
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	 //아이콘 유형
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		 //커서 유형
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//배경색   
	WndClass.lpszMenuName = NULL;								 //메뉴 이름
	WndClass.lpszClassName = lpszClass;						 //클래스 이름
	RegisterClass(&WndClass);									 //앞서 정의한 윈도우 클래스의 주소

	hwnd = CreateWindow(lpszClass,								 //윈도우가 생성되면 핸들(hwnd)이 반환
		lpszClass,												 //윈도우 클래스, 타이틀 이름
		WS_OVERLAPPEDWINDOW,									 //윈도우 스타일
		100,											 //윈도우 위치, x좌표
		50,											 //윈도우 위치, y좌표
		600,											 //윈도우 폭   
		400,											 //윈도우 높이   
		NULL,													 //부모 윈도우 핸들	 
		LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1)), // 여섯 번째 인수로 메뉴 핸들 전달
		hInstance,    											 //응용 프로그램 ID
		NULL     												 //생성된 윈도우 정보
	);
	ShowWindow(hwnd, nCmdShow);									 //윈도우의 화면 출력
	UpdateWindow(hwnd);											 //O/S 에 WM_PAINT 메시지 전송

	while (GetMessage(&msg, NULL, 0, 0))							 //WinProc()에서 PostQuitMessage() 호출 때까지 처리
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);									 //WinMain -> WinProc  
	}
	return (int)msg.wParam;
}

float Distance(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); // 두 점 간의 거리 공식을 사용하여 거리를 계산
}
HDC hdc;

static int clickedX = -1;
static int clickedY = -1;
static int  x, y;
int r = 0;
static int drawnX[MAX_POINTS]; // 그려진 위치의 X 좌표를 저장하는 배열
static int drawnY[MAX_POINTS]; // 그려진 위치의 Y 좌표를 저장하는 배열
static int numDrawnPoints = 0; // 그려진 점의 개수를 저장하는 변수

void CALLBACK TimerProc1(HWND hwnd, UINT iMsg, UINT_PTR ievent, DWORD dwTime)
{
	
	BOOL overlapped;

	// 새로운 위치 생성
	do {
		overlapped = FALSE;
		clickedX = rand() % 600;
		clickedY = rand() % 400;

		// 이미 그린 위치들과의 거리를 확인하여 일정 거리 이상 떨어진 위치인지 확인
		for (int i = 0; i < numDrawnPoints; ++i) {
			if (Distance(clickedX, clickedY, drawnX[i], drawnY[i]) < 100) {
				overlapped = TRUE;
				break;
			}
		}
	} while (overlapped);

	// 새로운 위치를 배열에 추가
	drawnX[numDrawnPoints] = clickedX;
	drawnY[numDrawnPoints] = clickedY;
	numDrawnPoints++;

	// 화면을 무효화하여 다시 그리도록 요청합니다.
	InvalidateRect(hwnd, NULL, FALSE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;

	static char linebfr[256] = { 0, };
	static int oldX, oldY, left_button,color;

	switch (iMsg)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 1000, (TIMERPROC)TimerProc1);//함수등록 초기화
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_on:
			// 프로그램을 실행합니다.
			SetTimer(hwnd, 1, 1000, (TIMERPROC)TimerProc1);
			break;
		case ID_off:
			// 프로그램을 멈춥니다.
			KillTimer(hwnd, 1); // 타이머 ID를 이용하여 타이머를 제거합니다.
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE); // 화면을 갱신합니다.
		break;



	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		SetTextColor(hdc, RGB(255, 0, 0)); // 빨간색으로 설정
		left_button = TRUE;
		//m_count++;
		//itoa(m_count, linebfr, 10);

		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
	
		if (count == 5) {
			
			InvalidateRect(hwnd, NULL, TRUE);
			TextOut(hdc, 230, 100, "Game Over", 9);
			TextOut(hdc, 230, 150, "5초뒤에 종료됩니다.", 18);
			Sleep(5000);
			EndPaint(hwnd, &ps);
			PostQuitMessage(0); // 프로그램 종료 메시지를 보냅니다.
			break;
		}
		// 만약 클릭한 위치가 설정되었다면 그 위치에 'H'를 그립니다.
		else {
			if (left_button == TRUE && ((clickedX - x) * (clickedX - x) + (clickedY - y) * (clickedY - y)) <= 400) {
				TextOut(hdc, clickedX, clickedY, "   ", 3);
				TextOut(hdc, x, y, "M", 1);
				r = 1;
				count++;
			}
			else {
				if (r == 1) {
					TextOut(hdc, clickedX, clickedY, "H", 1);
					r = 0;
				}
				else {
					TextOut(hdc, oldX, oldY, "   ", 3);
					TextOut(hdc, clickedX, clickedY, "H", 1);
					r = 0;
				}

			}
		}
		
		oldX = clickedX;
		oldY = clickedY;

		//Score 생성 오른쪽 위에 
		RECT rect;
		GetClientRect(hwnd, &rect);
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkColor(hdc, RGB(255, 255, 255));

		SetTextColor(hdc, RGB(255, 0, 0));
		char countStr[10];
		sprintf(countStr, "Score:%d", count);
		DrawText(hdc, countStr, -1, &rect, DT_RIGHT | DT_TOP | DT_SINGLELINE);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);			 //CASE에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}
