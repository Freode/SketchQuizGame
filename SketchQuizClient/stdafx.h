#pragma once

/*���� ���*/
#include "Winsock.h"
#include "PrjClient.h"
#include "Drawing.h"
#include "DrawingAdd.h"

/*���ҽ�*/
#include "resource.h" // �׸��� ���̾�α�â

/*�ĺ���*/
#define SERVERIP4  _T("192.168.45.186")
#define SERVERIP6  _T("::1")
#define SERVERPORT  9000

#define TYPE_CHAT     1000              // �޽��� Ÿ��: ä��
#define TYPE_DRAWLINE 1001              // �޽��� Ÿ��: �� �׸���
#define TYPE_ERASEPIC 1002              // �޽��� Ÿ��: �׸� �����

#define WM_DRAWLINE (WM_USER+1)         // ����� ���� ������ �޽���(1)
#define WM_ERASEPIC (WM_USER+2)         // ����� ���� ������ �޽���(2)