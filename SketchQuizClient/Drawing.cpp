#include "stdafx.h"

// ��ȭ ���ڸ� ����� ǥ���ϴ� �Լ�
void CreateAndShowDialog(HWND hWnd)
{
	g_hDialog = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
	ShowWindow(g_hDialog, SW_SHOW);
	g_bDialogVisible = true;
}

// ======================= ���� =======================
void SelectPenColor(DRAWLINE_MSG *g_drawlinemsg) {
	// ���� ��ȭ ���� ����
	CHOOSECOLOR cc = { sizeof(CHOOSECOLOR) };
	static COLORREF customColors[16] = { 0 }; // ����� ���� ����
	cc.hwndOwner = g_hDrawWnd;
	cc.lpCustColors = customColors;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.rgbResult = g_drawlinemsg->color;
	if (ChooseColor(&cc)) {
		g_drawlinemsg->color = cc.rgbResult;
	}
}

void AddLineWidthOption(HWND hDlg) {
	// �׸� �߰�
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_ADDSTRING, 0, (LPARAM)_T("���"));
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_ADDSTRING, 0, (LPARAM)_T("����"));
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_ADDSTRING, 0, (LPARAM)_T("����"));
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_SETCURSEL, 0, 0); // �ʱ� ������ "���"
}

void SelectLineWidth(HWND hDlg, DRAWLINE_MSG* g_drawlinemsg) {
	// Combo Box�� ������ ����Ǹ� �̺�Ʈ ó��
	int selectedIndex = SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_GETCURSEL, 0, 0);
	switch (selectedIndex)
	{
	case 0: // "���" ����
		// ���� ���⸦ 1px�� ����
		g_drawlinemsg->width = 1;
		break;
	case 1: // "����" ����
		// ���� ���⸦ 3px�� ����
		g_drawlinemsg->width = 3;
		break;
	case 2: // "����" ����
		// ���� ���⸦ 5px�� ����
		g_drawlinemsg->width = 5;
		break;
	}
}




// ======================= ��ȣ =======================

// ���� �ɼ� ���� �׸� �߰�
void AddFigureOption(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("���찳"));
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("��"));
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("Ÿ��"));
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("�簢��"));

	// �ʱ� ���� �ɼ��� "��"���� ���� 
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_SETCURSEL, 1, 0);
}

// �׸��� �ɼ� ����
void SelectFigureOption(HWND hDlg, int &g_currentSelectFigureOption)
{
	// ComboBox�� ������ ����Ǹ�, �̺�Ʈ ó��
	int selectedIndex = SendDlgItemMessage(hDlg, IDC_FIGURE, CB_GETCURSEL, 0, 0);
	switch (selectedIndex)
	{
	// "���찳" ��� ����
	case 0:
		g_currentSelectFigureOption = MODE_ERASE;
		break;
	// "��" ��� ����
	case 1: 
		g_currentSelectFigureOption = MODE_LINE;
		break;
	// "Ÿ��" ��� ����
	case 2: 
		g_currentSelectFigureOption = MODE_ELLIPSE;
		break;
	// "�簢��" ��� ����
	case 3:
		g_currentSelectFigureOption = MODE_RECTANGLE;
		break;
	// "�ﰢ��" ��� ����
	case 4:
		g_currentSelectFigureOption = MODE_TRIANGLE;
		break;
	}
}

// Ÿ�� �׸���
void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, int startX, int startY, HPEN& hPen)
{
	HDC hDC = GetDC(hWnd);

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