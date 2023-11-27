#include "stdafx.h"


int roundNum = 0;   //������ ���� ����. ���þ� �迭�� �ε��� ���ҵ� �Ѵ�.
int countdown = 30;
const TCHAR* quizWord[4] = { _T("���"), _T("�ٳ���"), _T("����"),_T("������") };   // ���þ� �迭
BOOL isGameOver = FALSE;
BOOL isOwner = FALSE;  // ������ ���� Ŭ���̾�Ʈ�� ��� isOwner�� TRUE�̴�. ������ ���ߴ� ����� ��� FALSE.

void gameStart() {
	int myScore = 0;
	//quizWord = { (char*)"���", (char*)"�ٳ���", (char*)"����", (char*)"������" };
	char* roundText = NULL;

	countdown = 30;

	while (!isGameOver) {

		sprintf(roundText, "%d", roundNum);
		Display(g_hQuizStatus, roundText);
	}
	
	//itoa(timer, timerText, 10);
	//itoa(roundNum, roundText, 10);

}

void Display(HWND g_status, const char* fmt, ...)
{

	va_list arg;
	va_start(arg, fmt);
	char cbuf[1024];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);

	int nLength = GetWindowTextLength(g_status);
	SendMessage(g_status, EM_SETSEL, nLength, nLength);
	SendMessageA(g_status, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
}

DWORD WINAPI TimerThread(LPVOID arg) {

	clock_t start = clock();

	int timer = (int)start;
	countdown = 30 - timer;   //30���� �������� ī��Ʈ�ٿ� ����
	char* timerText = NULL;

	while (!isGameOver) {
		if (countdown <= 0) {
			start = clock();
			newRound();

		}
		sprintf(timerText, "%d", countdown);
		Display(g_hTimerStatus, timerText);
	}

	return 0;
}

//�� ���尡 ���۵� �� ����
void newRound() {
	roundNum += 1;
	DisplayText("%d ��° ���� �Դϴ�.", roundNum+1);
	if (isOwner) {

		Display(g_hQuizStatus, (const char*)quizWord[roundNum]);
	}
}



