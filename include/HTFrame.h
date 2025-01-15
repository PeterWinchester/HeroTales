/* HTFrame.h
   2024/8/1
   Wirtten by Peter Winchester.
*/

#ifndef HTFRAME_H
#define HTFRAME_H

#include "CppLib.h"

#define GAME_RUNNING 1
#define GAME_OVER    0
#define ROYAL_AND_DAEMON 2
#define ROYAL            1
#define DAEMON           0
#define NO_ARMORY       -1
#define ORDINARY         0
#define ATTACK           1
#define CONSISTENT       2
#define TRIGGER          3 //������
#define YELLOW           0
#define PURPLE           1
#define NO_COLOR        -1
#define NO_LEVEL        -1
#define IN_LIBRARY      -1
#define IN_HAND          0
#define ON_FIELD         1
#define ABANDONED        3
#define NO_FIRE          0
#define FIRE             1
#define GET_COIN         0
#define GET_TWO_CARDS    1

typedef bool (*PCARDFUNC)(int, int); //������ҵĲ�������ָ������
typedef bool (*PCARDFUNC_AI)(int, int, int); //���� AI �Ĳ�������ָ������

const int MAX_NUM_PLAYER = 10;
const int NUM_YELLOW_CARD = 49, NUM_PURPLE_CARD = 49;

extern int g_nGameStatus, g_nNumPlayer, g_nStdCoinNumber, g_iWinPlayer, g_nAbandoned;
/* �� g_nGameStatus Ϊ 0 ʱ����ʾ��Ϸ������δ��ʼ��
   �� g_nGaneStatus Ϊ 1 ʱ����ʾ��Ϸ���ڽ��С�
   g_nStdCoinNumber Ϊ��ʤ�������
*/
extern bool g_bMonkMode; //�����Ƿ�Ϊɮ��ʹ��
extern bool g_bDragonMode; //�����Ƿ�Ϊ����ʹ��

extern int g_iMonkCard; //ɮ��ʹ�õĿ���
extern int g_iDragonCard; //����ʹ�õĿ���
extern int g_iAttacker;   //��ǰ�Ĺ�����

struct Card {
	int nLevel;  // Lv
	int nArmory; // ����
	int nType;   // ����������ȵ�
	int nColor;  // ��ɫ
	int nStatus; // ״̬�����á����ϡ����ϣ�
	int nFire;   // �Ƿ�Ϊ��
	bool bTrans; // �Ƿ��Ѿ����ù�
	string strName, strDescription;
	PCARDFUNC pfnOpt;
	PCARDFUNC_AI pfnOptAI;
	Card();
};
extern Card card[NUM_YELLOW_CARD + NUM_PURPLE_CARD + 1];
extern vector<Card> cardAbandoned;

struct Player {
	int nNumCoin;
	int nActionPoint;
	int nInHand;
	int nOnField;
	vector<Card> cardInHand, cardOnField;
	Player();
};
extern Player player[MAX_NUM_PLAYER + 1];
// ����
void DealCards();
// ��ʾ����ҵĿɼ���
void DisplayCards();
// ��ʾһ���ض�����
void DisplayCertainCard(Card c);
// ��ʾ����˵��
void DescribeCards();
// ��ʾ����˵��
void DescribeYellowCards();
// ��ʾ����˵��
void DescribePurpleCards();
// ��ʾ��ɫ������˵��
void DescribePurpleCardsWithArmory();
// ��ʾ��ɫ��������˵��
void DescribePurpleCardsWithoutArmory();
// ��ʾ��ɫ������˵��
void DescribeYellowCardsWithArmory();
// ��ʾ��ɫ��������˵��
void DescribeYellowCardsWithoutArmory();
// �Ƚ����ſ����Ƿ���ͬ
int  CompareCards(Card card1, Card card2);
// ��ʾ��Ϸ����
void DisplayRules();
// ��ʾ��Ϸ����
void DisplayGameProcess();
// �����ж���
void ExplainActionPoint();
// ���Ϳ���������ݣ����µȣ�
void ExplainCards();
// ������Ϸ����
void ExplainAreas();
int  RestrictedInput(int l, int r); //���Ʒ�Χ������
int  InputIntSafely();              //��ȫ������һ������

#endif
