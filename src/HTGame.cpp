/* HTGame.cpp
   2024/8/1
   Written by Peter Winchester
*/

#include "HTGame.h"

void ReadyStageAI(int iCurrentPlayer) {
	/* 准备阶段前。*/
	if (player[iCurrentPlayer].nOnField > 1) {
		/* 检查炸弹怪。*/
		int nBomb = 0;
		for (int i = 0; i < player[iCurrentPlayer].nOnField; i++) {
			if (player[iCurrentPlayer].cardOnField[i].strName == "炸弹怪") nBomb++;
		}
		if (nBomb) {
			int nReturn = 0;
			for (int i = 0; i < player[iCurrentPlayer].nOnField;) {
				if (player[iCurrentPlayer].cardOnField[i].strName != "炸弹怪") {
					ReturnCardOnField(iCurrentPlayer, i);
					nReturn++;
					if (nReturn == nBomb) break;
				} else i++;
			}
			if (nReturn) {
				printf("玩家 %d 场上有 %d 张 ", iCurrentPlayer, nBomb);
				DisplayCertainCard(card[69]);
				printf("，使其场上的 %d 张牌回到手牌。\n", nReturn);
				system("pause");
				DisplayCards();
			}
		}
	}
	AbandonBombAI(iCurrentPlayer);
	/* 准备阶段。*/
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
						if (player[iCurrentPlayer].cardInHand[j].strName == "送子鹤") {
							AbandonCertainCard(iCurrentPlayer, j, IN_HAND);
							break;
						}
					}
					DisplayCards();
					printf("在准备阶段，玩家 %d 有两张及以上的卡牌回到手牌，触发 ", iCurrentPlayer);
					DisplayCertainCard(card[24]);
					printf("，获得 1 金币。\n");
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
	/* 检查圣杯。*/
	if (CheckGrail(iCurrentPlayer)) {
		printf("玩家 %d 场上有 ", iCurrentPlayer);
		DisplayCertainCard(card[1]);
		printf("，此时可直接获胜。\n");
		g_iWinPlayer = iCurrentPlayer;
		g_nGameStatus = 0;
		return;
	}
	/* 检查公主。*/
	if (CheckPrincess(iCurrentPlayer)) {
		player[iCurrentPlayer].nNumCoin++;
		DisplayCards();
		printf("玩家 %d 场上有 ", iCurrentPlayer);
		DisplayCertainCard(card[3]);
		printf("，+1 金币。\n");
		system("pause");
		/* 再检查圣杯。*/
		if (CheckGrail(iCurrentPlayer)) {
			printf("玩家 %d 场上有 ", iCurrentPlayer);
			DisplayCertainCard(card[1]);
			printf("，此时可直接获胜。\n");
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
	/* 出牌 */
	for (int i = 0; i < player[iPlayer].nInHand; i++) {
		if (player[iPlayer].cardInHand[i].nType != TRIGGER) {
			player[iPlayer].cardInHand[i].pfnOptAI(iPlayer, i, IN_HAND);
			if (!g_nGameStatus) return;
		}
	}
	if (player[iPlayer].nActionPoint) {
		/* 横置 */
		/* 可独立横置：国王、妖精、火龙。*/
		for (int i = 0; i < player[iPlayer].nOnField; i++) {
			if (CheckTransverse(player[iPlayer].cardOnField[i])) player[iPlayer].cardOnField[i].pfnOptAI(iPlayer, i, ON_FIELD);
			if (!g_nGameStatus) return;
		}
	}
	/* 摸牌 */
	if (player[iPlayer].nActionPoint > 0) {
		bool bMimic = (player[iPlayer].nActionPoint == 2);
		printf("玩家 %d 正在摸牌......\n", iPlayer);
		while (player[iPlayer].nActionPoint > 0) {
			player[iPlayer].nActionPoint--;
			GetOneCardFromLibrary(iPlayer);
			AbandonBombAI(iPlayer);
		}
		printf("玩家 %d 摸牌结束。\n", iPlayer);
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
		printf("游戏结束，玩家 %d 获胜！\n", g_iWinPlayer);
		system("pause");
		return 1;
	}
	return 0;
}

void ReadyStage() {
	/* 准备阶段前。*/
	if (player[1].nOnField > 1) {
		/* 检查炸弹怪。*/
		int nBomb = 0;
		for (int i = 0; i < player[1].nOnField; i++) {
			if (player[1].cardOnField[i].strName == "炸弹怪") nBomb++;
		}
		if (nBomb) {
			printf("你场上有 %d 张 ", nBomb);
			DisplayCertainCard(card[69]);
			printf("，须选择你场上至多 %d 张其它牌回到手牌。\n", nBomb);
			int nReturn = 0;
			while (nReturn < nBomb && player[1].nOnField > nBomb) {
				int n = 0;
				vector<int> iRe;
				printf("请选择一张回到手牌：\n");
				for (int i = 0; i < player[1].nOnField; i++) {
					if (player[1].cardOnField[i].strName != "炸弹怪") {
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
	/* 准备阶段。*/
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
						if (player[1].cardInHand[j].strName == "送子鹤") {
							AbandonCertainCard(1, j, IN_HAND);
							break;
						}
					}
					DisplayCards();
					printf("你有两张及以上的卡牌回到手牌，触发 ");
					DisplayCertainCard(card[24]);
					printf("，获得 1 金币！\n");
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
	/* 检查圣杯。*/
	if (CheckGrail(1)) {
		printf("你场上有 ");
		DisplayCertainCard(card[1]);
		printf("，此时可直接获胜。\n");
		g_iWinPlayer = 1;
		g_nGameStatus = 0;
		return;
	}
	/* 检查公主。*/
	if (CheckPrincess(1)) {
		player[1].nNumCoin++;
		DisplayCards();
		printf("你场上有 ");
		DisplayCertainCard(card[3]);
		printf("，+1 金币。\n");
		system("pause");
		/* 检查圣杯。*/
		if (CheckGrail(1)) {
			printf("你场上有 ");
			DisplayCertainCard(card[1]);
			printf("，此时可直接获胜。\n");
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
		printf("你还有 %d 个行动点。\n", player[1].nActionPoint);
		printf("请选择你要进行的操作：1.摸牌 2.出牌 3.横置 4.查看弃牌区 5.卡牌说明 6.结束回合\n");
		while (1) {
			char ch = _getch();
			if (ch == '1') { //摸牌
				if (!player[1].nActionPoint) {
					printf("你没有行动点了！\n");
					system("pause");
					break;
				}
				nGetCard++;
				GetOneCardFromLibrary(1);
				DisplayCards();
				player[1].nActionPoint--;
				printf("你摸到了一张 ");
				DisplayCertainCard(player[1].cardInHand.back());
				printf("。\n");
				system("pause");
				AbandonBomb();
				break;
			} else if (ch == '2') { //出牌
				if (!CheckCardCanBeUsed()) {
					printf("你没有可以打出的手牌！\n");
					system("pause");
					break;
				}
				printf("（输入其它数字取消）\n");
				printf("请选择你要打出的手牌：\n");
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
					if (!player[1].nActionPoint && player[1].cardInHand[iCardToBeUsed[iCh - 1]].strName != "公主") {
						printf("你没有行动点了，不能打出该牌！\n");
						system("pause");
						break;
					}
					player[1].cardInHand[iCardToBeUsed[iCh - 1]].pfnOpt(iCardToBeUsed[iCh - 1], IN_HAND);
				}
				break;
			} else if (ch == '3') { //横置
				if (!CheckCardCanTransverse()) {
					printf("你没有可以横置的场牌！\n");
					system("pause");
				} else {
					printf("（输入其它数字取消）\n");
					printf("请选择你要横置的场牌：\n");
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
							printf("该牌已被横置过一次！\n");
							system("pause");
						} else {
							player[1].cardOnField[iCardToTransverse[iCh - 1]].pfnOpt(iCardToTransverse[iCh - 1], ON_FIELD);
							player[1].cardOnField[iCardToTransverse[iCh - 1]].bTrans = true;
						}
					}
				}
				break;
			} else if (ch == '4') { //查看弃牌区
				if (!g_nAbandoned) {
					printf("弃牌区没有牌。\n");
					system("pause");
				} else {
					printf("以下是弃牌区的卡牌：\n");
					for (int i = 0; i < g_nAbandoned; i++) {
						printf("%d.", i + 1);
						DisplayCertainCard(cardAbandoned[i]);
						printf("\n");
						if (i % 5 == 4) system("pause");
					}
					system("pause");
				}
				break;
			} else if (ch == '5') { //查看卡牌说明
				DescribeCards();
				break;
			} else if (ch == '6') {
				if (player[1].nActionPoint) {
					printf("你还有行动点，不能结束回合。\n");
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
		if (player[iPlayer].cardInHand[i].strName == "送子鹤") return true;
	}
	return false;
}

bool CheckGrail(int iPlayer) {
	if (player[iPlayer].nNumCoin < g_nStdCoinNumber - 1) return false;
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "圣杯") return true;
	}
	return false;
}

bool CheckPrincess(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "公主") return true;
	}
	return false;
}

void AbandonCardAI(int iPlayer) {
	if (player[iPlayer].nInHand <= 4) return;
	printf("玩家 %d 正在弃牌......\n", iPlayer);
	while (player[iPlayer].nInHand > 4) {
		int x = rand() % player[iPlayer].nInHand;
		Card c = player[iPlayer].cardInHand[x];
		AbandonCertainCard(iPlayer, x, IN_HAND);
		printf("玩家 %d 丢弃一张 ", iPlayer);
		DisplayCertainCard(c);
		printf("。\n");
		CheckNun(iPlayer);
	}
	printf("玩家 %d 弃牌完毕。\n", iPlayer);
	system("pause");
	DisplayCards();
}

void AbandonCard() {
	if (player[1].nInHand <= 4) return;
	while (player[1].nInHand > 4) {
		printf("你的手牌超过 4 张，需丢弃 %d 张。\n", player[1].nInHand - 4);
		printf("请选择你要丢弃的手牌：\n");
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
			} else printf("输入的数字超出了范围。\n");
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
			if (player[i].cardInHand[j].strName == "修女") {
				nPlayerWithNun++;
				iPlayerWithNun.push_back(i);
				break;
			}
		}
	}
	if (nPlayerWithNun == 0) return false;
	if (nPlayerWithNun == 1) {
		printf("玩家 %d 有修女，触发获得玩家 %d 弃置的 ", iPlayerWithNun[0], iPlayer);
		DisplayCertainCard(cardAbandoned.back());
		printf(" 。\n");
		player[iPlayerWithNun[0]].cardInHand.push_back(cardAbandoned.back());
		player[iPlayerWithNun[0]].nInHand++;
		cardAbandoned.pop_back();
		g_nAbandoned--;
		for (int i = 0; i < player[iPlayerWithNun[0]].nInHand; i++) {
			if (player[iPlayerWithNun[0]].cardInHand[i].strName == "修女") {
				AbandonCertainCard(iPlayerWithNun[0], i, IN_HAND);
				break;
			}
		}
		player[iPlayerWithNun[0]].cardInHand[player[iPlayerWithNun[0]].nInHand - 1].nStatus = IN_HAND;
	} else if (nPlayerWithNun > 1) {
		printf("共 %d 名玩家手牌有修女，现随机触发。\n", nPlayerWithNun);
		short x = rand() % nPlayerWithNun;
		printf("玩家 %d 成功触发修女，获得玩家 %d 弃置的 ", iPlayerWithNun[x], iPlayer);
		DisplayCertainCard(cardAbandoned.back());
		printf(" 。\n");
		player[iPlayerWithNun[x]].cardInHand.push_back(cardAbandoned.back());
		player[iPlayerWithNun[x]].nInHand++;
		cardAbandoned.pop_back();
		g_nAbandoned--;
		for (int i = 0; i < player[iPlayerWithNun[x]].nInHand; i++) {
			if (player[iPlayerWithNun[x]].cardInHand[i].strName == "修女") {
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
	if (c.strName == "国王" || c.strName == "妖精" || c.strName == "火龙") return true;
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
		if (player[iPlayer].cardInHand[i].strName == "卫兵") return true;
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
		if (player[iPlayer].cardInHand[i].strName == "圣骑士") return true;
	}
	return false;
}

bool CheckSaintSword(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "圣剑") return true;
	}
	return false;
}

bool CheckBlade(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "魔剑") return true;
	}
	return false;
}

bool CheckMinister(int iPlayer) {
	for (int i = 0; i < player[iPlayer].nOnField; i++) {
		if (player[iPlayer].cardOnField[i].strName == "大臣") {
			if (iPlayer == 1) {
				printf("你场上有大臣，是否横置？(y/n)\n");
				char ch = _getch();
				while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N') ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					GetOneCardFromLibrary(1);
					DisplayCards();
					printf("你横置了场上的大臣，抽了一张牌。\n");
					system("pause");
					AbandonBomb();
				} else return false;
			} else {
				GetOneCardFromLibrary(iPlayer);
				DisplayCards();
				printf("玩家 %d 横置了场上的大臣，抽了一张牌。\n", iPlayer);
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
			if (player[i].cardInHand[j].strName == "宝箱怪") {
				nPlayerWithMimic++;
				iPlayerWithMimic.push_back(i);
				break;
			}
		}
	}
	if (nPlayerWithMimic == 0) return false;
	if (nPlayerWithMimic == 1) {
		for (int i = 0; i < player[iPlayerWithMimic[0]].nInHand; i++) {
			if (player[iPlayerWithMimic[0]].cardInHand[i].strName == "宝箱怪") {
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
			printf("玩家 %d 有 ", iPlayerWithMimic[0]);
			DisplayCertainCard(card[54]);
			printf("，触发抽两张牌。\n");
			system("pause");
		} else if (nType == GET_TWO_CARDS) {
			player[iPlayerWithMimic[0]].nNumCoin++;
			DisplayCards();
			printf("玩家 %d 有 ", iPlayerWithMimic[0]);
			DisplayCertainCard(card[54]);
			printf("，触发 +1 金币。\n");
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
		printf("共 %d 名玩家手牌有 ", nPlayerWithMimic);
		DisplayCertainCard(card[54]);
		printf("，现随机触发。\n");
		short x = rand() % nPlayerWithMimic;
		for (int i = 0; i < player[iPlayerWithMimic[x]].nInHand; i++) {
			if (player[iPlayerWithMimic[x]].cardInHand[i].strName == "宝箱怪") {
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
			printf("玩家 %d 成功触发 ", iPlayerWithMimic[x]);
			DisplayCertainCard(card[54]);
			printf("，抽两张牌。\n");
			system("pause");
		} else if (nType == GET_TWO_CARDS) {
			player[iPlayerWithMimic[x]].nNumCoin++;
			DisplayCards();
			printf("玩家 %d 成功触发 ", iPlayerWithMimic[x]);
			DisplayCertainCard(card[54]);
			printf("， +1 金币。\n");
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
		if (player[iTarget].cardInHand[i].strName == "卫兵") {
			if (iTarget == 1) player[iTarget].cardInHand[i].pfnOpt(i, IN_HAND);
			else player[iTarget].cardInHand[i].pfnOptAI(iTarget, i, IN_HAND);
			break;
		}
	}
	DisplayCards();
	if (iTarget == 1) printf("你");
	else printf("玩家 %d ", iTarget);
	printf("的卫兵防住了");
	if (iAttacker == 1) printf("你");
	else printf("玩家 %d ", iAttacker);
	printf("的攻击。\n");
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
			if (player[i].cardOnField[j].strName == "舞娘") {
				if (i == 1) {
					printf("你场上有 ");
					DisplayCertainCard(player[i].cardOnField[j]);
					printf("，是否丢弃一张获得一个金币？(y/n)\n");
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
					printf("玩家 %d 丢弃了场上的 ", i);
					DisplayCertainCard(cardAbandoned.back());
					printf("，获得了一个金币！\n");
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
			printf("你手中同时有卫兵和圣骑士，请选择触发：\n");
			printf("1.卫兵 2.圣骑士\n");
			char ch = _getch();
			while (ch != '1' && ch != '2') {
				ch = _getch();
			}
			if (ch == '1') {
				UseGuard(iAttacker, iTarget, iCard);
				return true;
			} else if (ch == '2') {
				for (int i = 0; i < player[1].nInHand; i++) {
					if (player[1].cardInHand[i].strName == "圣骑士") {
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
				if (player[1].cardInHand[i].strName == "圣骑士") {
					bool res = player[1].cardInHand[i].pfnOpt(i, IN_HAND);
					if (res) break;
					else return true;
				}
			}
		}
	} else {
		/* 检查卫兵、圣骑士 */
		if (CheckGuard(iTarget)) {
			UseGuard(iAttacker, iTarget, iCard);
			return true;
		} else if (CheckHolyKnight(iTarget)) {
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].strName == "圣骑士") {
					bool res = player[iTarget].cardInHand[i].pfnOptAI(iTarget, i, IN_HAND);
					if (res == true) {
						/* 攻击有效 */
						return false;
					} else {
						/* 攻击无效 */
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
			if (player[1].cardOnField[i].strName == "炸弹怪") {
				bBomb = true;
				break;
			}
		}
		if (bBomb) {
			printf("现在你的手牌超过 4 张，是否丢弃你场上所有的 ");
			DisplayCertainCard(card[69]);
			printf("？(y/n)\n");
			char ch = _getch();
			while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N') ch = _getch();
			if (ch == 'y' || ch == 'Y') {
				for (int i = 0; i < player[1].nOnField;) {
					if (player[1].cardOnField[i].strName == "炸弹怪") {
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
			if (player[iPlayer].cardOnField[i].strName == "炸弹怪") {
				AbandonCertainCard(iPlayer, i, ON_FIELD);
				res = true;
			} else i++;
		}
	}
	if (!res) return;
	DisplayCards();
	printf("此时玩家 %d 的手牌超过 4 张，丢弃了场上所有的 ", iPlayer);
	DisplayCertainCard(card[69]);
	printf("。\n");
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
		if (player[iPlayer].cardOnField[i].strName == "魔王") {
			found = true;
			break;
		}
	}
	if (!found) return false;
	if (iPlayer == 1) {
		printf("你场上有 ");
		DisplayCertainCard(card[53]);
		printf("，是否横置？(y/n)\n");
		char ch = _getch();
		while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N') ch = _getch();
		if (ch == 'y' || ch == 'Y') {
			player[1].nNumCoin++;
			DisplayCards();
			printf("你横置了 ");
			DisplayCertainCard(card[53]);
			printf("，+1 金币！\n");
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
		printf("玩家 %d 横置了 ", iPlayer);
		DisplayCertainCard(card[53]);
		printf("，+1 金币！\n");
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
	cout << "：" << c.strDescription << "***" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
}