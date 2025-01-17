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
	printf("���ڷ���......");
	for (int i = 1; i <= g_nNumPlayer; i++) {
		for (int j = 1; j <= 3; j++) {
			int x = rand() % (NUM_YELLOW_CARD + NUM_PURPLE_CARD) + 1;
			player[i].cardInHand.push_back(card[x]);
			player[i].cardInHand[player[i].nInHand].nStatus = IN_HAND;
			player[i].nInHand++;
		}
	}
	printf("������ϣ�\n");
}

void DisplayCards() {
	system("cls");
	printf("�㣨���1����ң�%d/%d ���ƣ�", player[1].nNumCoin, g_nStdCoinNumber);
	if (!player[1].nInHand) printf("�� ");
	for (int i = 0; i < player[1].nInHand; i++) DisplayCertainCard(player[1].cardInHand[i]);
	printf("\n    ���ƣ�");
	if (!player[1].nOnField) printf("�� ");
	for (int i = 0; i < player[1].nOnField; i++) DisplayCertainCard(player[1].cardOnField[i]);
	printf("\n");
	for (int i = 2; i <= g_nNumPlayer; i++) {
		printf("���%d ��ң�", i);
		if (player[i].nNumCoin == g_nStdCoinNumber - 1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
		printf("%d", player[i].nNumCoin);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
		printf("/%d ���ƣ�", g_nStdCoinNumber);
		if (!player[i].nInHand) printf("�� ");
		else {
			int nYellow = 0, nPurple = 0;
			for (int j = 0; j < player[i].nInHand; j++) {
				if (player[i].cardInHand[j].nColor == YELLOW) nYellow++;
				else nPurple++;
			}
			printf("%d��%d��", nYellow, nPurple);
		}
		printf("\n    ���ƣ�");
		if (!player[i].nOnField) printf("�� ");
		for (int j = 0; j < player[i].nOnField; j++) DisplayCertainCard(player[i].cardOnField[j]);
		printf("\n");
	}
	printf("---------------------------------------\n");
}

void DescribeCards() {
	while (1) {
		system("cls");
		printf("���� 0 ���أ�\n");
		printf("��ѡ����Ҫ�鿴�Ŀ��ƣ�\n");
		printf("1.���� 2.����\n");
		char ch = _getch();
		if (ch == '0') return;
		else if (ch == '1') DescribeYellowCards();
		else if (ch == '2') DescribePurpleCards();
	}
}

void DescribePurpleCards() {
	while (1) {
		system("cls");
		printf("���� 0 ���أ�\n");
		printf("��ѡ����Ҫ�鿴���������ͣ�\n");
		printf("1.������ 2.������\n");
		char ch = _getch();
		if (ch == '0') return;
		else if (ch == '1') DescribePurpleCardsWithArmory();
		else if (ch == '2') DescribePurpleCardsWithoutArmory();
	}
}

void DescribeYellowCards() {
	while (1) {
		system("cls");
		printf("���� 0 ���أ�\n");
		printf("��ѡ����Ҫ�鿴�Ļ������ͣ�\n");
		printf("1.������ 2.������\n");
		char ch = _getch();
		if (ch == '0') return;
		else if (ch == '1') DescribeYellowCardsWithArmory();
		else if (ch == '2') DescribeYellowCardsWithoutArmory();
	}
}

void DescribePurpleCardsWithArmory() {
	while (1) {
		system("cls");
		printf("������ 0 ���أ��ǵð� Enter��\n");
		printf("��ѡ����Ҫ�鿴����ɫ�����ƣ�\n");
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
			cout << "���ƣ�" << cardDescribe[i].strName << endl;
			cout << "���£�";
			if (cardDescribe[i].nArmory == ROYAL) printf("[��]\n");
			else if (cardDescribe[i].nArmory == DAEMON) printf("[ħ]\n");
			else if (cardDescribe[i].nArmory == ROYAL_AND_DAEMON) printf("[��][ħ]\n");
			if (cardDescribe[i].nFire != NO_FIRE || cardDescribe[i].nType != ORDINARY) {
				printf("���ԣ�");
				if (cardDescribe[i].nFire == FIRE) printf("[��]");
				if (cardDescribe[i].nType != ORDINARY) {
					if (cardDescribe[i].nType == ATTACK) printf("[����]");
					else if (cardDescribe[i].nType == CONSISTENT) printf("[����]");
					else if (cardDescribe[i].nType == TRIGGER) printf("[����]");
				}
				printf("\n");
			}
			printf("˵����");
			cout << cardDescribe[i].strDescription << endl;
			printf("-----------------\n");
			system("pause");
		}
	}
}

void DescribePurpleCardsWithoutArmory() {
	while (1) {
		system("cls");
		printf("������ 0 ���أ��ǵð� Enter��\n");
		printf("��ѡ����Ҫ�鿴����ɫ�������ƣ�\n");
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
			cout << "���ƣ�" << cardDescribe[i].strName << endl;
			if (cardDescribe[i].nLevel != NO_LEVEL) printf("�ȼ���%d\n", cardDescribe[i].nLevel);
			if (cardDescribe[i].nFire != NO_FIRE || cardDescribe[i].nType != ORDINARY) {
				printf("���ԣ�");
				if (cardDescribe[i].nFire == FIRE) printf("[��]");
				if (cardDescribe[i].nType != ORDINARY) {
					if (cardDescribe[i].nType == ATTACK) printf("[����]");
					else if (cardDescribe[i].nType == CONSISTENT) printf("[����]");
					else if (cardDescribe[i].nType == TRIGGER) printf("[����]");
				}
				printf("\n");
			}
			printf("˵����");
			cout << cardDescribe[i].strDescription << endl;
			printf("-----------------\n");
			system("pause");
		}
	}
}

void DescribeYellowCardsWithArmory() {
	while (1) {
		system("cls");
		printf("������ 0 ���أ��ǵð� Enter��\n");
		printf("��ѡ����Ҫ�鿴�Ļ�ɫ�����ƣ�\n");
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
			cout << "���ƣ�" << cardDescribe[i].strName << endl;
			cout << "���£�";
			if (cardDescribe[i].nArmory == ROYAL) printf("[��]\n");
			else if (cardDescribe[i].nArmory == DAEMON) printf("[ħ]\n");
			else if (cardDescribe[i].nArmory == ROYAL_AND_DAEMON) printf("[��][ħ]\n");
			if (cardDescribe[i].nFire != NO_FIRE || cardDescribe[i].nType != ORDINARY) {
				printf("���ԣ�");
				if (cardDescribe[i].nFire == FIRE) printf("[��]");
				if (cardDescribe[i].nType != ORDINARY) {
					if (cardDescribe[i].nType == ATTACK) printf("[����]");
					else if (cardDescribe[i].nType == CONSISTENT) printf("[����]");
					else if (cardDescribe[i].nType == TRIGGER) printf("[����]");
				}
				printf("\n");
			}
			printf("˵����");
			cout << cardDescribe[i].strDescription << endl;
			printf("-----------------\n");
			system("pause");
		}
	}
}

void DescribeYellowCardsWithoutArmory() {
	while (1) {
		system("cls");
		printf("������ 0 ���أ��ǵð� Enter��\n");
		printf("��ѡ����Ҫ�鿴����ɫ�������ƣ�\n");
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
			cout << "���ƣ�" << cardDescribe[i].strName << endl;
			if (cardDescribe[i].nLevel != NO_LEVEL) printf("�ȼ���%d\n", cardDescribe[i].nLevel);
			if (cardDescribe[i].nFire != NO_FIRE || cardDescribe[i].nType != ORDINARY) {
				printf("���ԣ�");
				if (cardDescribe[i].nFire == FIRE) printf("[��]");
				if (cardDescribe[i].nType != ORDINARY) {
					if (cardDescribe[i].nType == ATTACK) printf("[����]");
					else if (cardDescribe[i].nType == CONSISTENT) printf("[����]");
					else if (cardDescribe[i].nType == TRIGGER) printf("[����]");
				}
				printf("\n");
			}
			printf("˵����");
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
		printf("���� 0 ���أ�\n");
		printf("��ѡ����Ҫ�鿴�Ĺ������ݣ�\n");
		printf("1.��Ϸ���� 2.�ж��� 3.������� 4.��Ϸ����\n");
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
	printf("��Ϸ�������£�\n");
	printf("1.�����ȷ��ƣ�ÿ�����ţ�Ȼ�����������һ�غ��ߣ��غ������������Ŵ�С������ѭ����\n");
	printf("2.����ʱ������Ҷ�û�н�ң������������Ϸ�л�ã���������ȴﵽ��ʤ���������һ�ʤ��\n");
	printf("3.ÿ���˵ĻغϷ�Ϊ�ĸ��׶Σ�׼���׶�ǰ��׼���׶Ρ������׶Ρ����ƽ׶Ρ�");
	printf("  ׼���׶�ǰһ�㲻��Ҫ���κ��£�ֻ����Щ�����ڴ�ʱ�����ã�����ɿ�����˵������\n");
	printf("  ׼���׶�ʱ���Ƚ����ϵ�û��[����]���ԣ�����ɿ�������ع������ݣ������ջ����У�Ȼ���ϵ�һЩ�ƿ��ܻ������á�\n");
	printf("  �����׶�ʱ����Ұ��ж��㣨����ɿ��ж�����ع������ݣ����в������������������ƿ���һ���ơ����һ�����ơ����ó��ϵ��ơ�\n");
	printf("  ���ƽ׶�ʱ������ҵ����ƶ������ţ����Լ�ѡ����һЩ��ʹ������Ϊ�ġ�\n");
	printf("--------------------------\n");
	system("pause");
}

void ExplainActionPoint() {
	printf("--------------------------\n");
	printf("�ж�����ͣ�\n");
	printf("�ж����Ǿ��������һ���غϵĲ����׶Σ�����ɿ���Ϸ���̣��Ƿ���Լ�����������ֵ����ÿ��\n");
	printf("�غϵĲ����׶ο�ʼʱ�ж�����ֵΪ 2�����������ر�˵�������ơ����ƾ����� 1 �ж��㣬�ж�\n");
	printf("��Ϊ 0 ʱ�޷����������ж���Ĳ��������ó��ϵĿ��Ʋ������ж��㣬��ÿ����ÿ�غ����ֻ��\n");
	printf("������һ�Ρ�\n");
	printf("--------------------------\n");
	system("pause");
}

void ExplainCards() {
	printf("--------------------------\n");
	printf("������ؽ��ͣ�\n");
	printf("���£����°���[��]��[ħ]���֣�����ʿͬʱ�������������£�\n");
	printf("���ԣ����԰���[��]��[����]��[����]��[����]\n");
	printf("     [��]��û���������ã�ֻ��һ����ǩ��\n");
	printf("     [����]��Ҳ��һ����ǩ��������ݶ��ѣ���\n");
	printf("     [����]�����д˱�ǩ�Ŀ��ƴ����д��ʱ����ڳ��ϣ������������Ϊ�������Ƶ����ã���\n");
	printf("            �࿨����׼���׶β���ص����ơ�\n");
	printf("     [����]�������࿨��������ʱ��ӵ���߲�����������������ۺ�ʱ��������˭�Ļغϣ���ֻ\n");
	printf("            Ҫ�ﵽ�˴������������࿨�ƻ��Զ����������ͬ���������ᴥ����������ʱ��ӵ\n");
	printf("            ������ѡ�񴥷����е�һ�š�\n");
	printf("PS��û�еȼ���Lv�����ƣ��ں�������һ�����ȼ��ܺ�ʱ���� Lv.0��\n");
	printf("--------------------------\n");
	system("pause");
}

void ExplainAreas() {
	printf("--------------------------\n");
	printf("��Ϸ������ͣ�\n");
	printf("��Ϸ�����У��ƿ⡢����������ҵ��֡�������ҵ���������͡��ƿ⼴������Դ�أ������кܶ�\n");
	printf("�ƣ������д���������ƶ��������������ÿ����Ҷ���һ���������������ÿ��Ƶ���������\n");
	printf("����ֻ�������⼸�������ڡ�\n");
	printf("--------------------------\n");
	system("pause");
}

void DisplayCertainCard(Card c) {
	if (c.nColor == YELLOW) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
	else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0d);
	if (c.nFire == FIRE) printf("[��]");
	if (c.nArmory == ROYAL) printf("[��]");
	else if (c.nArmory == DAEMON) printf("[ħ]");
	else if (c.nArmory == ROYAL_AND_DAEMON) printf("[��][ħ]");
	else if (c.nLevel != NO_LEVEL) printf("Lv.%d", c.nLevel);
	cout << c.strName;
	if (c.nType == ATTACK) printf("[����]");
	else if (c.nType == CONSISTENT) printf("[����]");
	else if (c.nType == TRIGGER) printf("[����]");
	printf(" ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
}

int RestrictedInput(int l, int r) {
	int x = InputIntSafely();
	while (x < l || x > r) {
		printf("��������ֳ����˷�Χ��\n");
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
		else printf("���벻�淶�����������룺");
	}
	for (int i = 0; i < s.length(); i++) x = x * 10 + s[i] - '0';
	return x;
}