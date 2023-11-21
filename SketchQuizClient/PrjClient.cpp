#include "stdafx.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// �̺�Ʈ ����(���� ��ȣ, ���ȣ ����)
	g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (g_hReadEvent == NULL) return 1;
	g_hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (g_hWriteEvent == NULL) return 1;

	// ���� ���� �ʱ�ȭ(�Ϻ�)
	g_chatmsg.type = TYPE_CHAT;
	g_drawlinemsg.type = TYPE_DRAWLINE;
	g_drawlinemsg.color = RGB(255, 0, 0);
	g_erasepicmsg.type = TYPE_ERASEPIC;

	g_hInstance = hInstance;

	// ���� ������(ù ȭ��) ����
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, MainWndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("MainWindow"), NULL };
	RegisterClassEx(&wcex);
	g_hMainWindow = CreateWindow(_T("MainWindow"), _T("Main Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 200, NULL, NULL, hInstance, NULL);

	ShowWindow(g_hMainWindow, nCmdShow);
	UpdateWindow(g_hMainWindow);

	// �޽��� ����
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(g_hDialog, &msg)) // ��ȭ ���� �޽��� ó��
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// �̺�Ʈ ��ü ����
	CloseHandle(g_hReadEvent);
	CloseHandle(g_hWriteEvent);
	// ���� ����
	WSACleanup();
	return 0;
}

// ���� ������ ���ν���
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		// '�׸���' ��ư ����
		CreateWindow(_T("BUTTON"), _T("�׸���"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 10, 100, 30, hWnd, (HMENU)1, g_hInstance, NULL);

		// '�α���' ��ư ����
		CreateWindow(_T("BUTTON"), _T("�α���"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 120, 10, 100, 30, hWnd, (HMENU)2, g_hInstance, NULL);

		// '��ŷ' ��ư ���� (�� ��°�� ��ġ)
		CreateWindow(_T("BUTTON"), _T("��ŷ"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 10, 100, 30, hWnd, (HMENU)4, g_hInstance, NULL);

		// '����' ��ư ���� (�� ��°�� ��ġ)
		CreateWindow(_T("BUTTON"), _T("����"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 10, 100, 30, hWnd, (HMENU)3, g_hInstance, NULL);



		break;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 1) // '�׸���' ��ư Ŭ��
		{
			if (!g_bDialogVisible) // ��ȭ ���ڰ� ���� ������ �ʴ� ���
			{
				// ��ȭ ���ڸ� ����� ǥ���ϴ� �Լ� ȣ��
				CreateAndShowDialog(hWnd);
			}
			else
			{
				// ��ȭ ���ڰ� �̹� ���̴� ���, ��ȭ ���ڸ� Ȱ��ȭ�մϴ�.
				SetForegroundWindow(g_hDialog);
			}
			// �� ������â �����
			ShowWindow(hWnd, SW_HIDE);
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ����Ʈ ��Ʈ�� ��� �Լ�
void DisplayText(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char cbuf[1024];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);

	int nLength = GetWindowTextLength(g_hEditStatus);
	SendMessage(g_hEditStatus, EM_SETSEL, nLength, nLength);
	SendMessageA(g_hEditStatus, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
}
