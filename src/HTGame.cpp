/* HTGame.cpp
   2024/8/1
   Written by Peter Winchester
*/

#include "HTGame.h"

void ReadyStageAI(int iCurrentPlayer) {
	/* ׼���׶�ǰ��*/
	if (player[iCurrentPlayer].nOnField > 1) {
		/* ���ը���֡�*/
		int nBomb = 0;
		for (int i = 0; i < player[iCurrentPlayer].nOnField; i++) {
			if (player[iCurrentPlayer].cardOnField[i].strName == "ը����") nBomb++;
		}
		if (nBomb) {
			int nReturn = 0;
			for (int i = 0; i < player[iCurrentPlayer].nOnField;) {
				if (player[iCurrentPlayer].cardOnField[i].strName != "ը����") {
					ReturnCardOnField(iCurrentPlayer, i);
					nReturn++;
					if (nReturn == nBomb) break;
				} else i++;
			}
			if (nReturn) {
				printf("��� %d ������ %d �� ", iCurrentPlayer, nBomb);
				DisplayCertainCard(card[69]);
				printf("��ʹ�䳡�ϵ� %d ���ƻص����ơ�\n", nReturn);
				system("pause");
				DisplayCards();
			}
		}
	}
	AbandonBombAI(iCurrentPlayer);
	/* ׼���׶Ρ�*/
	int nReturn = 0;
	bool bSonBird = CheckSonBird(iCurrentPlayer);
	for (int i = 0; i < player[iCurrentPlayer].nOnField;) {
		if (player[iCurrentPlayer].cardOnField[i].nType != CONSISTENT) {
			ReturnCardOnField(iCurrentPlayer, i);
			nReturn++;
			AbandonBombAI(iCurrentPlayer);
			if (nReturn == 2) {
				if (bSonBird) {
					player[iCurrentPlayer].nNumCoin++;
					for (int j = 0; j < player[iCurrentPlayer].nInHand; j++) {
						if (player[iCurrentPlayer].cardInHand[j].strName == "���Ӻ�") {
							AbandonCertainCard(iCurrentPlayer, j, IN_HAND);
							break;
						}
					}
					DisplayCards();
					printf("��׼���׶Σ���� %d �����ż����ϵĿ��ƻص����ƣ����� ", iCurrentPlayer);
					DisplayCertainCard(card[24]);
					printf("����� 1 ��ҡ�\n");
					system("pause");
					if (player[iCurrentPlayer].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = iCurrentPlayer;
						g_nGameStatus = 0;
						return;
					}
					CheckNun(iCurrentPlayer);
					CheckDancer(iCurrentPlayer);
					if (!g_nGameStatus) return;
					CheckMimic(iCurrentPlayer, GET_COIN);
				}
			}
		} else i++;
	}
	DisplayCards();
	/* ���ʥ����*/
	if (CheckGrail(iCurrentPlayer)) {
		printf("��� %d ������ ", iCurrentPlayer);
		DisplayCertainCard(card[1]);
		printf("����ʱ��ֱ�ӻ�ʤ��\n");
		g_iWinPlayer = iCurrentPlayer;
		g_nGameStatus = 0;
		return;
	}
	/* ��鹫����*/
	if (CheckPrincess(iCurrentPlayer)) {
		player[iCurrentPlayer].nNumCoin++;
		DisplayCards();
		printf("��� %d ������ ", iCurrentPlayer);
		DisplayCertainCard(card[3]);
		printf("��+1 ��ҡ�\n");
		system("pause");
		/* �ټ��ʥ����*/
		if (CheckGrail(iCurrentPlayer)) {
			printf("��� %d ������ ", iCurrentPlayer);
			DisplayCertainCard(card[1]);
			printf("����ʱ��ֱ�ӻ�ʤ��\n");
			g_iWinPlayer = iCurrentPlayer;
			g_nGameStatus = 0;
			return;
		}
		if (player[iCurrentPlayer].nNumCoin == g_nStdCoinNumber) {
			g_iWinPlayer = iCurrentPlayer;
			g_nGameStatus = 0;
			return;
		}
		CheckDancer(iCurrentPlayer);
		if (!g_nGameStatus) return;
		CheckMimic(iCurrentPlayer, GET_COIN);
	}
}

void OperationStageAI(int iPlayer) {
	/* ���� */
	for (int i = 0; i < player[iPlayer].nInHand; i++) {
		if (player[iPlayer].cardInHand[i].nType != TRIGGER) {
			player[iPlayer].cardInHand[i].pfnOptAI(iPlayer, i, IN_HAND);
			if (!g_nGameStatus) return;
		}
	}
	if (player[iPlayer].nActionPoint) {
		/* ���� */
		/* �ɶ������ã�������������������*/
		for (int i = 0; i < player[iPlayer].nOnField; i++) {
			if (CheckTransverse(player[iPlayer].cardOnField[i])) player[iPlayer].cardOnField[i].pfnOptAI(iPlayer, i, ON_FIELD);
			if (!g_nGameStatus) return;
		}
	}
	/* ���� */
	if (player[iPlayer].nActionPoint > 0) {
		bool bMimic = (player[iPlayer].nActionPoint == 2);
		printf("��� %d ��������......\n", iPlayer);
		while (player[iPlayer].nActionPoint > 0) {
			player[iPlayer].nActionPoint--;
			GetOneCardFromLibrary(iPlayer);
			AbandonBombAI(iPlayer);
		}
		printf("��� %d ���ƽ�����\n", iPlayer);
		system("pause");
		DisplayCards();
		if (bMimic) {
			CheckMimic(iPlayer, GET_TWO_CARDS);
			if (!g_nGameStatus) return;
		}
	}
	AbandonCardAI(iPlayer);
}

int CheckGameFinish() {
	if (!g_nGameStatus) {
		printf("��Ϸ��������� %d ��ʤ��\n", g_iWinPlayer);
		system("pause");
		return 1;
	}
	return 0;
}

void ReadyStage() {
	/* ׼���׶�ǰ��*/
	if (player[1].nOnField > 1) {
		/* ���ը���֡�*/
		int nBomb = 0;
		for (int i = 0; i < player[1].nOnField; i++) {
			if (player[1].cardOnField[i].strName == "ը����") nBomb++;
		}
		if (nBomb) {
			printf("�㳡���� %d �� ", nBomb);
			DisplayCertainCard(card[69]);
			printf("����ѡ���㳡������ %d �������ƻص����ơ�\n", nBomb);
			int nReturn = 0;
			while (nReturn < nBomb && player[1].nOnField > nBomb) {
				int n = 0;
				vector<int> iRe;
				printf("��ѡ��һ�Żص����ƣ�\n");
				for (int i = 0; i < player[1].nOnField; i++) {
					if (player[1].cardOnField[i].strName != "ը����") {
						printf("%d.", ++n);
						DisplayCertainCard(player[1].cardOnField[i]);
						iRe.push_back(i);
					}
				}
				printf("\n");
				int ch = RestrictedInput(1, n);
				ReturnCardOnField(1, iRe[ch - 1]);
				nReturn++;
			}
		}
	}
	AbandonBomb();
	/* ׼���׶Ρ�*/
	int nReturn = 0;
	bool bSonBird = CheckSonBird(1);
	for (int i = 0; i < player[1].nOnField;) {
		if (player[1].cardOnField[i].nType != CONSISTENT) {
			ReturnCardOnField(1, i);
			nReturn++;
			AbandonBomb();
			if (nReturn == 2) {
				if (bSonBird) {
					player[1].nNumCoin++;
					for (int j = 0; j < player[1].nInHand; j++) {
						if (player[1].cardInHand[j].strName == "���Ӻ�") {
							AbandonCertainCard(1, j, IN_HAND);
							break;
						}
					}
					DisplayCards();
					printf("�������ż����ϵĿ��ƻص����ƣ����� ");
					DisplayCertainCard(card[24]);
					printf("����� 1 ��ң�\n");
					system("pause");
					if (player[1].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = 1;
						g_nGameStatus = 0;
						return;
					}
					CheckNun(1);
					CheckDancer(1);
					if (!g_nGameStatus) return;
					CheckMimic(1, GET_COIN);
				}
			}
		} else i++;
	}
	DisplayCards();
	/* ���ʥ����*/
	if (CheckGrail(1)) {
		printf("�㳡���� ");
		DisplayCertainCard(card[1]);
		printf("����ʱ��ֱ�ӻ�ʤ��\n");
		g_iWinPlayer = 1;
		g_nGameStatus = 0;
		return;
	}
	/* ��鹫����*/
	if (CheckPrincess(1)) {
		player[1].nNumCoin++;
		DisplayCards();
		printf("�㳡���� ");
		DisplayCertainCard(card[3]);
		printf("��+1 ��ҡ�\n");
		system("pause");
		/* ���ʥ����*/
		if (CheckGrail(1)) {
			printf("�㳡���� ");
			DisplayCertainCard(card[1]);
			printf("����ʱ��ֱ�ӻ�ʤ��\n");
			g_iWinPlayer = 1;
			g_nGameStatus = 0;
			return;
		}
		if (player[1].nNumCoin == g_nStdCoinNumber) {
			g_iWinPlayer = 1;
			g_nGameStatus = 0;
			return;
		}
		CheckDancer(1);
		if (!g_nGameStatus) return;
		CheckMimic(1, GET_COIN);
	}
}

void OperationStage() {
	int nGetCard = 0;
	for (int i = 0; i < player[1].nOnField; i++) {
		player[1].cardOnField[i].bTrans = false;
	}
	bool bRound = true;
	while (bRound) {
		DisplayCards();
		if (!g_nGameStatus) return;
		printf("�㻹�� %d ���ж��㡣\n", player[1].nActionPoint);
		printf("��ѡ����Ҫ���еĲ�����1.���� 2.���� 3.���� 4.�鿴������ 5.����˵�� 6.�����غ�\n");
		while (1) {
			char ch = _getch();
			if (ch == '1') { //����
				if (!player[1].nActionPoint) {
					printf("��û���ж����ˣ�\n");
					system("pause");
					break;
				}
				nGetCard++;
				GetOneCardFromLibrary(1);
				DisplayCards();
				player[1].nActionPoint--;
				printf("��������һ�� ");
				DisplayCertainCard(player[1].cardInHand.back());
				printf("��\n");
				system("pause");
				AbandonBomb();
				break;
			} else if (ch == '2') { //����
				if (!CheckCardCanBeUsed()) {
					printf("��û�п��Դ�������ƣ�\n");
					system("pause");
					break;
				}
				printf("��������������ȡ����\n");
				printf("��ѡ����Ҫ��������ƣ�\n");
				int nCardToBeUsed = 0;
				vector<int> iCardToBeUsed;
				for (int i = 0; i < player[1].nInHand; i++) {
					if (player[1].cardInHand[i].nType != TRIGGER) {
						iCardToBeUsed.push_back(i);
						nCardToBeUsed++;
					}
				}
				for (int i = 0; i < nCardToBeUsed; i++) {
					printf("%d.", i + 1);
					DisplayCertainCard(player[1].cardInHand[iCardToBeUsed[i]]);
				}
				printf("\n");
				int iCh = InputIntSafely();
				if (iCh >= 1 && iCh <= nCardToBeUsed) {
					if (!player[1].nActionPoint && player[1].cardInHand[iCardToBeUsed[iCh - 1]].strName != "����") {
						printf("��û���ж����ˣ����ܴ�����ƣ�\n");
						system("pause");
						break;
					}
					player[1].cardInHand[iCardToBeUsed[iCh - 1]].pfnOpt(iCardToBeUsed[iCh - 1], IN_HAND);
				}
				break;
			} else if (ch == '3') { //����
				if (!CheckCardCanTransverse()) {
					printf("��û�п��Ժ��õĳ��ƣ�\n");
					system("pause");
				} else {
					printf("��������������ȡ����\n");
					printf("��ѡ����Ҫ���õĳ��ƣ�\n");
					int nCardToTransverse = 0;
					vector<int> iCardToTransverse;
					for (int i = 0; i < player[1].nOnField; i++) {
						if (CheckTransverse(player[1].cardOnField[i])) {
							iCardToTransverse.push_back(i);
							nCardToTransverse++;
						}
					}
					for (int i = 0; i < nCardToTransverse; i++) {
						printf("%d.", i + 1);
						DisplayCertainCard(player[1].cardOnField[iCardToTransverse[i]]);
					}
					printf("\n");
					int iCh = InputIntSafely();
					if (iCh >= 1 && iCh <= nCardToTransverse) {
						if (player[1].cardOnField[iCardToTransverse[iCh - 1]].bTrans) {
							printf("�����ѱ����ù�һ�Σ�\n");
							system("pause");
						} else {
							player[1].cardOnField[iCardToTransverse[iCh - 1]].pfnOpt(iCardToTransverse[iCh - 1], ON_FIELD);
							player[1].cardOnField[iCardToTransverse[iCh - 1]].bTrans = true;
						}
					}
				}
				break;
			} else if (ch == '4') { //�鿴������
				if (!g_nAbandoned) {
					printf("������û���ơ�\n");
					system("pause");
				} else {
					printf("�������������Ŀ��ƣ�\n");
					for (int i = 0; i < g_nAbandoned; i++) {
						printf("%d.", i + 1);
						DisplayCertainCard(cardAbandoned[i]);
						printf("\n");
						if (i % 5 == 4) system("pause");
					}
					system("pause");
				}
				break;
			} else if (ch == '5') { //�鿴����˵��
				DescribeCards();
				break;
			} else if (ch == '6') {
				if (player[1].nActionPoint) {
					printf("�㻹���ж��㣬���ܽ����غϡ�\n");
					system("pause");
				} else bRound = false;
				break;
			}
		}
	}
	if (nGetCard == 2) {
		CheckMimic(1, GET_TWO_CARDS);
		if (!g_nGameStatus) return;
	}
	AbandonCard();
}

void ReturnCardOnField(int iPlayer, int iCard) {
	player[iPlayer].cardOnField[iCard].bTrans = false;
	player[iPlayer].cardOnField[iCard].nStatus = IN_HAND;
	player[iPlayer].cardInHand.push_back(player[iPlayer].cardOnField[iCard]);
	player[iPlayer].cardOnField.erase(player[iPlayer].cardOnField.begin() + iCard);
	player[iPlayer].nOnField--;
	player[iPlayer].nInHand++;
}

bool CheckSonBird(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nInHand; i++) {
		if (player[iPlayer].cardInHand[i].strName == "���Ӻ�") return true;
	}
	return false;
}

bool CheckGrail(int iPlayer) {
	if (player[iPlayer].nNumCoin < g_nStdCoinNumber - 1) return false;
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "ʥ��") return true;
	}
	return false;
}

bool CheckPrincess(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "����") return true;
	}
	return false;
}

void AbandonCardAI(int iPlayer) {
	if (player[iPlayer].nInHand <= 4) return;
	printf("��� %d ��������......\n", iPlayer);
	while (player[iPlayer].nInHand > 4) {
		int x = rand() % player[iPlayer].nInHand;
		Card c = player[iPlayer].cardInHand[x];
		AbandonCertainCard(iPlayer, x, IN_HAND);
		printf("��� %d ����һ�� ", iPlayer);
		DisplayCertainCard(c);
		printf("��\n");
		CheckNun(iPlayer);
	}
	printf("��� %d ������ϡ�\n", iPlayer);
	system("pause");
	DisplayCards();
}

void AbandonCard() {
	if (player[1].nInHand <= 4) return;
	while (player[1].nInHand > 4) {
		printf("������Ƴ��� 4 �ţ��趪�� %d �š�\n", player[1].nInHand - 4);
		printf("��ѡ����Ҫ���������ƣ�\n");
		for (int i = 0; i < player[1].nInHand; i++) {
			printf("%d.", i + 1);
			DisplayCertainCard(player[1].cardInHand[i]);
		}
		printf("\n");
		while (1) {
			int ch = InputIntSafely();
			if (ch >= 1 && ch <= player[1].nInHand) {
				AbandonCertainCard(1, ch - 1, IN_HAND);
				CheckNun(1);
				break;
			} else printf("��������ֳ����˷�Χ��\n");
		}
	}
	DisplayCards();
}

bool CheckNun(int iPlayer) {
	if (cardAbandoned.back().nLevel == 1) return false;
	short nPlayerWithNun = 0;
	vector<short> iPlayerWithNun;
	for (int i = 1; i <= g_nNumPlayer; i++) {
		if (i == iPlayer) continue;
		for (int j = 0; j < player[i].nInHand; j++) {
			if (player[i].cardInHand[j].strName == "��Ů") {
				nPlayerWithNun++;
				iPlayerWithNun.push_back(i);
				break;
			}
		}
	}
	if (nPlayerWithNun == 0) return false;
	if (nPlayerWithNun == 1) {
		printf("��� %d ����Ů������������ %d ���õ� ", iPlayerWithNun[0], iPlayer);
		DisplayCertainCard(cardAbandoned.back());
		printf(" ��\n");
		player[iPlayerWithNun[0]].cardInHand.push_back(cardAbandoned.back());
		player[iPlayerWithNun[0]].nInHand++;
		cardAbandoned.pop_back();
		g_nAbandoned--;
		for (int i = 0; i < player[iPlayerWithNun[0]].nInHand; i++) {
			if (player[iPlayerWithNun[0]].cardInHand[i].strName == "��Ů") {
				AbandonCertainCard(iPlayerWithNun[0], i, IN_HAND);
				break;
			}
		}
		player[iPlayerWithNun[0]].cardInHand[player[iPlayerWithNun[0]].nInHand - 1].nStatus = IN_HAND;
	} else if (nPlayerWithNun > 1) {
		printf("�� %d �������������Ů�������������\n", nPlayerWithNun);
		short x = rand() % nPlayerWithNun;
		printf("��� %d �ɹ�������Ů�������� %d ���õ� ", iPlayerWithNun[x], iPlayer);
		DisplayCertainCard(cardAbandoned.back());
		printf(" ��\n");
		player[iPlayerWithNun[x]].cardInHand.push_back(cardAbandoned.back());
		player[iPlayerWithNun[x]].nInHand++;
		cardAbandoned.pop_back();
		g_nAbandoned--;
		for (int i = 0; i < player[iPlayerWithNun[x]].nInHand; i++) {
			if (player[iPlayerWithNun[x]].cardInHand[i].strName == "��Ů") {
				AbandonCertainCard(iPlayerWithNun[x], i, IN_HAND);
				break;
			}
		}
		player[iPlayerWithNun[x]].cardInHand[player[iPlayerWithNun[x]].nInHand - 1].nStatus = IN_HAND;
	}
	system("pause");
	return true;
}

bool CheckTransverse(Card c) {
	if (c.strName == "����" || c.strName == "����" || c.strName == "����") return true;
	return false;
}

bool CheckCardCanBeUsed() {
	for (int i = 0; i < player[1].nInHand; i++) {
		if (player[1].cardInHand[i].nType != TRIGGER) return true;
	}
	return false;
}

bool CheckCardCanTransverse() {
	for (int i = 0; i < player[1].nOnField; i++) {
		if (CheckTransverse(player[1].cardOnField[i])) return true;
	}
	return false;
}

void PutOnField(int iPlayer, int iCard) {
	player[iPlayer].cardInHand[iCard].bTrans = false;
	player[iPlayer].cardInHand[iCard].nStatus = ON_FIELD;
	player[iPlayer].cardOnField.push_back(player[iPlayer].cardInHand[iCard]);
	player[iPlayer].cardInHand.erase(player[iPlayer].cardInHand.begin() + iCard);
	player[iPlayer].nInHand--;
	player[iPlayer].nOnField++;
}

void PutOnField_Monk(int iPlayer, int iCard) {
	Card c = cardAbandoned[iCard];
	c.bTrans = false;
	c.nStatus = ON_FIELD;
	player[iPlayer].cardOnField.push_back(c);
	cardAbandoned.erase(cardAbandoned.begin() + iCard);
	g_nAbandoned--;
	player[iPlayer].nOnField++;
}

int CheckPurpleCards(int iPlayer) {
	for (int i = 1; i <= g_nNumPlayer; i++) {
		if (i == iPlayer) continue;
		if (CheckPurpleCards_CertainPlayer(i)) return i;
	}
	return 0;
}

bool CheckPurpleCards_CertainPlayer(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nInHand; i++) {
		if (player[iPlayer].cardInHand[i].nColor == PURPLE) return true;
	}
	return false;
}

bool CheckGuard(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nInHand; i++) {
		if (player[iPlayer].cardInHand[i].strName == "����") return true;
	}
	return false;
}

void AbandonCertainCard(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[iPlayer].cardInHand[iCard].nStatus = ABANDONED;
		cardAbandoned.push_back(player[iPlayer].cardInHand[iCard]);
		player[iPlayer].cardInHand.erase(player[iPlayer].cardInHand.begin() + iCard);
		player[iPlayer].nInHand--;
	} else if (iStatus == ON_FIELD) {
		player[iPlayer].cardOnField[iCard].bTrans = false;
		player[iPlayer].cardOnField[iCard].nStatus = ABANDONED;
		cardAbandoned.push_back(player[iPlayer].cardOnField[iCard]);
		player[iPlayer].cardOnField.erase(player[iPlayer].cardOnField.begin() + iCard);
		player[iPlayer].nOnField--;
	}
	g_nAbandoned++;
}

void GetOneCardFromLibrary(int iPlayer) {
	int x = rand() % (NUM_YELLOW_CARD + NUM_PURPLE_CARD) + 1;
	card[x].nStatus = IN_HAND;
	player[iPlayer].cardInHand.push_back(card[x]);
	player[iPlayer].nInHand++;
	card[x].nStatus = IN_LIBRARY;
}

bool CheckHolyKnight(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nInHand; i++) {
		if (player[iPlayer].cardInHand[i].strName == "ʥ��ʿ") return true;
	}
	return false;
}

bool CheckSaintSword(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "ʥ��") return true;
	}
	return false;
}

bool CheckBlade(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "ħ��") return true;
	}
	return false;
}

bool CheckMinister(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "��") {
			if (iPlayer == 1) {
				printf("�㳡���д󳼣��Ƿ���ã�(y/n)\n");
				char ch = _getch();
				while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N') ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					GetOneCardFromLibrary(1);
					DisplayCards();
					printf("������˳��ϵĴ󳼣�����һ���ơ�\n");
					system("pause");
					AbandonBomb();
				} else return false;
			} else {
				GetOneCardFromLibrary(iPlayer);
				DisplayCards();
				printf("��� %d �����˳��ϵĴ󳼣�����һ���ơ�\n", iPlayer);
				system("pause");
				AbandonBombAI(iPlayer);
				return true;
			}
		}
	}
	return false;
}

bool CheckMimic(int iPlayer, int nType) {
	short nPlayerWithMimic = 0;
	vector<short> iPlayerWithMimic;
	for (int i = 1; i <= g_nNumPlayer; i++) {
		if (i == iPlayer) continue;
		for (int j = 0; j < player[i].nInHand; j++) {
			if (player[i].cardInHand[j].strName == "�����") {
				nPlayerWithMimic++;
				iPlayerWithMimic.push_back(i);
				break;
			}
		}
	}
	if (nPlayerWithMimic == 0) return false;
	if (nPlayerWithMimic == 1) {
		for (int i = 0; i < player[iPlayerWithMimic[0]].nInHand; i++) {
			if (player[iPlayerWithMimic[0]].cardInHand[i].strName == "�����") {
				AbandonCertainCard(iPlayerWithMimic[0], i, IN_HAND);
				break;
			}
		}
		if (nType == GET_COIN) {
			GetOneCardFromLibrary(iPlayerWithMimic[0]);
			if (iPlayerWithMimic[0] == 1) AbandonBomb();
			else AbandonBombAI(iPlayerWithMimic[0]);
			GetOneCardFromLibrary(iPlayerWithMimic[0]);
			if (iPlayerWithMimic[0] == 1) AbandonBomb();
			else AbandonBombAI(iPlayerWithMimic[0]);
			DisplayCards();
			printf("��� %d �� ", iPlayerWithMimic[0]);
			DisplayCertainCard(card[54]);
			printf("�������������ơ�\n");
			system("pause");
		} else if (nType == GET_TWO_CARDS) {
			player[iPlayerWithMimic[0]].nNumCoin++;
			DisplayCards();
			printf("��� %d �� ", iPlayerWithMimic[0]);
			DisplayCertainCard(card[54]);
			printf("������ +1 ��ҡ�\n");
			system("pause");
			if (player[iPlayerWithMimic[0]].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = iPlayerWithMimic[0];
				g_nGameStatus = 0;
				return true;
			}
			CheckDancer(iPlayerWithMimic[0]);
			if (!g_nGameStatus) return true;
			CheckMimic(iPlayerWithMimic[0], GET_COIN);
		}
	} else if (nPlayerWithMimic > 1) {
		printf("�� %d ����������� ", nPlayerWithMimic);
		DisplayCertainCard(card[54]);
		printf("�������������\n");
		short x = rand() % nPlayerWithMimic;
		for (int i = 0; i < player[iPlayerWithMimic[x]].nInHand; i++) {
			if (player[iPlayerWithMimic[x]].cardInHand[i].strName == "�����") {
				AbandonCertainCard(iPlayerWithMimic[x], i, IN_HAND);
				break;
			}
		}
		if (nType == GET_COIN) {
			GetOneCardFromLibrary(iPlayerWithMimic[x]);
			if (iPlayerWithMimic[0] == 1) AbandonBomb();
			else AbandonBombAI(iPlayerWithMimic[0]);
			GetOneCardFromLibrary(iPlayerWithMimic[x]);
			if (iPlayerWithMimic[0] == 1) AbandonBomb();
			else AbandonBombAI(iPlayerWithMimic[0]);
			DisplayCards();
			printf("��� %d �ɹ����� ", iPlayerWithMimic[x]);
			DisplayCertainCard(card[54]);
			printf("���������ơ�\n");
			system("pause");
		} else if (nType == GET_TWO_CARDS) {
			player[iPlayerWithMimic[x]].nNumCoin++;
			DisplayCards();
			printf("��� %d �ɹ����� ", iPlayerWithMimic[x]);
			DisplayCertainCard(card[54]);
			printf("�� +1 ��ҡ�\n");
			system("pause");
			if (player[iPlayerWithMimic[x]].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = iPlayerWithMimic[x];
				g_nGameStatus = 0;
				return true;
			}
			CheckDancer(iPlayerWithMimic[x]);
			if (!g_nGameStatus) return true;
			CheckMimic(iPlayerWithMimic[x], GET_COIN);
		}
	}
	return true;
}

void UseGuard(int iAttacker, int iTarget, int iCard) {
	if (!g_bMonkMode) PutOnField(iAttacker, iCard);
	else PutOnField_Monk(iAttacker, iCard);
	for (int i = 0; i < player[iTarget].nInHand; i++) {
		if (player[iTarget].cardInHand[i].strName == "����") {
			if (iTarget == 1) player[iTarget].cardInHand[i].pfnOpt(i, IN_HAND);
			else player[iTarget].cardInHand[i].pfnOptAI(iTarget, i, IN_HAND);
			break;
		}
	}
	DisplayCards();
	if (iTarget == 1) printf("��");
	else printf("��� %d ", iTarget);
	printf("��������ס��");
	if (iAttacker == 1) printf("��");
	else printf("��� %d ", iAttacker);
	printf("�Ĺ�����\n");
	system("pause");
}

void AddToHand(int iPlayer, int iCard) {
	cardAbandoned[iCard].nStatus = IN_HAND;
	player[iPlayer].cardInHand.push_back(cardAbandoned[iCard]);
	player[iPlayer].nInHand++;
	cardAbandoned.erase(cardAbandoned.begin() + iCard);
	g_nAbandoned--;
}

bool CheckDancer(int iPlayer) {
	for (int i = 1; i <= g_nNumPlayer; i++) {
		if (i == iPlayer) continue;
		for (int j = 0; j < player[i].nOnField; j++) {
			if (player[i].cardOnField[j].strName == "����") {
				if (i == 1) {
					printf("�㳡���� ");
					DisplayCertainCard(player[i].cardOnField[j]);
					printf("���Ƿ���һ�Ż��һ����ң�(y/n)\n");
					char ch = _getch();
					while (ch != 'y' && ch != 'n' && ch != 'Y' && ch != 'N') ch = _getch();
					if (ch == 'y' || ch == 'Y') {
						AbandonCertainCard(1, j, ON_FIELD);
						player[1].nNumCoin++;
						DisplayCards();
						if (player[1].nNumCoin == g_nStdCoinNumber) {
							g_iWinPlayer = 1;
							g_nGameStatus = 0;
							return true;
						}
						CheckMimic(1, GET_COIN);
						CheckNun(1);
					}
				} else {
					AbandonCertainCard(i, j, ON_FIELD);
					player[i].nNumCoin++;
					DisplayCards();
					printf("��� %d �����˳��ϵ� ", i);
					DisplayCertainCard(cardAbandoned.back());
					printf("�������һ����ң�\n");
					system("pause");
					if (player[i].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = i;
						g_nGameStatus = 0;
						return true;
					}
					CheckMimic(i, GET_COIN);
					CheckNun(i);
				}
			}
		}
	}
	return true;
}

bool Defend(int iAttacker, int iTarget, int iCard, int iStatus) {
	if (iTarget == 1) {
		bool bGuard = CheckGuard(1), bHolyKnight = CheckHolyKnight(1);
		if (bGuard && bHolyKnight) {
			printf("������ͬʱ��������ʥ��ʿ����ѡ�񴥷���\n");
			printf("1.���� 2.ʥ��ʿ\n");
			char ch = _getch();
			while (ch != '1' && ch != '2') {
				ch = _getch();
			}
			if (ch == '1') {
				UseGuard(iAttacker, iTarget, iCard);
				return true;
			} else if (ch == '2') {
				for (int i = 0; i < player[1].nInHand; i++) {
					if (player[1].cardInHand[i].strName == "ʥ��ʿ") {
						bool res = player[1].cardInHand[i].pfnOpt(i, IN_HAND);
						if (res) break;
						else return true;
					}
				}
			}
		} else if (bGuard) {
			UseGuard(iAttacker, iTarget, iCard);
			return true;
		} else if (bHolyKnight) {
			for (int i = 0; i < player[1].nInHand; i++) {
				if (player[1].cardInHand[i].strName == "ʥ��ʿ") {
					bool res = player[1].cardInHand[i].pfnOpt(i, IN_HAND);
					if (res) break;
					else return true;
				}
			}
		}
	} else {
		/* ���������ʥ��ʿ */
		if (CheckGuard(iTarget)) {
			UseGuard(iAttacker, iTarget, iCard);
			return true;
		} else if (CheckHolyKnight(iTarget)) {
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].strName == "ʥ��ʿ") {
					bool res = player[iTarget].cardInHand[i].pfnOptAI(iTarget, i, IN_HAND);
					if (res == true) {
						/* ������Ч */
						return false;
					} else {
						/* ������Ч */
						return true;
					}
				}
			}
		}
	}
	return false;
}

void AbandonBomb() {
	if (player[1].nInHand > 4) {
		bool bBomb = false;
		for (int i = 0; i < player[1].nOnField; i++) {
			if (player[1].cardOnField[i].strName == "ը����") {
				bBomb = true;
				break;
			}
		}
		if (bBomb) {
			printf("����������Ƴ��� 4 �ţ��Ƿ����㳡�����е� ");
			DisplayCertainCard(card[69]);
			printf("��(y/n)\n");
			char ch = _getch();
			while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N') ch = _getch();
			if (ch == 'y' || ch == 'Y') {
				for (int i = 0; i < player[1].nOnField;) {
					if (player[1].cardOnField[i].strName == "ը����") {
						AbandonCertainCard(1, i, ON_FIELD);
					} else i++;
				}
				DisplayCards();
			}
		}
	}
}

void AbandonBombAI(int iPlayer) {
	bool res = false;
	if (player[iPlayer].nInHand > 4) {
		for (int i = 0; i < player[iPlayer].nOnField;) {
			if (player[iPlayer].cardOnField[i].strName == "ը����") {
				AbandonCertainCard(iPlayer, i, ON_FIELD);
				res = true;
			} else i++;
		}
	}
	if (!res) return;
	DisplayCards();
	printf("��ʱ��� %d �����Ƴ��� 4 �ţ������˳������е� ", iPlayer);
	DisplayCertainCard(card[69]);
	printf("��\n");
	system("pause");
}

void PutOnField_Dragon(int iPlayer, int iCard) {
	Card c = cardAbandoned[iCard];
	c.bTrans = false;
	c.nStatus = ON_FIELD;
	player[iPlayer].cardOnField.push_back(c);
	cardAbandoned.erase(cardAbandoned.begin() + iCard);
	g_nAbandoned--;
	player[iPlayer].nOnField++;
}

bool CheckTyrant(int iPlayer) {
	bool found = false;
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "ħ��") {
			found = true;
			break;
		}
	}
	if (!found) return false;
	if (iPlayer == 1) {
		printf("�㳡���� ");
		DisplayCertainCard(card[53]);
		printf("���Ƿ���ã�(y/n)\n");
		char ch = _getch();
		while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N') ch = _getch();
		if (ch == 'y' || ch == 'Y') {
			player[1].nNumCoin++;
			DisplayCards();
			printf("������� ");
			DisplayCertainCard(card[53]);
			printf("��+1 ��ң�\n");
			system("pause");
			if (player[1].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = 1;
				g_nGameStatus = 0;
				return true;
			}
			CheckDancer(1);
			if (!g_nGameStatus) return true;
			CheckMimic(1, GET_COIN);
		}
	} else {
		player[iPlayer].nNumCoin++;
		DisplayCards();
		printf("��� %d ������ ", iPlayer);
		DisplayCertainCard(card[53]);
		printf("��+1 ��ң�\n");
		if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
			g_iWinPlayer = iPlayer;
			g_nGameStatus = 0;
			return true;
		}
		CheckDancer(iPlayer);
		if (!g_nGameStatus) return true;
		CheckMimic(iPlayer, GET_COIN);
	}
	return true;
}

void IllustrateCard(Card c) {
	if (c.nColor == YELLOW) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
	else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0d);
	printf("*** ");
	DisplayCertainCard(c);
	if (c.nColor == YELLOW) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
	else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0d);
	cout << "��" << c.strDescription << "***" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
}