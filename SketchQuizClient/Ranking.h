#pragma once

#define BUFSIZE     256                    // ���� �޽��� ��ü ũ��
#define MSGSIZE     (BUFSIZE-sizeof(int))  // ä�� �޽��� �ִ� ����

BOOL CALLBACK DlgRanking(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);