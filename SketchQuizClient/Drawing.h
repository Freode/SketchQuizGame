#pragma once

// ��ȭ ���ڸ� ����� ǥ���ϴ� �Լ�
void CreateAndShowDialog(HWND hWnd);


// ======================= ���� =======================
void SelectPenColor(DRAWLINE_MSG* g_drawlinemsg);
void AddLineWidthOption(HWND hDlg);
void SelectLineWidth(HWND hDlg, DRAWLINE_MSG* g_drawlinemsg);





// ======================= ��ȣ =======================

// sizeof(DRAWELLIPSE_MSG) == 256
typedef struct _DRAWELLIPSE_MSG
{
	int  type;
	int  color;
	int  x0, y0;
	int  x1, y1;
	char dummy[SIZE_TOT - 6 * sizeof(int)];
} DRAWELLIPSE_MSG;

// ���� �ɼ� ���� �׸� �߰�
void AddFigureOption(HWND hDlg);

// �׸��� �ɼ� ����
void SelectFigureOption(HWND hDlg, int &g_currentSelectFigureOption);

// Ÿ�� �׸���
void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, int startX, int startY, HPEN& hPen);