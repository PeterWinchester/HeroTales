/* HTGame.h
   2024/8/1
   Written by Peter Winchester.
*/

#ifndef HTGAME_H
#define HTGAME_H

#include "HeroTales.h"

void ReadyStageAI(int iCurrentPlayer);
void OperationStageAI(int iPlayer);
void AbandonCardAI(int iPlayer);
int  CheckGameFinish();
void ReadyStage();
void OperationStage();
void AbandonCard();
void ReturnCardOnField(int iPlayer, int iCard); // 回手
bool CheckSonBird(int iPlayer);                 // 检查送子鹤
bool CheckGrail(int iPlayer);                   // 检查圣杯
bool CheckPrincess(int iPlayer);                // 检查公主
bool CheckNun(int iPlayer);                     // 检查修女
bool CheckTransverse(Card c);                   // 判断卡牌是否可以独立横置
bool CheckCardCanBeUsed();                      // 检查是否有可以出的牌
bool CheckCardCanTransverse();                  // 检查是否有可以横置的牌
void PutOnField(int iPlayer, int iCard);        // 将一张牌放在场上
int  CheckPurpleCards(int iPlayer);             // 检查紫牌（返回玩家编号）
bool CheckPurpleCards_CertainPlayer(int iPlayer); //检查特定玩家的紫牌
bool CheckGuard(int iPlayer);                     //检查卫兵
void AbandonCertainCard(int iPlayer, int iCard, int iStatus); //丢弃特定一张牌
void GetOneCardFromLibrary(int iPlayer);                      //从牌库摸一张牌
bool CheckHolyKnight(int iPlayer);                            //检查圣骑士
bool CheckSaintSword(int iPlayer);                            //检查圣剑
bool CheckBlade(int iPlayer);                                 //检查魔剑
bool CheckMinister(int iPlayer);                              //检查大臣
bool CheckMimic(int iPlayer, int nType);                      //检查宝箱怪
void UseGuard(int iAttacker, int iTarget, int iCard);         //触发卫兵
void AddToHand(int iPlayer, int iCard);                       //将弃牌区的指定一张牌加入手牌
void PutOnField_Monk(int iPlayer, int iCard);                 //僧侣模式下放置在场上
bool CheckDancer(int iPlayer);                                //检查舞娘
bool Defend(int iAttacker, int iTarget, int iCard, int iStatus); //使用防牌
void AbandonBombAI(int iPlayer);                                 //检查是否可以丢弃场上的炸弹怪
void AbandonBomb();                                              //检查是否可以丢弃场上的炸弹怪
void PutOnField_Dragon(int iPlayer, int iCard);                  //火龙模式下放置在场上
bool CheckTyrant(int iPlayer);                                   //检查魔王
void IllustrateCard(Card c);                                     //解释卡牌

#endif
