#include "stdafx.h"

// ============ ��ȣ ==============

// �ش� byte��ŭ �ޱ�
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) 
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0)
		{
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}

// �ش� byte��ŭ ������
int sendn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) 
	{
		received = send(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0)
		{
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}

// ���� ũ�� : ���� ũ�⸦ �˷��ִ� �޽��� ����
void sendMsgLen(SOCKET s, int len)
{
	// g_sock�� static���� ��������������, �Ű����� ���� ����ϸ�, ���� �Ѿ���� ����.
	int retval;
	retval = sendn(s, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("sendMsgSize()");
	}
}