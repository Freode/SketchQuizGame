#pragma once

#include "stdafx.h"




void gameStart();//���� �����ϴ� �Լ�
void Display(HWND g_status, const char* fmt, ...);//EditText�κп� Text ǥ��
DWORD WINAPI TimerThread(LPVOID arg); //Ÿ�̸� ������
DWORD WINAPI GameThread(LPVOID arg);  //���� ������
void newRound();  // �� ���� ���� �� ����
