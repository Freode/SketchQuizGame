#include "stdafx.h"

// Ȩ â�� ����� ǥ���ϴ� �Լ�
void CreateAndShowWindow_Home_Notice(HWND hWnd)
{
	// Ȩ â ����
	hwndHome_Notice = CreateWindow(_T("Home_NoticeWindowClass"), _T("�������� �Է�â"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, g_hInstance, NULL);

	if (hwndHome_Notice == NULL)
	{
		MessageBox(NULL, L"������ ���� ����!", L"����", MB_ICONERROR);
	}
	ShowWindow(hwndHome_Notice, SW_SHOW);
	g_bDialogVisible = true;
}


