#pragma once

// ���ϰ� ���õ� ���

// =========== ��ȣ ===========

// ���� ũ�� : �ش� byte��ŭ �ޱ�
int recvn(SOCKET s, char* buf, int len, int flags);

// ���� ũ�� : �ش� byte��ŭ ������
int sendn(SOCKET s, char* buf, int len, int flags);

// ���� ũ�� : ���� ũ�⸦ �˷��ִ� �޽��� ����
void sendMsgLen(SOCKET s, int len);