/* HTFrame.cpp
   Created - 2024/8/1
   Written by Peter Winchester.
*/

#include "HTFrame.h"

int g_nGameStatus, g_nNumPlayer, g_nStdCoinNumber, g_iWinPlayer, g_nAbandoned;
int g_iMonkCard, g_iDragonCard, g_iAttacker;
bool g_bMonkMode, g_bDragonMode;
Card card[NUM_YELLOW_CARD + NUM_PURPLE_CARD + 1];
vector<Card> cardAbandoned;
Player player[MAX_NUM_PLAYER + 1];

Card::Card() {
	nLevel      = NO_LEVEL;
	nArmory     = NO_ARMORY;
	nType       = ORDINARY;
	nColor      = NO_COLOR;
	nStatus     = IN_LIBRARY;
	nFire       = NO_FIRE;
	nNumber     = 0;
	nPrefer     = 0;
	bTrans      = false;
	pfnOpt      = NULL;
	pfnOptAI    = NULL;
	pfnCalcPref = NULL;
}

Player::Player() {
	nNumCoin     = 0;
	nActionPoint = 2;
	nInHand      = 0;
	nOnField     = 0;
}

void DealCards() {
	printf("正在发牌......");
	for (int i = 1; i <= g_nNumPlayer; i++) {
		for (int j = 1; j <= 3; j++) {
			int x = rand() % (NUM_YELLOW_CARD + NUM_PURPLE_CARD) + 1;
			player[i].cardInHand.push_back(card[x]);
			player[i].cardInHand[player[i].nInHand].nStatus = IN_HAND;
			player[i].nInHand++;
		}
	}
	printf("发牌完毕！\n");
}

void DisplayCards() {
	system("cls");
	printf("你（玩家1）金币：%d/%d 手牌：", player[1].nNumCoin, g_nStdCoinNumber);
	if (!player[1].nInHand) printf("无 ");
	for (int i = 0; i < player[1].nInHand; i++) DisplayCertainCard(player[1].cardInHand[i]);
	printf("\n    场牌：");
	if (!player[1].nOnField) printf("无 ");
	for (int i = 0; i < player[1].nOnField; i++) DisplayCertainCard(player[1].cardOnField[i]);
	printf("\n");
	for (int i = 2; i <= g_nNumPlayer; i++) {
		printf("玩家%d 金币：", i);
		if (player[i].nNumCoin == g_nStdCoinNumber - 1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
		printf("%d", player[i].nNumCoin);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
		printf("/%d 手牌：", g_nStdCoinNumber);
		if (!player[i].nInHand) printf("无 ");
		else {
			int nYellow = 0, nPurple = 0;
			for (int j = 0; j < player[i].nInHand; j++) {
				if (player[i].cardInHand[j].nColor == YELLOW) nYellow++;
				else nPurple++;
			}
			printf("%d黄%d紫", nYellow, nPurple);
		}
		printf("\n    场牌：");
		if (!player[i].nOnField) printf("无 ");
		for (int j = 0; j < player[i].nOnField; j++) DisplayCertainCard(player[i].cardOnField[j]);
		printf("\n");
	}
	printf("---------------------------------------\n");
}

void DescribeCards() {
	while (1) {
		system("cls");
		printf("（按 0 返回）\n");
		printf("请选择你要查看的卡牌：\n");
		printf("1.黄牌 2.紫牌\n");
		char ch = _getch();
		if (ch == '0') return;
		else if (ch == '1') DescribeYellowCards();
		else if (ch == '2') DescribePurpleCards();
	}
}

void DescribePurpleCards() {
	while (1) {
		system("cls");
		printf("（按 0 返回）\n");
		printf("请选择你要查看的紫牌类型：\n");
		printf("1.纹章牌 2.无纹章\n");
		char ch = _getch();
		if (ch == '0') return;
		else if (ch == '1') DescribePurpleCardsWithArmory();
		else if (ch == '2') DescribePurpleCardsWithoutArmory();
	}
}

void DescribeYellowCards() {
	while (1) {
		system("cls");
		printf("（按 0 返回）\n");
		printf("请选择你要查看的黄牌类型：\n");
		printf("1.纹章牌 2.无纹章\n");
		char ch = _getch();
		if (ch == '0') return;
		else if (ch == '1') DescribeYellowCardsWithArmory();
		else if (ch == '2') DescribeYellowCardsWithoutArmory();
	}
}

void DescribePurpleCardsWithArmory() {
	while (1) {
		system("cls");
		printf("（输入 0 返回，记得按 Enter）\n");
		printf("请选择你要查看的紫色纹章牌：\n");
		int iCurrentCard = 0;
		vector<Card> cardDescribe;
		for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
			if (!CompareCards(card[i], card[i - 1]) && card[i].nArmory != NO_ARMORY) {
				cardDescribe.push_back(card[i]);
				iCurrentCard++;
				printf("%d.", iCurrentCard);
				DisplayCertainCard(card[i]);
			}
		}
		printf("\n");
		int ch = InputIntSafely();
		if (ch == 0) return;
		else if (ch >= 1 && ch <= iCurrentCard) {
			int i = ch - 1;
			printf("-----------------\n");
			cout << "名称：" << cardDescribe[i].strName << endl;
			cout << "纹章：";
			if (cardDescribe[i].nArmory == ROYAL) printf("[皇]\n");
			else if (cardDescribe[i].nArmory == DAEMON) printf("[魔]\n");
			else if (cardDescribe[i].nArmory == ROYAL_AND_DAEMON) printf("[皇][魔]\n");
			if (cardDescribe[i].nFire != NO_FIRE || cardDescribe[i].nType != ORDINARY) {
				printf("属性：");
				if (cardDescribe[i].nFire == FIRE) printf("[火]");
				if (cardDescribe[i].nType != ORDINARY) {
					if (cardDescribe[i].nType == ATTACK) printf("[攻击]");
					else if (cardDescribe[i].nType == CONSISTENT) printf("[持续]");
					else if (cardDescribe[i].nType == TRIGGER) printf("[触发]");
				}
				printf("\n");
			}
			printf("说明：");
			cout << cardDescribe[i].strDescription << endl;
			printf("-----------------\n");
			system("pause");
		}
	}
}

void DescribePurpleCardsWithoutArmory() {
	while (1) {
		system("cls");
		printf("（输入 0 返回，记得按 Enter）\n");
		printf("请选择你要查看的紫色无纹章牌：\n");
		int iCurrentCard = 0;
		vector<Card> cardDescribe;
		for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
			if (!CompareCards(card[i], card[i - 1]) && card[i].nArmory == NO_ARMORY) {
				cardDescribe.push_back(card[i]);
				iCurrentCard++;
				printf("%d.", iCurrentCard);
				DisplayCertainCard(card[i]);
			}
		}
		printf("\n");
		int ch = InputIntSafely();
		if (ch == 0) return;
		else if (ch >= 1 && ch <= iCurrentCard) {
			int i = ch - 1;
			printf("-----------------\n");
			cout << "名称：" << cardDescribe[i].strName << endl;
			if (cardDescribe[i].nLevel != NO_LEVEL) printf("等级：%d\n", cardDescribe[i].nLevel);
			if (cardDescribe[i].nFire != NO_FIRE || cardDescribe[i].nType != ORDINARY) {
				printf("属性：");
				if (cardDescribe[i].nFire == FIRE) printf("[火]");
				if (cardDescribe[i].nType != ORDINARY) {
					if (cardDescribe[i].nType == ATTACK) printf("[攻击]");
					else if (cardDescribe[i].nType == CONSISTENT) printf("[持续]");
					else if (cardDescribe[i].nType == TRIGGER) printf("[触发]");
				}
				printf("\n");
			}
			printf("说明：");
			cout << cardDescribe[i].strDescription << endl;
			printf("-----------------\n");
			system("pause");
		}
	}
}

void DescribeYellowCardsWithArmory() {
	while (1) {
		system("cls");
		printf("（输入 0 返回，记得按 Enter）\n");
		printf("请选择你要查看的黄色纹章牌：\n");
		int iCurrentCard = 0;
		vector<Card> cardDescribe;
		for (int i = 1; i <= NUM_YELLOW_CARD; i++) {
			if (!CompareCards(card[i], card[i - 1]) && card[i].nArmory != NO_ARMORY) {
				cardDescribe.push_back(card[i]);
				iCurrentCard++;
				printf("%d.", iCurrentCard);
				DisplayCertainCard(card[i]);
			}
		}
		printf("\n");
		int ch = InputIntSafely();
		if (ch == 0) return;
		else if (ch >= 1 && ch <= iCurrentCard) {
			int i = ch - 1;
			printf("-----------------\n");
			cout << "名称：" << cardDescribe[i].strName << endl;
			cout << "纹章：";
			if (cardDescribe[i].nArmory == ROYAL) printf("[皇]\n");
			else if (cardDescribe[i].nArmory == DAEMON) printf("[魔]\n");
			else if (cardDescribe[i].nArmory == ROYAL_AND_DAEMON) printf("[皇][魔]\n");
			if (cardDescribe[i].nFire != NO_FIRE || cardDescribe[i].nType != ORDINARY) {
				printf("属性：");
				if (cardDescribe[i].nFire == FIRE) printf("[火]");
				if (cardDescribe[i].nType != ORDINARY) {
					if (cardDescribe[i].nType == ATTACK) printf("[攻击]");
					else if (cardDescribe[i].nType == CONSISTENT) printf("[持续]");
					else if (cardDescribe[i].nType == TRIGGER) printf("[触发]");
				}
				printf("\n");
			}
			printf("说明：");
			cout << cardDescribe[i].strDescription << endl;
			printf("-----------------\n");
			system("pause");
		}
	}
}

void DescribeYellowCardsWithoutArmory() {
	while (1) {
		system("cls");
		printf("（输入 0 返回，记得按 Enter）\n");
		printf("请选择你要查看的紫色无纹章牌：\n");
		int iCurrentCard = 0;
		vector<Card> cardDescribe;
		for (int i = 1; i <= NUM_YELLOW_CARD; i++) {
			if (!CompareCards(card[i], card[i - 1]) && card[i].nArmory == NO_ARMORY) {
				cardDescribe.push_back(card[i]);
				iCurrentCard++;
				printf("%d.", iCurrentCard);
				DisplayCertainCard(card[i]);
			}
		}
		printf("\n");
		int ch = InputIntSafely();
		if (ch == 0) return;
		else if (ch >= 1 && ch <= iCurrentCard) {
			int i = ch - 1;
			printf("-----------------\n");
			cout << "名称：" << cardDescribe[i].strName << endl;
			if (cardDescribe[i].nLevel != NO_LEVEL) printf("等级：%d\n", cardDescribe[i].nLevel);
			if (cardDescribe[i].nFire != NO_FIRE || cardDescribe[i].nType != ORDINARY) {
				printf("属性：");
				if (cardDescribe[i].nFire == FIRE) printf("[火]");
				if (cardDescribe[i].nType != ORDINARY) {
					if (cardDescribe[i].nType == ATTACK) printf("[攻击]");
					else if (cardDescribe[i].nType == CONSISTENT) printf("[持续]");
					else if (cardDescribe[i].nType == TRIGGER) printf("[触发]");
				}
				printf("\n");
			}
			printf("说明：");
			cout << cardDescribe[i].strDescription << endl;
			printf("-----------------\n");
			system("pause");
		}
	}
}

int CompareCards(Card card1, Card card2) {
	if (card1.nLevel != card2.nLevel) return 0;
	if (card1.nArmory != card2.nArmory) return 0;
	if (card1.nType != card2.nType) return 0;
	if (card1.nColor != card2.nColor) return 0;
	if (card1.nFire != card2.nFire) return 0;
	if (card1.strName != card2.strName) return 0;
	return 1;
}

void DisplayRules() {
	while (1) {
		system("cls");
		printf("（按 0 返回）\n");
		printf("请选择你要查看的规则内容：\n");
		printf("1.游戏流程 2.行动点 3.卡牌相关 4.游戏区域\n");
		char ch = _getch();
		if (ch == '0') return;
		else if (ch == '1') DisplayGameProcess();
		else if (ch == '2') ExplainActionPoint();
		else if (ch == '3') ExplainCards();
		else if (ch == '4') ExplainAreas();
	}
}

void DisplayGameProcess() {
	printf("--------------------------\n");
	printf("游戏过程如下：\n");
	printf("1.开局先发牌，每人三张，然后随机决定第一回合者，回合轮流按玩家序号从小到大方向循环。\n");
	printf("2.开局时所有玩家都没有金币，金币来自于游戏中获得，金币数量先达到获胜金币数的玩家获胜。\n");
	printf("3.每个人的回合分为四个阶段：准备阶段前、准备阶段、操作阶段、弃牌阶段。");
	printf("  准备阶段前一般不需要做任何事，只是有些卡牌在此时起作用（具体可看卡牌说明）。\n");
	printf("  准备阶段时，先将场上的没有[持续]属性（具体可看卡牌相关规则内容）的牌收回手中，然后场上的一些牌可能会起作用。\n");
	printf("  操作阶段时，玩家按行动点（具体可看行动点相关规则内容）进行操作，操作包括：从牌库摸一张牌、打出一张手牌、横置场上的牌。\n");
	printf("  弃牌阶段时，若玩家的手牌多于四张，需自己选择丢弃一些牌使手牌数为四。\n");
	printf("--------------------------\n");
	system("pause");
}

void ExplainActionPoint() {
	printf("--------------------------\n");
	printf("行动点解释：\n");
	printf("行动点是决定玩家在一个回合的操作阶段（具体可看游戏过程）是否可以继续操作的数值，在每个\n");
	printf("回合的操作阶段开始时行动点数值为 2，除卡牌有特别说明，摸牌、打牌均消耗 1 行动点，行动\n");
	printf("点为 0 时无法进行消耗行动点的操作。横置场上的卡牌不消耗行动点，但每张牌每回合最多只能\n");
	printf("被横置一次。\n");
	printf("--------------------------\n");
	system("pause");
}

void ExplainCards() {
	printf("--------------------------\n");
	printf("卡牌相关解释：\n");
	printf("纹章：纹章包括[皇]、[魔]两种（黑骑士同时带有这两个纹章）\n");
	printf("属性：属性包括[火]、[攻击]、[持续]、[触发]\n");
	printf("     [火]：没有特殊作用，只是一个标签。\n");
	printf("     [攻击]：也是一个标签（标明身份而已）。\n");
	printf("     [持续]：带有此标签的卡牌从手中打出时需放在场上，且如果不是因为其它卡牌的作用，此\n");
	printf("            类卡牌在准备阶段不会回到手牌。\n");
	printf("     [触发]：当此类卡牌在手中时，拥有者不能主动打出，而无论何时（即无论谁的回合），只\n");
	printf("            要达到了触发条件，此类卡牌会自动打出。当相同触发条件会触发多张手牌时，拥\n");
	printf("            有者需选择触发其中的一张。\n");
	printf("PS：没有等级（Lv）的牌，在和其它牌一起计算等级总和时视作 Lv.0。\n");
	printf("--------------------------\n");
	system("pause");
}

void ExplainAreas() {
	printf("--------------------------\n");
	printf("游戏区域解释：\n");
	printf("游戏区域有：牌库、弃牌区、玩家的手、场。玩家的手无需解释。牌库即抽牌来源地（那里有很多\n");
	printf("牌）。所有打出或丢弃的牌都会进入弃牌区。每个玩家都有一个场，即公开放置卡牌的区域。所有\n");
	printf("卡牌只可能在这几个区域内。\n");
	printf("--------------------------\n");
	system("pause");
}

void DisplayCertainCard(Card c) {
	if (c.nColor == YELLOW) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
	else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0d);
	if (c.nFire == FIRE) printf("[火]");
	if (c.nArmory == ROYAL) printf("[皇]");
	else if (c.nArmory == DAEMON) printf("[魔]");
	else if (c.nArmory == ROYAL_AND_DAEMON) printf("[皇][魔]");
	else if (c.nLevel != NO_LEVEL) printf("Lv.%d", c.nLevel);
	cout << c.strName;
	if (c.nType == ATTACK) printf("[攻击]");
	else if (c.nType == CONSISTENT) printf("[持续]");
	else if (c.nType == TRIGGER) printf("[触发]");
	printf(" ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
}

int RestrictedInput(int l, int r) {
	int x = InputIntSafely();
	while (x < l || x > r) {
		printf("输入的数字超出了范围！\n");
		x = InputIntSafely();
	}
	return x;
}

int InputIntSafely() {
	string s;
	int x = 0;
	while (1) {
		getline(cin, s);
		bool b = true;
		for (int i = 0; i < s.length(); i++) {
			if (s[i] >= '0' && s[i] <= '9') continue;
			b = false;
			break;
		}
		if (b) break;
		else printf("输入不规范，请重新输入：");
	}
	for (int i = 0; i < s.length(); i++) x = x * 10 + s[i] - '0';
	return x;
}