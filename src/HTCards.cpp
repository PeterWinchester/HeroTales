/* HTCards.cpp
   2024/08/04
   Written by Peter Winchester.
*/

#include "HTCards.h"

bool Grail(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if (g_bMonkMode) PutOnField_Monk(1, iCard);
		else PutOnField(1, iCard);
		DisplayCards();
		printf("你将 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		player[1].nActionPoint--;
		return true;
	}
	printf("错误：该圣杯状态不是在手中！\n");
	system("pause");
	return false;
}

bool Minister(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if (g_bMonkMode) PutOnField_Monk(1, iCard);
		else PutOnField(1, iCard);
		DisplayCards();
		printf("你将 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		player[1].nActionPoint--;
		return true;
	}
	return false;
}

bool Princess(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if (g_bMonkMode) PutOnField_Monk(1, iCard);
		else PutOnField(1, iCard);
		DisplayCards();
		printf("你将 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		return true;
	}
	return false;
}

bool Warrior_Royal(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		printf("请选择你的攻击对象（输入玩家编号）：");
		int iTarget = InputIntSafely();
		if (iTarget == 1) {
			printf("你不能攻击自己！\n");
			system("pause");
			return false;
		} else if (iTarget >= 2 && iTarget <= g_nNumPlayer) {
			g_iAttacker = 1;
			player[1].nActionPoint--;
			/* 检查卫兵、圣骑士 */
			if (CheckGuard(iTarget)) {
				UseGuard(1, iTarget, iCard);
				return true;
			} else if (CheckHolyKnight(iTarget)) {
				for (int i = 0; i < player[iTarget].nInHand; i++) {
					if (player[iTarget].cardInHand[i].strName == "圣骑士") {
						bool res = player[iTarget].cardInHand[i].pfnOptAI(iTarget, i, IN_HAND);
						if (res == true) {
							/* 攻击有效 */
							break;
						} else {
							/* 攻击无效 */
							return true;
						}
					}
				}
			}
			/* 检查魔剑、圣剑 */
			bool bSaintSword = false, bBlade = false;
			if (CheckSaintSword(1)) {
				printf("你场上有 ");
				DisplayCertainCard(card[6]);
				printf("，是否横置？(y/n)\n");
				while (1) {
					char ch = _getch();
					if (ch == 'y' || ch == 'Y') {
						bSaintSword = true;
						break;
					} else if (ch == 'n' || ch == 'N') break;
				}
			}
			if (!bSaintSword && CheckBlade(1)) {
				printf("你场上有 ");
				DisplayCertainCard(card[55]);
				printf("，是否横置？(y/n)\n");
				while (1) {
					char ch = _getch();
					if (ch == 'y' || ch == 'Y') {
						bBlade = true;
						break;
					} else if (ch == 'n' || ch == 'N') break;
				}
			}
			if (bSaintSword) {
				/* 使用圣剑 */
				if (player[iTarget].nOnField == 0) {
					if (!g_bMonkMode) {
						AbandonCertainCard(1, iCard, IN_HAND);
						DisplayCards();
						CheckNun(1);
					}
					printf("攻击失败，对手场上没牌！\n");
					system("pause");
					CheckMinister(iTarget);
					return true;
				}
				printf("请选择你要丢弃的对手场上的牌：\n");
				for (int i = 0; i < player[iTarget].nOnField; i++) {
					printf("%d.", i + 1);
					DisplayCertainCard(player[iTarget].cardOnField[i]);
				}
				printf("\n");
				int ch = InputIntSafely();
				while (ch <= 0 || ch > player[iTarget].nOnField) {
					printf("输入的数字超出了范围！\n");
					ch = InputIntSafely();
				}
				printf("由于你的勇者没有 Lv，所以无法获得金币。\n");
				system("pause");
				AbandonCertainCard(iTarget, ch - 1, ON_FIELD);
				DisplayCards();
				CheckNun(iTarget);
				if (!g_bMonkMode) {
					AbandonCertainCard(1, iCard, IN_HAND);
					DisplayCards();
					CheckNun(1);
				}
				system("pause");
				CheckMinister(iTarget);
				return true;
			} else if (bBlade) {
				/* 使用魔剑 */
				if (g_bMonkMode) PutOnField_Monk(1, iCard);
				else PutOnField(1, iCard);
				DisplayCards();
				if (player[iTarget].nInHand == 0) {
					printf("该玩家没有手牌，攻击失败！\n");
					system("pause");
					CheckMinister(iTarget);
					return true;
				}
				int nYellow = 0, nPurple = 0;
				vector<int> iYellow, iPurple;
				for (int i = 0; i < player[iTarget].nInHand; i++) {
					if (player[iTarget].cardInHand[i].nColor == YELLOW) {
						nYellow++;
						iYellow.push_back(i);
					} else {
						nPurple++;
						iPurple.push_back(i);
					}
				}
				if (nYellow && nPurple) {
					printf("该玩家有 %d 张黄牌和 %d 张紫牌，请选择：1.黄 2.紫", nYellow, nPurple);
					while (1) {
						char ch = _getch();
						if (ch == '1') {
							int x = iYellow[rand() % nYellow];
							Card c = player[iTarget].cardInHand[x];
							PutOnField(iTarget, x);
							DisplayCards();
							if (c.nLevel != NO_LEVEL || c.nArmory == DAEMON || c.nArmory == ROYAL_AND_DAEMON) {
								player[1].nNumCoin++;
								printf("攻击成功，你 +1 金币！\n");
								if (player[1].nNumCoin == g_nStdCoinNumber) {
									g_iWinPlayer = 1;
									g_nGameStatus = 0;
									return true;
								}
								CheckDancer(1);
								if (!g_nGameStatus) return true;
								CheckMimic(1, GET_COIN);
							} else printf("攻击失败！\n");
							break;
						} else if (ch == '2') {
							int x = iPurple[rand() % nPurple];
							Card c = player[iTarget].cardInHand[x];
							PutOnField(iTarget, x);
							DisplayCards();
							if (c.nLevel != NO_LEVEL || c.nArmory == DAEMON || c.nArmory == ROYAL_AND_DAEMON) {
								player[1].nNumCoin++;
								DisplayCards();
								printf("攻击成功，你 +1 金币！\n");
								if (player[1].nNumCoin == g_nStdCoinNumber) {
									g_iWinPlayer = 1;
									g_nGameStatus = 0;
									return true;
								}
								CheckDancer(1);
								if (!g_nGameStatus) return true;
								CheckMimic(1, GET_COIN);
							} else printf("攻击失败！\n");
							break;
						}
					}
					system("pause");
					CheckMinister(iTarget);
					return true;
				} else {
					int x = rand() % player[iTarget].nInHand;
					Card c = player[iTarget].cardInHand[x];
					PutOnField(iTarget, x);
					DisplayCards();
					if (c.nLevel != NO_LEVEL || c.nArmory == DAEMON || c.nArmory == ROYAL_AND_DAEMON) {
						player[1].nNumCoin++;
						DisplayCards();
						printf("攻击成功，你 +1 金币！\n");
						if (player[1].nNumCoin == g_nStdCoinNumber) {
							g_iWinPlayer = 1;
							g_nGameStatus = 0;
							return true;
						}
						CheckDancer(1);
						if (!g_nGameStatus) return true;
						CheckMimic(1, GET_COIN);
					} else printf("攻击失败！\n");
					system("pause");
					CheckMinister(iTarget);
					return true;
				}
			} else {
				/* 不使用 */
				if (!g_bMonkMode) {
					AbandonCertainCard(1, iCard, IN_HAND);
					DisplayCards();
					CheckNun(1);
				}
				int nPurple = 0;
				vector<int> iPurple;
				for (int i = 0; i < player[iTarget].nInHand; i++) {
					if (player[iTarget].cardInHand[i].nColor == PURPLE) {
						nPurple++;
						iPurple.push_back(i);
					}
				}
				if (!nPurple) {
					printf("玩家 %d 手上没有紫牌，攻击失败！\n", iTarget);
					system("pause");
					return true;
				}
				int x = iPurple[rand() % nPurple];
				Card c = player[iTarget].cardInHand[x];
				PutOnField(iTarget, x);
				DisplayCards();
				if (c.nLevel != NO_LEVEL || c.nArmory == DAEMON || c.nArmory == ROYAL_AND_DAEMON) {
					player[1].nNumCoin++;
					DisplayCards();
					printf("攻击成功，你 +1 金币！\n");
					system("pause");
					if (player[1].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = 1;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(1);
					if (!g_nGameStatus) return true;
					CheckMimic(1, GET_COIN);
				} else printf("攻击失败！\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			return false;
		}
		printf("输入的数字超出了范围！\n");
		system("pause");
		return false;
	}
	return false;
}

bool King(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[1].nActionPoint--;
		if (g_bMonkMode) PutOnField_Monk(1, iCard);
		else PutOnField(1, iCard);
		DisplayCards();
		printf("你将 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		return true;
	} else if (iStatus == ON_FIELD) {
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].strName == "勇者" && cardAbandoned[i].nLevel == 4) {
				AddToHand(1, i);
				DisplayCards();
				printf("你横置 ");
				DisplayCertainCard(player[1].cardOnField[iCard]);
				printf("，将弃牌区的一张 ");
				DisplayCertainCard(player[1].cardInHand.back());
				printf("加入了手牌。\n");
				system("pause");
				AbandonBomb();
				return true;
			}
		}
		printf("你横置了 ");
		DisplayCertainCard(player[1].cardOnField[iCard]);
		printf("，但是弃牌区没有 ");
		DisplayCertainCard(card[48]);
		printf("。\n");
		system("pause");
		return true;
	}
	return false;
}

bool SaintSword(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[1].nActionPoint--;
		if (g_bMonkMode) PutOnField_Monk(1, iCard);
		else PutOnField(1, iCard);
		DisplayCards();
		printf("你将 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		return true;
	}
	return false;
}

bool BlackSmith(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if ((g_bMonkMode && player[1].nInHand >= 2) || (!g_bMonkMode && player[1].nInHand >= 3)) {
			player[1].nActionPoint--;
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, iStatus);
				DisplayCards();
				CheckNun(1);
			}
			printf("请选择你要放在场上的两张牌（输入两个数字，一个数字各一行）：\n");
			for (int i = 0; i < player[1].nInHand; i++) {
				printf("%d.", i + 1);
				DisplayCertainCard(player[1].cardInHand[i]);
			}
			printf("\n");
			int ch1, ch2;
			while (1) {
				ch1 = InputIntSafely();
				ch2 = InputIntSafely();
				if (ch1 >= 1 && ch1 <= player[1].nInHand && ch2 >= 1 && ch2 <= player[1].nInHand) {
					if (ch1 == ch2) {
						printf("请选择不同的两张牌！\n");
						system("pause");
					} else {
						Card card1 = player[1].cardInHand[ch1 - 1], card2 = player[1].cardInHand[ch2 - 1];
						PutOnField(1, min(ch1, ch2) - 1);
						PutOnField(1, max(ch1, ch2) - 2);
						if (card1.nArmory != NO_ARMORY && card2.nArmory == card2.nArmory) {
							player[1].nNumCoin++;
							DisplayCards();
							printf("你放的两张牌纹章相同，你 +1 金币！\n");
							system("pause");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
							return true;
						} else if (card1.nLevel == card2.nLevel && card1.nLevel != NO_LEVEL) {
							player[1].nNumCoin++;
							DisplayCards();
							printf("你放的两张牌 Lv 相同，你 +1 金币！\n");
							system("pause");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
							return true;
						}
						return true;
					}
				} else {
					printf("输入的数字超出了范围！\n");
					system("pause");
				}
			}
		} else {
			printf("你的手牌不足，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
	}
	return false;
}

bool Fighter(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if ((g_bMonkMode && player[1].nInHand >= 1) || (!g_bMonkMode && player[1].nInHand >= 2)) {
			player[1].nActionPoint--;
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, iStatus);
				DisplayCards();
				CheckNun(1);
			}
			int nSumLevel = 0;
			while (player[1].nInHand) {
				if (player[1].cardInHand[0].nLevel != NO_LEVEL) {
					nSumLevel += player[1].cardInHand[0].nLevel;
				}
				PutOnField(1, 0);
			}
			if (nSumLevel % 2 == 0) {
				player[1].nNumCoin++;
				DisplayCards();
				printf("你放置在场上的手牌 Lv 之和为偶数，+1 金币！\n");
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
			return true;
		} else {
			printf("你的手牌不足！\n");
			system("pause");
			return false;
		}
	}
	return false;
}

bool Monk(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nYellow = 0;
		vector<int> iYellow;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nColor == YELLOW && cardAbandoned[i].nLevel != 1) {
				nYellow++;
				iYellow.push_back(i);
			}
		}
		if (nYellow) {
			AbandonCertainCard(1, iCard, iStatus);
			DisplayCards();
			printf("请选择弃牌区的一张 Lv.1 以外的黄色手牌：\n");
			for (int i = 0; i < nYellow; i++) {
				printf("%d.", i + 1);
				DisplayCertainCard(cardAbandoned[iYellow[i]]);
			}
			printf("\n");
			int ch;
			while (1) {
				ch = InputIntSafely();
				if (ch >= 1 && ch <= nYellow) break;
				else printf("输入的数字超出了范围！\n");
			}
			printf("你选中了 ");
			DisplayCertainCard(cardAbandoned[iYellow[ch - 1]]);
			printf("。\n");
			printf("请选择你要对其进行的操作：\n");
			printf("1.不消耗行动点打出 2.加入手牌\n");
			char ch1 = _getch();
			while (ch1 != '1' && ch1 != '2') ch1 = _getch();
			if (ch1 == '1') {
				if (cardAbandoned[iYellow[ch - 1]].nType == TRIGGER) {
					printf("该卡牌为触发牌，不能主动打出。\n");
					system("pause");
					return false;
				}
				player[1].nActionPoint--;
				int tmp = player[1].nActionPoint;
				g_bMonkMode = true;
				g_iMonkCard = iYellow[ch - 1];
				cardAbandoned[iYellow[ch - 1]].pfnOpt(iYellow[ch - 1], IN_HAND);
				g_bMonkMode = false;
				player[1].nActionPoint = tmp;
			} else {
				player[1].nActionPoint--;
				AddToHand(1, iYellow[ch - 1]);
				DisplayCards();
				AbandonBomb();
			}
			return true;
		} else {
			printf("弃牌区没有 Lv.1 以外的黄色手牌！\n");
			system("pause");
			return false;
		}
	}
	return false;
}

bool Nun(int iCard, int iStatus) {
	return false;
}

bool Businessman(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if (!player[1].nOnField) {
			printf("你场上没牌，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("。\n");
			system("pause");
			return false;
		} else {
			int nCoin = 0;
			vector<int> iCoin;
			for (int i = 2; i <= g_nNumPlayer; i++) {
				if (player[i].nNumCoin) {
					nCoin++;
					iCoin.push_back(i);
				}
			}
			if (!nCoin) {
				printf("其他玩家没有金币，不能使用 ");
				DisplayCertainCard(player[1].cardInHand[iCard]);
				printf("。\n");
				system("pause");
				return false;
			}
			g_iAttacker = 1;
			player[1].nActionPoint--;
			printf("请选择你的攻击对象：");
			for (int i = 0; i < nCoin; i++) printf("%d.玩家%d ", i + 1, iCoin[i]);
			printf("\n");
			int iTarget = iCoin[RestrictedInput(1, nCoin) - 1];
			if (Defend(1, iTarget, iCard, iStatus)) return true;
			printf("请选择你场上的一张牌交付给玩家 %d ：\n", iTarget);
			for (int i = 0; i < player[1].nOnField; i++) {
				printf("%d.", i + 1);
				DisplayCertainCard(player[1].cardOnField[i]);
			}
			printf("\n");
			int iCardToGive = RestrictedInput(1, player[1].nOnField) - 1;
			player[1].cardOnField[iCardToGive].nStatus = IN_HAND;
			player[iTarget].cardInHand.push_back(player[1].cardOnField[iCardToGive]);
			player[iTarget].nInHand++;
			player[1].cardOnField.erase(player[1].cardOnField.begin() + iCardToGive);
			player[1].nOnField--;
			player[iTarget].nNumCoin--;
			player[1].nNumCoin++;
			AbandonCertainCard(1, iCard, iStatus);
			DisplayCards();
			printf("攻击成功，你从玩家 %d 处获得 1 金币！\n", iTarget);
			system("pause");
			if (player[1].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = 1;
				g_nGameStatus = 0;
				return true;
			}
			CheckDancer(1);
			if (!g_nGameStatus) return true;
			CheckMimic(1, GET_COIN);
			CheckMinister(iTarget);
			return true;
		}
	}
	return false;
}

bool BabyCrane(int iCard, int iStatus) {
	return false;
}

bool Dancer(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[1].nActionPoint--;
		if (!g_bMonkMode) PutOnField(1, iCard);
		else PutOnField_Monk(1, iCard);
		DisplayCards();
		printf("你将 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		return true;
	}
	return false;
}

bool Master(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nMaster = 0;
		vector<int> iMaster;
		for (int i = 2; i <= g_nNumPlayer; i++) {
			if (player[i].nInHand) {
				nMaster++;
				iMaster.push_back(i);
			}
		}
		if (!nMaster) {
			printf("其他玩家都没有手牌，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
		g_iAttacker = 1;
		player[1].nActionPoint--;
		printf("请指定你的攻击对象：\n");
		for (int i = 0; i < nMaster; i++) printf("%d.玩家%d ", i + 1, iMaster[i]);
		printf("\n");
		int iTarget = iMaster[RestrictedInput(1, nMaster) - 1];
		if (Defend(1, iTarget, iCard, iStatus)) return true;
		if (!g_bMonkMode) {
			AbandonCertainCard(1, iCard, iStatus);
			DisplayCards();
			CheckNun(1);
		}
		printf("请输入你要求放置的手牌 Lv（1/2/3/4）：");
		int iLv = RestrictedInput(1, 4);
		for (int i = 0; i < player[iTarget].nInHand; i++) {
			if (player[iTarget].cardInHand[i].nLevel == iLv) {
				PutOnField(iTarget, i);
				DisplayCards();
				printf("太遗憾了，玩家 %d 有 Lv.%d 的手牌！\n", iTarget, iLv);
				system("pause");
				return true;
			}
		}
		player[1].nNumCoin++;
		DisplayCards();
		printf("玩家 %d 没有 Lv.%d 的手牌，你 +1 金币！\n", iTarget, iLv);
		system("pause");
		if (player[1].nNumCoin == g_nStdCoinNumber) {
			g_iWinPlayer = 1;
			g_nGameStatus = 0;
		}
		CheckDancer(1);
		if (!g_nGameStatus) return true;
		CheckMimic(1, GET_COIN);
		return true;
	}
	return false;
}

bool Guard(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		AbandonCertainCard(1, iCard, IN_HAND);
		DisplayCards();
		CheckNun(1);
		GetOneCardFromLibrary(1);
		AbandonBomb();
		return true;
	}
	return false;
}

bool HolyKnight(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		printf("你正在使用 ");
		DisplayCertainCard(player[1].cardInHand[iCard]);
		printf("。\n");
		printf("请选择使用方案：\n");
		printf("1.攻击有效，你抽两张牌\n");
		printf("2.攻击无效，你抽一张牌。\n");
		char ch = _getch();
		while (ch != '1' && ch != '2') ch = _getch();
		if (ch == '1') {
			AbandonCertainCard(1, iCard, iStatus);
			DisplayCards();
			CheckNun(1);
			GetOneCardFromLibrary(1);
			AbandonBomb();
			GetOneCardFromLibrary(1);
			AbandonBomb();
			DisplayCards();
			return true;
		} else {
			if (g_bMonkMode) AddToHand(g_iAttacker, g_iMonkCard);
			else if (g_bDragonMode) AddToHand(g_iAttacker, g_iDragonCard);
			AbandonCertainCard(1, iCard, iStatus);
			DisplayCards();
			CheckNun(1);
			GetOneCardFromLibrary(1);
			AbandonBomb();
			DisplayCards();
			return false;
		}
	}
	return true;
}

bool Berserker(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		g_iAttacker = 1;
		printf("请选择你的攻击对象（输入玩家编号）：");
		int iTarget = RestrictedInput(2, g_nNumPlayer);
		player[1].nActionPoint--;
		if (Defend(1, iTarget, iCard, iStatus)) return true;
		bool bSaintSword = false, bBlade = false;
		if (CheckSaintSword(1)) {
			printf("你场上有 ");
			DisplayCertainCard(card[6]);
			printf("，是否横置？(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bSaintSword = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (!bSaintSword && CheckBlade(1)) {
			printf("你场上有 ");
			DisplayCertainCard(card[55]);
			printf("，是否横置？(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bBlade = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (bSaintSword) {
			/* 使用圣剑 */
			if (player[iTarget].nOnField == 0) {
				if (!g_bMonkMode) {
					AbandonCertainCard(1, iCard, IN_HAND);
					DisplayCards();
					CheckNun(1);
				}
				printf("攻击失败，对手场上没牌！\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, IN_HAND);
				DisplayCards();
				CheckNun(1);
			}
			printf("请选择你要丢弃的对手场上的牌：\n");
			for (int i = 0; i < player[iTarget].nOnField; i++) {
				printf("%d.", i + 1);
				DisplayCertainCard(player[iTarget].cardOnField[i]);
			}
			printf("\n");
			int ch = RestrictedInput(1, player[iTarget].nOnField);
			AbandonCertainCard(iTarget, ch - 1, ON_FIELD);
			DisplayCards();
			CheckNun(iTarget);
			if (cardAbandoned.back().nLevel >= 0 && cardAbandoned.back().nLevel < 4) {
				player[1].nNumCoin++;
				DisplayCards();
				printf("你丢弃的卡牌 Lv 比你打出的勇者低，你 +1 金币！\n");
				system("pause");
				if (player[1].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = 1;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(1);
				if (!g_nGameStatus) return true;
				CheckMimic(1, GET_COIN);
			} else printf("攻击失败！\n");
			system("pause");
			CheckMinister(iTarget);
			return true;
		} else if (bBlade) {
			if (g_bMonkMode) PutOnField_Monk(1, iCard);
			else PutOnField(1, iCard);
			DisplayCards();
			if (player[iTarget].nInHand == 0) {
				printf("该玩家没有手牌，攻击失败！\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			bool bYellow = false, bPurple = false;
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].nColor == YELLOW) bYellow = true;
				else bPurple = true;
			}
			if (bYellow && bPurple) {
				printf("该玩家手上同时拥有黄色和紫色手牌，请选择：1.黄 2.紫\n");
				while (1) {
					char ch = _getch();
					if (ch == '1') {
						bool res = true;
						for (int i = 0; i < player[iTarget].nInHand;) {
							if (player[iTarget].cardInHand[i].nColor == YELLOW) {
								if (player[iTarget].cardInHand[i].nLevel == NO_LEVEL) res = false;
								PutOnField(iTarget, i);

							} else i++;
						}
						DisplayCards();
						if (res) {
							player[1].nNumCoin++;
							DisplayCards();
							printf("攻击成功，你 +1 金币！\n");
							system("pause");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
						} else {
							printf("攻击失败！\n");
							system("pause");
						}
						break;
					} else if (ch == '2') {
						bool res = true;
						for (int i = 0; i < player[iTarget].nInHand;) {
							if (player[iTarget].cardInHand[i].nColor == PURPLE) {
								if (player[iTarget].cardInHand[i].nLevel == NO_LEVEL) res = false;
								PutOnField(iTarget, i);
							} else i++;
						}
						DisplayCards();
						if (res) {
							player[1].nNumCoin++;
							DisplayCards();
							printf("攻击成功，你 +1 金币！\n");
							system("pause");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
						} else {
							printf("攻击失败！\n");
							system("pause");
						}
						break;
					} //else if
				} //while
			} else {
				bool res = true;
				for (int i = 0; i < player[iTarget].nInHand; i++) {
					if (player[iTarget].cardInHand[i].nLevel == NO_LEVEL) res = false;
					PutOnField(iTarget, i);
				}
				DisplayCards();
				if (res) {
					player[1].nNumCoin++;
					DisplayCards();
					printf("攻击成功，你 +1 金币！\n");
					system("pause");
					if (player[1].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = 1;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(1);
					if (!g_nGameStatus) return true;
					CheckMimic(1, GET_COIN);
				} else {
					printf("攻击失败！\n");
					system("pause");
				}
			} //else
		} else {
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, iStatus);
				DisplayCards();
				CheckNun(1);
			}
			if (!CheckPurpleCards_CertainPlayer(iTarget)) {
				printf("该玩家手上没有紫牌，攻击失败！\n");
				system("pause");
				return true;
			}
			bool res = true;
			for (int i = 0; i < player[iTarget].nInHand;) {
				if (player[iTarget].cardInHand[i].nColor == PURPLE) {
					if (player[iTarget].cardInHand[i].nLevel == NO_LEVEL) res = false;
					PutOnField(iTarget, i);
				} else i++;
			}
			DisplayCards();
			if (res) {
				player[1].nNumCoin++;
				DisplayCards();
				printf("攻击成功，你 +1 金币！\n");
				system("pause");
				if (player[1].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = 1;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(1);
				if (!g_nGameStatus) return true;
				CheckMimic(1, GET_COIN);
			} else {
				printf("攻击失败！\n");
				system("pause");
			}
		}
		return true;
	}
	return false;
}

bool Warrior_Lv4(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		g_iAttacker = 1;
		printf("请选择你的攻击对象（输入玩家编号）：");
		int iTarget = RestrictedInput(2, g_nNumPlayer);
		player[1].nActionPoint--;
		if (Defend(1, iTarget, iCard, iStatus)) return true;
		bool bSaintSword = false, bBlade = false;
		if (CheckSaintSword(1)) {
			printf("你场上有 ");
			DisplayCertainCard(card[6]);
			printf("，是否横置？(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bSaintSword = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (!bSaintSword && CheckBlade(1)) {
			printf("你场上有 ");
			DisplayCertainCard(card[55]);
			printf("，是否横置？(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bBlade = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (bSaintSword) {
			/* 使用圣剑 */
			if (player[iTarget].nOnField == 0) {
				if (!g_bMonkMode) {
					AbandonCertainCard(1, iCard, IN_HAND);
					DisplayCards();
					CheckNun(1);
				}
				printf("攻击失败，对手场上没牌！\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, IN_HAND);
				DisplayCards();
				CheckNun(1);
			}
			printf("请选择你要丢弃的对手场上的牌：\n");
			for (int i = 0; i < player[iTarget].nOnField; i++) {
				printf("%d.", i + 1);
				DisplayCertainCard(player[iTarget].cardOnField[i]);
			}
			printf("\n");
			int ch = RestrictedInput(1, player[iTarget].nOnField);
			AbandonCertainCard(iTarget, ch - 1, ON_FIELD);
			DisplayCards();
			CheckNun(iTarget);
			if (cardAbandoned.back().nLevel >= 0 && cardAbandoned.back().nLevel < 4) {
				player[1].nNumCoin++;
				DisplayCards();
				printf("你丢弃的卡牌 Lv 比你打出的勇者低，你 +1 金币！\n");
				system("pause");
				if (player[1].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = 1;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(1);
				if (!g_nGameStatus) return true;
				CheckMimic(1, GET_COIN);
			} else printf("攻击失败！\n");
			system("pause");
			CheckMinister(iTarget);
			return true;
		} else if (bBlade) {
			if (g_bMonkMode) PutOnField_Monk(1, iCard);
			else PutOnField(1, iCard);
			DisplayCards();
			if (player[iTarget].nInHand == 0) {
				printf("该玩家没有手牌，攻击失败！\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			int nYellow = 0, nPurple = 0;
			vector<int> iYellow, iPurple;
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].nColor == YELLOW) {
					nYellow++;
					iYellow.push_back(i);
				} else {
					nPurple++;
					iPurple.push_back(i);
				}
			}
			if (nYellow && nPurple) {
				printf("该玩家有 %d 张黄牌和 %d 张紫牌，请选择：1.黄 2.紫", nYellow, nPurple);
				while (1) {
					char ch = _getch();
					if (ch == '1') {
						int x = iYellow[rand() % nYellow];
						Card c = player[iTarget].cardInHand[x];
						PutOnField(iTarget, x);
						DisplayCards();
						if (c.nLevel != NO_LEVEL && c.nLevel < 4) {
							player[1].nNumCoin++;
							printf("攻击成功，你 +1 金币！\n");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
						} else printf("攻击失败！\n");
						break;
					} else if (ch == '2') {
						int x = iPurple[rand() % nPurple];
						Card c = player[iTarget].cardInHand[x];
						PutOnField(iTarget, x);
						DisplayCards();
						if (c.nLevel != NO_LEVEL && c.nLevel < 4) {
							player[1].nNumCoin++;
							DisplayCards();
							printf("攻击成功，你 +1 金币！\n");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
						} else printf("攻击失败！\n");
						break;
					}
				}
				system("pause");
				CheckMinister(iTarget);
				return true;
			} else {
				int x = rand() % player[iTarget].nInHand;
				Card c = player[iTarget].cardInHand[x];
				PutOnField(iTarget, x);
				DisplayCards();
				if (c.nLevel != NO_LEVEL && c.nLevel < 4) {
					player[1].nNumCoin++;
					DisplayCards();
					printf("攻击成功，你 +1 金币！\n");
					if (player[1].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = 1;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(1);
					if (!g_nGameStatus) return true;
					CheckMimic(1, GET_COIN);
				} else printf("攻击失败！\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
		} else {
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, iStatus);
				DisplayCards();
				CheckNun(1);
			}
			if (!CheckPurpleCards_CertainPlayer(iTarget)) {
				printf("该玩家手上没有紫牌，攻击失败！\n");
				system("pause");
				return true;
			}
			bool res = false;
			int nPurple = 0;
			vector<int> iPurple;
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].nColor == PURPLE) {
					nPurple++;
					iPurple.push_back(i);
				}
			}
			int x = iPurple[rand() % nPurple];
			Card c = player[iTarget].cardInHand[x];
			PutOnField(iTarget, x);
			DisplayCards();
			if (c.nLevel >= 0 && c.nLevel < 4) res = true;
			if (res) {
				player[1].nNumCoin++;
				DisplayCards();
				printf("攻击成功，你 +1 金币！\n");
				system("pause");
				if (player[1].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = 1;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(1);
				if (!g_nGameStatus) return true;
				CheckMimic(1, GET_COIN);
			} else {
				printf("攻击失败！\n");
				system("pause");
			}
		}
		return true;
	}
	return false;
}

bool Warrior_Lv5(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		g_iAttacker = 1;
		printf("请选择你的攻击对象（输入玩家编号）：");
		int iTarget = RestrictedInput(2, g_nNumPlayer);
		player[1].nActionPoint--;
		if (Defend(1, iTarget, iCard, iStatus)) return true;
		bool bSaintSword = false, bBlade = false;
		if (CheckSaintSword(1)) {
			printf("你场上有 ");
			DisplayCertainCard(card[6]);
			printf("，是否横置？(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bSaintSword = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (!bSaintSword && CheckBlade(1)) {
			printf("你场上有 ");
			DisplayCertainCard(card[55]);
			printf("，是否横置？(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bBlade = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (bSaintSword) {
			/* 使用圣剑 */
			if (player[iTarget].nOnField == 0) {
				if (!g_bMonkMode) {
					AbandonCertainCard(1, iCard, IN_HAND);
					DisplayCards();
					CheckNun(1);
				}
				printf("攻击失败，对手场上没牌！\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, IN_HAND);
				DisplayCards();
				CheckNun(1);
			}
			printf("请选择你要丢弃的对手场上的牌：\n");
			for (int i = 0; i < player[iTarget].nOnField; i++) {
				printf("%d.", i + 1);
				DisplayCertainCard(player[iTarget].cardOnField[i]);
			}
			printf("\n");
			int ch = RestrictedInput(1, player[iTarget].nOnField);
			AbandonCertainCard(iTarget, ch - 1, ON_FIELD);
			DisplayCards();
			CheckNun(iTarget);
			if (cardAbandoned.back().nLevel >= 0 && cardAbandoned.back().nLevel < 5) {
				player[1].nNumCoin++;
				DisplayCards();
				printf("你丢弃的卡牌 Lv 比你打出的勇者低，你 +1 金币！\n");
				system("pause");
				if (player[1].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = 1;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(1);
				if (!g_nGameStatus) return true;
				CheckMimic(1, GET_COIN);
			} else printf("攻击失败！\n");
			system("pause");
			CheckMinister(iTarget);
			return true;
		} else if (bBlade) {
			if (g_bMonkMode) PutOnField_Monk(1, iCard);
			else PutOnField(1, iCard);
			DisplayCards();
			if (player[iTarget].nInHand == 0) {
				printf("该玩家没有手牌，攻击失败！\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			int nYellow = 0, nPurple = 0;
			vector<int> iYellow, iPurple;
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].nColor == YELLOW) {
					nYellow++;
					iYellow.push_back(i);
				} else {
					nPurple++;
					iPurple.push_back(i);
				}
			}
			if (nYellow && nPurple) {
				printf("该玩家有 %d 张黄牌和 %d 张紫牌，请选择：1.黄 2.紫", nYellow, nPurple);
				while (1) {
					char ch = _getch();
					if (ch == '1') {
						int x = iYellow[rand() % nYellow];
						Card c = player[iTarget].cardInHand[x];
						PutOnField(iTarget, x);
						DisplayCards();
						if (c.nLevel != NO_LEVEL && c.nLevel < 5) {
							player[1].nNumCoin++;
							printf("攻击成功，你 +1 金币！\n");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
						} else printf("攻击失败！\n");
						break;
					} else if (ch == '2') {
						int x = iPurple[rand() % nPurple];
						Card c = player[iTarget].cardInHand[x];
						PutOnField(iTarget, x);
						DisplayCards();
						if (c.nLevel != NO_LEVEL && c.nLevel < 5) {
							player[1].nNumCoin++;
							DisplayCards();
							printf("攻击成功，你 +1 金币！\n");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
						} else printf("攻击失败！\n");
						break;
					}
				}
				system("pause");
				CheckMinister(iTarget);
				return true;
			} else {
				int x = rand() % player[iTarget].nInHand;
				Card c = player[iTarget].cardInHand[x];
				PutOnField(iTarget, x);
				DisplayCards();
				if (c.nLevel != NO_LEVEL && c.nLevel < 5) {
					player[1].nNumCoin++;
					DisplayCards();
					printf("攻击成功，你 +1 金币！\n");
					if (player[1].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = 1;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(1);
					if (!g_nGameStatus) return true;
					CheckMimic(1, GET_COIN);
				} else printf("攻击失败！\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
		} else {
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, iStatus);
				DisplayCards();
				CheckNun(1);
			}
			if (!CheckPurpleCards_CertainPlayer(iTarget)) {
				printf("该玩家手上没有紫牌，攻击失败！\n");
				system("pause");
				return true;
			}
			bool res = false;
			int nPurple = 0;
			vector<int> iPurple;
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].nColor == PURPLE) {
					nPurple++;
					iPurple.push_back(i);
				}
			}
			int x = iPurple[rand() % nPurple];
			Card c = player[iTarget].cardInHand[x];
			PutOnField(iTarget, x);
			DisplayCards();
			if (c.nLevel >= 0 && c.nLevel < 5) res = true;
			if (res) {
				player[1].nNumCoin++;
				DisplayCards();
				printf("攻击成功，你 +1 金币！\n");
				system("pause");
				if (player[1].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = 1;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(1);
				if (!g_nGameStatus) return true;
				CheckMimic(1, GET_COIN);
			} else {
				printf("攻击失败！\n");
				system("pause");
			}
		}
		return true;
	}
	return false;
}

bool BlackKnight(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		g_iAttacker = 1;
		printf("请选择你的攻击对象：");
		int iTarget = RestrictedInput(2, g_nNumPlayer);
		if (!player[iTarget].nInHand) {
			printf("该玩家没有手牌！\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		if (Defend(1, iTarget, iCard, iStatus)) return true;
		AbandonCertainCard(1, iCard, IN_HAND);
		DisplayCards();
		CheckNun(1);
		int x = rand() % player[iTarget].nInHand;
		Card c = player[iTarget].cardInHand[x];
		PutOnField(iTarget, x);
		DisplayCards();
		if (c.nLevel != NO_LEVEL) {
			player[1].nNumCoin++;
			DisplayCards();
			printf("攻击成功，你 +1 金币！\n");
			system("pause");
			if (player[1].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = 1;
				g_nStdCoinNumber = 0;
				return true;
			}
			CheckMimic(1, GET_COIN);
			CheckDancer(1);
			if (!g_nStdCoinNumber) return true;
		} else if (c.nArmory != NO_ARMORY) {
			int iBladeAbandoned = -1;
			for (int i = 0; i < g_nAbandoned; i++) {
				if (cardAbandoned[i].strName == "魔剑") {
					iBladeAbandoned = i;
					break;
				}
			}
			if (iBladeAbandoned != -1) {
				printf("牌库和弃牌区都有 ");
				DisplayCertainCard(card[55]);
				printf("，你想从哪里拿取？\n");
				printf("1.牌库 2.弃牌区\n");
				int ch = RestrictedInput(1, 2);
				if (ch == 1) {
					card[55].nStatus = IN_HAND;
					player[1].cardInHand.push_back(card[55]);
					player[1].nInHand++;
					card[55].nStatus = IN_LIBRARY;
				} else AddToHand(1, iBladeAbandoned);
			} else {
				player[1].cardInHand.push_back(card[55]);
				player[1].nInHand++;
			}
			DisplayCards();
			printf("你将 ");
			DisplayCertainCard(card[55]);
			printf("加入了手牌。\n");
			system("pause");
			AbandonBomb();
		} else {
			printf("攻击失败！\n");
			system("pause");
			return true;
		}
	}
	return false;
}

bool Demon(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[1].nActionPoint--;
		PutOnField(1, iCard);
		DisplayCards();
		printf("你将 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		return true;
	} else if (iStatus == ON_FIELD) {
		if (player[1].nOnField > 1) {
			int nToRe = player[1].nOnField - 1;
			vector<int> iToRe;
			for (int i = 0; i < player[1].nOnField; i++) {
				if (i == iCard) continue;
				iToRe.push_back(i);
			}
			printf("请选择你场上的一张牌加入手牌：\n");
			for (int i = 0; i < nToRe; i++) {
				printf("%d.", i + 1);
				DisplayCertainCard(player[1].cardOnField[iToRe[i]]);
			}
			printf("\n");
			int ch = RestrictedInput(1, nToRe) - 1;
			ReturnCardOnField(1, iToRe[ch]);
			DisplayCards();
			return true;
		} else {
			printf("你场上没有其它牌！\n");
			system("pause");
			return false;
		}
		return false;
	}
	return false;
}

bool FireDragon(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[1].nActionPoint--;
		if (!g_bDragonMode) PutOnField(1, iCard);
		else PutOnField_Dragon(1, iCard);
		DisplayCards();
		printf("你将 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		return true;
	} else if (iStatus == ON_FIELD) {
		int nFireAban = 0;
		vector<int> iFireAban;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nFire == FIRE) {
				nFireAban++;
				iFireAban.push_back(i);
			}
		}
		if (!nFireAban) {
			printf("弃牌区没有[火]！\n");
			system("pause");
			return false;
		}
		printf("请选择弃牌区的一张[火]：\n");
		for (int i = 0; i < nFireAban; i++) {
			printf("%d.", i + 1);
			DisplayCertainCard(cardAbandoned[iFireAban[i]]);
		}
		printf("\n");
		int iCh = iFireAban[RestrictedInput(1, nFireAban) - 1];
		printf("请选择你要对其进行的操作：\n");
		printf("1.不消耗行动点打出 2.加入手牌\n");
		char cCh = _getch();
		while (cCh != '1' && cCh != '2') cCh = _getch();
		if (cCh == '1') {
			g_bDragonMode = true;
			g_iDragonCard = iCh;
			int tmp = player[1].nActionPoint;
			cardAbandoned[iCh].pfnOpt(iCh, IN_HAND);
			player[1].nActionPoint = tmp;
			g_bDragonMode = false;
		} else {
			AddToHand(1, iCh);
			DisplayCards();
		}
		return true;
	}
	return false;
}

bool Tyrant(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[1].nActionPoint--;
		PutOnField(1, iCard);
		DisplayCards();
		printf("你将 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		return true;
	}
	return false;
}

bool Mimic(int iCard, int iStatus) {
	return false;
}

bool Blade(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[1].nActionPoint--;
		PutOnField(1, iCard);
		DisplayCards();
		printf("你将 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		return true;
	}
	return false;
}

bool DragonCub(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0, nDefender = 0;
		vector<int> iTarget, iDefender;
		for (int i = 2; i <= g_nNumPlayer; i++) {
			if (player[i].nOnField) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) {
			printf("其他玩家场上都没有牌，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		g_iAttacker = 1;
		for (int i = 0; i < nTarget; i++) {
			if (CheckGuard(iTarget[i]) || CheckHolyKnight(iTarget[i])) {
				nDefender++;
				iDefender.push_back(iTarget[i]);
			}
		}
		g_iAttacker = 1;
		if (nDefender == 1) {
			if (Defend(1, iDefender[0], iCard, iStatus)) return true;
		} else if (nDefender > 1) {
			printf("共 %d 名目标玩家有防牌，现随机触发。\n", nDefender);
			system("pause");
			if (Defend(1, iDefender[rand() % nDefender], iCard, iStatus)) return true;
		}
		if (!g_bDragonMode) {
			AbandonCertainCard(1, iCard, iStatus);
			DisplayCards();
			CheckNun(1);
		}
		bool res = true;
		for (int i = 0; i < nTarget; i++) {
			printf("请选择玩家 %d 场上的一张牌回到手牌：\n", iTarget[i]);
			for (int j = 0; j < player[iTarget[i]].nOnField; j++) {
				printf("%d.", j + 1);
				DisplayCertainCard(player[iTarget[i]].cardOnField[j]);
			}
			printf("\n");
			int ch = RestrictedInput(1, player[iTarget[i]].nOnField) - 1;
			ReturnCardOnField(iTarget[i], ch);
			if (player[iTarget[i]].cardInHand.back().nLevel == NO_LEVEL) res = false;
			DisplayCards();
			CheckMinister(iTarget[i]);
		}
		if (res) {
			player[1].nNumCoin++;
			DisplayCards();
			printf("攻击成功，你 +1 金币！\n");
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
		return true;
	}
	return false;
}

bool DragonFlame(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 2; i <= g_nNumPlayer; i++) {
			if (player[i].nOnField) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) {
			printf("其他玩家场上都没有牌，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		g_iAttacker = 1;
		printf("请选择你的攻击对象：\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.玩家%d ", i + 1, iTarget[i]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nTarget) - 1;
		if (Defend(1, ch, iCard, iStatus)) return true;
		if (!g_bDragonMode) {
			AbandonCertainCard(1, iCard, iStatus);
			DisplayCards();
			CheckNun(1);
		}
		printf("请选择你要丢弃的对手场上的牌：\n");
		for (int i = 0; i < player[iTarget[ch]].nOnField; i++) {
			printf("%d.", i + 1);
			DisplayCertainCard(player[iTarget[ch]].cardOnField[i]);
		}
		printf("\n");
		int ch1 = RestrictedInput(1, player[iTarget[ch]].nOnField) - 1;
		AbandonCertainCard(iTarget[ch], ch1, ON_FIELD);
		Card c = cardAbandoned.back();
		DisplayCards();
		CheckNun(iTarget[ch]);
		if (c.nLevel != NO_LEVEL) {
			player[1].nNumCoin++;
			DisplayCards();
			printf("你丢弃的卡牌带有 Lv，+1 金币！\n");
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
		CheckMinister(iTarget[ch]);
		return true;
	}
	return false;
}

bool DragonEgg(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[1].nActionPoint--;
		if (!g_bDragonMode) {
			AbandonCertainCard(1, iCard, iStatus);
			DisplayCards();
			CheckNun(1);
		}
		int nFireAban = 0;
		vector<int> iFireAban;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nFire == FIRE) {
				nFireAban++;
				iFireAban.push_back(i);
			}
		}
		if (nFireAban) {
			printf("牌库和弃牌区都有[火]，你想从哪里拿取？\n");
			printf("1.牌库 2.弃牌区\n");
			int ch = RestrictedInput(1, 2);
			if (ch == 1) {
				printf("请选择你想要的[火]：\n");
				printf("1.火龙 2.龙炎 3.龙幼崽 4.龙蛋\n");
				int ch1 = RestrictedInput(1, 4);
				if (ch1 == 1) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "火龙") {
							card[i].nStatus = IN_HAND;
							player[1].cardInHand.push_back(card[i]);
							player[1].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 2) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "龙炎") {
							card[i].nStatus = IN_HAND;
							player[1].cardInHand.push_back(card[i]);
							player[1].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 3) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "龙幼崽") {
							card[i].nStatus = IN_HAND;
							player[1].cardInHand.push_back(card[i]);
							player[1].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 4) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "龙蛋") {
							card[i].nStatus = IN_HAND;
							player[1].cardInHand.push_back(card[i]);
							player[1].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				}
			} else {
				printf("请选择弃牌区的一张[火]：\n");
				for (int i = 0; i < nFireAban; i++) {
					printf("%d.", i + 1);
					DisplayCertainCard(cardAbandoned[iFireAban[i]]);
				}
				printf("\n");
				int ch1 = RestrictedInput(1, nFireAban) - 1;
				AddToHand(1, iFireAban[ch1]);
			}
		} else {
			printf("只有牌库有[火]，请从牌库选择：\n");
			printf("1.火龙 2.龙炎 3.龙幼崽 4.龙蛋\n");
			int ch1 = RestrictedInput(1, 4);
			if (ch1 == 1) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "火龙") {
						card[i].nStatus = IN_HAND;
						player[1].cardInHand.push_back(card[i]);
						player[1].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 2) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "龙炎") {
						card[i].nStatus = IN_HAND;
						player[1].cardInHand.push_back(card[i]);
						player[1].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 3) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "龙幼崽") {
						card[i].nStatus = IN_HAND;
						player[1].cardInHand.push_back(card[i]);
						player[1].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 4) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "龙蛋") {
						card[i].nStatus = IN_HAND;
						player[1].cardInHand.push_back(card[i]);
						player[1].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			}
		}
		DisplayCards();
		return true;
	}
	return false;
}

bool Slime(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nc = 0;
		vector<int> ic;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nLevel == 1 || cardAbandoned[i].nLevel == 2) {
				nc++;
				ic.push_back(i);
			}
		}
		if (!nc) {
			printf("弃牌区没有 Lv.1 或 Lv.2 卡牌！\n");
			system("pause");
			return true;
		}
		player[1].nActionPoint--;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		printf("请选择一张牌加入手中：\n");
		for (int i = 0; i < nc; i++) {
			printf("%d.", i + 1);
			DisplayCertainCard(cardAbandoned[ic[i]]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nc) - 1;
		AddToHand(1, ic[ch]);
		DisplayCards();
		return true;
	}
	return false;
}

bool Bomb(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[1].nActionPoint--;
		printf("请选择你要在其场上放置 ");
		DisplayCertainCard(player[1].cardInHand[iCard]);
		printf("的玩家（输入玩家编号）：\n");
		int iTarget = RestrictedInput(2, g_nNumPlayer);
		player[1].cardInHand[iCard].nStatus = ON_FIELD;
		player[iTarget].cardOnField.push_back(player[1].cardInHand[iCard]);
		player[iTarget].nOnField++;
		player[1].cardInHand.erase(player[1].cardInHand.begin() + iCard);
		player[1].nInHand--;
		DisplayCards();
		CheckTyrant(1);
		return true;
	}
	return false;
}

bool Bat(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 2; i <= g_nNumPlayer; i++) {
			if (!player[i].nInHand && !player[i].nOnField) continue;
			iTarget.push_back(i);
			nTarget++;
		}
		if (!nTarget) {
			printf("其他玩家都没有牌，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		g_iAttacker = 1;
		printf("请选择你的攻击对象：\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.玩家%d ", i + 1, iTarget[i]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nTarget) - 1;
		if (Defend(1, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		if (player[iTarget[ch]].nInHand) {
			printf("这些是玩家 %d 的手牌：\n", iTarget[ch]);
			for (int i = 0; i < player[iTarget[ch]].nInHand; i++) {
				DisplayCertainCard(player[iTarget[ch]].cardInHand[i]);
			}
			printf("\n");
			system("pause");
		}
		while (player[iTarget[ch]].nOnField) ReturnCardOnField(iTarget[ch], 0);
		DisplayCards();
		CheckMinister(iTarget[ch]);
		return true;
	}
	return false;
}

bool Bone(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nPurple = 0;
		vector<int> iPurple;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nColor == PURPLE) {
				nPurple++;
				iPurple.push_back(i);
			}
		}
		if (!nPurple) {
			printf("弃牌区没有紫色卡牌，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		printf("请选择弃牌区的一张紫色手牌放在场上：\n");
		for (int i = 0; i < nPurple; i++) {
			printf("%d.", i + 1);
			DisplayCertainCard(cardAbandoned[iPurple[i]]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nPurple) - 1;
		Card c = cardAbandoned[iPurple[ch]];
		c.nStatus = ON_FIELD;
		player[1].cardOnField.push_back(c);
		player[1].nOnField++;
		cardAbandoned.erase(cardAbandoned.begin() + iPurple[ch]);
		g_nAbandoned--;
		DisplayCards();
		return true;
	}
	return false;
}

bool Wolfman(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nPurple = 0, nArmo = 0;
		vector<int> iPurple, iArmo;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nColor == PURPLE && cardAbandoned[i].nLevel != NO_LEVEL) {
				nPurple++;
				iPurple.push_back(i);
			}
			if (cardAbandoned[i].nArmory != NO_ARMORY) {
				nArmo++;
				iArmo.push_back(i);
			}
		}
		if (!nPurple && !nArmo) {
			printf("弃牌区既没有带 Lv 的紫牌，也没有带纹章的牌，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		printf("请选择弃牌区的一张牌：\n");
		for (int i = 0; i < nPurple; i++) {
			printf("%d.", i + 1);
			DisplayCertainCard(cardAbandoned[iPurple[i]]);
		}
		for (int i = 0; i < nArmo; i++) {
			printf("%d.", i + 1 + nPurple);
			DisplayCertainCard(cardAbandoned[iArmo[i]]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nPurple + nArmo) - 1;
		if (ch < nPurple) {
			Card c = cardAbandoned[iPurple[ch]];
			c.nStatus = ON_FIELD;
			player[1].cardOnField.push_back(c);
			player[1].nOnField++;
			cardAbandoned.erase(cardAbandoned.begin() + iPurple[ch]);
			g_nAbandoned--;
			DisplayCards();
			printf("你将弃牌区的一张 ");
			DisplayCertainCard(c);
			printf("放在了场上。\n");
		} else {
			AddToHand(1, iArmo[ch - nPurple]);
			DisplayCards();
			printf("你将弃牌区的一张 ");
			DisplayCertainCard(player[1].cardInHand.back());
			printf("加入了手牌。\n");
		}
		system("pause");
		return true;
	}
	return false;
}

bool HarleyQuinn(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 2; i <= g_nNumPlayer; i++) {
			if (player[i].nInHand) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) {
			printf("其他玩家都没有手牌，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		g_iAttacker = 1;
		printf("请选择你的攻击对象：\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.玩家%d ", i + 1, iTarget[i]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nTarget) - 1;
		if (Defend(1, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		printf("请选择你要宣言的卡牌：\n");
		printf("1.黄牌 2.紫牌\n");
		char ch1 = _getch();
		while (ch1 != '1' && ch1 != '2') ch1 = _getch();
		printf("请选择你要宣言的卡牌：\n");
		int num = 0;
		vector<int> ic;
		if (ch1 == '1') {
			for (int i = 1; i <= NUM_YELLOW_CARD; i++) {
				if (!CompareCards(card[i], card[i - 1])) {
					printf("%d.", ++num);
					DisplayCertainCard(card[i]);
					ic.push_back(i);
				}
			}
			printf("\n");
		} else {
			for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
				if (!CompareCards(card[i], card[i - 1])) {
					printf("%d.", ++num);
					DisplayCertainCard(card[i]);
					ic.push_back(i);
				}
			}
			printf("\n");
		}
		int ch2 = RestrictedInput(1, num) - 1;
		Card c = card[ic[ch2]];
		for (int i = 0; i < player[iTarget[ch]].nInHand; i++) {
			if (CompareCards(c, player[iTarget[ch]].cardInHand[i])) {
				AbandonCertainCard(iTarget[ch], i, IN_HAND);
				DisplayCards();
				printf("玩家 %d 丢弃了一张 ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("。\n");
				system("pause");
				CheckNun(iTarget[ch]);
				if (c.nLevel != NO_LEVEL) {
					player[1].nNumCoin++;
					DisplayCards();
					printf("被丢弃的卡牌带有 Lv，你 +1 金币！\n");
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
				return true;
			}
		}
		printf("攻击失败，玩家 %d 手上没有该牌！\n", iTarget[ch]);
		system("pause");
		CheckMinister(iTarget[ch]);
		return true;
	}
	return false;
}

bool Witch(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 2; i <= g_nNumPlayer; i++) {
			if (player[i].nInHand || player[i].nOnField) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) {
			printf("其他玩家都没有牌，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		g_iAttacker = 1;
		printf("请选择你的攻击对象：\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.玩家%d ", i + 1, iTarget[i]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nTarget) - 1;
		if (Defend(1, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		printf("请选择你要宣言的卡牌：\n");
		printf("1.黄牌 2.紫牌\n");
		char ch1 = _getch();
		while (ch1 != '1' && ch1 != '2') ch1 = _getch();
		printf("请选择你要宣言的卡牌：\n");
		int num = 0;
		vector<int> ic;
		if (ch1 == '1') {
			for (int i = 1; i <= NUM_YELLOW_CARD; i++) {
				if (!CompareCards(card[i], card[i - 1])) {
					printf("%d.", ++num);
					DisplayCertainCard(card[i]);
					ic.push_back(i);
				}
			}
			printf("\n");
		} else {
			for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
				if (!CompareCards(card[i], card[i - 1])) {
					printf("%d.", ++num);
					DisplayCertainCard(card[i]);
					ic.push_back(i);
				}
			}
			printf("\n");
		}
		int ch2 = RestrictedInput(1, num) - 1;
		Card c = card[ic[ch2]];
		for (int i = 0; i < player[iTarget[ch]].nInHand; i++) {
			if (CompareCards(c, player[iTarget[ch]].cardInHand[i])) {
				player[1].cardInHand.push_back(player[iTarget[ch]].cardInHand[i]);
				player[1].nInHand++;
				player[iTarget[ch]].cardInHand.erase(player[iTarget[ch]].cardInHand.begin() + i);
				player[iTarget[ch]].nInHand--;
				DisplayCards();
				printf("玩家 %d 将手中的一张 ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("交付到了你的手上。\n");
				system("pause");
				AbandonBomb();
				CheckMinister(iTarget[ch]);
				return true;
			}
		}
		for (int i = 0; i < player[iTarget[ch]].nOnField; i++) {
			if (CompareCards(c, player[iTarget[ch]].cardOnField[i])) {
				player[1].cardInHand.push_back(player[iTarget[ch]].cardOnField[i]);
				player[1].nInHand++;
				player[iTarget[ch]].cardOnField.erase(player[iTarget[ch]].cardOnField.begin() + i);
				player[iTarget[ch]].nOnField--;
				DisplayCards();
				printf("玩家 %d 将场上的一张 ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("交付到了你的手上。\n");
				system("pause");
				AbandonBomb();
				CheckMinister(iTarget[ch]);
				return true;
			}
		}
		printf("攻击失败，玩家 %d 没有该牌！\n", iTarget[ch]);
		system("pause");
		CheckMinister(iTarget[ch]);
		return true;
	}
	return false;
}

bool Goblin(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 2; i <= g_nNumPlayer; i++) {
			if (player[i].nNumCoin > player[1].nNumCoin) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) {
			printf("没有比你钱多的玩家，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		g_iAttacker = 1;
		printf("请选择你的攻击对象：\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.玩家%d ", i + 1, iTarget[i]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nTarget) - 1;
		if (Defend(1, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		player[iTarget[ch]].nNumCoin--;
		player[1].nNumCoin++;
		DisplayCards();
		printf("玩家 %d 向你支付了 1 金币！\n", iTarget[ch]);
		system("pause");
		if (player[1].nNumCoin == g_nStdCoinNumber) {
			g_iWinPlayer = 1;
			g_nGameStatus = 0;
			return true;
		}
		CheckMimic(1, GET_COIN);
		CheckDancer(1);
		if (!g_nGameStatus) return true;
		CheckMinister(iTarget[ch]);
		return true;
	}
	return false;
}

bool Tauren(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 2; i <= g_nNumPlayer; i++) {
			bool bArmo = false;
			for (int j = 0; j < player[i].nOnField; j++) {
				if (player[i].cardOnField[j].nArmory != NO_ARMORY) {
					bArmo = true;
					break;
				}
			}
			if (bArmo) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) {
			printf("其他玩家场上没有纹章牌，不能使用 ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("！\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		g_iAttacker = 1;
		printf("请选择你的攻击对象：\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.玩家%d ", i + 1, iTarget[i]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nTarget) - 1;
		if (Defend(1, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		int ch1 = rand() % 2;
		if (ch1 && !player[iTarget[ch]].nNumCoin) ch1 = 0;
		if (ch1) {
			player[iTarget[ch]].nNumCoin--;
			player[1].nNumCoin++;
			DisplayCards();
			printf("玩家 %d 向你支付了 1 金币！\n", iTarget[ch]);
			system("pause");
			if (player[1].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = 1;
				g_nGameStatus = 0;
				return true;
			}
			CheckMimic(1, GET_COIN);
			CheckDancer(1);
			if (!g_nGameStatus) return true;
			CheckMinister(iTarget[ch]);
		} else {
			for (int i = 0; i < player[iTarget[ch]].nOnField;) {
				if (player[iTarget[ch]].cardOnField[i].nArmory != NO_ARMORY) {
					Card c = player[iTarget[ch]].cardOnField[i];
					c.nStatus = IN_HAND;
					player[1].cardInHand.push_back(c);
					player[1].nInHand++;
					player[iTarget[ch]].cardOnField.erase(player[iTarget[ch]].cardOnField.begin() + i);
					player[iTarget[ch]].nOnField--;
				} else i++;
			}
			DisplayCards();
			printf("玩家 %d 将其场上的所有纹章牌交付到了你手上。\n", iTarget[ch]);
			system("pause");
			AbandonBomb();
		}
		return true;
	}
	return false;
}

bool GrailAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if (!g_bMonkMode) PutOnField(iPlayer, iCard);
		else PutOnField_Monk(iPlayer, iCard);
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("放在了场上。\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		player[iPlayer].nActionPoint--;
		return true;
	}
	return false;
}

bool MinisterAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if (g_bMonkMode) PutOnField_Monk(iPlayer, iCard);
		else PutOnField(iPlayer, iCard);
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("放在了场上。\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		player[iPlayer].nActionPoint--;
		return true;
	}
	return false;
}

bool PrincessAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if (g_bMonkMode) PutOnField_Monk(iPlayer, iCard);
		else PutOnField(iPlayer, iCard);
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("放在了场上。\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		return true;
	}
	return false;
}

bool Warrior_RoyalAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		g_iAttacker = iPlayer;
		int nOrdinary = 0, nBlade = 0, nSaintSword = 0;
		vector<int> iOrdinary, iBlade, iSaintSword;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nInHand) {
				nBlade++;
				iBlade.push_back(i);
			}
			if (player[i].nOnField) {
				nSaintSword++;
				iSaintSword.push_back(i);
			}
			for (int j = 0; j < player[i].nInHand; j++) {
				if (player[i].cardInHand[j].nColor == PURPLE) {
					nOrdinary++;
					iOrdinary.push_back(i);
					break;
				}
			}
		}
		if (nBlade && CheckBlade(iPlayer)) {
			player[iPlayer].nActionPoint--;
			int iTarget = iBlade[rand() % nBlade];
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d ，并横置了 ", iTarget);
			DisplayCertainCard(card[55]);
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			IllustrateCard(card[55]);
			printf("！\n");
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			int x = rand() % player[iTarget].nInHand;
			Card c = player[iTarget].cardInHand[x];
			PutOnField(iTarget, x);
			DisplayCards();
			if (c.nLevel != NO_LEVEL || c.nArmory == DAEMON || c.nArmory == ROYAL_AND_DAEMON) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
				if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = iPlayer;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(iPlayer);
				if (!g_nGameStatus) return true;
				CheckMimic(iPlayer, GET_COIN);
			} else printf("攻击失败。\n");
			system("pause");
			CheckMinister(iTarget);
			return true;
		} else if (nSaintSword && CheckSaintSword(iPlayer)) {
			player[iPlayer].nActionPoint--;
			int iTarget = iSaintSword[rand() % nSaintSword];
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d ，并横置了 ", iTarget);
			DisplayCertainCard(card[6]);
			printf("！\n");
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			IllustrateCard(card[6]);
			system("pause");
			if (iTarget == 1) {
				bool bGuard = CheckGuard(1), bHolyKnight = CheckHolyKnight(1);
				if (bGuard && bHolyKnight) {
					printf("你手中同时有卫兵和圣骑士，请选择触发：\n");
					printf("1.卫兵 2.圣骑士\n");
					IllustrateCard(card[33]);
					IllustrateCard(card[37]);
					char ch = _getch();
					while (ch != '1' && ch != '2') {
						ch = _getch();
					}
					if (ch == '1') {
						UseGuard(iPlayer, iTarget, iCard);
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
					UseGuard(iPlayer, iTarget, iCard);
					return true;
				} else if (bHolyKnight) {
					for (int i = 0; i < player[1].nInHand; i++) {
						if (player[1].cardInHand[i].strName == "圣骑士") {
							bool res = player[1].cardInHand[i].pfnOpt(1, i);
							if (res) break;
							else return true;
						}
					}
				}
				if (!g_bMonkMode) {
					AbandonCertainCard(iPlayer, iCard, iStatus);
					DisplayCards();
					CheckNun(iPlayer);
				}
				int x = rand() % player[1].nOnField;
				AbandonCertainCard(1, x, ON_FIELD);
				DisplayCards();
				CheckNun(1);
				printf("玩家 %d 丢弃了你场上的一张 ", iPlayer);
				DisplayCertainCard(cardAbandoned.back());
				printf(" 。\n");
				printf("但是玩家 %d 打出的勇者没有 Lv，你逃过一劫。\n", iPlayer);
				system("pause");
				CheckMinister(iTarget);
				return true;
			} else {
				bool bGuard = CheckGuard(iTarget), bHolyKnight = CheckHolyKnight(iTarget);
				if (bGuard) {
					UseGuard(iPlayer, iTarget, iCard);
					return true;
				} else if (bHolyKnight) {
					for (int i = 0; i < player[iTarget].nInHand; i++) {
						if (player[iTarget].cardInHand[i].strName == "圣骑士") {
							bool res = player[iTarget].cardInHand[i].pfnOptAI(iPlayer, i, IN_HAND);
							if (res) break;
							else return true;
						}
					}
				}
				if (!g_bMonkMode) {
					AbandonCertainCard(iPlayer, iCard, iStatus);
					DisplayCards();
					CheckNun(iPlayer);
				}
				int x = rand() % player[iTarget].nOnField;
				AbandonCertainCard(iTarget, x, ON_FIELD);
				DisplayCards();
				CheckNun(iTarget);
				printf("玩家 %d 丢弃了玩家 %d 场上的一张 ", iPlayer, iTarget);
				DisplayCertainCard(cardAbandoned.back());
				printf(" 。\n");
				printf("但是玩家 %d 打出的勇者没有 Lv，玩家 %d 逃过一劫。\n", iPlayer, iTarget);
				system("pause");
				return true;
			}
			CheckMinister(iTarget);
		} else {
			player[iPlayer].nActionPoint--;
			int iTarget = iOrdinary[rand() % nOrdinary];
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d 。\n", iTarget);
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			system("pause");
			if (iTarget == 1) {
				bool bGuard = CheckGuard(1), bHolyKnight = CheckHolyKnight(1);
				if (bGuard && bHolyKnight) {
					printf("你手中同时有卫兵和圣骑士，请选择触发：\n");
					printf("1.卫兵 2.圣骑士\n");
					IllustrateCard(card[33]);
					IllustrateCard(card[37]);
					char ch = _getch();
					while (ch != '1' && ch != '2') {
						ch = _getch();
					}
					if (ch == '1') {
						UseGuard(iPlayer, iTarget, iCard);
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
					UseGuard(iPlayer, iTarget, iCard);
					return true;
				} else if (bHolyKnight) {
					for (int i = 0; i < player[1].nInHand; i++) {
						if (player[1].cardInHand[i].strName == "圣骑士") {
							bool res = player[1].cardInHand[i].pfnOpt(1, i);
							if (res) break;
							else return true;
						}
					}
				}
				if (!g_bMonkMode) {
					AbandonCertainCard(iPlayer, iCard, iStatus);
					DisplayCards();
					CheckNun(iPlayer);
				}
				int nPurple = 0;
				vector<int> iPurple;
				for (int i = 0; i < player[1].nInHand; i++) {
					if (player[1].cardInHand[i].nColor == PURPLE) {
						nPurple++;
						iPurple.push_back(i);
					}
				}
				int x = iPurple[rand() % nPurple];
				PutOnField(1, x);
				DisplayCards();
				Card c = player[1].cardOnField.back();
				if (c.nArmory == DAEMON || c.nArmory == ROYAL_AND_DAEMON || c.nLevel != NO_LEVEL) {
					player[iPlayer].nNumCoin++;
					DisplayCards();
					printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
					system("pause");
					if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = iPlayer;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(iPlayer);
					if (!g_nGameStatus) return true;
					CheckMimic(iPlayer, GET_COIN);
				} else {
					printf("攻击失败！\n");
					system("pause");
				}
				CheckMinister(iTarget);
				return true;
			} else {
				bool bGuard = CheckGuard(iTarget), bHolyKnight = CheckHolyKnight(iTarget);
				if (bGuard) {
					UseGuard(iPlayer, iTarget, iCard);
					return true;
				} else if (bHolyKnight) {
					for (int i = 0; i < player[iTarget].nInHand; i++) {
						if (player[iTarget].cardInHand[i].strName == "圣骑士") {
							bool res = player[iTarget].cardInHand[i].pfnOptAI(iPlayer, i, IN_HAND);
							if (res) break;
							else return true;
						}
					}
				}
				if (!g_bMonkMode) {
					AbandonCertainCard(iPlayer, iCard, iStatus);
					DisplayCards();
					CheckNun(iPlayer);
				}
				int nPurple = 0;
				vector<int> iPurple;
				for (int i = 0; i < player[iTarget].nInHand; i++) {
					if (player[iTarget].cardInHand[i].nColor == PURPLE) {
						nPurple++;
						iPurple.push_back(i);
					}
				}
				int x = iPurple[rand() % nPurple];
				PutOnField(iTarget, x);
				DisplayCards();
				Card c = player[iTarget].cardOnField.back();
				if (c.nArmory == DAEMON || c.nLevel != NO_LEVEL) {
					player[iPlayer].nNumCoin++;
					DisplayCards();
					printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
					system("pause");
					if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = iPlayer;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(iPlayer);
					if (!g_nGameStatus) return true;
					CheckMimic(iPlayer, GET_COIN);
				} else {
					printf("攻击失败！\n");
					system("pause");
				}
				CheckMinister(iTarget);
				return true;
			}
		}
	}
	return false;
}

bool KingAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[iPlayer].nActionPoint--;
		if (!g_bMonkMode) PutOnField(iPlayer, iCard);
		else PutOnField_Monk(iPlayer, iCard);
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("放在了场上。\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		return true;
	} else if (iStatus == ON_FIELD) {
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].strName == "勇者" && cardAbandoned[i].nLevel == 4) {
				AddToHand(iPlayer, i);
				DisplayCards();
				printf("玩家 %d 横置 ", iPlayer);
				DisplayCertainCard(player[iPlayer].cardOnField[iCard]);
				printf("，将弃牌区的一张 ");
				DisplayCertainCard(card[48]);
				printf("加入了手牌。\n");
				system("pause");
				AbandonBombAI(iPlayer);
				return true;
			}
		}
		printf("玩家 %d 横置了 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField[iCard]);
		printf("，但是弃牌区没有 ");
		DisplayCertainCard(card[48]);
		printf("。\n");
		system("pause");
		return true;
	}
	return false;
}

bool SaintSwordAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[iPlayer].nActionPoint--;
		if (!g_bMonkMode) PutOnField(iPlayer, iCard);
		else PutOnField_Monk(iPlayer, iCard);
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("放在了场上。\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		return true;
	}
	return false;
}

bool BlackSmithAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if ((!g_bMonkMode && player[iPlayer].nInHand >= 3) || (g_bMonkMode && player[iPlayer].nInHand >= 2)) {
			player[iPlayer].nActionPoint--;
			printf("玩家 %d 打出了 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("。\n");
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			system("pause");
			if (!g_bMonkMode) {
				AbandonCertainCard(iPlayer, iCard, iStatus);
				DisplayCards();
				CheckNun(iPlayer);
			}
			if (!g_bMonkMode) CheckNun(iPlayer);
			for (int i = 0; i < player[iPlayer].nInHand - 1; i++) {
				for (int j = i + 1; j < player[iPlayer].nInHand; j++) {
					if (player[iPlayer].cardInHand[i].nArmory != NO_ARMORY && player[iPlayer].cardInHand[i].nArmory == player[iPlayer].cardInHand[j].nArmory) {
						PutOnField(iPlayer, i);
						PutOnField(iPlayer, j - 1);
						player[iPlayer].nNumCoin++;
						DisplayCards();
						printf("玩家 %d 放置在场上的两张牌纹章相同，+1 金币！\n", iPlayer);
						system("pause");
						if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
							g_iWinPlayer = iPlayer;
							g_nGameStatus = 0;
							return true;
						}
						CheckDancer(iPlayer);
						if (!g_nGameStatus) return true;
						CheckMimic(iPlayer, GET_COIN);
						return true;
					} else if (player[iPlayer].cardInHand[i].nLevel != NO_LEVEL && player[iPlayer].cardInHand[i].nLevel == player[iPlayer].cardInHand[j].nLevel) {
						PutOnField(iPlayer, i);
						PutOnField(iPlayer, j - 1);
						player[iPlayer].nNumCoin++;
						DisplayCards();
						printf("玩家 %d 放置在场上的两张牌 Lv 相同，+1 金币！\n", iPlayer);
						system("pause");
						if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
							g_iWinPlayer = iPlayer;
							g_nGameStatus = 0;
							return true;
						}
						CheckDancer(iPlayer);
						if (!g_nGameStatus) return true;
						CheckMimic(iPlayer, GET_COIN);
						return true;
					}
				}
			}
			PutOnField(iPlayer, 0);
			PutOnField(iPlayer, 0);
			DisplayCards();
			system("pause");
			return true;
		} else return false;
	}
	return false;
}

bool FighterAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if ((!g_bMonkMode && player[iPlayer].nInHand >= 2) || (g_bMonkMode && player[iPlayer].nInHand)) {
			player[iPlayer].nActionPoint--;
			if (!g_bMonkMode) {
				AbandonCertainCard(iPlayer, iCard, iStatus);
				DisplayCards();
			}
			printf("玩家 %d 打出了 ", iPlayer);
			DisplayCertainCard(card[11]);
			printf("。\n");
			IllustrateCard(card[11]);
			system("pause");
			if (!g_bMonkMode) CheckNun(iPlayer);
			int nSumLevel = 0;
			while (player[iPlayer].nInHand) {
				if (player[iPlayer].cardInHand[0].nLevel != NO_LEVEL) {
					nSumLevel += player[iPlayer].cardInHand[0].nLevel;
				}
				PutOnField(iPlayer, 0);
			}
			DisplayCards();
			if (nSumLevel % 2 == 0) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("玩家 %d 放置在场上的手牌 Lv 之和为偶数，+1 金币！\n", iPlayer);
				system("pause");
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
		} else return false;
	}
	return false;
}

bool MonkAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nYellow = 0;
		vector<int> iYellow;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nColor == YELLOW && cardAbandoned[i].nLevel != 1) {
				nYellow++;
				iYellow.push_back(i);
			}
		}
		if (nYellow) {
			AbandonCertainCard(iPlayer, iCard, iStatus);
			DisplayCards();
			int ch = rand() % nYellow;
			printf("玩家 %d 打出了 ", iPlayer);
			DisplayCertainCard(cardAbandoned.back());
			printf("，并选择了弃牌区的一张 ");
			DisplayCertainCard(cardAbandoned[iYellow[ch]]);
			printf(" 。\n");
			IllustrateCard(cardAbandoned.back());
			system("pause");
			int ch1 = rand() % 2;
			if (ch1) {
				if (cardAbandoned[iYellow[ch]].nType == TRIGGER) {
					player[iPlayer].nActionPoint--;
					AddToHand(iPlayer, iYellow[ch]);
					DisplayCards();
					printf("玩家 %d 将其加入了手牌。\n", iPlayer);
					system("pause");
					AbandonBombAI(iPlayer);
					return true;
				}
				player[iPlayer].nActionPoint--;
				int tmp = player[iPlayer].nActionPoint;
				g_bMonkMode = true;
				cardAbandoned[iYellow[ch]].pfnOptAI(iPlayer, iYellow[ch], IN_HAND);
				g_bMonkMode = false;
				player[iPlayer].nActionPoint = tmp;
			} else {
				player[iPlayer].nActionPoint--;
				AddToHand(iPlayer, iYellow[ch]);
				DisplayCards();
				printf("玩家 %d 将其加入了手牌。\n", iPlayer);
				system("pause");
				AbandonBombAI(iPlayer);
			}
			return true;
		} else return false;
	}
	return false;
}

bool NunAI(int iPlayer, int iCard, int iStatus) {
	return false;
}

bool BusinessmanAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if (!player[iPlayer].nOnField) return false;
		else {
			int nCoin = 0;
			vector<int> iCoin;
			for (int i = 1; i <= g_nNumPlayer; i++) {
				if (i == iPlayer) continue;
				if (player[i].nNumCoin) {
					nCoin++;
					iCoin.push_back(i);
				}
			}
			if (!nCoin) return false;
			g_iAttacker = iPlayer;
			player[iPlayer].nActionPoint--;
			int iTarget = iCoin[rand() % nCoin];
			printf("玩家 %d 打出了 ", iPlayer);
			DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			printf("，目标指定为玩家 %d ！\n", iTarget);
			IllustrateCard(player[iPlayer].cardInHand[iCard]);
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			int iCardToGive = rand() % player[iPlayer].nOnField;
			player[iPlayer].cardOnField[iCardToGive].nStatus = IN_HAND;
			player[iTarget].cardInHand.push_back(player[iPlayer].cardOnField[iCardToGive]);
			player[iTarget].nInHand++;
			player[iPlayer].cardOnField.erase(player[iPlayer].cardOnField.begin() + iCardToGive);
			player[iPlayer].nOnField--;
			player[iTarget].nNumCoin--;
			player[iPlayer].nNumCoin++;
			AbandonCertainCard(iPlayer, iCard, iStatus);
			DisplayCards();
			printf("攻击成功，玩家 %d 从玩家 %d 处获得一金币！\n", iPlayer, iTarget);
			system("pause");
			if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = 1;
				g_nGameStatus = 0;
				return true;
			}
			CheckDancer(iPlayer);
			if (!g_nGameStatus) return true;
			CheckMimic(iPlayer, GET_COIN);
			CheckMinister(iTarget);
			return true;
		}
	}
	return false;
}

bool BabyCraneAI(int iPlayer, int iCard, int iStatus) {
	return false;
}

bool DancerAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[iPlayer].nActionPoint--;
		if (!g_bMonkMode) PutOnField(iPlayer, iCard);
		else PutOnField_Monk(iPlayer, iCard);
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("放在了场上。\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		return true;
	}
	return false;
}

bool MasterAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nMaster = 0;
		vector<int> iMaster;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nInHand) {
				nMaster++;
				iMaster.push_back(i);
			}
		}
		if (!nMaster) return false;
		g_iAttacker = iPlayer;
		player[iPlayer].nActionPoint--;
		int iTarget = iMaster[rand() % nMaster];
		printf("玩家 %d 打出了 ", iPlayer);
		if (g_bMonkMode) DisplayCertainCard(cardAbandoned[iCard]);
		else DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("，目标指定为玩家 %d！\n", iTarget);
		if (g_bMonkMode) IllustrateCard(cardAbandoned[iCard]);
		else IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
		if (!g_bMonkMode) {
			AbandonCertainCard(iPlayer, iCard, iStatus);
			DisplayCards();
			CheckNun(iPlayer);
		}
		int iLv = rand() % 4 + 1;
		printf("玩家 %d 要求玩家 %d 放置一张 Lv.%d 的手牌在场上。\n", iPlayer, iTarget, iLv);
		system("pause");
		bool res = true;
		for (int i = 0; i < player[iTarget].nInHand; i++) {
			if (player[iTarget].cardInHand[i].nLevel == iLv) {
				res = false;
				break;
			}
		}
		if (!res) {
			if (iTarget == 1) {
				int num = 0;
				vector<int> iToPut;
				printf("请选择你的一张 Lv.%d 手牌放在场上：\n", iLv);
				for (int i = 0; i < player[1].nInHand; i++) {
					if (player[1].cardInHand[i].nLevel == iLv) {
						printf("%d.", ++num);
						iToPut.push_back(i);
						DisplayCertainCard(player[1].cardInHand[i]);
					}
				}
				printf("\n");
				int ch = RestrictedInput(1, num);
				PutOnField(1, iToPut[ch - 1]);
				DisplayCards();
			} else {
				for (int i = 0; i < player[iTarget].nInHand; i++) {
					if (player[iTarget].cardInHand[i].nLevel == iLv) {
						PutOnField(iTarget, i);
						DisplayCards();
						break;
					}
				}
			}
			printf("攻击失败，玩家 %d 有 Lv.%d 的手牌！\n", iTarget, iLv);
			system("pause");
			return true;
		}
		player[iPlayer].nNumCoin++;
		DisplayCards();
		printf("玩家 %d 没有 Lv.%d 的手牌，玩家 %d +1 金币！\n", iTarget, iLv, iPlayer);
		system("pause");
		if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
			g_iWinPlayer = iPlayer;
			g_nGameStatus = 0;
			return true;
		}
		CheckDancer(iPlayer);
		if (!g_nGameStatus) return true;
		CheckMimic(iPlayer, GET_COIN);
		CheckMinister(iTarget);
		return true;
	}
	return false;
}

bool GuardAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		GetOneCardFromLibrary(iPlayer);
		AbandonBombAI(iPlayer);
		return true;
	}
	return false;
}

bool HolyKnightAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int ch = rand() % 2;
		if (!ch) {
			GetOneCardFromLibrary(iPlayer);
			GetOneCardFromLibrary(iPlayer);
			AbandonCertainCard(iPlayer, iCard, iStatus);
			DisplayCards();
			printf("玩家 %d 触发 ", iPlayer);
			DisplayCertainCard(cardAbandoned.back());
			printf("，选择抽两张牌，此时攻击有效。\n");
			system("pause");
			DisplayCards();
			CheckNun(iPlayer);
			AbandonBombAI(iPlayer);
			return true;
		}
		GetOneCardFromLibrary(iPlayer);
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		printf("玩家 %d 触发 ", iPlayer);
		DisplayCertainCard(cardAbandoned.back());
		printf("，选择抽一张牌，此时攻击无效。\n");
		system("pause");
		if (g_bDragonMode) AddToHand(g_iAttacker, g_iDragonCard);
		else if (g_bMonkMode) AddToHand(g_iAttacker, g_iMonkCard);
		DisplayCards();
		CheckNun(iPlayer);
		AbandonBombAI(iPlayer);
		return false;
	}
	return false;
}

bool BerserkerAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		g_iAttacker = iPlayer;
		int nOrdinary = 0, nBlade = 0, nSaintSword = 0;
		vector<int> iOrdinary, iBlade, iSaintSword;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nInHand) {
				nBlade++;
				iBlade.push_back(i);
			}
			if (player[i].nOnField) {
				nSaintSword++;
				iSaintSword.push_back(i);
			}
			for (int j = 0; j < player[i].nInHand; j++) {
				if (player[i].cardInHand[j].nColor == PURPLE) {
					nOrdinary++;
					iOrdinary.push_back(i);
					break;
				}
			}
		}
		if (nBlade && CheckBlade(iPlayer)) {
			player[iPlayer].nActionPoint--;
			int iTarget = iBlade[rand() % nBlade];
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d ，并横置了 ", iTarget);
			DisplayCertainCard(card[55]);
			printf("！\n");
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(player[iPlayer].cardInHand[iCard]);
			IllustrateCard(card[55]);
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			if (!g_bMonkMode) PutOnField(iPlayer, iCard);
			else PutOnField_Monk(iPlayer, iCard);
			bool bYellow = false, bPurple = false, res = true;
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].nColor == YELLOW) bYellow = true;
				else bPurple = true;
			}
			if (bYellow && bPurple) {
				int x = rand() % 2;
				if (x) {
					for (int i = 0; i < player[iTarget].nInHand;) {
						if (player[iTarget].cardInHand[i].nColor == YELLOW) {
							if (player[iTarget].cardInHand[i].nLevel == NO_LEVEL) res = false;
							PutOnField(iTarget, i);
						} else i++;
					}
					DisplayCards();
					printf("玩家 %d 将玩家 %d 的所有黄色手牌放在了场上。\n", iPlayer, iTarget);
				} else {
					for (int i = 0; i < player[iTarget].nInHand;) {
						if (player[iTarget].cardInHand[i].nColor == PURPLE) {
							if (player[iTarget].cardInHand[i].nLevel == NO_LEVEL) res = false;
							PutOnField(iTarget, i);
						} else i++;
					}
					DisplayCards();
					printf("玩家 %d 将玩家 %d 的所有紫色手牌放在了场上。\n", iPlayer, iTarget);
				}
			} else {
				while (player[iTarget].nInHand) {
					if (player[iTarget].cardInHand[0].nLevel == NO_LEVEL) res = false;
					PutOnField(iTarget, 0);
				}
				DisplayCards();
				printf("玩家 %d 将玩家 %d 的所有手牌放在了场上。\n", iPlayer, iTarget);
			}
			system("pause");
			if (res) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
				if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = iPlayer;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(iPlayer);
				if (!g_nGameStatus) return true;
				CheckMimic(iPlayer, GET_COIN);
			} else printf("攻击失败。\n");
			system("pause");
			CheckMinister(iTarget);
			return true;
		} else if (nSaintSword && CheckSaintSword(iPlayer)) {
			player[iPlayer].nActionPoint--;
			int iTarget = iSaintSword[rand() % nSaintSword];
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d ，并横置了 ", iTarget);
			DisplayCertainCard(card[6]);
			printf("！\n");
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			IllustrateCard(card[6]);
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			if (!g_bMonkMode) {
				AbandonCertainCard(iPlayer, iCard, IN_HAND);
				DisplayCards();
				CheckNun(iPlayer);
			}
			for (int i = 0; i < player[iTarget].nOnField; i++) {
				if (player[iTarget].cardOnField[i].nLevel < 4 && player[iTarget].cardOnField[i].nLevel >= 0) {
					AbandonCertainCard(iTarget, i, ON_FIELD);
					DisplayCards();
					printf("玩家 %d 丢弃了玩家 %d 场上的一张 ", iPlayer, iTarget);
					DisplayCertainCard(cardAbandoned.back());
					printf("。\n");
					system("pause");
					CheckNun(iTarget);
					player[iPlayer].nNumCoin++;
					DisplayCards();
					printf("玩家 %d 丢弃的卡牌 Lv 小于 4，+1 金币！\n", iPlayer);
					system("pause");
					if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = iPlayer;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(iPlayer);
					if (!g_nGameStatus) return true;
					CheckMimic(iPlayer, GET_COIN);
					return true;
				}
			}
			int x = rand() % player[iTarget].nOnField;
			AbandonCertainCard(iTarget, x, ON_FIELD);
			DisplayCards();
			printf("玩家 %d 丢弃了玩家 %d 场上的一张 ", iPlayer, iTarget);
			DisplayCertainCard(cardAbandoned.back());
			printf("。\n");
			system("pause");
			CheckNun(iTarget);
			CheckMinister(iTarget);
			return true;
		} else {
			int iTarget = CheckPurpleCards(iPlayer);
			if (!iTarget) return false;
			player[iPlayer].nActionPoint--;
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d 。\n", iTarget);
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			if (!g_bMonkMode) {
				AbandonCertainCard(iPlayer, iCard, iStatus);
				DisplayCards();
				CheckNun(iPlayer);
			}
			bool res = true;
			for (int i = 0; i < player[iTarget].nInHand;) {
				if (player[iTarget].cardInHand[i].nColor == PURPLE) {
					if (player[iTarget].cardInHand[i].nLevel == NO_LEVEL) res = false;
					PutOnField(iTarget, i);
				} else i++;
			}
			DisplayCards();
			if (res) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
				system("pause");
				if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = iPlayer;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(iPlayer);
				if (!g_nGameStatus) return true;
				CheckMimic(iPlayer, GET_COIN);
			} else {
				printf("攻击失败！\n");
				system("pause");
			}
			CheckMinister(iTarget);
			return true;
		}
	}
	return false;
}

bool Warrior_Lv4AI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		g_iAttacker = iPlayer;
		int nOrdinary = 0, nBlade = 0, nSaintSword = 0;
		vector<int> iOrdinary, iBlade, iSaintSword;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nInHand) {
				nBlade++;
				iBlade.push_back(i);
			}
			if (player[i].nOnField) {
				nSaintSword++;
				iSaintSword.push_back(i);
			}
			for (int j = 0; j < player[i].nInHand; j++) {
				if (player[i].cardInHand[j].nColor == PURPLE) {
					nOrdinary++;
					iOrdinary.push_back(i);
					break;
				}
			}
		}
		if (nBlade && CheckBlade(iPlayer)) {
			player[iPlayer].nActionPoint--;
			int iTarget = iBlade[rand() % nBlade];
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d ，并横置了 ", iTarget);
			DisplayCertainCard(card[55]);
			printf("！\n");
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			IllustrateCard(card[55]);
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			if (!g_bMonkMode) PutOnField(iPlayer, iCard);
			else PutOnField_Monk(iPlayer, iCard);
			int nYellow = 0, nPurple = 0;
			vector<int> iYellow, iPurple;
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].nColor == YELLOW) {
					nYellow++;
					iYellow.push_back(i);
				} else {
					nPurple++;
					iPurple.push_back(i);
				}
			}
			Card c;
			int x;
			if (nYellow && nPurple) {
				int ch = rand() % 2;
				if (ch) x = iPurple[rand() % nPurple];
				else x = iYellow[rand() % nYellow];
				c = player[iTarget].cardInHand[x];
			} else {
				x = rand() % player[iTarget].nInHand;
				c = player[iTarget].cardInHand[x];
			}
			PutOnField(iTarget, x);
			DisplayCards();
			printf("玩家 %d 将玩家 %d 的一张 ", iPlayer, iTarget);
			DisplayCertainCard(c);
			printf("放在了场上。\n");
			system("pause");
			if (c.nLevel >= 0 && c.nLevel < 4) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
				system("pause");
				if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = iPlayer;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(iPlayer);
				if (!g_nGameStatus) return true;
				CheckMimic(iPlayer, GET_COIN);
			}
			CheckMinister(iTarget);
			return true;
		} else if (nSaintSword && CheckSaintSword(iPlayer)) {
			player[iPlayer].nActionPoint--;
			int iTarget = iSaintSword[rand() % nSaintSword];
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d ，并横置了 ", iTarget);
			DisplayCertainCard(card[6]);
			printf("！\n");
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			IllustrateCard(card[6]);
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			if (!g_bMonkMode) {
				AbandonCertainCard(iPlayer, iCard, IN_HAND);
				DisplayCards();
				CheckNun(iPlayer);
			}
			for (int i = 0; i < player[iTarget].nOnField; i++) {
				if (player[iTarget].cardOnField[i].nLevel < 4 && player[iTarget].cardOnField[i].nLevel >= 0) {
					AbandonCertainCard(iTarget, i, ON_FIELD);
					DisplayCards();
					printf("玩家 %d 丢弃了玩家 %d 场上的一张 ", iPlayer, iTarget);
					DisplayCertainCard(cardAbandoned.back());
					printf("。\n");
					system("pause");
					CheckNun(iTarget);
					player[iPlayer].nNumCoin++;
					DisplayCards();
					printf("玩家 %d 丢弃的卡牌 Lv 小于 4，+1 金币！\n", iPlayer);
					system("pause");
					if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = iPlayer;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(iPlayer);
					if (!g_nGameStatus) return true;
					CheckMimic(iPlayer, GET_COIN);
					CheckMinister(iTarget);
					return true;
				}
			}
			int x = rand() % player[iTarget].nOnField;
			AbandonCertainCard(iTarget, x, ON_FIELD);
			DisplayCards();
			printf("玩家 %d 丢弃了玩家 %d 场上的一张 ", iPlayer, iTarget);
			DisplayCertainCard(cardAbandoned.back());
			printf("。\n");
			system("pause");
			CheckNun(iTarget);
			CheckMinister(iTarget);
			return true;
		} else {
			int iTarget = CheckPurpleCards(iPlayer);
			if (!iTarget) return false;
			player[iPlayer].nActionPoint--;
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d 。\n", iTarget);
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			if (!g_bMonkMode) {
				AbandonCertainCard(iPlayer, iCard, iStatus);
				DisplayCards();
				CheckNun(iPlayer);
			}
			int nPurple = 0;
			vector<int> iPurple;
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].nColor == PURPLE) {
					nPurple++;
					iPurple.push_back(i);
				}
			}
			int x = iPurple[rand() % nPurple];
			Card c = player[iTarget].cardInHand[x];
			PutOnField(iTarget, x);
			DisplayCards();
			printf("玩家 %d 将玩家 %d 的一张 ", iPlayer, iTarget);
			DisplayCertainCard(c);
			printf("放在了场上。\n");
			system("pause");
			if (c.nLevel >= 0 && c.nLevel < 4) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
				system("pause");
				if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = iPlayer;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(iPlayer);
				if (!g_nGameStatus) return true;
				CheckMimic(iPlayer, GET_COIN);
			} else {
				printf("攻击失败！\n");
				system("pause");
			}
			CheckMinister(iTarget);
			return true;
		}
	}
	return false;
}

bool Warrior_Lv5AI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		g_iAttacker = iPlayer;
		int nOrdinary = 0, nBlade = 0, nSaintSword = 0;
		vector<int> iOrdinary, iBlade, iSaintSword;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nInHand) {
				nBlade++;
				iBlade.push_back(i);
			}
			if (player[i].nOnField) {
				nSaintSword++;
				iSaintSword.push_back(i);
			}
			for (int j = 0; j < player[i].nInHand; j++) {
				if (player[i].cardInHand[j].nColor == PURPLE) {
					nOrdinary++;
					iOrdinary.push_back(i);
					break;
				}
			}
		}
		if (nBlade && CheckBlade(iPlayer)) {
			player[iPlayer].nActionPoint--;
			int iTarget = iBlade[rand() % nBlade];
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d ，并横置了 ", iTarget);
			DisplayCertainCard(card[55]);
			printf("！\n");
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			IllustrateCard(card[55]);
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			if (!g_bMonkMode) PutOnField(iPlayer, iCard);
			else PutOnField_Monk(iPlayer, iCard);
			int nYellow = 0, nPurple = 0;
			vector<int> iYellow, iPurple;
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].nColor == YELLOW) {
					nYellow++;
					iYellow.push_back(i);
				} else {
					nPurple++;
					iPurple.push_back(i);
				}
			}
			Card c;
			int x;
			if (nYellow && nPurple) {
				int ch = rand() % 2;
				if (ch) x = iPurple[rand() % nPurple];
				else x = iYellow[rand() % nYellow];
				c = player[iTarget].cardInHand[x];
			} else {
				x = rand() % player[iTarget].nInHand;
				c = player[iTarget].cardInHand[x];
			}
			PutOnField(iTarget, x);
			DisplayCards();
			printf("玩家 %d 将玩家 %d 的一张 ", iPlayer, iTarget);
			DisplayCertainCard(c);
			printf("放在了场上。\n");
			system("pause");
			if (c.nLevel >= 0 && c.nLevel < 5) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
				system("pause");
				if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = iPlayer;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(iPlayer);
				if (!g_nGameStatus) return true;
				CheckMimic(iPlayer, GET_COIN);
			}
			CheckMinister(iTarget);
			return true;
		} else if (nSaintSword && CheckSaintSword(iPlayer)) {
			player[iPlayer].nActionPoint--;
			int iTarget = iSaintSword[rand() % nSaintSword];
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d ，并横置了 ", iTarget);
			DisplayCertainCard(card[6]);
			printf("！\n");
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			IllustrateCard(card[6]);
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			if (!g_bMonkMode) {
				AbandonCertainCard(iPlayer, iCard, IN_HAND);
				DisplayCards();
				CheckNun(iPlayer);
			}
			for (int i = 0; i < player[iTarget].nOnField; i++) {
				if (player[iTarget].cardOnField[i].nLevel < 5 && player[iTarget].cardOnField[i].nLevel >= 0) {
					AbandonCertainCard(iTarget, i, ON_FIELD);
					DisplayCards();
					printf("玩家 %d 丢弃了玩家 %d 场上的一张 ", iPlayer, iTarget);
					DisplayCertainCard(cardAbandoned.back());
					printf("。\n");
					system("pause");
					CheckNun(iTarget);
					player[iPlayer].nNumCoin++;
					DisplayCards();
					printf("玩家 %d 丢弃的卡牌 Lv 小于 5，+1 金币！\n", iPlayer);
					system("pause");
					if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = iPlayer;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(iPlayer);
					if (!g_nGameStatus) return true;
					CheckMimic(iPlayer, GET_COIN);
					CheckMinister(iTarget);
					return true;
				}
			}
			int x = rand() % player[iTarget].nOnField;
			AbandonCertainCard(iTarget, x, ON_FIELD);
			DisplayCards();
			printf("玩家 %d 丢弃了玩家 %d 场上的一张 ", iPlayer, iTarget);
			DisplayCertainCard(cardAbandoned.back());
			printf("。\n");
			system("pause");
			CheckNun(iTarget);
			CheckMinister(iTarget);
			return true;
		} else {
			int iTarget = CheckPurpleCards(iPlayer);
			if (!iTarget) return false;
			player[iPlayer].nActionPoint--;
			printf("玩家 %d 打出 ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("，目标指定为玩家 %d 。\n", iTarget);
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			if (!g_bMonkMode) {
				AbandonCertainCard(iPlayer, iCard, iStatus);
				DisplayCards();
				CheckNun(iPlayer);
			}
			int nPurple = 0;
			vector<int> iPurple;
			for (int i = 0; i < player[iTarget].nInHand; i++) {
				if (player[iTarget].cardInHand[i].nColor == PURPLE) {
					nPurple++;
					iPurple.push_back(i);
				}
			}
			int x = iPurple[rand() % nPurple];
			Card c = player[iTarget].cardInHand[x];
			PutOnField(iTarget, x);
			DisplayCards();
			printf("玩家 %d 将玩家 %d 的一张 ", iPlayer, iTarget);
			DisplayCertainCard(c);
			printf("放在了场上。\n");
			system("pause");
			if (c.nLevel >= 0 && c.nLevel < 5) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
				system("pause");
				if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = iPlayer;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(iPlayer);
				if (!g_nGameStatus) return true;
				CheckMimic(iPlayer, GET_COIN);
			} else {
				printf("攻击失败！\n");
				system("pause");
			}
			CheckMinister(iTarget);
			return true;
		}
	}
	return false;
}

bool BlackKnightAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nHand = 0;
		vector<int> iHand;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nInHand) {
				nHand++;
				iHand.push_back(i);
			}
		}
		if (!nHand) return false;
		g_iAttacker = iPlayer;
		int iTarget = iHand[rand() % nHand];
		player[iPlayer].nActionPoint--;
		printf("玩家 %d 打出了 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("，目标指定为玩家 %d ！\n", iTarget);
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
		AbandonCertainCard(iPlayer, iCard, IN_HAND);
		DisplayCards();
		CheckNun(iPlayer);
		int x = rand() % player[iTarget].nInHand;
		Card c = player[iTarget].cardInHand[x];
		PutOnField(iTarget, x);
		DisplayCards();
		printf("玩家 %d 将玩家 %d 手中的一张 ", iPlayer, iTarget);
		DisplayCertainCard(player[iTarget].cardOnField.back());
		printf("放在了场上。\n");
		system("pause");
		if (c.nLevel != NO_LEVEL) {
			player[iPlayer].nNumCoin++;
			DisplayCards();
			printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
			system("pause");
			if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = iPlayer;
				g_nStdCoinNumber = 0;
				return true;
			}
			CheckMimic(iPlayer, GET_COIN);
			CheckDancer(iPlayer);
			if (!g_nStdCoinNumber) return true;
		} else if (c.nArmory != NO_ARMORY) {
			int iBladeAbandoned = -1;
			for (int i = 0; i < g_nAbandoned; i++) {
				if (cardAbandoned[i].strName == "魔剑") {
					iBladeAbandoned = i;
					break;
				}
			}
			if (iBladeAbandoned != -1) {
				AddToHand(iPlayer, iBladeAbandoned);
			} else {
				card[55].nStatus = IN_HAND;
				player[iPlayer].cardInHand.push_back(card[55]);
				player[iPlayer].nInHand++;
				card[55].nStatus = IN_LIBRARY;
			}
			DisplayCards();
			if (iBladeAbandoned != -1) {
				printf("玩家 %d 将弃牌区的一张 ", iPlayer);
				DisplayCertainCard(card[55]);
				printf("加入了手牌。\n");
			} else {
				printf("玩家 %d 将牌库的一张 ", iPlayer);
				DisplayCertainCard(card[55]);
				printf("加入了手牌。\n");
			}
			system("pause");
			AbandonBomb();
		} else {
			printf("攻击失败！\n");
			system("pause");
			return true;
		}
	}
	return false;
}

bool DemonAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[iPlayer].nActionPoint--;
		PutOnField(iPlayer, iCard);
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("放在了场上。\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		return true;
	} else if (iStatus == ON_FIELD) {
		if (player[iPlayer].nOnField > 1) {
			printf("玩家 %d 横置了场上的 ", iPlayer);
			DisplayCertainCard(player[iPlayer].cardOnField[iCard]);
			printf("。\n");
			IllustrateCard(player[iPlayer].cardOnField[iCard]);
			system("pause");
			int nToRe = player[iPlayer].nOnField - 1;
			vector<int> iToRe;
			for (int i = 0; i < player[iPlayer].nOnField; i++) {
				if (i == iCard) continue;
				iToRe.push_back(i);
			}
			int ch = rand() % nToRe;
			ReturnCardOnField(iPlayer, iToRe[ch]);
			DisplayCards();
			printf("玩家 %d 将其场上的一张 ", iPlayer);
			DisplayCertainCard(player[iPlayer].cardInHand.back());
			printf("加入了手牌。\n");
			system("pause");
			return true;
		}
		return false;
	}
	return false;
}

bool FireDragonAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[iPlayer].nActionPoint--;
		if (!g_bDragonMode) PutOnField(iPlayer, iCard);
		else PutOnField_Dragon(iPlayer, iCard);
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("放在了场上。\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		return true;
	} else if (iStatus == ON_FIELD) {
		int nFireAban = 0;
		vector<int> iFireAban;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nFire == FIRE) {
				nFireAban++;
				iFireAban.push_back(i);
			}
		}
		if (!nFireAban) return false;
		int iCh = iFireAban[rand() % nFireAban];
		char cCh = '1' + rand() % 2;
		printf("玩家 %d 横置了 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField[iCard]);
		printf("，并选择了弃牌区的一张 ");
		DisplayCertainCard(cardAbandoned[iCh]);
		printf("。\n");
		IllustrateCard(player[iPlayer].cardOnField[iCard]);
		system("pause");
		if (cCh == '1') {
			g_bDragonMode = true;
			int tmp = player[iPlayer].nActionPoint;
			cardAbandoned[iCh].pfnOptAI(iPlayer, iCh, IN_HAND);
			player[iPlayer].nActionPoint = tmp;
			g_bDragonMode = false;
		} else {
			AddToHand(iPlayer, iCh);
			DisplayCards();
			printf("玩家 %d 将其加入了手牌。\n", iPlayer);
			system("pause");
		}
		return true;
	}
	return false;
}

bool TyrantAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[iPlayer].nActionPoint--;
		PutOnField(iPlayer, iCard);
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("放在了场上。\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		return true;
	}
	return false;
}

bool MimicAI(int iPlayer, int iCard, int iStatus) {
	return false;
}

bool BladeAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[iPlayer].nActionPoint--;
		PutOnField(iPlayer, iCard);
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("放在了场上。\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		return true;
	}
	return false;
}

bool DragonCubAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0, nDefender = 0;
		vector<int> iTarget, iDefender;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nOnField) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) return false;
		player[iPlayer].nActionPoint--;
		g_iAttacker = iPlayer;
		printf("玩家 %d 打出了 ", iPlayer);
		if (g_bDragonMode) DisplayCertainCard(cardAbandoned[iCard]);
		else DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("。\n");
		if (g_bDragonMode) IllustrateCard(cardAbandoned[iCard]);
		else IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		for (int i = 0; i < nTarget; i++) {
			if (CheckGuard(iTarget[i]) || CheckHolyKnight(iTarget[i])) {
				nDefender++;
				iDefender.push_back(iTarget[i]);
			}
		}
		g_iAttacker = iPlayer;
		if (nDefender == 1) {
			if (Defend(iPlayer, iDefender[0], iCard, iStatus)) return true;
		} else if (nDefender > 1) {
			printf("共 %d 名目标玩家有防牌，现随机触发。\n", nDefender);
			system("pause");
			if (Defend(iPlayer, iDefender[rand() % nDefender], iCard, iStatus)) return true;
		}
		if (!g_bDragonMode) {
			AbandonCertainCard(iPlayer, iCard, iStatus);
			DisplayCards();
			CheckNun(iPlayer);
		}
		bool res = true;
		for (int i = 0; i < nTarget; i++) {
			bool find = false;
			for (int j = 0; j < player[iTarget[i]].nOnField; j++) {
				if (player[iTarget[i]].cardOnField[j].nLevel != NO_LEVEL) {
					ReturnCardOnField(iTarget[i], j);
					find = true;
					break;
				}
			}
			if (!find) {
				ReturnCardOnField(iTarget[i], rand() % player[iTarget[i]].nOnField);
				res = false;
			}
			DisplayCards();
			printf("玩家 %d 将玩家 %d 场上的一张 ", iPlayer, iTarget[i]);
			DisplayCertainCard(player[iTarget[i]].cardInHand.back());
			printf("收回了手上。\n");
			system("pause");
			CheckMinister(iTarget[i]);
		}
		if (res) {
			player[iPlayer].nNumCoin++;
			DisplayCards();
			printf("攻击成功，玩家 %d +1 金币！\n", iPlayer);
			system("pause");
			if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = iPlayer;
				g_nGameStatus = 0;
				return true;
			}
			CheckDancer(iPlayer);
			if (!g_nGameStatus) return true;
			CheckMimic(iPlayer, GET_COIN);
		} else {
			printf("攻击失败！\n");
			system("pause");
		}
		return true;
	}
	return false;
}

bool DragonFlameAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nOnField) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) return false;
		player[iPlayer].nActionPoint--;
		g_iAttacker = iPlayer;
		int ch = rand() % nTarget;
		g_iAttacker = iPlayer;
		printf("玩家 %d 打出了 ", iPlayer);
		if (g_bDragonMode) DisplayCertainCard(cardAbandoned[iCard]);
		else DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("，目标指定为玩家 %d ！\n", iTarget[ch]);
		if (g_bDragonMode) IllustrateCard(cardAbandoned[iCard]);
		else IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		if (Defend(iPlayer, iTarget[ch], iCard, iStatus)) return true;
		if (!g_bDragonMode) {
			AbandonCertainCard(iPlayer, iCard, iStatus);
			DisplayCards();
			CheckNun(iPlayer);
		}
		int ch1 = rand() % player[iTarget[ch]].nOnField;
		AbandonCertainCard(iTarget[ch], ch1, ON_FIELD);
		Card c = cardAbandoned.back();
		DisplayCards();
		printf("玩家 %d 丢弃了玩家 %d 场上的一张 ", iPlayer, iTarget[ch]);
		DisplayCertainCard(c);
		printf("。\n");
		system("pause");
		CheckNun(iTarget[ch]);
		if (c.nLevel != NO_LEVEL) {
			player[iPlayer].nNumCoin++;
			DisplayCards();
			printf("玩家 %d 丢弃的卡牌带有 Lv，+1 金币！\n", iPlayer);
			system("pause");
			if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = iPlayer;
				g_nGameStatus = 0;
				return true;
			}
			CheckDancer(iPlayer);
			if (!g_nGameStatus) return true;
			CheckMimic(iPlayer, GET_COIN);
		}
		CheckMinister(iTarget[ch]);
		return true;
	}
	return false;
}

bool DragonEggAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[iPlayer].nActionPoint--;
		if (!g_bDragonMode) {
			AbandonCertainCard(iPlayer, iCard, iStatus);
			DisplayCards();
		}
		printf("玩家 %d 打出了 ", iPlayer);
		if (g_bDragonMode) DisplayCertainCard(cardAbandoned[iCard]);
		else DisplayCertainCard(cardAbandoned.back());
		printf("。\n");
		if (g_bDragonMode) IllustrateCard(cardAbandoned[iCard]);
		else IllustrateCard(cardAbandoned.back());
		system("pause");
		if (!g_bDragonMode) CheckNun(iPlayer);
		int nFireAban = 0;
		vector<int> iFireAban;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nFire == FIRE) {
				nFireAban++;
				iFireAban.push_back(i);
			}
		}
		if (nFireAban) {
			int ch = rand() % 2 + 1;
			if (ch == 1) {
				int ch1 = rand() % 4 + 1;
				if (ch1 == 1) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "火龙") {
							card[i].nStatus = IN_HAND;
							player[iPlayer].cardInHand.push_back(card[i]);
							player[iPlayer].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 2) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "龙炎") {
							card[i].nStatus = IN_HAND;
							player[iPlayer].cardInHand.push_back(card[i]);
							player[iPlayer].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 3) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "龙幼崽") {
							card[i].nStatus = IN_HAND;
							player[iPlayer].cardInHand.push_back(card[i]);
							player[iPlayer].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 4) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "龙蛋") {
							card[i].nStatus = IN_HAND;
							player[iPlayer].cardInHand.push_back(card[i]);
							player[iPlayer].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				}
				DisplayCards();
				printf("玩家 %d 从牌库获取了一张 ", iPlayer);
				DisplayCertainCard(player[iPlayer].cardInHand.back());
				printf("。\n");
			} else {
				int ch1 = rand() % nFireAban;
				AddToHand(iPlayer, iFireAban[ch1]);
				DisplayCards();
				printf("玩家 %d 从弃牌区获取了一张 ", iPlayer);
				DisplayCertainCard(player[iPlayer].cardInHand.back());
				printf("。\n");
			}
		} else {
			int ch1 = rand() % 4 + 1;
			if (ch1 == 1) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "火龙") {
						card[i].nStatus = IN_HAND;
						player[iPlayer].cardInHand.push_back(card[i]);
						player[iPlayer].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 2) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "龙炎") {
						card[i].nStatus = IN_HAND;
						player[iPlayer].cardInHand.push_back(card[i]);
						player[iPlayer].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 3) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "龙幼崽") {
						card[i].nStatus = IN_HAND;
						player[iPlayer].cardInHand.push_back(card[i]);
						player[iPlayer].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 4) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "龙蛋") {
						card[i].nStatus = IN_HAND;
						player[iPlayer].cardInHand.push_back(card[i]);
						player[iPlayer].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			}
			DisplayCards();
			printf("玩家 %d 从牌库获取了一张 ", iPlayer);
			DisplayCertainCard(player[iPlayer].cardInHand.back());
			printf("。\n");
		}
		system("pause");
		return true;
	}
	return false;
}

bool SlimeAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nc = 0;
		vector<int> ic;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nLevel == 1 || cardAbandoned[i].nLevel == 2) {
				nc++;
				ic.push_back(i);
			}
		}
		if (!nc) return false;
		player[iPlayer].nActionPoint--;
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		printf("玩家 %d 打出了 ", iPlayer);
		DisplayCertainCard(cardAbandoned.back());
		printf("。\n");
		IllustrateCard(cardAbandoned.back());
		system("pause");
		CheckNun(iPlayer);
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nc;
		Card c = cardAbandoned[ic[ch]];
		AddToHand(iPlayer, ic[ch]);
		DisplayCards();
		printf("玩家 %d 将弃牌区的一张 ", iPlayer);
		DisplayCertainCard(c);
		printf("加入了手牌。\n");
		system("pause");
		return true;
	}
	return false;
}

bool BombAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		player[iPlayer].nActionPoint--;
		int iTarget = rand() % g_nNumPlayer + 1;
		while (iTarget == iPlayer) iTarget = rand() % g_nNumPlayer + 1;
		player[iPlayer].cardInHand[iCard].nStatus = ON_FIELD;
		player[iTarget].cardOnField.push_back(player[iPlayer].cardInHand[iCard]);
		player[iTarget].nOnField++;
		player[iPlayer].cardInHand.erase(player[iPlayer].cardInHand.begin() + iCard);
		player[iPlayer].nInHand--;
		DisplayCards();
		printf("玩家 %d 将 ", iPlayer);
		DisplayCertainCard(player[iTarget].cardOnField.back());
		printf("放在了玩家 %d 的场上。\n", iTarget);
		IllustrateCard(player[iTarget].cardOnField.back());
		system("pause");
		CheckTyrant(iPlayer);
		return true;
	}
	return false;
}

bool BatAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (!player[i].nInHand && !player[i].nOnField) continue;
			iTarget.push_back(i);
			nTarget++;
		}
		if (!nTarget) return false;
		player[iPlayer].nActionPoint--;
		printf("玩家 %d 打出了 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("。\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nTarget;
		printf("玩家 %d 将攻击目标指定为玩家 %d 。\n", iPlayer, iTarget[ch]);
		system("pause");
		if (Defend(iPlayer, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(iPlayer, iCard, iStatus);
		while (player[iTarget[ch]].nOnField) ReturnCardOnField(iTarget[ch], 0);
		DisplayCards();
		CheckMinister(iTarget[ch]);
		return true;
	}
	return false;
}

bool BoneAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nPurple = 0;
		vector<int> iPurple;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nColor == PURPLE) {
				nPurple++;
				iPurple.push_back(i);
			}
		}
		if (!nPurple) return false;
		player[iPlayer].nActionPoint--;
		printf("玩家 %d 打出了 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("。\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nPurple;
		Card c = cardAbandoned[iPurple[ch]];
		c.nStatus = ON_FIELD;
		player[iPlayer].cardOnField.push_back(c);
		player[iPlayer].nOnField++;
		cardAbandoned.erase(cardAbandoned.begin() + iPurple[ch]);
		g_nAbandoned--;
		DisplayCards();
		printf("玩家 %d 将弃牌区的一张 ", iPlayer);
		DisplayCertainCard(c);
		printf("放在了场上。\n");
		system("pause");
		return true;
	}
	return false;
}

bool WolfmanAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nPurple = 0, nArmo = 0;
		vector<int> iPurple, iArmo;
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].nColor == PURPLE && cardAbandoned[i].nLevel != NO_LEVEL) {
				nPurple++;
				iPurple.push_back(i);
			}
			if (cardAbandoned[i].nArmory != NO_ARMORY) {
				nArmo++;
				iArmo.push_back(i);
			}
		}
		if (!nPurple && !nArmo) return false;
		player[iPlayer].nActionPoint--;
		printf("玩家 %d 打出了 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("。\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % (nPurple + nArmo);
		if (ch < nPurple) {
			Card c = cardAbandoned[iPurple[ch]];
			c.nStatus = ON_FIELD;
			player[iPlayer].cardOnField.push_back(c);
			player[iPlayer].nOnField++;
			cardAbandoned.erase(cardAbandoned.begin() + iPurple[ch]);
			g_nAbandoned--;
			DisplayCards();
			printf("玩家 %d 将弃牌区的一张 ", iPlayer);
			DisplayCertainCard(c);
			printf("放在了场上。\n");
		} else {
			AddToHand(iPlayer, iArmo[ch - nPurple]);
			DisplayCards();
			printf("玩家 %d 将弃牌区的一张 ", iPlayer);
			DisplayCertainCard(player[iPlayer].cardInHand.back());
			printf("加入了手牌。\n");
		}
		system("pause");
		return true;
	}
	return false;
}

bool HarleyQuinnAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nInHand) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) return false;
		player[iPlayer].nActionPoint--;
		printf("玩家 %d 打出了 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("！\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nTarget;
		printf("玩家 %d 将攻击目标指定为玩家 %d 。\n", iPlayer, iTarget[ch]);
		system("pause");
		if (Defend(iPlayer, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		Card c = card[rand() % (NUM_YELLOW_CARD + NUM_PURPLE_CARD) + 1];
		printf("玩家 %d 向玩家 %d 宣言了一张 ", iPlayer, iTarget[ch]);
		DisplayCertainCard(c);
		printf("。\n");
		system("pause");
		for (int i = 0; i < player[iTarget[ch]].nInHand; i++) {
			if (CompareCards(c, player[iTarget[ch]].cardInHand[i])) {
				AbandonCertainCard(iTarget[ch], i, IN_HAND);
				DisplayCards();
				printf("玩家 %d 丢弃了一张 ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("。\n");
				system("pause");
				CheckNun(iTarget[ch]);
				if (c.nLevel != NO_LEVEL) {
					player[iPlayer].nNumCoin++;
					DisplayCards();
					printf("被丢弃的卡牌带有 Lv，玩家 %d +1 金币！\n", iPlayer);
					system("pause");
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
		}
		printf("攻击失败，玩家 %d 手上没有该牌！\n", iTarget[ch]);
		system("pause");
		CheckMinister(iTarget[ch]);
		return true;
	}
	return false;
}

bool WitchAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nInHand || player[i].nOnField) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) return false;
		player[iPlayer].nActionPoint--;
		printf("玩家 %d 打出了 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("！\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nTarget;
		printf("玩家 %d 将攻击目标指定为玩家 %d 。\n", iPlayer, iTarget[ch]);
		system("pause");
		if (Defend(iPlayer, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		Card c = card[rand() % (NUM_PURPLE_CARD + NUM_YELLOW_CARD) + 1];
		printf("玩家 %d 向玩家 %d 宣言了一张 ", iPlayer, iTarget[ch]);
		DisplayCertainCard(c);
		printf("。\n");
		system("pause");
		for (int i = 0; i < player[iTarget[ch]].nInHand; i++) {
			if (CompareCards(c, player[iTarget[ch]].cardInHand[i])) {
				player[iPlayer].cardInHand.push_back(player[iTarget[ch]].cardInHand[i]);
				player[iPlayer].nInHand++;
				player[iTarget[ch]].cardInHand.erase(player[iTarget[ch]].cardInHand.begin() + i);
				player[iTarget[ch]].nInHand--;
				DisplayCards();
				printf("玩家 %d 将手中的一张 ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("交付到了玩家 %d 的手上。\n", iPlayer);
				system("pause");
				CheckMinister(iTarget[ch]);
				AbandonBombAI(iPlayer);
				return true;
			}
		}
		for (int i = 0; i < player[iTarget[ch]].nOnField; i++) {
			if (CompareCards(c, player[iTarget[ch]].cardOnField[i])) {
				player[iPlayer].cardInHand.push_back(player[iTarget[ch]].cardInHand[i]);
				player[iPlayer].nInHand++;
				player[iTarget[ch]].cardOnField.erase(player[iTarget[ch]].cardOnField.begin() + i);
				player[iTarget[ch]].nOnField--;
				DisplayCards();
				printf("玩家 %d 将场上的一张 ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("交付到了玩家 %d 的手上。\n", iPlayer);
				system("pause");
				CheckMinister(iTarget[ch]);
				AbandonBombAI(iPlayer);
				return true;
			}
		}
		printf("攻击失败，玩家 %d 没有该牌！\n", iTarget[ch]);
		system("pause");
		CheckMinister(iTarget[ch]);
		return true;
	}
	return false;
}

bool GoblinAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			if (player[i].nNumCoin > player[iPlayer].nNumCoin) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) return false;
		player[iPlayer].nActionPoint--;
		printf("玩家 %d 打出了 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("！\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nTarget;
		printf("玩家 %d 将攻击对象指定为玩家 %d 。\n", iPlayer, iTarget[ch]);
		system("pause");
		if (Defend(iPlayer, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		player[iTarget[ch]].nNumCoin--;
		player[iPlayer].nNumCoin++;
		DisplayCards();
		printf("玩家 %d 向玩家 %d 支付了 1 金币！\n", iTarget[ch], iPlayer);
		system("pause");
		if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
			g_iWinPlayer = iPlayer;
			g_nGameStatus = 0;
			return true;
		}
		CheckMimic(iPlayer, GET_COIN);
		CheckDancer(iPlayer);
		if (!g_nGameStatus) return true;
		CheckMinister(iTarget[ch]);
		return true;
	}
	return false;
}

bool TaurenAI(int iPlayer, int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		int nTarget = 0;
		vector<int> iTarget;
		for (int i = 1; i <= g_nNumPlayer; i++) {
			if (i == iPlayer) continue;
			bool bArmo = false;
			for (int j = 0; j < player[i].nOnField; j++) {
				if (player[i].cardOnField[j].nArmory != NO_ARMORY) {
					bArmo = true;
					break;
				}
			}
			if (bArmo) {
				nTarget++;
				iTarget.push_back(i);
			}
		}
		if (!nTarget) return false;
		player[iPlayer].nActionPoint--;
		printf("玩家 %d 打出了 ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("！\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nTarget;
		printf("玩家 %d 将攻击对象指定为玩家 %d 。\n", iPlayer, iTarget[ch]);
		system("pause");
		if (Defend(iPlayer, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		int ch1;
		if (iTarget[ch] == 1) {
			printf("请选择：\n");
			printf("1.支付 1 金币 2.交出场上所有的纹章牌\n");
			ch1 = RestrictedInput(1, 2);
			if (ch1 == 1 && !player[1].nNumCoin) {
				printf("你没有金币，只能交出场上的纹章牌！\n");
				ch1 = 2;
			}
		} else ch1 = rand() % 2 + 1;
		if (ch1 == 1 && !player[iTarget[ch]].nNumCoin) ch1 = 2;
		if (ch1 == 1) {
			player[iTarget[ch]].nNumCoin--;
			player[iPlayer].nNumCoin++;
			DisplayCards();
			printf("玩家 %d 向玩家 %d 支付了 1 金币！\n", iTarget[ch], iPlayer);
			system("pause");
			if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
				g_iWinPlayer = iPlayer;
				g_nGameStatus = 0;
				return true;
			}
			CheckMimic(iPlayer, GET_COIN);
			CheckDancer(iPlayer);
			if (!g_nGameStatus) return true;
			CheckMinister(iTarget[ch]);
		} else {
			for (int i = 0; i < player[iTarget[ch]].nOnField;) {
				if (player[iTarget[ch]].cardOnField[i].nArmory != NO_ARMORY) {
					Card c = player[iTarget[ch]].cardOnField[i];
					c.nStatus = IN_HAND;
					player[iPlayer].cardInHand.push_back(c);
					player[iPlayer].nInHand++;
					player[iTarget[ch]].cardOnField.erase(player[iTarget[ch]].cardOnField.begin() + i);
					player[iTarget[ch]].nOnField--;
				} else i++;
			}
			DisplayCards();
			printf("玩家 %d 将其场上的所有纹章牌交付到了玩家 %d 手上。\n", iTarget[ch], iPlayer);
			system("pause");
			AbandonBombAI(iPlayer);
		}
		return true;
	}
	return false;
}
