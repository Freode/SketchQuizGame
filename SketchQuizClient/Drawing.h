#pragma once

// ��ȭ ���ڸ� ����� ǥ���ϴ� �Լ�
void CreateAndShowDialog(HWND hWnd);


// ======================= ���� =======================






// ======================= ��ȣ =======================
#define TYPE_DRAWELLIPSE 4000			// �޽��� Ÿ�� : Ÿ�� �׸���
#define WM_DRAWELLIPSE (WM_USER+3)		// Ÿ�� �׸��� ������ �޽���

// sizeof(DRAWELLIPSE_MSG) == 256
typedef struct _DRAWELLIPSE_MSG
{
	int  type;
	int  color;
	int  x0, y0;
	int  x1, y1;
	char dummy[SIZE_TOT - 6 * sizeof(int)];
} DRAWELLIPSE_MSG;

void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, int startX, int startY);