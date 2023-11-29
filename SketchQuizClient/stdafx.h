#pragma once

/*���� ���*/
#include "Winsock.h"
#include "PrjClient.h"

// ====== ���� ======
#include "Drawing.h"

//* ���� ���� ��� *//
#include "Home.h"
#include "Login.h"
#include "Home_Notice.h"
#include "Home_Pass.h"

// ====== ��ȣ ======
#include "math.h"

// ====== ���� ======
#include "Ranking.h"
#include "Chatting.h"
#include "Game.h"
#include <time.h>

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

// ============= ���� =============
static HWND          g_hRankDlg;      // ��ŷ ���̾�α� �ڵ�
static HWND          g_hChattingDlg;  // ä�� ���̾�α� �ڵ�
static HWND          g_hTimerStatus;  // Ÿ�̸� ����
static HWND          g_hWordStatus;   // ���þ� ����
static int           gameScore;       // ���� ����

extern _TCHAR* messageQueue[10];      // �޽��� ť(���߿� ���� Ŭ���̾�Ʈ���� ���� ä�� ���� ǥ��

// ============= ���� =============
static HWND			 g_hBtnPenColor;  // [���� ����] ��ư
static HWND			 g_hLineWidth;    // �� ���� �޺��ڽ�

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

// ============= ���� =============
static int			 g_lineWidth;     // �� �׸��� ����


/* ���� (2000������) */
// �α��� ����� ���� ���
#define ID_LOGIN_BUTTON			2000	// �α��� ��ư
#define ID_ID_INPUT				2001	//�α��� input
#define ID_DUPLICATION_BUTTON	2002	// �ߺ� Ȯ�� ��ư
#define ID_BACKHOME_BUTTON		2003	// ���ư��� ��ư

// �α��� ���� ���� ����
extern _TCHAR		ID_NICKNAME[256];	// ���� ����� ���̵� ��������

// Ȩ ����� ���� ���
#define ID_CHANNEL_A_BUTTON		2500	// ä�� A ��ư
#define ID_CHANNEL_B_BUTTON		2501	// ä�� B ��ư
#define ID_CHANNEL_RANDOM_BUTTON 2502	// ���� ���� ��ư
#define ID_NOTICE_BUTTON		2506	// ���� ���� ��ư

// Ȩ ���� ���� ����
extern _TCHAR		ID_NOTICE_TEXT[256];	// �������� ����
extern _TCHAR		ID_NOTICE_INPUT_TEXT[256];	// �������� �Է� ����

// Ȩ �������� ����� ���� ��� (2600~ )
#define ID_NOTICE_INPUT			2600	// Ȩ �������� �Է� input
#define ID_OK_BUTTON			2601	// Ȯ�� ��ư
#define ID_CANCLE_BUTTON		2602	// ��� ��ư

// Ȩ �������� ��й�ȣ ����� ���� ��� (2700 ~)
#define ID_PASSWORD_INPUT		2700	// ��й�ȣ �Է� input

// ������ ���� ���� ����
static HWND			hwndLogin;			// �α��� ������ (2000..)
static HWND			hwndHome;			// Ȩ ȭ�� ������ (2500..)
static HWND			hwndHome_Pass;		// �������� ��й�ȣ ������
static HWND			hwndHome_Notice;	// �������� �Է� ������

// ======= ��ȣ =========
#define TYPE_DRAWELLIPSE 4000			// �޽��� Ÿ�� : Ÿ�� �׸���
#define WM_DRAWELLIPSE (WM_USER+3)		// Ÿ�� �׸��� ������ �޽���

// �׸��� ��� ����
#define MODE_ERASE 4500
#define MODE_LINE 4501
#define MODE_ELLIPSE 4502
#define MODE_RECTANGLE 4503
#define MODE_TRIANGLE 4504

// ���� �׸��� ��� ���� ����
static int g_currentSelectFigureMode = MODE_LINE;

// ���� ���� �׸��� ���� ���� ����
static DRAW_DETAIL_INFORMATION g_drawDetailInformation;

static HWND g_hFigureSelect; // �׸� ���� �����ϴ� �κ�

static DRAWELLIPSE_MSG g_drawellipsemsg; // Ÿ�� �׸��� �޽���
//