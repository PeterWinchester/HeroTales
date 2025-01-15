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
		printf("�㽫 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("�����˳��ϡ�\n");
		system("pause");
		player[1].nActionPoint--;
		return true;
	}
	printf("���󣺸�ʥ��״̬���������У�\n");
	system("pause");
	return false;
}

bool Minister(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		if (g_bMonkMode) PutOnField_Monk(1, iCard);
		else PutOnField(1, iCard);
		DisplayCards();
		printf("�㽫 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
		printf("�㽫 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("�����˳��ϡ�\n");
		system("pause");
		return true;
	}
	return false;
}

bool Warrior_Royal(int iCard, int iStatus) {
	if (iStatus == IN_HAND) {
		printf("��ѡ����Ĺ�������������ұ�ţ���");
		int iTarget = InputIntSafely();
		if (iTarget == 1) {
			printf("�㲻�ܹ����Լ���\n");
			system("pause");
			return false;
		} else if (iTarget >= 2 && iTarget <= g_nNumPlayer) {
			g_iAttacker = 1;
			player[1].nActionPoint--;
			/* ���������ʥ��ʿ */
			if (CheckGuard(iTarget)) {
				UseGuard(1, iTarget, iCard);
				return true;
			} else if (CheckHolyKnight(iTarget)) {
				for (int i = 0; i < player[iTarget].nInHand; i++) {
					if (player[iTarget].cardInHand[i].strName == "ʥ��ʿ") {
						bool res = player[iTarget].cardInHand[i].pfnOptAI(iTarget, i, IN_HAND);
						if (res == true) {
							/* ������Ч */
							break;
						} else {
							/* ������Ч */
							return true;
						}
					}
				}
			}
			/* ���ħ����ʥ�� */
			bool bSaintSword = false, bBlade = false;
			if (CheckSaintSword(1)) {
				printf("�㳡���� ");
				DisplayCertainCard(card[6]);
				printf("���Ƿ���ã�(y/n)\n");
				while (1) {
					char ch = _getch();
					if (ch == 'y' || ch == 'Y') {
						bSaintSword = true;
						break;
					} else if (ch == 'n' || ch == 'N') break;
				}
			}
			if (!bSaintSword && CheckBlade(1)) {
				printf("�㳡���� ");
				DisplayCertainCard(card[55]);
				printf("���Ƿ���ã�(y/n)\n");
				while (1) {
					char ch = _getch();
					if (ch == 'y' || ch == 'Y') {
						bBlade = true;
						break;
					} else if (ch == 'n' || ch == 'N') break;
				}
			}
			if (bSaintSword) {
				/* ʹ��ʥ�� */
				if (player[iTarget].nOnField == 0) {
					if (!g_bMonkMode) {
						AbandonCertainCard(1, iCard, IN_HAND);
						DisplayCards();
						CheckNun(1);
					}
					printf("����ʧ�ܣ����ֳ���û�ƣ�\n");
					system("pause");
					CheckMinister(iTarget);
					return true;
				}
				printf("��ѡ����Ҫ�����Ķ��ֳ��ϵ��ƣ�\n");
				for (int i = 0; i < player[iTarget].nOnField; i++) {
					printf("%d.", i + 1);
					DisplayCertainCard(player[iTarget].cardOnField[i]);
				}
				printf("\n");
				int ch = InputIntSafely();
				while (ch <= 0 || ch > player[iTarget].nOnField) {
					printf("��������ֳ����˷�Χ��\n");
					ch = InputIntSafely();
				}
				printf("�����������û�� Lv�������޷���ý�ҡ�\n");
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
				/* ʹ��ħ�� */
				if (g_bMonkMode) PutOnField_Monk(1, iCard);
				else PutOnField(1, iCard);
				DisplayCards();
				if (player[iTarget].nInHand == 0) {
					printf("�����û�����ƣ�����ʧ�ܣ�\n");
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
					printf("������� %d �Ż��ƺ� %d �����ƣ���ѡ��1.�� 2.��", nYellow, nPurple);
					while (1) {
						char ch = _getch();
						if (ch == '1') {
							int x = iYellow[rand() % nYellow];
							Card c = player[iTarget].cardInHand[x];
							PutOnField(iTarget, x);
							DisplayCards();
							if (c.nLevel != NO_LEVEL || c.nArmory == DAEMON || c.nArmory == ROYAL_AND_DAEMON) {
								player[1].nNumCoin++;
								printf("�����ɹ����� +1 ��ң�\n");
								if (player[1].nNumCoin == g_nStdCoinNumber) {
									g_iWinPlayer = 1;
									g_nGameStatus = 0;
									return true;
								}
								CheckDancer(1);
								if (!g_nGameStatus) return true;
								CheckMimic(1, GET_COIN);
							} else printf("����ʧ�ܣ�\n");
							break;
						} else if (ch == '2') {
							int x = iPurple[rand() % nPurple];
							Card c = player[iTarget].cardInHand[x];
							PutOnField(iTarget, x);
							DisplayCards();
							if (c.nLevel != NO_LEVEL || c.nArmory == DAEMON || c.nArmory == ROYAL_AND_DAEMON) {
								player[1].nNumCoin++;
								DisplayCards();
								printf("�����ɹ����� +1 ��ң�\n");
								if (player[1].nNumCoin == g_nStdCoinNumber) {
									g_iWinPlayer = 1;
									g_nGameStatus = 0;
									return true;
								}
								CheckDancer(1);
								if (!g_nGameStatus) return true;
								CheckMimic(1, GET_COIN);
							} else printf("����ʧ�ܣ�\n");
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
						printf("�����ɹ����� +1 ��ң�\n");
						if (player[1].nNumCoin == g_nStdCoinNumber) {
							g_iWinPlayer = 1;
							g_nGameStatus = 0;
							return true;
						}
						CheckDancer(1);
						if (!g_nGameStatus) return true;
						CheckMimic(1, GET_COIN);
					} else printf("����ʧ�ܣ�\n");
					system("pause");
					CheckMinister(iTarget);
					return true;
				}
			} else {
				/* ��ʹ�� */
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
					printf("��� %d ����û�����ƣ�����ʧ�ܣ�\n", iTarget);
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
					printf("�����ɹ����� +1 ��ң�\n");
					system("pause");
					if (player[1].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = 1;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(1);
					if (!g_nGameStatus) return true;
					CheckMimic(1, GET_COIN);
				} else printf("����ʧ�ܣ�\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			return false;
		}
		printf("��������ֳ����˷�Χ��\n");
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
		printf("�㽫 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("�����˳��ϡ�\n");
		system("pause");
		return true;
	} else if (iStatus == ON_FIELD) {
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].strName == "����" && cardAbandoned[i].nLevel == 4) {
				AddToHand(1, i);
				DisplayCards();
				printf("����� ");
				DisplayCertainCard(player[1].cardOnField[iCard]);
				printf("������������һ�� ");
				DisplayCertainCard(player[1].cardInHand.back());
				printf("���������ơ�\n");
				system("pause");
				AbandonBomb();
				return true;
			}
		}
		printf("������� ");
		DisplayCertainCard(player[1].cardOnField[iCard]);
		printf("������������û�� ");
		DisplayCertainCard(card[48]);
		printf("��\n");
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
		printf("�㽫 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
			printf("��ѡ����Ҫ���ڳ��ϵ������ƣ������������֣�һ�����ָ�һ�У���\n");
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
						printf("��ѡ��ͬ�������ƣ�\n");
						system("pause");
					} else {
						Card card1 = player[1].cardInHand[ch1 - 1], card2 = player[1].cardInHand[ch2 - 1];
						PutOnField(1, min(ch1, ch2) - 1);
						PutOnField(1, max(ch1, ch2) - 2);
						if (card1.nArmory != NO_ARMORY && card2.nArmory == card2.nArmory) {
							player[1].nNumCoin++;
							DisplayCards();
							printf("��ŵ�������������ͬ���� +1 ��ң�\n");
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
							printf("��ŵ������� Lv ��ͬ���� +1 ��ң�\n");
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
					printf("��������ֳ����˷�Χ��\n");
					system("pause");
				}
			}
		} else {
			printf("������Ʋ��㣬����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
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
				printf("������ڳ��ϵ����� Lv ֮��Ϊż����+1 ��ң�\n");
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
			printf("������Ʋ��㣡\n");
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
			printf("��ѡ����������һ�� Lv.1 ����Ļ�ɫ���ƣ�\n");
			for (int i = 0; i < nYellow; i++) {
				printf("%d.", i + 1);
				DisplayCertainCard(cardAbandoned[iYellow[i]]);
			}
			printf("\n");
			int ch;
			while (1) {
				ch = InputIntSafely();
				if (ch >= 1 && ch <= nYellow) break;
				else printf("��������ֳ����˷�Χ��\n");
			}
			printf("��ѡ���� ");
			DisplayCertainCard(cardAbandoned[iYellow[ch - 1]]);
			printf("��\n");
			printf("��ѡ����Ҫ������еĲ�����\n");
			printf("1.�������ж����� 2.��������\n");
			char ch1 = _getch();
			while (ch1 != '1' && ch1 != '2') ch1 = _getch();
			if (ch1 == '1') {
				if (cardAbandoned[iYellow[ch - 1]].nType == TRIGGER) {
					printf("�ÿ���Ϊ�����ƣ��������������\n");
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
			printf("������û�� Lv.1 ����Ļ�ɫ���ƣ�\n");
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
			printf("�㳡��û�ƣ�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
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
				printf("�������û�н�ң�����ʹ�� ");
				DisplayCertainCard(player[1].cardInHand[iCard]);
				printf("��\n");
				system("pause");
				return false;
			}
			g_iAttacker = 1;
			player[1].nActionPoint--;
			printf("��ѡ����Ĺ�������");
			for (int i = 0; i < nCoin; i++) printf("%d.���%d ", i + 1, iCoin[i]);
			printf("\n");
			int iTarget = iCoin[RestrictedInput(1, nCoin) - 1];
			if (Defend(1, iTarget, iCard, iStatus)) return true;
			printf("��ѡ���㳡�ϵ�һ���ƽ�������� %d ��\n", iTarget);
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
			printf("�����ɹ��������� %d ����� 1 ��ң�\n", iTarget);
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
		printf("�㽫 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
			printf("������Ҷ�û�����ƣ�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
			system("pause");
			return false;
		}
		g_iAttacker = 1;
		player[1].nActionPoint--;
		printf("��ָ����Ĺ�������\n");
		for (int i = 0; i < nMaster; i++) printf("%d.���%d ", i + 1, iMaster[i]);
		printf("\n");
		int iTarget = iMaster[RestrictedInput(1, nMaster) - 1];
		if (Defend(1, iTarget, iCard, iStatus)) return true;
		if (!g_bMonkMode) {
			AbandonCertainCard(1, iCard, iStatus);
			DisplayCards();
			CheckNun(1);
		}
		printf("��������Ҫ����õ����� Lv��1/2/3/4����");
		int iLv = RestrictedInput(1, 4);
		for (int i = 0; i < player[iTarget].nInHand; i++) {
			if (player[iTarget].cardInHand[i].nLevel == iLv) {
				PutOnField(iTarget, i);
				DisplayCards();
				printf("̫�ź��ˣ���� %d �� Lv.%d �����ƣ�\n", iTarget, iLv);
				system("pause");
				return true;
			}
		}
		player[1].nNumCoin++;
		DisplayCards();
		printf("��� %d û�� Lv.%d �����ƣ��� +1 ��ң�\n", iTarget, iLv);
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
		printf("������ʹ�� ");
		DisplayCertainCard(player[1].cardInHand[iCard]);
		printf("��\n");
		printf("��ѡ��ʹ�÷�����\n");
		printf("1.������Ч�����������\n");
		printf("2.������Ч�����һ���ơ�\n");
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
		printf("��ѡ����Ĺ�������������ұ�ţ���");
		int iTarget = RestrictedInput(2, g_nNumPlayer);
		player[1].nActionPoint--;
		if (Defend(1, iTarget, iCard, iStatus)) return true;
		bool bSaintSword = false, bBlade = false;
		if (CheckSaintSword(1)) {
			printf("�㳡���� ");
			DisplayCertainCard(card[6]);
			printf("���Ƿ���ã�(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bSaintSword = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (!bSaintSword && CheckBlade(1)) {
			printf("�㳡���� ");
			DisplayCertainCard(card[55]);
			printf("���Ƿ���ã�(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bBlade = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (bSaintSword) {
			/* ʹ��ʥ�� */
			if (player[iTarget].nOnField == 0) {
				if (!g_bMonkMode) {
					AbandonCertainCard(1, iCard, IN_HAND);
					DisplayCards();
					CheckNun(1);
				}
				printf("����ʧ�ܣ����ֳ���û�ƣ�\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, IN_HAND);
				DisplayCards();
				CheckNun(1);
			}
			printf("��ѡ����Ҫ�����Ķ��ֳ��ϵ��ƣ�\n");
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
				printf("�㶪���Ŀ��� Lv �����������ߵͣ��� +1 ��ң�\n");
				system("pause");
				if (player[1].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = 1;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(1);
				if (!g_nGameStatus) return true;
				CheckMimic(1, GET_COIN);
			} else printf("����ʧ�ܣ�\n");
			system("pause");
			CheckMinister(iTarget);
			return true;
		} else if (bBlade) {
			if (g_bMonkMode) PutOnField_Monk(1, iCard);
			else PutOnField(1, iCard);
			DisplayCards();
			if (player[iTarget].nInHand == 0) {
				printf("�����û�����ƣ�����ʧ�ܣ�\n");
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
				printf("���������ͬʱӵ�л�ɫ����ɫ���ƣ���ѡ��1.�� 2.��\n");
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
							printf("�����ɹ����� +1 ��ң�\n");
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
							printf("����ʧ�ܣ�\n");
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
							printf("�����ɹ����� +1 ��ң�\n");
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
							printf("����ʧ�ܣ�\n");
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
					printf("�����ɹ����� +1 ��ң�\n");
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
					printf("����ʧ�ܣ�\n");
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
				printf("���������û�����ƣ�����ʧ�ܣ�\n");
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
				printf("�����ɹ����� +1 ��ң�\n");
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
				printf("����ʧ�ܣ�\n");
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
		printf("��ѡ����Ĺ�������������ұ�ţ���");
		int iTarget = RestrictedInput(2, g_nNumPlayer);
		player[1].nActionPoint--;
		if (Defend(1, iTarget, iCard, iStatus)) return true;
		bool bSaintSword = false, bBlade = false;
		if (CheckSaintSword(1)) {
			printf("�㳡���� ");
			DisplayCertainCard(card[6]);
			printf("���Ƿ���ã�(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bSaintSword = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (!bSaintSword && CheckBlade(1)) {
			printf("�㳡���� ");
			DisplayCertainCard(card[55]);
			printf("���Ƿ���ã�(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bBlade = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (bSaintSword) {
			/* ʹ��ʥ�� */
			if (player[iTarget].nOnField == 0) {
				if (!g_bMonkMode) {
					AbandonCertainCard(1, iCard, IN_HAND);
					DisplayCards();
					CheckNun(1);
				}
				printf("����ʧ�ܣ����ֳ���û�ƣ�\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, IN_HAND);
				DisplayCards();
				CheckNun(1);
			}
			printf("��ѡ����Ҫ�����Ķ��ֳ��ϵ��ƣ�\n");
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
				printf("�㶪���Ŀ��� Lv �����������ߵͣ��� +1 ��ң�\n");
				system("pause");
				if (player[1].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = 1;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(1);
				if (!g_nGameStatus) return true;
				CheckMimic(1, GET_COIN);
			} else printf("����ʧ�ܣ�\n");
			system("pause");
			CheckMinister(iTarget);
			return true;
		} else if (bBlade) {
			if (g_bMonkMode) PutOnField_Monk(1, iCard);
			else PutOnField(1, iCard);
			DisplayCards();
			if (player[iTarget].nInHand == 0) {
				printf("�����û�����ƣ�����ʧ�ܣ�\n");
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
				printf("������� %d �Ż��ƺ� %d �����ƣ���ѡ��1.�� 2.��", nYellow, nPurple);
				while (1) {
					char ch = _getch();
					if (ch == '1') {
						int x = iYellow[rand() % nYellow];
						Card c = player[iTarget].cardInHand[x];
						PutOnField(iTarget, x);
						DisplayCards();
						if (c.nLevel != NO_LEVEL && c.nLevel < 4) {
							player[1].nNumCoin++;
							printf("�����ɹ����� +1 ��ң�\n");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
						} else printf("����ʧ�ܣ�\n");
						break;
					} else if (ch == '2') {
						int x = iPurple[rand() % nPurple];
						Card c = player[iTarget].cardInHand[x];
						PutOnField(iTarget, x);
						DisplayCards();
						if (c.nLevel != NO_LEVEL && c.nLevel < 4) {
							player[1].nNumCoin++;
							DisplayCards();
							printf("�����ɹ����� +1 ��ң�\n");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
						} else printf("����ʧ�ܣ�\n");
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
					printf("�����ɹ����� +1 ��ң�\n");
					if (player[1].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = 1;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(1);
					if (!g_nGameStatus) return true;
					CheckMimic(1, GET_COIN);
				} else printf("����ʧ�ܣ�\n");
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
				printf("���������û�����ƣ�����ʧ�ܣ�\n");
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
				printf("�����ɹ����� +1 ��ң�\n");
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
				printf("����ʧ�ܣ�\n");
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
		printf("��ѡ����Ĺ�������������ұ�ţ���");
		int iTarget = RestrictedInput(2, g_nNumPlayer);
		player[1].nActionPoint--;
		if (Defend(1, iTarget, iCard, iStatus)) return true;
		bool bSaintSword = false, bBlade = false;
		if (CheckSaintSword(1)) {
			printf("�㳡���� ");
			DisplayCertainCard(card[6]);
			printf("���Ƿ���ã�(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bSaintSword = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (!bSaintSword && CheckBlade(1)) {
			printf("�㳡���� ");
			DisplayCertainCard(card[55]);
			printf("���Ƿ���ã�(y/n)\n");
			while (1) {
				char ch = _getch();
				if (ch == 'y' || ch == 'Y') {
					bBlade = true;
					break;
				} else if (ch == 'n' || ch == 'N') break;
			}
		}
		if (bSaintSword) {
			/* ʹ��ʥ�� */
			if (player[iTarget].nOnField == 0) {
				if (!g_bMonkMode) {
					AbandonCertainCard(1, iCard, IN_HAND);
					DisplayCards();
					CheckNun(1);
				}
				printf("����ʧ�ܣ����ֳ���û�ƣ�\n");
				system("pause");
				CheckMinister(iTarget);
				return true;
			}
			if (!g_bMonkMode) {
				AbandonCertainCard(1, iCard, IN_HAND);
				DisplayCards();
				CheckNun(1);
			}
			printf("��ѡ����Ҫ�����Ķ��ֳ��ϵ��ƣ�\n");
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
				printf("�㶪���Ŀ��� Lv �����������ߵͣ��� +1 ��ң�\n");
				system("pause");
				if (player[1].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = 1;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(1);
				if (!g_nGameStatus) return true;
				CheckMimic(1, GET_COIN);
			} else printf("����ʧ�ܣ�\n");
			system("pause");
			CheckMinister(iTarget);
			return true;
		} else if (bBlade) {
			if (g_bMonkMode) PutOnField_Monk(1, iCard);
			else PutOnField(1, iCard);
			DisplayCards();
			if (player[iTarget].nInHand == 0) {
				printf("�����û�����ƣ�����ʧ�ܣ�\n");
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
				printf("������� %d �Ż��ƺ� %d �����ƣ���ѡ��1.�� 2.��", nYellow, nPurple);
				while (1) {
					char ch = _getch();
					if (ch == '1') {
						int x = iYellow[rand() % nYellow];
						Card c = player[iTarget].cardInHand[x];
						PutOnField(iTarget, x);
						DisplayCards();
						if (c.nLevel != NO_LEVEL && c.nLevel < 5) {
							player[1].nNumCoin++;
							printf("�����ɹ����� +1 ��ң�\n");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
						} else printf("����ʧ�ܣ�\n");
						break;
					} else if (ch == '2') {
						int x = iPurple[rand() % nPurple];
						Card c = player[iTarget].cardInHand[x];
						PutOnField(iTarget, x);
						DisplayCards();
						if (c.nLevel != NO_LEVEL && c.nLevel < 5) {
							player[1].nNumCoin++;
							DisplayCards();
							printf("�����ɹ����� +1 ��ң�\n");
							if (player[1].nNumCoin == g_nStdCoinNumber) {
								g_iWinPlayer = 1;
								g_nGameStatus = 0;
								return true;
							}
							CheckDancer(1);
							if (!g_nGameStatus) return true;
							CheckMimic(1, GET_COIN);
						} else printf("����ʧ�ܣ�\n");
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
					printf("�����ɹ����� +1 ��ң�\n");
					if (player[1].nNumCoin == g_nStdCoinNumber) {
						g_iWinPlayer = 1;
						g_nGameStatus = 0;
						return true;
					}
					CheckDancer(1);
					if (!g_nGameStatus) return true;
					CheckMimic(1, GET_COIN);
				} else printf("����ʧ�ܣ�\n");
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
				printf("���������û�����ƣ�����ʧ�ܣ�\n");
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
				printf("�����ɹ����� +1 ��ң�\n");
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
				printf("����ʧ�ܣ�\n");
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
		printf("��ѡ����Ĺ�������");
		int iTarget = RestrictedInput(2, g_nNumPlayer);
		if (!player[iTarget].nInHand) {
			printf("�����û�����ƣ�\n");
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
			printf("�����ɹ����� +1 ��ң�\n");
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
				if (cardAbandoned[i].strName == "ħ��") {
					iBladeAbandoned = i;
					break;
				}
			}
			if (iBladeAbandoned != -1) {
				printf("�ƿ������������ ");
				DisplayCertainCard(card[55]);
				printf("�������������ȡ��\n");
				printf("1.�ƿ� 2.������\n");
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
			printf("�㽫 ");
			DisplayCertainCard(card[55]);
			printf("���������ơ�\n");
			system("pause");
			AbandonBomb();
		} else {
			printf("����ʧ�ܣ�\n");
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
		printf("�㽫 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
			printf("��ѡ���㳡�ϵ�һ���Ƽ������ƣ�\n");
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
			printf("�㳡��û�������ƣ�\n");
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
		printf("�㽫 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
			printf("������û��[��]��\n");
			system("pause");
			return false;
		}
		printf("��ѡ����������һ��[��]��\n");
		for (int i = 0; i < nFireAban; i++) {
			printf("%d.", i + 1);
			DisplayCertainCard(cardAbandoned[iFireAban[i]]);
		}
		printf("\n");
		int iCh = iFireAban[RestrictedInput(1, nFireAban) - 1];
		printf("��ѡ����Ҫ������еĲ�����\n");
		printf("1.�������ж����� 2.��������\n");
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
		printf("�㽫 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
		printf("�㽫 ");
		DisplayCertainCard(player[1].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
			printf("������ҳ��϶�û���ƣ�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
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
			printf("�� %d ��Ŀ������з��ƣ������������\n", nDefender);
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
			printf("��ѡ����� %d ���ϵ�һ���ƻص����ƣ�\n", iTarget[i]);
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
			printf("�����ɹ����� +1 ��ң�\n");
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
			printf("������ҳ��϶�û���ƣ�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		g_iAttacker = 1;
		printf("��ѡ����Ĺ�������\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.���%d ", i + 1, iTarget[i]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nTarget) - 1;
		if (Defend(1, ch, iCard, iStatus)) return true;
		if (!g_bDragonMode) {
			AbandonCertainCard(1, iCard, iStatus);
			DisplayCards();
			CheckNun(1);
		}
		printf("��ѡ����Ҫ�����Ķ��ֳ��ϵ��ƣ�\n");
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
			printf("�㶪���Ŀ��ƴ��� Lv��+1 ��ң�\n");
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
			printf("�ƿ������������[��]�������������ȡ��\n");
			printf("1.�ƿ� 2.������\n");
			int ch = RestrictedInput(1, 2);
			if (ch == 1) {
				printf("��ѡ������Ҫ��[��]��\n");
				printf("1.���� 2.���� 3.������ 4.����\n");
				int ch1 = RestrictedInput(1, 4);
				if (ch1 == 1) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "����") {
							card[i].nStatus = IN_HAND;
							player[1].cardInHand.push_back(card[i]);
							player[1].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 2) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "����") {
							card[i].nStatus = IN_HAND;
							player[1].cardInHand.push_back(card[i]);
							player[1].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 3) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "������") {
							card[i].nStatus = IN_HAND;
							player[1].cardInHand.push_back(card[i]);
							player[1].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 4) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "����") {
							card[i].nStatus = IN_HAND;
							player[1].cardInHand.push_back(card[i]);
							player[1].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				}
			} else {
				printf("��ѡ����������һ��[��]��\n");
				for (int i = 0; i < nFireAban; i++) {
					printf("%d.", i + 1);
					DisplayCertainCard(cardAbandoned[iFireAban[i]]);
				}
				printf("\n");
				int ch1 = RestrictedInput(1, nFireAban) - 1;
				AddToHand(1, iFireAban[ch1]);
			}
		} else {
			printf("ֻ���ƿ���[��]������ƿ�ѡ��\n");
			printf("1.���� 2.���� 3.������ 4.����\n");
			int ch1 = RestrictedInput(1, 4);
			if (ch1 == 1) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "����") {
						card[i].nStatus = IN_HAND;
						player[1].cardInHand.push_back(card[i]);
						player[1].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 2) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "����") {
						card[i].nStatus = IN_HAND;
						player[1].cardInHand.push_back(card[i]);
						player[1].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 3) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "������") {
						card[i].nStatus = IN_HAND;
						player[1].cardInHand.push_back(card[i]);
						player[1].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 4) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "����") {
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
			printf("������û�� Lv.1 �� Lv.2 ���ƣ�\n");
			system("pause");
			return true;
		}
		player[1].nActionPoint--;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		printf("��ѡ��һ���Ƽ������У�\n");
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
		printf("��ѡ����Ҫ���䳡�Ϸ��� ");
		DisplayCertainCard(player[1].cardInHand[iCard]);
		printf("����ң�������ұ�ţ���\n");
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
			printf("������Ҷ�û���ƣ�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		g_iAttacker = 1;
		printf("��ѡ����Ĺ�������\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.���%d ", i + 1, iTarget[i]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nTarget) - 1;
		if (Defend(1, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		if (player[iTarget[ch]].nInHand) {
			printf("��Щ����� %d �����ƣ�\n", iTarget[ch]);
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
			printf("������û����ɫ���ƣ�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		printf("��ѡ����������һ����ɫ���Ʒ��ڳ��ϣ�\n");
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
			printf("��������û�д� Lv �����ƣ�Ҳû�д����µ��ƣ�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		printf("��ѡ����������һ���ƣ�\n");
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
			printf("�㽫��������һ�� ");
			DisplayCertainCard(c);
			printf("�����˳��ϡ�\n");
		} else {
			AddToHand(1, iArmo[ch - nPurple]);
			DisplayCards();
			printf("�㽫��������һ�� ");
			DisplayCertainCard(player[1].cardInHand.back());
			printf("���������ơ�\n");
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
			printf("������Ҷ�û�����ƣ�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		g_iAttacker = 1;
		printf("��ѡ����Ĺ�������\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.���%d ", i + 1, iTarget[i]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nTarget) - 1;
		if (Defend(1, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		printf("��ѡ����Ҫ���ԵĿ��ƣ�\n");
		printf("1.���� 2.����\n");
		char ch1 = _getch();
		while (ch1 != '1' && ch1 != '2') ch1 = _getch();
		printf("��ѡ����Ҫ���ԵĿ��ƣ�\n");
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
				printf("��� %d ������һ�� ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("��\n");
				system("pause");
				CheckNun(iTarget[ch]);
				if (c.nLevel != NO_LEVEL) {
					player[1].nNumCoin++;
					DisplayCards();
					printf("�������Ŀ��ƴ��� Lv���� +1 ��ң�\n");
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
		printf("����ʧ�ܣ���� %d ����û�и��ƣ�\n", iTarget[ch]);
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
			printf("������Ҷ�û���ƣ�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		g_iAttacker = 1;
		printf("��ѡ����Ĺ�������\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.���%d ", i + 1, iTarget[i]);
		}
		printf("\n");
		int ch = RestrictedInput(1, nTarget) - 1;
		if (Defend(1, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(1, iCard, iStatus);
		DisplayCards();
		CheckNun(1);
		printf("��ѡ����Ҫ���ԵĿ��ƣ�\n");
		printf("1.���� 2.����\n");
		char ch1 = _getch();
		while (ch1 != '1' && ch1 != '2') ch1 = _getch();
		printf("��ѡ����Ҫ���ԵĿ��ƣ�\n");
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
				printf("��� %d �����е�һ�� ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("��������������ϡ�\n");
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
				printf("��� %d �����ϵ�һ�� ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("��������������ϡ�\n");
				system("pause");
				AbandonBomb();
				CheckMinister(iTarget[ch]);
				return true;
			}
		}
		printf("����ʧ�ܣ���� %d û�и��ƣ�\n", iTarget[ch]);
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
			printf("û�б���Ǯ�����ң�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		g_iAttacker = 1;
		printf("��ѡ����Ĺ�������\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.���%d ", i + 1, iTarget[i]);
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
		printf("��� %d ����֧���� 1 ��ң�\n", iTarget[ch]);
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
			printf("������ҳ���û�������ƣ�����ʹ�� ");
			DisplayCertainCard(player[1].cardInHand[iCard]);
			printf("��\n");
			system("pause");
			return false;
		}
		player[1].nActionPoint--;
		CheckTyrant(1);
		if (!g_nGameStatus) return true;
		g_iAttacker = 1;
		printf("��ѡ����Ĺ�������\n");
		for (int i = 0; i < nTarget; i++) {
			printf("%d.���%d ", i + 1, iTarget[i]);
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
			printf("��� %d ����֧���� 1 ��ң�\n", iTarget[ch]);
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
			printf("��� %d ���䳡�ϵ����������ƽ������������ϡ�\n", iTarget[ch]);
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ���������� ", iTarget);
			DisplayCertainCard(card[55]);
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			IllustrateCard(card[55]);
			printf("��\n");
			system("pause");
			if (Defend(iPlayer, iTarget, iCard, iStatus)) return true;
			int x = rand() % player[iTarget].nInHand;
			Card c = player[iTarget].cardInHand[x];
			PutOnField(iTarget, x);
			DisplayCards();
			if (c.nLevel != NO_LEVEL || c.nArmory == DAEMON || c.nArmory == ROYAL_AND_DAEMON) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
				if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = iPlayer;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(iPlayer);
				if (!g_nGameStatus) return true;
				CheckMimic(iPlayer, GET_COIN);
			} else printf("����ʧ�ܡ�\n");
			system("pause");
			CheckMinister(iTarget);
			return true;
		} else if (nSaintSword && CheckSaintSword(iPlayer)) {
			player[iPlayer].nActionPoint--;
			int iTarget = iSaintSword[rand() % nSaintSword];
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ���������� ", iTarget);
			DisplayCertainCard(card[6]);
			printf("��\n");
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			IllustrateCard(card[6]);
			system("pause");
			if (iTarget == 1) {
				bool bGuard = CheckGuard(1), bHolyKnight = CheckHolyKnight(1);
				if (bGuard && bHolyKnight) {
					printf("������ͬʱ��������ʥ��ʿ����ѡ�񴥷���\n");
					printf("1.���� 2.ʥ��ʿ\n");
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
							if (player[1].cardInHand[i].strName == "ʥ��ʿ") {
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
						if (player[1].cardInHand[i].strName == "ʥ��ʿ") {
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
				printf("��� %d �������㳡�ϵ�һ�� ", iPlayer);
				DisplayCertainCard(cardAbandoned.back());
				printf(" ��\n");
				printf("������� %d ���������û�� Lv�����ӹ�һ�١�\n", iPlayer);
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
						if (player[iTarget].cardInHand[i].strName == "ʥ��ʿ") {
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
				printf("��� %d ��������� %d ���ϵ�һ�� ", iPlayer, iTarget);
				DisplayCertainCard(cardAbandoned.back());
				printf(" ��\n");
				printf("������� %d ���������û�� Lv����� %d �ӹ�һ�١�\n", iPlayer, iTarget);
				system("pause");
				return true;
			}
			CheckMinister(iTarget);
		} else {
			player[iPlayer].nActionPoint--;
			int iTarget = iOrdinary[rand() % nOrdinary];
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ��\n", iTarget);
			if (!g_bMonkMode) IllustrateCard(player[iPlayer].cardInHand[iCard]);
			else IllustrateCard(cardAbandoned[iCard]);
			system("pause");
			if (iTarget == 1) {
				bool bGuard = CheckGuard(1), bHolyKnight = CheckHolyKnight(1);
				if (bGuard && bHolyKnight) {
					printf("������ͬʱ��������ʥ��ʿ����ѡ�񴥷���\n");
					printf("1.���� 2.ʥ��ʿ\n");
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
							if (player[1].cardInHand[i].strName == "ʥ��ʿ") {
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
						if (player[1].cardInHand[i].strName == "ʥ��ʿ") {
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
					printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
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
					printf("����ʧ�ܣ�\n");
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
						if (player[iTarget].cardInHand[i].strName == "ʥ��ʿ") {
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
					printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
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
					printf("����ʧ�ܣ�\n");
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("�����˳��ϡ�\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		return true;
	} else if (iStatus == ON_FIELD) {
		for (int i = 0; i < g_nAbandoned; i++) {
			if (cardAbandoned[i].strName == "����" && cardAbandoned[i].nLevel == 4) {
				AddToHand(iPlayer, i);
				DisplayCards();
				printf("��� %d ���� ", iPlayer);
				DisplayCertainCard(player[iPlayer].cardOnField[iCard]);
				printf("������������һ�� ");
				DisplayCertainCard(card[48]);
				printf("���������ơ�\n");
				system("pause");
				AbandonBombAI(iPlayer);
				return true;
			}
		}
		printf("��� %d ������ ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField[iCard]);
		printf("������������û�� ");
		DisplayCertainCard(card[48]);
		printf("��\n");
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
			printf("��� %d ����� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��\n");
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
						printf("��� %d �����ڳ��ϵ�������������ͬ��+1 ��ң�\n", iPlayer);
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
						printf("��� %d �����ڳ��ϵ������� Lv ��ͬ��+1 ��ң�\n", iPlayer);
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
			printf("��� %d ����� ", iPlayer);
			DisplayCertainCard(card[11]);
			printf("��\n");
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
				printf("��� %d �����ڳ��ϵ����� Lv ֮��Ϊż����+1 ��ң�\n", iPlayer);
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
			printf("��� %d ����� ", iPlayer);
			DisplayCertainCard(cardAbandoned.back());
			printf("����ѡ������������һ�� ");
			DisplayCertainCard(cardAbandoned[iYellow[ch]]);
			printf(" ��\n");
			IllustrateCard(cardAbandoned.back());
			system("pause");
			int ch1 = rand() % 2;
			if (ch1) {
				if (cardAbandoned[iYellow[ch]].nType == TRIGGER) {
					player[iPlayer].nActionPoint--;
					AddToHand(iPlayer, iYellow[ch]);
					DisplayCards();
					printf("��� %d ������������ơ�\n", iPlayer);
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
				printf("��� %d ������������ơ�\n", iPlayer);
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
			printf("��� %d ����� ", iPlayer);
			DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ��\n", iTarget);
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
			printf("�����ɹ������ %d ����� %d �����һ��ң�\n", iPlayer, iTarget);
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
		printf("��� %d ����� ", iPlayer);
		if (g_bMonkMode) DisplayCertainCard(cardAbandoned[iCard]);
		else DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��Ŀ��ָ��Ϊ��� %d��\n", iTarget);
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
		printf("��� %d Ҫ����� %d ����һ�� Lv.%d �������ڳ��ϡ�\n", iPlayer, iTarget, iLv);
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
				printf("��ѡ�����һ�� Lv.%d ���Ʒ��ڳ��ϣ�\n", iLv);
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
			printf("����ʧ�ܣ���� %d �� Lv.%d �����ƣ�\n", iTarget, iLv);
			system("pause");
			return true;
		}
		player[iPlayer].nNumCoin++;
		DisplayCards();
		printf("��� %d û�� Lv.%d �����ƣ���� %d +1 ��ң�\n", iTarget, iLv, iPlayer);
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
			printf("��� %d ���� ", iPlayer);
			DisplayCertainCard(cardAbandoned.back());
			printf("��ѡ��������ƣ���ʱ������Ч��\n");
			system("pause");
			DisplayCards();
			CheckNun(iPlayer);
			AbandonBombAI(iPlayer);
			return true;
		}
		GetOneCardFromLibrary(iPlayer);
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		printf("��� %d ���� ", iPlayer);
		DisplayCertainCard(cardAbandoned.back());
		printf("��ѡ���һ���ƣ���ʱ������Ч��\n");
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
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ���������� ", iTarget);
			DisplayCertainCard(card[55]);
			printf("��\n");
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
					printf("��� %d ����� %d �����л�ɫ���Ʒ����˳��ϡ�\n", iPlayer, iTarget);
				} else {
					for (int i = 0; i < player[iTarget].nInHand;) {
						if (player[iTarget].cardInHand[i].nColor == PURPLE) {
							if (player[iTarget].cardInHand[i].nLevel == NO_LEVEL) res = false;
							PutOnField(iTarget, i);
						} else i++;
					}
					DisplayCards();
					printf("��� %d ����� %d ��������ɫ���Ʒ����˳��ϡ�\n", iPlayer, iTarget);
				}
			} else {
				while (player[iTarget].nInHand) {
					if (player[iTarget].cardInHand[0].nLevel == NO_LEVEL) res = false;
					PutOnField(iTarget, 0);
				}
				DisplayCards();
				printf("��� %d ����� %d ���������Ʒ����˳��ϡ�\n", iPlayer, iTarget);
			}
			system("pause");
			if (res) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
				if (player[iPlayer].nNumCoin == g_nStdCoinNumber) {
					g_iWinPlayer = iPlayer;
					g_nGameStatus = 0;
					return true;
				}
				CheckDancer(iPlayer);
				if (!g_nGameStatus) return true;
				CheckMimic(iPlayer, GET_COIN);
			} else printf("����ʧ�ܡ�\n");
			system("pause");
			CheckMinister(iTarget);
			return true;
		} else if (nSaintSword && CheckSaintSword(iPlayer)) {
			player[iPlayer].nActionPoint--;
			int iTarget = iSaintSword[rand() % nSaintSword];
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ���������� ", iTarget);
			DisplayCertainCard(card[6]);
			printf("��\n");
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
					printf("��� %d ��������� %d ���ϵ�һ�� ", iPlayer, iTarget);
					DisplayCertainCard(cardAbandoned.back());
					printf("��\n");
					system("pause");
					CheckNun(iTarget);
					player[iPlayer].nNumCoin++;
					DisplayCards();
					printf("��� %d �����Ŀ��� Lv С�� 4��+1 ��ң�\n", iPlayer);
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
			printf("��� %d ��������� %d ���ϵ�һ�� ", iPlayer, iTarget);
			DisplayCertainCard(cardAbandoned.back());
			printf("��\n");
			system("pause");
			CheckNun(iTarget);
			CheckMinister(iTarget);
			return true;
		} else {
			int iTarget = CheckPurpleCards(iPlayer);
			if (!iTarget) return false;
			player[iPlayer].nActionPoint--;
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ��\n", iTarget);
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
				printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
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
				printf("����ʧ�ܣ�\n");
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
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ���������� ", iTarget);
			DisplayCertainCard(card[55]);
			printf("��\n");
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
			printf("��� %d ����� %d ��һ�� ", iPlayer, iTarget);
			DisplayCertainCard(c);
			printf("�����˳��ϡ�\n");
			system("pause");
			if (c.nLevel >= 0 && c.nLevel < 4) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
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
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ���������� ", iTarget);
			DisplayCertainCard(card[6]);
			printf("��\n");
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
					printf("��� %d ��������� %d ���ϵ�һ�� ", iPlayer, iTarget);
					DisplayCertainCard(cardAbandoned.back());
					printf("��\n");
					system("pause");
					CheckNun(iTarget);
					player[iPlayer].nNumCoin++;
					DisplayCards();
					printf("��� %d �����Ŀ��� Lv С�� 4��+1 ��ң�\n", iPlayer);
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
			printf("��� %d ��������� %d ���ϵ�һ�� ", iPlayer, iTarget);
			DisplayCertainCard(cardAbandoned.back());
			printf("��\n");
			system("pause");
			CheckNun(iTarget);
			CheckMinister(iTarget);
			return true;
		} else {
			int iTarget = CheckPurpleCards(iPlayer);
			if (!iTarget) return false;
			player[iPlayer].nActionPoint--;
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ��\n", iTarget);
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
			printf("��� %d ����� %d ��һ�� ", iPlayer, iTarget);
			DisplayCertainCard(c);
			printf("�����˳��ϡ�\n");
			system("pause");
			if (c.nLevel >= 0 && c.nLevel < 4) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
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
				printf("����ʧ�ܣ�\n");
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
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ���������� ", iTarget);
			DisplayCertainCard(card[55]);
			printf("��\n");
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
			printf("��� %d ����� %d ��һ�� ", iPlayer, iTarget);
			DisplayCertainCard(c);
			printf("�����˳��ϡ�\n");
			system("pause");
			if (c.nLevel >= 0 && c.nLevel < 5) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
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
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ���������� ", iTarget);
			DisplayCertainCard(card[6]);
			printf("��\n");
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
					printf("��� %d ��������� %d ���ϵ�һ�� ", iPlayer, iTarget);
					DisplayCertainCard(cardAbandoned.back());
					printf("��\n");
					system("pause");
					CheckNun(iTarget);
					player[iPlayer].nNumCoin++;
					DisplayCards();
					printf("��� %d �����Ŀ��� Lv С�� 5��+1 ��ң�\n", iPlayer);
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
			printf("��� %d ��������� %d ���ϵ�һ�� ", iPlayer, iTarget);
			DisplayCertainCard(cardAbandoned.back());
			printf("��\n");
			system("pause");
			CheckNun(iTarget);
			CheckMinister(iTarget);
			return true;
		} else {
			int iTarget = CheckPurpleCards(iPlayer);
			if (!iTarget) return false;
			player[iPlayer].nActionPoint--;
			printf("��� %d ��� ", iPlayer);
			if (!g_bMonkMode) DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
			else DisplayCertainCard(cardAbandoned[iCard]);
			printf("��Ŀ��ָ��Ϊ��� %d ��\n", iTarget);
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
			printf("��� %d ����� %d ��һ�� ", iPlayer, iTarget);
			DisplayCertainCard(c);
			printf("�����˳��ϡ�\n");
			system("pause");
			if (c.nLevel >= 0 && c.nLevel < 5) {
				player[iPlayer].nNumCoin++;
				DisplayCards();
				printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
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
				printf("����ʧ�ܣ�\n");
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
		printf("��� %d ����� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��Ŀ��ָ��Ϊ��� %d ��\n", iTarget);
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
		printf("��� %d ����� %d ���е�һ�� ", iPlayer, iTarget);
		DisplayCertainCard(player[iTarget].cardOnField.back());
		printf("�����˳��ϡ�\n");
		system("pause");
		if (c.nLevel != NO_LEVEL) {
			player[iPlayer].nNumCoin++;
			DisplayCards();
			printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
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
				if (cardAbandoned[i].strName == "ħ��") {
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
				printf("��� %d ����������һ�� ", iPlayer);
				DisplayCertainCard(card[55]);
				printf("���������ơ�\n");
			} else {
				printf("��� %d ���ƿ��һ�� ", iPlayer);
				DisplayCertainCard(card[55]);
				printf("���������ơ�\n");
			}
			system("pause");
			AbandonBomb();
		} else {
			printf("����ʧ�ܣ�\n");
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("�����˳��ϡ�\n");
		IllustrateCard(player[iPlayer].cardOnField.back());
		system("pause");
		return true;
	} else if (iStatus == ON_FIELD) {
		if (player[iPlayer].nOnField > 1) {
			printf("��� %d �����˳��ϵ� ", iPlayer);
			DisplayCertainCard(player[iPlayer].cardOnField[iCard]);
			printf("��\n");
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
			printf("��� %d ���䳡�ϵ�һ�� ", iPlayer);
			DisplayCertainCard(player[iPlayer].cardInHand.back());
			printf("���������ơ�\n");
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
		printf("��� %d ������ ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField[iCard]);
		printf("����ѡ������������һ�� ");
		DisplayCertainCard(cardAbandoned[iCh]);
		printf("��\n");
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
			printf("��� %d ������������ơ�\n", iPlayer);
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardOnField.back());
		printf("�����˳��ϡ�\n");
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
		printf("��� %d ����� ", iPlayer);
		if (g_bDragonMode) DisplayCertainCard(cardAbandoned[iCard]);
		else DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��\n");
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
			printf("�� %d ��Ŀ������з��ƣ������������\n", nDefender);
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
			printf("��� %d ����� %d ���ϵ�һ�� ", iPlayer, iTarget[i]);
			DisplayCertainCard(player[iTarget[i]].cardInHand.back());
			printf("�ջ������ϡ�\n");
			system("pause");
			CheckMinister(iTarget[i]);
		}
		if (res) {
			player[iPlayer].nNumCoin++;
			DisplayCards();
			printf("�����ɹ������ %d +1 ��ң�\n", iPlayer);
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
			printf("����ʧ�ܣ�\n");
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
		printf("��� %d ����� ", iPlayer);
		if (g_bDragonMode) DisplayCertainCard(cardAbandoned[iCard]);
		else DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��Ŀ��ָ��Ϊ��� %d ��\n", iTarget[ch]);
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
		printf("��� %d ��������� %d ���ϵ�һ�� ", iPlayer, iTarget[ch]);
		DisplayCertainCard(c);
		printf("��\n");
		system("pause");
		CheckNun(iTarget[ch]);
		if (c.nLevel != NO_LEVEL) {
			player[iPlayer].nNumCoin++;
			DisplayCards();
			printf("��� %d �����Ŀ��ƴ��� Lv��+1 ��ң�\n", iPlayer);
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
		printf("��� %d ����� ", iPlayer);
		if (g_bDragonMode) DisplayCertainCard(cardAbandoned[iCard]);
		else DisplayCertainCard(cardAbandoned.back());
		printf("��\n");
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
						if (card[i].strName == "����") {
							card[i].nStatus = IN_HAND;
							player[iPlayer].cardInHand.push_back(card[i]);
							player[iPlayer].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 2) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "����") {
							card[i].nStatus = IN_HAND;
							player[iPlayer].cardInHand.push_back(card[i]);
							player[iPlayer].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 3) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "������") {
							card[i].nStatus = IN_HAND;
							player[iPlayer].cardInHand.push_back(card[i]);
							player[iPlayer].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				} else if (ch1 == 4) {
					for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
						if (card[i].strName == "����") {
							card[i].nStatus = IN_HAND;
							player[iPlayer].cardInHand.push_back(card[i]);
							player[iPlayer].nInHand++;
							card[i].nStatus = IN_LIBRARY;
							break;
						}
					}
				}
				DisplayCards();
				printf("��� %d ���ƿ��ȡ��һ�� ", iPlayer);
				DisplayCertainCard(player[iPlayer].cardInHand.back());
				printf("��\n");
			} else {
				int ch1 = rand() % nFireAban;
				AddToHand(iPlayer, iFireAban[ch1]);
				DisplayCards();
				printf("��� %d ����������ȡ��һ�� ", iPlayer);
				DisplayCertainCard(player[iPlayer].cardInHand.back());
				printf("��\n");
			}
		} else {
			int ch1 = rand() % 4 + 1;
			if (ch1 == 1) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "����") {
						card[i].nStatus = IN_HAND;
						player[iPlayer].cardInHand.push_back(card[i]);
						player[iPlayer].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 2) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "����") {
						card[i].nStatus = IN_HAND;
						player[iPlayer].cardInHand.push_back(card[i]);
						player[iPlayer].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 3) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "������") {
						card[i].nStatus = IN_HAND;
						player[iPlayer].cardInHand.push_back(card[i]);
						player[iPlayer].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			} else if (ch1 == 4) {
				for (int i = NUM_YELLOW_CARD + 1; i <= NUM_YELLOW_CARD + NUM_PURPLE_CARD; i++) {
					if (card[i].strName == "����") {
						card[i].nStatus = IN_HAND;
						player[iPlayer].cardInHand.push_back(card[i]);
						player[iPlayer].nInHand++;
						card[i].nStatus = IN_LIBRARY;
						break;
					}
				}
			}
			DisplayCards();
			printf("��� %d ���ƿ��ȡ��һ�� ", iPlayer);
			DisplayCertainCard(player[iPlayer].cardInHand.back());
			printf("��\n");
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
		printf("��� %d ����� ", iPlayer);
		DisplayCertainCard(cardAbandoned.back());
		printf("��\n");
		IllustrateCard(cardAbandoned.back());
		system("pause");
		CheckNun(iPlayer);
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nc;
		Card c = cardAbandoned[ic[ch]];
		AddToHand(iPlayer, ic[ch]);
		DisplayCards();
		printf("��� %d ����������һ�� ", iPlayer);
		DisplayCertainCard(c);
		printf("���������ơ�\n");
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
		printf("��� %d �� ", iPlayer);
		DisplayCertainCard(player[iTarget].cardOnField.back());
		printf("��������� %d �ĳ��ϡ�\n", iTarget);
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
		printf("��� %d ����� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nTarget;
		printf("��� %d ������Ŀ��ָ��Ϊ��� %d ��\n", iPlayer, iTarget[ch]);
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
		printf("��� %d ����� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��\n");
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
		printf("��� %d ����������һ�� ", iPlayer);
		DisplayCertainCard(c);
		printf("�����˳��ϡ�\n");
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
		printf("��� %d ����� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��\n");
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
			printf("��� %d ����������һ�� ", iPlayer);
			DisplayCertainCard(c);
			printf("�����˳��ϡ�\n");
		} else {
			AddToHand(iPlayer, iArmo[ch - nPurple]);
			DisplayCards();
			printf("��� %d ����������һ�� ", iPlayer);
			DisplayCertainCard(player[iPlayer].cardInHand.back());
			printf("���������ơ�\n");
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
		printf("��� %d ����� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nTarget;
		printf("��� %d ������Ŀ��ָ��Ϊ��� %d ��\n", iPlayer, iTarget[ch]);
		system("pause");
		if (Defend(iPlayer, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		Card c = card[rand() % (NUM_YELLOW_CARD + NUM_PURPLE_CARD) + 1];
		printf("��� %d ����� %d ������һ�� ", iPlayer, iTarget[ch]);
		DisplayCertainCard(c);
		printf("��\n");
		system("pause");
		for (int i = 0; i < player[iTarget[ch]].nInHand; i++) {
			if (CompareCards(c, player[iTarget[ch]].cardInHand[i])) {
				AbandonCertainCard(iTarget[ch], i, IN_HAND);
				DisplayCards();
				printf("��� %d ������һ�� ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("��\n");
				system("pause");
				CheckNun(iTarget[ch]);
				if (c.nLevel != NO_LEVEL) {
					player[iPlayer].nNumCoin++;
					DisplayCards();
					printf("�������Ŀ��ƴ��� Lv����� %d +1 ��ң�\n", iPlayer);
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
		printf("����ʧ�ܣ���� %d ����û�и��ƣ�\n", iTarget[ch]);
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
		printf("��� %d ����� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nTarget;
		printf("��� %d ������Ŀ��ָ��Ϊ��� %d ��\n", iPlayer, iTarget[ch]);
		system("pause");
		if (Defend(iPlayer, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		Card c = card[rand() % (NUM_PURPLE_CARD + NUM_YELLOW_CARD) + 1];
		printf("��� %d ����� %d ������һ�� ", iPlayer, iTarget[ch]);
		DisplayCertainCard(c);
		printf("��\n");
		system("pause");
		for (int i = 0; i < player[iTarget[ch]].nInHand; i++) {
			if (CompareCards(c, player[iTarget[ch]].cardInHand[i])) {
				player[iPlayer].cardInHand.push_back(player[iTarget[ch]].cardInHand[i]);
				player[iPlayer].nInHand++;
				player[iTarget[ch]].cardInHand.erase(player[iTarget[ch]].cardInHand.begin() + i);
				player[iTarget[ch]].nInHand--;
				DisplayCards();
				printf("��� %d �����е�һ�� ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("����������� %d �����ϡ�\n", iPlayer);
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
				printf("��� %d �����ϵ�һ�� ", iTarget[ch]);
				DisplayCertainCard(c);
				printf("����������� %d �����ϡ�\n", iPlayer);
				system("pause");
				CheckMinister(iTarget[ch]);
				AbandonBombAI(iPlayer);
				return true;
			}
		}
		printf("����ʧ�ܣ���� %d û�и��ƣ�\n", iTarget[ch]);
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
		printf("��� %d ����� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nTarget;
		printf("��� %d ����������ָ��Ϊ��� %d ��\n", iPlayer, iTarget[ch]);
		system("pause");
		if (Defend(iPlayer, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		player[iTarget[ch]].nNumCoin--;
		player[iPlayer].nNumCoin++;
		DisplayCards();
		printf("��� %d ����� %d ֧���� 1 ��ң�\n", iTarget[ch], iPlayer);
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
		printf("��� %d ����� ", iPlayer);
		DisplayCertainCard(player[iPlayer].cardInHand[iCard]);
		printf("��\n");
		IllustrateCard(player[iPlayer].cardInHand[iCard]);
		system("pause");
		CheckTyrant(iPlayer);
		if (!g_nGameStatus) return true;
		int ch = rand() % nTarget;
		printf("��� %d ����������ָ��Ϊ��� %d ��\n", iPlayer, iTarget[ch]);
		system("pause");
		if (Defend(iPlayer, iTarget[ch], iCard, iStatus)) return true;
		AbandonCertainCard(iPlayer, iCard, iStatus);
		DisplayCards();
		CheckNun(iPlayer);
		int ch1;
		if (iTarget[ch] == 1) {
			printf("��ѡ��\n");
			printf("1.֧�� 1 ��� 2.�����������е�������\n");
			ch1 = RestrictedInput(1, 2);
			if (ch1 == 1 && !player[1].nNumCoin) {
				printf("��û�н�ң�ֻ�ܽ������ϵ������ƣ�\n");
				ch1 = 2;
			}
		} else ch1 = rand() % 2 + 1;
		if (ch1 == 1 && !player[iTarget[ch]].nNumCoin) ch1 = 2;
		if (ch1 == 1) {
			player[iTarget[ch]].nNumCoin--;
			player[iPlayer].nNumCoin++;
			DisplayCards();
			printf("��� %d ����� %d ֧���� 1 ��ң�\n", iTarget[ch], iPlayer);
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
			printf("��� %d ���䳡�ϵ����������ƽ���������� %d ���ϡ�\n", iTarget[ch], iPlayer);
			system("pause");
			AbandonBombAI(iPlayer);
		}
		return true;
	}
	return false;
}
