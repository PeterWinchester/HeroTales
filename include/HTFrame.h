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
#define TRIGGER          3 //触发牌
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

typedef bool (*PCARDFUNC)(int, int); //用于玩家的操作函数指针类型
typedef bool (*PCARDFUNC_AI)(int, int, int); //用于 AI 的操作函数指针类型

const int MAX_NUM_PLAYER = 10;
const int NUM_YELLOW_CARD = 49, NUM_PURPLE_CARD = 49;

extern int g_nGameStatus, g_nNumPlayer, g_nStdCoinNumber, g_iWinPlayer, g_nAbandoned;
/* 当 g_nGameStatus 为 0 时，表示游戏结束或未开始。
   当 g_nGaneStatus 为 1 时，表示游戏正在进行。
   g_nStdCoinNumber 为获胜金币数。
*/
extern bool g_bMonkMode; //卡牌是否为僧侣使用
extern bool g_bDragonMode; //卡牌是否为火龙使用

extern int g_iMonkCard; //僧侣使用的卡牌
extern int g_iDragonCard; //火龙使用的卡牌
extern int g_iAttacker;   //当前的攻击者

struct Card {
	int nLevel;  // Lv
	int nArmory; // 纹章
	int nType;   // 攻击或持续等等
	int nColor;  // 颜色
	int nStatus; // 状态（横置、手上、场上）
	int nFire;   // 是否为火
	bool bTrans; // 是否已经横置过
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
// 发牌
void DealCards();
// 显示各玩家的可见牌
void DisplayCards();
// 显示一张特定的牌
void DisplayCertainCard(Card c);
// 显示卡牌说明
void DescribeCards();
// 显示黄牌说明
void DescribeYellowCards();
// 显示紫牌说明
void DescribePurpleCards();
// 显示紫色纹章牌说明
void DescribePurpleCardsWithArmory();
// 显示紫色无纹章牌说明
void DescribePurpleCardsWithoutArmory();
// 显示黄色纹章牌说明
void DescribeYellowCardsWithArmory();
// 显示黄色无纹章牌说明
void DescribeYellowCardsWithoutArmory();
// 比较两张卡牌是否相同
int  CompareCards(Card card1, Card card2);
// 显示游戏规则
void DisplayRules();
// 显示游戏过程
void DisplayGameProcess();
// 解释行动点
void ExplainActionPoint();
// 解释卡牌相关内容（纹章等）
void ExplainCards();
// 解释游戏区域
void ExplainAreas();
int  RestrictedInput(int l, int r); //限制范围的输入
int  InputIntSafely();              //安全地输入一个整数

#endif
