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

// ��ȭ���� ���ν���(�׸��� â)
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hChkIsIPv6;
	static HWND hEditIPaddr;
	static HWND hEditPort;
	static HWND hChkIsUDP;
	static HWND hBtnConnect;
	static HWND hBtnSendFile; // ���� �������� ����
	static HWND hBtnSendMsg; // ���� �������� ����
	static HWND hEditMsg;
	static HWND hEditStatus; // ���� �������� ����
	static HWND hColorRed;
	static HWND hColorGreen;
	static HWND hColorBlue;
	static HWND hBtnErasePic; // ���� �������� ����
	static HWND hStaticDummy;

	switch (uMsg) {
	case WM_INITDIALOG:
		// ��Ʈ�� �ڵ� ���
		hChkIsIPv6 = GetDlgItem(hDlg, IDC_ISIPV6);
		hEditIPaddr = GetDlgItem(hDlg, IDC_IPADDR);
		hEditPort = GetDlgItem(hDlg, IDC_PORT);
		hChkIsUDP = GetDlgItem(hDlg, IDC_ISUDP);
		hBtnConnect = GetDlgItem(hDlg, IDC_CONNECT);
		hBtnSendFile = GetDlgItem(hDlg, IDC_SENDFILE);
		g_hBtnSendFile = hBtnSendFile; // ���� ������ ����
		hBtnSendMsg = GetDlgItem(hDlg, IDC_SENDMSG);
		g_hBtnSendMsg = hBtnSendMsg; // ���� ������ ����
		hEditMsg = GetDlgItem(hDlg, IDC_MSG);
		hEditStatus = GetDlgItem(hDlg, IDC_STATUS);
		g_hEditStatus = hEditStatus; // ���� ������ ����
		hColorRed = GetDlgItem(hDlg, IDC_COLORRED);
		hColorGreen = GetDlgItem(hDlg, IDC_COLORGREEN);
		hColorBlue = GetDlgItem(hDlg, IDC_COLORBLUE);
		hBtnErasePic = GetDlgItem(hDlg, IDC_ERASEPIC);
		g_hBtnErasePic = hBtnErasePic; // ���� ������ ����
		hStaticDummy = GetDlgItem(hDlg, IDC_DUMMY);

		// ��Ʈ�� �ʱ�ȭ
		SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP4);
		SetDlgItemInt(hDlg, IDC_PORT, SERVERPORT, FALSE);
		EnableWindow(g_hBtnSendFile, FALSE);
		EnableWindow(g_hBtnSendMsg, FALSE);
		SendMessage(hEditMsg, EM_SETLIMITTEXT, SIZE_DAT / 2, 0);
		SendMessage(hColorRed, BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(hColorGreen, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(hColorBlue, BM_SETCHECK, BST_UNCHECKED, 0);
		EnableWindow(g_hBtnErasePic, FALSE);

		// ������ Ŭ���� ���
		WNDCLASS wndclass;
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = ChildWndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = g_hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = _T("MyWndClass");
		wndclass.lpszClassName = _T("MyWndClass");
		if (!RegisterClass(&wndclass)) exit(1);

		// �ڽ� ������ ����
		RECT rect; GetWindowRect(hStaticDummy, &rect);
		POINT pt; pt.x = rect.left; pt.y = rect.top;
		ScreenToClient(hDlg, &pt);
		g_hDrawWnd = CreateWindow(_T("MyWndClass"), _T(""), WS_CHILD,
			pt.x, pt.y, rect.right - rect.left, rect.bottom - rect.top,
			hDlg, (HMENU)NULL, g_hInstance, NULL);
		if (g_hDrawWnd == NULL) exit(1);
		ShowWindow(g_hDrawWnd, SW_SHOW);
		UpdateWindow(g_hDrawWnd);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_ISIPV6:
			g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
			if (g_isIPv6 == false)
				SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP4);
			else
				SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP6);
			return TRUE;
		case IDC_CONNECT:
			// ��Ʈ�� ���� ���
			GetDlgItemTextA(hDlg, IDC_IPADDR, g_ipaddr, sizeof(g_ipaddr));
			g_port = GetDlgItemInt(hDlg, IDC_PORT, NULL, TRUE);
			g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
			g_isUDP = SendMessage(hChkIsUDP, BM_GETCHECK, 0, 0);
			// ���� ��� ������ ����
			g_hClientThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
			if (g_hClientThread == NULL) exit(0);
			// ���� ���� ���� ��ٸ�
			while (g_bCommStarted == false);
			// ��Ʈ�� ���� ����
			EnableWindow(hChkIsIPv6, FALSE);
			EnableWindow(hEditIPaddr, FALSE);
			EnableWindow(hEditPort, FALSE);
			EnableWindow(hChkIsUDP, FALSE);
			EnableWindow(hBtnConnect, FALSE);
			EnableWindow(g_hBtnSendFile, TRUE);
			EnableWindow(g_hBtnSendMsg, TRUE);
			SetFocus(hEditMsg);
			EnableWindow(g_hBtnErasePic, TRUE);
			return TRUE;
		case IDC_SENDFILE:
			MessageBox(NULL, _T("���� �������� �ʾҽ��ϴ�."), _T("�˸�"), MB_ICONERROR);
			// 1. GetOpenFileName() ������ API�� �̿��ؼ� ���� ���� ��ȭ���ڸ� ����
			// ������ ������ �����Ѵ�. 2. ������ ������ �о ������ �����Ѵ�.
			return TRUE;
		case IDC_SENDMSG:
			// ������ ���� ä�� �޽��� �б� �ϷḦ ��ٸ�
			WaitForSingleObject(g_hReadEvent, INFINITE);
			// ���ο� ä�� �޽����� ��� ���� �ϷḦ �˸�
			GetDlgItemTextA(hDlg, IDC_MSG, g_chatmsg.msg, SIZE_DAT);
			SetEvent(g_hWriteEvent);
			// �Էµ� �ؽ�Ʈ ��ü�� ���� ǥ��
			SendMessage(hEditMsg, EM_SETSEL, 0, -1);
			return TRUE;
		case IDC_COLORRED:
			g_drawlinemsg.color = RGB(255, 0, 0);
			return TRUE;
		case IDC_COLORGREEN:
			g_drawlinemsg.color = RGB(0, 255, 0);
			return TRUE;
		case IDC_COLORBLUE:
			g_drawlinemsg.color = RGB(0, 0, 255);
			return TRUE;
		case IDC_ERASEPIC:
			send(g_sock, (char*)&g_erasepicmsg, SIZE_TOT, 0);
			return TRUE;
		case IDCANCEL:
			closesocket(g_sock);
			EndDialog(hDlg, IDCANCEL);

			return TRUE;
		}
	}
	return FALSE;
}

// �ڽ� ������ ���ν���(�׸��� ����)
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	HPEN hPen, hOldPen;
	PAINTSTRUCT ps;
	static int cx, cy;
	static HBITMAP hBitmap;
	static HDC hDCMem;
	static int x0, y0;
	static int x1, y1;
	static bool bDrawing;

	switch (uMsg) {
	case WM_SIZE:
		// ȭ�� ��¿� DC �ڵ� ���
		hDC = GetDC(hWnd);
		// ��� ��Ʈ�ʰ� �޸� DC ����
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);
		hBitmap = CreateCompatibleBitmap(hDC, cx, cy);
		hDCMem = CreateCompatibleDC(hDC);
		SelectObject(hDCMem, hBitmap);
		// ��� ��Ʈ�� ������� ä��
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// ȭ�� ��¿� DC �ڵ� ����
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_PAINT:
		// ȭ�� ��¿� DC �ڵ� ���
		hDC = BeginPaint(hWnd, &ps);
		// ��� ��Ʈ���� ȭ�鿡 ����
		BitBlt(hDC, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);
		// ȭ�� ��¿� DC �ڵ� ����
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		// ���콺 Ŭ�� ��ǥ ���
		x0 = LOWORD(lParam);
		y0 = HIWORD(lParam);
		bDrawing = true;
		return 0;
	case WM_MOUSEMOVE:
		if (bDrawing && g_bCommStarted) {
			// ���콺 Ŭ�� ��ǥ ���
			x1 = LOWORD(lParam);
			y1 = HIWORD(lParam);
			// �� �׸��� �޽��� ������
			g_drawlinemsg.x0 = x0;
			g_drawlinemsg.y0 = y0;
			g_drawlinemsg.x1 = x1;
			g_drawlinemsg.y1 = y1;
			send(g_sock, (char*)&g_drawlinemsg, SIZE_TOT, 0);
			// ���콺 Ŭ�� ��ǥ ����
			x0 = x1;
			y0 = y1;
		}
		return 0;
	case WM_LBUTTONUP:
		bDrawing = false;
		return 0;
	case WM_DRAWLINE:
		// ȭ�� ��¿� DC�� Pen �ڵ� ���
		hDC = GetDC(hWnd);
		hPen = CreatePen(PS_SOLID, 3, g_drawcolor);
		// ������ ȭ�鿡 1���� �׸���
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		MoveToEx(hDC, LOWORD(wParam), HIWORD(wParam), NULL);
		LineTo(hDC, LOWORD(lParam), HIWORD(lParam));
		SelectObject(hDC, hOldPen);
		// ��� ��Ʈ�ʿ� 2���� �׸���
		hOldPen = (HPEN)SelectObject(hDCMem, hPen);
		MoveToEx(hDCMem, LOWORD(wParam), HIWORD(wParam), NULL);
		LineTo(hDCMem, LOWORD(lParam), HIWORD(lParam));
		SelectObject(hDCMem, hOldPen);
		// ȭ�� ��¿� DC�� Pen �ڵ� ����
		DeleteObject(hPen);
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_ERASEPIC:
		// ��� ��Ʈ�� ������� ä��
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// WM_PAINT �޽��� ���� ����
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_DESTROY:
		DeleteDC(hDCMem);
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// �α��� ������ ���ν��� (�α��� ����) -----------------------------------------------------------------------------------//
LRESULT CALLBACK LoginWndProc(HWND hwndLogin, UINT msg, WPARAM wParam, LPARAM lParam) {
	// �α��� ������ Ŭ���� ���
	WNDCLASS wcLogin = { 0 };
	wcLogin.lpfnWndProc = LoginWndProc;
	wcLogin.hInstance = g_hInstance;
	wcLogin.lpszClassName = L"LoginWindowClass";
	RegisterClass(&wcLogin);

	switch (msg) {

	case WM_CREATE:
		// �α��� ȭ�� �ʱ�ȭ �� ��Ʈ�� ����
																								   //x,y,width,height
		CreateWindow(L"STATIC", L"����ġ����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 500, 100, 300, 100, hwndLogin, NULL, NULL, NULL); // ����ġ���� Ÿ��Ʋ
		CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 400, 300, 400, 40, hwndLogin, (HMENU)ID_ID_INPUT, NULL, NULL);
		CreateWindow(L"BUTTON", L"�ߺ�Ȯ��", WS_VISIBLE | WS_CHILD, 850, 300, 100, 40, hwndLogin, (HMENU)ID_DUPLICATION_BUTTON, NULL, NULL); // �ߺ�Ȯ�� ��ư
		CreateWindow(L"STATIC", L"��� ������ ID�Դϴ�!", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 400, 360, 400, 40, hwndLogin, NULL, NULL, NULL); // ��� ������ ID�Դϴ�!
		CreateWindow(L"BUTTON", L"�α���", WS_VISIBLE | WS_CHILD, 400, 500, 500, 100, hwndLogin, (HMENU)ID_LOGIN_BUTTON, NULL, NULL); // �α��� ��ư
		CreateWindow(L"BUTTON", L"���ư���", WS_VISIBLE | WS_CHILD, 100, 100, 100, 30, hwndLogin, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL);
		break;

	case WM_COMMAND:
		// ��ư Ŭ�� �̺�Ʈ ó��
		switch (LOWORD(wParam)) {
			// '���ư���' ��ư Ŭ�� ó��
		case ID_BACKHOME_BUTTON:
			// �α��� â�� ����� ����â�� ���̰� �ؾ��ϴµ� �ϴ� �α���â ����
			ShowWindow(hwndLogin, SW_HIDE);
			break;

		case ID_LOGIN_BUTTON: // �α��� ��ư�� Ŭ������ �� 
			break;

		case ID_ID_INPUT: // ID�� �Է����ּ���! �Է����� ��
			break;

		case ID_DUPLICATION_BUTTON: //�ߺ� Ȯ�� ��ư Ŭ������ ��
			break;

		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwndLogin, msg, wParam, lParam);
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------//

//-------------------------------------Ȩ ������ ���ν��� -----------------------------------------------------------------------//
// ������ ���ν���
LRESULT CALLBACK HomeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	// Ȩ ����ȭ�� ������ Ŭ���� ���
	WNDCLASS wcMain = { 0 };
	wcMain.lpfnWndProc = MainWndProc;
	wcMain.hInstance = g_hInstance;
	wcMain.lpszClassName = L"MainWindowClass";
	RegisterClass(&wcMain);

	switch (msg) {

	case WM_CREATE:
		// Ȩ ���� ȭ�� �ʱ�ȭ �� ��Ʈ�� ����
// 
																								   //x,y,width,height
		CreateWindow(L"STATIC", L"id�� �ݰ����ϴ�!", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 1000, 10, 200, 30, hwnd, NULL, NULL, NULL); // id �� �ݰ����ϴ�!
		CreateWindow(L"STATIC", L"���ڻ��� ����", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 50, 1150, 100, hwnd, NULL, NULL, NULL); // ����ġ���� Ÿ��Ʋ

		CreateWindow(L"BUTTON", L"���� ����", WS_VISIBLE | WS_CHILD, 1042, 185, 174, 54, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // ���� ����

		CreateWindow(L"BUTTON", L"ä�� A ����", WS_VISIBLE | WS_CHILD, 300, 200, 640, 100, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // ä�� A ����
		CreateWindow(L"BUTTON", L"ä�� B ����", WS_VISIBLE | WS_CHILD, 300, 350, 640, 100, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // ä�� B ����

		//CreateWindow(L"BUTTON", L"�游���", WS_VISIBLE | WS_CHILD, 282, 600, 320, 67, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // �� �����
		CreateWindow(L"BUTTON", L"��������", WS_VISIBLE | WS_CHILD, 300, 500, 640, 100, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // ���� ����


		CreateWindow(L"BUTTON", L"���ư���", WS_VISIBLE | WS_CHILD, 100, 100, 100, 30, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // ���ư���
		break;

	case WM_COMMAND:
		// ��ư Ŭ�� �̺�Ʈ ó��
		switch (LOWORD(wParam)) {
			// '���ư���' ��ư Ŭ�� ó��
		case ID_BACKHOME_BUTTON:
			// �α��� â�� ����� ���� â�� �ٽ� ���̰� ��
		/*	ShowWindow(hwnd, SW_SHOW);*/
			ShowWindow(hwnd, SW_HIDE);
			break;

		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------//


// ���� ��� ������ �Լ�(1) - ����
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	if (g_isIPv6 == false && g_isUDP == false) { // TCP/IPv4 ����
		// socket()
		g_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (g_sock == INVALID_SOCKET) err_quit("socket()");

		// connect()
		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, g_ipaddr, &serveraddr.sin_addr);
		serveraddr.sin_port = htons(g_port);
		retval = connect(g_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) err_quit("connect()");
	}
	else if (g_isIPv6 == true && g_isUDP == false) { // TCP/IPv6 ����
		// socket()
		g_sock = socket(AF_INET6, SOCK_STREAM, 0);
		if (g_sock == INVALID_SOCKET) err_quit("socket()");

		// connect()
		struct sockaddr_in6 serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin6_family = AF_INET6;
		inet_pton(AF_INET6, g_ipaddr, &serveraddr.sin6_addr);
		serveraddr.sin6_port = htons(g_port);
		retval = connect(g_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) err_quit("connect()");
	}
	else if (g_isIPv6 == false && g_isUDP == true) { // UDP/IPv4 ����
		MessageBox(NULL, _T("���� �������� �ʾҽ��ϴ�."), _T("�˸�"), MB_ICONERROR);
		exit(1);
	}
	else if (g_isIPv6 == true && g_isUDP == true) { // UDP/IPv6 ����
		MessageBox(NULL, _T("���� �������� �ʾҽ��ϴ�."), _T("�˸�"), MB_ICONERROR);
		exit(1);
	}
	MessageBox(NULL, _T("������ �����߽��ϴ�."), _T("�˸�"), MB_ICONINFORMATION);

	// �б� & ���� ������ ����
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	if (hThread[0] == NULL || hThread[1] == NULL) exit(1);
	g_bCommStarted = true;

	// ������ ���� ��� (�� �� �ϳ��� ������ ������)
	retval = WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
	retval -= WAIT_OBJECT_0;
	if (retval == 0)
		TerminateThread(hThread[1], 1);
	else
		TerminateThread(hThread[0], 1);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	MessageBox(NULL, _T("������ ������ϴ�."), _T("�˸�"), MB_ICONERROR);
	EnableWindow(g_hBtnSendFile, FALSE);
	EnableWindow(g_hBtnSendMsg, FALSE);
	EnableWindow(g_hBtnErasePic, FALSE);
	g_bCommStarted = false;
	closesocket(g_sock);
	return 0;
}

// ���� ��� ������ �Լ�(2) - ������ ����
DWORD WINAPI ReadThread(LPVOID arg)
{
	int retval;
	COMM_MSG comm_msg;
	CHAT_MSG* chat_msg;
	DRAWLINE_MSG* drawline_msg;
	ERASEPIC_MSG* erasepic_msg;
	char reciever[20], sender[20], tmp[5];

	while (1) {
		retval = recv(g_sock, (char*)&comm_msg, SIZE_TOT, MSG_WAITALL);
		if (retval == 0 || retval == SOCKET_ERROR) {
			break;
		}
		if (comm_msg.type == TYPE_CHAT) {
			chat_msg = (CHAT_MSG*)&comm_msg;
			DisplayText("[���� �޽���] %s\r\n", chat_msg->msg);
			if (strncmp(chat_msg->msg, "/w ", 2) == 0) {
				sscanf(chat_msg->msg, "%s %s %s", tmp, sender, reciever);
				sendFile(sender, reciever, chat_msg->msg);
				
			}
		}
		else if (comm_msg.type == TYPE_DRAWLINE) {
			drawline_msg = (DRAWLINE_MSG*)&comm_msg;
			g_drawcolor = drawline_msg->color;
			SendMessage(g_hDrawWnd, WM_DRAWLINE,
				MAKEWPARAM(drawline_msg->x0, drawline_msg->y0),
				MAKELPARAM(drawline_msg->x1, drawline_msg->y1));
		}
		else if (comm_msg.type == TYPE_ERASEPIC) {
			erasepic_msg = (ERASEPIC_MSG*)&comm_msg;
			SendMessage(g_hDrawWnd, WM_ERASEPIC, 0, 0);
		}
	}
	return 0;
}

// ���� ��� ������ �Լ�(3) - ������ �۽�
DWORD WINAPI WriteThread(LPVOID arg)
{
	int retval;

	// ������ ������ ���
	while (1) {
		// ���� �Ϸ� ��ٸ���
		WaitForSingleObject(g_hWriteEvent, INFINITE);
		// ���ڿ� ���̰� 0�̸� ������ ����
		if (strlen(g_chatmsg.msg) == 0) {
			// [�޽��� ����] ��ư Ȱ��ȭ
			EnableWindow(g_hBtnSendMsg, TRUE);
			// �б� �Ϸ� �˸���
			SetEvent(g_hReadEvent);
			continue;
		}
		// ������ ������
		retval = send(g_sock, (char*)&g_chatmsg, SIZE_TOT, 0);
		if (retval == SOCKET_ERROR) break;
		// [�޽��� ����] ��ư Ȱ��ȭ
		EnableWindow(g_hBtnSendMsg, TRUE);
		// �б� �Ϸ� �˸���
		SetEvent(g_hReadEvent);
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
