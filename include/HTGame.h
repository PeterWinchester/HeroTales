/* HTGame.h
   Created - 2024/8/1
   Written by Peter Winchester.
*/

#ifndef HTGAME_H
#define HTGAME_H

#include "HeroTales.h"

void ReadyStageAI(int iCurrentPlayer);
void OperationStageAI(int iPlayer);
void AbandonCardAI(int iPlayer);
void ReadyStage();
void OperationStage();
void AbandonCard();
void ReturnCardOnField(int iPlayer, int iCard);                  // ����
void PutOnField(int iPlayer, int iCard);                         // ��һ���Ʒ��ڳ���
void AbandonCertainCard(int iPlayer, int iCard, int iStatus);    // �����ض�һ����
void GetOneCardFromLibrary(int iPlayer);                         // ���ƿ���һ����
void UseGuard(int iAttacker, int iTarget, int iCard);            // ��������
void AddToHand(int iPlayer, int iCard);                          // ����������ָ��һ���Ƽ�������
void PutOnField_Monk(int iPlayer, int iCard);                    // ɮ��ģʽ�·����ڳ���
void AbandonBombAI(int iPlayer);                                 // ����Ƿ���Զ������ϵ�ը����
void AbandonBomb();                                              // ����Ƿ���Զ������ϵ�ը����
void PutOnField_Dragon(int iPlayer, int iCard);                  // ����ģʽ�·����ڳ���
void IllustrateCard(Card c);                                     // ���Ϳ���

int  CheckGameFinish();
int  CheckPurpleCards(int iPlayer);                              // ������ƣ�������ұ�ţ�

bool CheckSonBird(int iPlayer);                                  // ������Ӻ�
bool CheckGrail(int iPlayer);                                    // ���ʥ��
bool CheckPrincess(int iPlayer);                                 // ��鹫��
bool CheckNun(int iPlayer);                                      // �����Ů
bool CheckTransverse(Card c);                                    // �жϿ����Ƿ���Զ�������
bool CheckCardCanBeUsed();                                       // ����Ƿ��п��Գ�����
bool CheckCardCanTransverse();                                   // ����Ƿ��п��Ժ��õ���
bool CheckPurpleCards_CertainPlayer(int iPlayer);                // ����ض���ҵ�����
bool CheckGuard(int iPlayer);                                    // �������
bool CheckHolyKnight(int iPlayer);                               // ���ʥ��ʿ
bool CheckSaintSword(int iPlayer);                               // ���ʥ��
bool CheckBlade(int iPlayer);                                    // ���ħ��
bool CheckMinister(int iPlayer);                                 // ����
bool CheckMimic(int iPlayer, int nType);                         // ��鱦���
bool CheckDancer(int iPlayer);                                   // �������
bool Defend(int iAttacker, int iTarget, int iCard, int iStatus); // ʹ�÷���
bool CheckTyrant(int iPlayer);                                   // ���ħ��
bool CheckBomb(int iPlayer);                                     // ���ը����
bool CheckDragon(int iPlayer);                                   // ������

#endif
