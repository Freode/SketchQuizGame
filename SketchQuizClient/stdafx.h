#pragma once

/*���� ���*/
#include "Winsock.h"
#include "PrjClient.h"
#include "Drawing.h"

/*���ҽ�*/
#include "resource.h" // �׸��� ���̾�α�â

/*�ĺ���*/
#define SERVERIP4  _T("127.0.0.1")
#define SERVERIP6  _T("::1")
#define SERVERPORT  9000

/* ������ ���� ���� ���� */
static HINSTANCE     g_hInstance;     // ���α׷� �ν��Ͻ� �ڵ�
static HWND          g_hBtnSendFile;  // [���� ����] ��ư
static HWND          g_hBtnSendMsg;   // [�޽��� ����] ��ư
static HWND          g_hEditStatus;   // ���� �޽��� ��� ����
static HWND          g_hBtnErasePic;  // [�׸� �����] ��ư
static HWND          g_hDrawWnd;      // �׸��� �׸� ������
static HWND			 g_hMainWindow;   // ó�� ������â �ڵ�
static HWND			 g_hDialog;       // �׸��� ���̾�α� �ڵ�

// ���̾�α� ���� �÷���
static bool          g_bDialogVisible = false;

/* ��� ���� ���� ���� */
static volatile bool g_isIPv6;        // IPv4 or IPv6 �ּ�
static char          g_ipaddr[64];    // ���� IP �ּ�(���ڿ�)
static int           g_port;          // ���� ��Ʈ ��ȣ
static volatile bool g_isUDP;         // TCP or UDP ��������
static HANDLE        g_hClientThread; // ������ �ڵ�
static volatile bool g_bCommStarted;  // ��� ���� ����
static SOCKET        g_sock;          // Ŭ���̾�Ʈ ����
static HANDLE        g_hReadEvent;    // �̺�Ʈ �ڵ�(1)
static HANDLE        g_hWriteEvent;   // �̺�Ʈ �ڵ�(2)

/* �޽��� ���� ���� ���� */
static CHAT_MSG      g_chatmsg;       // ä�� �޽���
static DRAWLINE_MSG  g_drawlinemsg;   // �� �׸��� �޽���
static int           g_drawcolor;     // �� �׸��� ����
static ERASEPIC_MSG  g_erasepicmsg;   // �׸� ����� �޽���