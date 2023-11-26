#include "stdafx.h"

// ��ȭ ���ڸ� ����� ǥ���ϴ� �Լ�
void CreateAndShowDialog(HWND hWnd)
{
	g_hDialog = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
	ShowWindow(g_hDialog, SW_SHOW);
	g_bDialogVisible = true;
}

// ======================= ���� =======================






// ======================= ��ȣ =======================

void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, int startX, int startY)
{
	HDC hDC = GetDC(hWnd);
	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

	// ������ DC�� Ÿ�� ���
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	int endX = LOWORD(lParam);
	int endY = HIWORD(lParam);

	int centerX = (startX + endX) / 2;
	int centerY = (startY + endY) / 2;

	int ellipseAxisX = centerX - startX;
	int ellipseAxisY = centerY - startY;
	double angle;
	int newX, newY;

	int oldX = endX;
	int oldY = centerY;
	for (int i = 0; i <= 360; i++)
	{
		angle = 2 * 3.1416 * i / 360;
		newX = centerX + ellipseAxisX * cos(angle);
		newY = centerY + ellipseAxisY * sin(angle);

		MoveToEx(hDC, oldX, oldY, NULL);
		LineTo(hDC, newX, newY);

		oldX = newX;
		oldY = newY;
	}

	SelectObject(hDC, hOldPen);
	hOldPen = (HPEN)SelectObject(hDCMem, hPen);

	for (int i = 0; i <= 360; i++)
	{
		angle = 2 * 3.1416 * i / 360;
		newX = centerX + ellipseAxisX * cos(angle);
		newY = centerY + ellipseAxisY * sin(angle);

		MoveToEx(hDCMem, oldX, oldY, NULL);
		LineTo(hDCMem, newX, newY);

		oldX = newX;
		oldY = newY;
	}

	SelectObject(hDCMem, hOldPen);

	// ȭ�� ��¿� DC�� Pen �ڵ� ����
	DeleteObject(hPen);
	ReleaseDC(hWnd, hDC);
}