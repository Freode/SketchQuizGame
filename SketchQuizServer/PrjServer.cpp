#include "..\Common.h"
#include "resource.h"

#define SERVERPORT 9000
#define BUFSIZE    256
// ==== ��ȣ ====
#define WM_SOCKET (WM_USER+1)

// ���� ���� ������ ���� ����ü�� ����
typedef struct _SOCKETINFO
{
	SOCKET sock;
	char   buf[BUFSIZE + 1];
	int    recvbytes;

	// ===== ���� ======
	_TCHAR id_nickname[BUFSIZE]; // ����� �г���
	int    score=0;       // ���� �÷��� ����
} SOCKETINFO;

// ======= ���� ======= 
typedef struct _MESSAGEQUEUE {
	char queue[BUFSIZE][100] = { NULL };         // �޽��� ���� ť: ���� ��ȭ���� ǥ��. �� ���� ���� ������ �޽������� ��������.
	int head = 0;                 // ���� ť �ε���
	int tail = 0;
} MESSAGEQUEUE;

//char* g_msgQueue[BUFSIZE];    // �޽��� ���� ť: ���� ��ȭ���� ǥ��. �� ���� ���� ������ �޽������� ��������.
//int head = 0, tail = 0;           // ���� ť �ε���
MESSAGEQUEUE g_msgQueue;
// ====================

// ==== ��ȣ ====
// Ŭ���̾�Ʈ ���� �迭
int nTotalSockets = 0;
int nTotalUDPSockets = 0;
SOCKETINFO *SocketInfoArray[FD_SETSIZE];
SOCKADDR_IN UDPSocketInfoArray[FD_SETSIZE];

// ���̾�α� ���ν���
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
// ���� ���� ���� �Լ�
bool AddSocketInfoTCP(SOCKET sock);
bool AddSocketInfoUDP(SOCKADDR_IN addr);
void RemoveSocketInfo(int nIndex);
void addMessage(char* message); 
SOCKETINFO* GetSocketInfo(SOCKET sock);

// =========== ��ȣ =============
SOCKET listen_sock4;
SOCKADDR_IN serveraddr;
SOCKET socket_UDP;

// ������ �޽��� ó�� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ========= ��ȣ ========
	// ������ Ŭ���� ���
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = _T("MyWndClass");
	if (!RegisterClass(&wndclass)) return 1;

	// �ӽ� ������ ����
	HWND hWnd = CreateWindow(_T("MyWndClass"), _T("TCP ����"), WS_OVERLAPPEDWINDOW,
		0, 0, 600, 200, NULL, NULL, NULL, NULL);
	if (hWnd == NULL) return 1;
	//ShowWindow(hWnd, SW_SHOWNORMAL);
	//UpdateWindow(hWnd);

	HWND hDlg = (HWND)DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);

	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	/*----- TCP/IPv4 ���� �ʱ�ȭ ���� -----*/
	// ���� ����
	listen_sock4 = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock4 == INVALID_SOCKET) err_quit("socket()");

	// bind()
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock4, (struct sockaddr *)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock4, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	/*----- TCP/IPv4 ���� �ʱ�ȭ ���� -----*/

	/*----- TCP/IPv6 ���� �ʱ�ȭ ���� -----*/
	// ���� ����
	SOCKET listen_sock6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sock6 == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in6 serveraddr6;
	memset(&serveraddr6, 0, sizeof(serveraddr6));
	serveraddr6.sin6_family = AF_INET6;
	serveraddr6.sin6_addr = in6addr_any;
	serveraddr6.sin6_port = htons(SERVERPORT);
	retval = bind(listen_sock6, (struct sockaddr *)&serveraddr6,
		sizeof(serveraddr6));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock6, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	/*----- TCP/IPv6 ���� �ʱ�ȭ ���� -----*/

	/*----- UDP/IPv4 ���� �ʱ�ȭ ���� -----*/
	// TODO: ������ �����ϰ� �ʱ�ȭ�Ѵ�. == ��ȣ ==
	socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_UDP == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	retval = bind(socket_UDP, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}
	/*----- UDP/IPv4 ���� �ʱ�ȭ ���� -----*/

	/*----- UDP/IPv6 ���� �ʱ�ȭ ���� -----*/
	// TODO: ������ �����ϰ� �ʱ�ȭ�Ѵ�.
	/*----- UDP/IPv6 ���� �ʱ�ȭ ���� -----*/

	// ������ ��ſ� ����� ����(����)
	fd_set rset;
	SOCKET client_sock;
	int addrlen;
	// ������ ��ſ� ����� ����(IPv4)
	struct sockaddr_in clientaddr4;
	// ������ ��ſ� ����� ����(IPv6)
	struct sockaddr_in6 clientaddr6;
	
	// ========== ��ȣ ==========
	int recvLen; // ���� ���� ������ ũ��
	int sendLen; // ���� ���� ������ ũ��

	// WSAAsyncSelect()

	// TCP�� ������ �ؾ��ϹǷ� FD_ACCEPT�� �߰�
	retval = WSAAsyncSelect(listen_sock4, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR) err_quit("WSAAsyncSelect()");

	// UDP�� TCP�� �޸� ������ �ʿ�����Ƿ�
	// FD_ACCEPT�� ���� ����.
	// FD_READ�� �����͸� ������ �� �ֵ��� ����
	retval = WSAAsyncSelect(socket_UDP, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	if (retval == SOCKET_ERROR) err_quit("WSAAsyncSelect()");

	//while (1) {
	//	// ���� �� �ʱ�ȭ
	//	FD_ZERO(&rset);
	//	FD_SET(listen_sock4, &rset);
	//	FD_SET(listen_sock6, &rset);
	//	for (int i = 0; i < nTotalSockets; i++) {
	//		FD_SET(SocketInfoArray[i]->sock, &rset);
	//	}

	//	// select()
	//	retval = select(0, &rset, NULL, NULL, NULL);
	//	if (retval == SOCKET_ERROR) err_quit("select()");

	//	// ���� �� �˻�(1): Ŭ���̾�Ʈ ���� ����
	//	if (FD_ISSET(listen_sock4, &rset)) { // TCP/IPv4
	//		addrlen = sizeof(clientaddr4);
	//		client_sock = accept(listen_sock4,
	//			(struct sockaddr *)&clientaddr4, &addrlen);
	//		if (client_sock == INVALID_SOCKET) {
	//			err_display("accept()");
	//			break;
	//		}
	//		else {
	//			// Ŭ���̾�Ʈ ���� ���
	//			char addr[INET_ADDRSTRLEN];
	//			inet_ntop(AF_INET, &clientaddr4.sin_addr, addr, sizeof(addr));
	//			printf("\n[TCP/IPv4 ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
	//				addr, ntohs(clientaddr4.sin_port));
	//			// ���� ���� �߰�: ���� �� ���� ����
	//			if (!AddSocketInfo(client_sock, false, false))
	//				closesocket(client_sock);
	//		}
	//	}
	//	if (FD_ISSET(listen_sock6, &rset)) { // TCP/IPv6
	//		addrlen = sizeof(clientaddr6);
	//		client_sock = accept(listen_sock6,
	//			(struct sockaddr *)&clientaddr6, &addrlen);
	//		if (client_sock == INVALID_SOCKET) {
	//			err_display("accept()");
	//			break;
	//		}
	//		else {
	//			// Ŭ���̾�Ʈ ���� ���
	//			char addr[INET6_ADDRSTRLEN];
	//			inet_ntop(AF_INET6, &clientaddr6.sin6_addr, addr, sizeof(addr));
	//			printf("\n[TCP/IPv6 ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
	//				addr, ntohs(clientaddr6.sin6_port));
	//			// ���� ���� �߰�: ���� �� ���� ����
	//			if (!AddSocketInfo(client_sock, true, false))
	//				closesocket(client_sock);
	//		}
	//	}

	//	// ====== ���� ======
	//	//// ���� ���� Ŭ���̾�Ʈ���� ���� ��ȭ ���� ����
	//	//SOCKETINFO* curClientPtr = SocketInfoArray[nTotalSockets - 1];
	//	//int sendLen = sizeof(g_msgQueue);
	//	//// ���� ���� ����
	//	//retval = sendn(curClientPtr->sock, (char*)&sendLen, sizeof(int), 0);
	//	//if (retval == SOCKET_ERROR)
	//	//{
	//	//	err_display("send()");
	//	//	continue;
	//	//}

	// 

	//	//// ���� ���� ����
	//	//retval = sendn(curClientPtr->sock, (char*)&g_msgQueue, recvLen, 0);
	//	//if (retval == SOCKET_ERROR)
	//	//{
	//	//	err_display("send()");
	//	//	continue;
	//	//}

	//	// =========================
	//	

	//	// ���� �� �˻�(2): ������ ���
	//	for (int i = 0; i < nTotalSockets; i++) {
	//		SOCKETINFO *ptr = SocketInfoArray[i];
	//		if (FD_ISSET(ptr->sock, &rset)) {
	//			// ������ �ޱ�

	//			// ============ ��ȣ ============
	//			// ���� ũ�� ������ �ޱ�
	//			retval = recvn(ptr->sock, (char*)&recvLen, sizeof(int), 0);
	//			if (retval == 0 || retval == SOCKET_ERROR)
	//			{
	//				RemoveSocketInfo(i);
	//				continue;
	//			}
	//			// ���� ũ�� ������ �ޱ�
	//			retval = recvn(ptr->sock, ptr->buf, recvLen, 0);
	//			if (retval == 0 || retval == SOCKET_ERROR) {
	//				RemoveSocketInfo(i);
	//				continue;
	//			}

	//			//// ���� ����Ʈ �� ����
	//			//ptr->recvbytes += retval;
	//			//sendLen = ptr->recvbytes;
	//			//// ���� ����Ʈ �� ����
	//			//ptr->recvbytes = 0;

	//			// ���� ������ ��� Ŭ���̾�Ʈ�� ������ ����
	//			for (int j = 0; j < nTotalSockets; j++) {
	//				SOCKETINFO *ptr2 = SocketInfoArray[j];

	//				// ======== ��ȣ ==========
	//				// ���� ���� ����
	//				retval = sendn(ptr2->sock, (char*)&recvLen, sizeof(int), 0);
	//				if (retval == SOCKET_ERROR) 
	//				{
	//					err_display("send()");
	//					RemoveSocketInfo(j);
	//					--j; // ���� �ε��� ����
	//					continue;
	//				}

	//				// ���� ���� ����
	//				retval = sendn(ptr2->sock, ptr->buf, recvLen, 0);
	//				if (retval == SOCKET_ERROR) 
	//				{
	//					err_display("send()");
	//					RemoveSocketInfo(j);
	//					--j; // ���� �ε��� ����
	//					continue;
	//				}
	//			}
	//		}
	//	} /* end of for */
	//} /* end of while (1) */

	// �޽��� ����
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ���� ����
	WSACleanup();
	return 0;
}

// ======= ��ȣ =======
// ������ �޽��� ó��
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_SOCKET: // ���� ���� ������ �޽���
		ProcessSocketMessage(hWnd, uMsg, wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// ���� ���� ������ �޽��� ó��
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ������ ��ſ� ����� ����
	SOCKETINFO* ptr;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen, retval;
	char buf[BUFSIZE + 1];

	// ���� �߻� ���� Ȯ��
	if (WSAGETSELECTERROR(lParam)) {
		err_display(WSAGETSELECTERROR(lParam));
		RemoveSocketInfo(wParam);
		return;
	}

	// �޽��� ó��
	switch (WSAGETSELECTEVENT(lParam)) {
	// ����
	case FD_ACCEPT:
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock4, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			return;
		}
		else {
			// ������ Ŭ���̾�Ʈ ���� ���
			printf("\n[TCP/IPv4 ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", 
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			// ���� ���� �߰�
			AddSocketInfoTCP(client_sock);
			retval = WSAAsyncSelect(client_sock, hWnd,
				WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
			if (retval == SOCKET_ERROR) {
				err_display("WSAAsyncSelect()");
				RemoveSocketInfo(client_sock);
			}
		}
		break;
	case FD_READ:
		// TCP socket
		if (wParam != socket_UDP)
		{
			ptr = GetSocketInfo(wParam);
			if (ptr->recvbytes > 0) {
				return;
			}
			// ���� ������ �ޱ�
			retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				RemoveSocketInfo(wParam);
				return;
			}
			// ���� ����Ʈ �� ����
			ptr->recvbytes += retval;

			// ä�� �����͸� ǥ���Ѵ�.
		}
		// UDP socket
		else
		{
			// ������ �ޱ�
			addrlen = sizeof(clientaddr);
			retval = recvfrom(socket_UDP, buf, BUFSIZE, 0, (SOCKADDR*)&clientaddr, &addrlen);
			if (retval == SOCKET_ERROR) {
				err_display("recvfrom()");
				return;
			}

			// UDP�� ������ Ŭ�� ���� ����
			AddSocketInfoUDP(clientaddr);
		}
	case FD_WRITE:
		// UDP ������ �ƴ� ��� (TCP�� ���)
		if (wParam != socket_UDP)
		{
			ptr = GetSocketInfo(wParam);
			//for (int i = 0; i < nTotalSockets; i++) {
			//	SOCKETINFO* ptr = SocketInfoArray[i];
			if (ptr->recvbytes == BUFSIZE) {
				// ���� ����Ʈ �� ����
				ptr->recvbytes = 0;

				// ���� ������ ��� Ŭ���̾�Ʈ���� �����͸� ����!
				for (int j = 0; j < nTotalSockets; j++) {
					SOCKETINFO* ptr2 = SocketInfoArray[j];
					retval = send(ptr2->sock, ptr->buf, BUFSIZE, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						RemoveSocketInfo(j);
						--j; // ���� �ε��� ����
						continue;
					}
				}
			}
			//}
		}
		else
		{
			for (int i = 0; i < nTotalUDPSockets; i++)
			{
				SOCKADDR_IN clientUDP = UDPSocketInfoArray[i];
				// ������ ������
				retval = sendto(socket_UDP, buf, BUFSIZE, 0, (SOCKADDR*)&clientUDP, sizeof(clientUDP));
				if (retval == SOCKET_ERROR) {
					err_display("sendto()");
					return;
				}
			}
		}
		break;
	case FD_CLOSE:
		RemoveSocketInfo(wParam);
		break;
	}
}

// ���̾�α� ���ν���
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hIDListTabCtrl, hChatDataTabCtrl;
	TCITEM tItem_ID, tItem_ChatData;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hIDListTabCtrl = GetDlgItem(hDlg, IDC_IDLIST_TAB);
		tItem_ID.mask = TCIF_TEXT;
		tItem_ID.pszText = (LPTSTR)_T("��ü");
		TabCtrl_InsertItem(hIDListTabCtrl, 0, &tItem_ID); // ù ��° �� �߰�

		tItem_ID.pszText = (LPTSTR)_T("TCP ä��");
		TabCtrl_InsertItem(hIDListTabCtrl, 1, &tItem_ID); // �� ��° �� �߰�
		return TRUE;
	case WM_COMMAND:

	case WM_CLOSE:

		break;
	}

	return FALSE;
}

// ���� ���� ���
SOCKETINFO* GetSocketInfo(SOCKET sock)
{
	// ���� ������ Ŭ���̾�Ʈ �߿��� ��ġ�ϴ� ���� Ž��
	for (int i = 0; i < nTotalSockets; i++)
	{
		SOCKETINFO* ptr = SocketInfoArray[i];
		// ã���� ���, �ش� ���� ��ȯ
		if (ptr->sock == sock)
		{
			return ptr;
		}
	}
	return NULL;
}

// UDP Ŭ�� ���� �߰�
bool AddSocketInfoUDP(SOCKADDR_IN addr)
{
	// ������ ������ ���� �ִ� �������� Ȯ��
	for (int i = 0; i < nTotalUDPSockets; i++)
	{
		if (inet_ntoa(UDPSocketInfoArray[i].sin_addr) == inet_ntoa(addr.sin_addr) &&
			ntohs(UDPSocketInfoArray[i].sin_port) == ntohs(addr.sin_port) &&
			ntohs(UDPSocketInfoArray[i].sin_family) == ntohs(addr.sin_family))
		{
			return false;
		}
	}

	// UDP Ŭ�� ���� �߰�
	UDPSocketInfoArray[nTotalUDPSockets++] = addr;
	return true;
}

// TCP ���� ���� �߰�
bool AddSocketInfoTCP(SOCKET sock)
{
	if (nTotalSockets >= FD_SETSIZE) {
		printf("[����] ���� ������ �߰��� �� �����ϴ�!\n");
		return false;
	}
	SOCKETINFO *ptr = new SOCKETINFO;
	if (ptr == NULL) {
		printf("[����] �޸𸮰� �����մϴ�!\n");
		return false;
	}
	ptr->sock = sock;
	ptr->recvbytes = 0;


	// TCP ���� �迭�� �߰�
	SocketInfoArray[nTotalSockets++] = ptr;

	return true;
}

// ���� ���� ����
void RemoveSocketInfo(int nIndex)
{
	SOCKETINFO *ptr = SocketInfoArray[nIndex];

		// Ŭ���̾�Ʈ ���� ���
		struct sockaddr_in clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (struct sockaddr *)&clientaddr, &addrlen);
		// Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("[TCP/IPv4 ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin_port));


	// ���� �ݱ�
	closesocket(ptr->sock);
	delete ptr;

	if (nIndex != (nTotalSockets - 1))
		SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];
	--nTotalSockets;
}


void addMessage(char* message) {
	//if ((g_msgQueue.tail + 1) % BUFSIZE == g_msgQueue.head) { //ť�� ���� ���: 
	//	g_msgQueue.head = (g_msgQueue.head + 1) % BUFSIZE; //������ ��Ҹ� �ϳ� ����� ���� �ϳ��� Ȯ���Ѵ�.
	//}
	//g_msgQueue.queue[g_msgQueue.tail] = message;
	//g_msgQueue.tail = (g_msgQueue.tail + 1) % BUFSIZE;

}
