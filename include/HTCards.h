/* HTCards.h
   Created - 2024/08/04
   Written by Peter Winchester.
*/

#ifndef HTCARDS_H
#define HTCARDS_H

#include "HeroTales.h"

bool Grail(int, int);         //圣杯
bool Minister(int, int);      //大臣
bool Princess(int, int);      //公主
bool Warrior_Royal(int, int); //[皇]勇者
bool King(int, int);          //国王
bool SaintSword(int, int);    //圣剑
bool BlackSmith(int, int);    //铁匠
bool Fighter(int, int);       //格斗家
bool Monk(int, int);          //僧侣
bool Nun(int, int);           //修女
bool Businessman(int, int);   //商人
bool BabyCrane(int, int);     //送子鹤
bool Dancer(int, int);        //舞娘
bool Master(int, int);        //法师
bool Guard(int, int);         //卫兵
bool HolyKnight(int, int);    //圣骑士
bool Berserker(int, int);     //狂战士
bool Warrior_Lv4(int, int);   //Lv.4勇者
bool Warrior_Lv5(int, int);   //Lv.5勇者
bool BlackKnight(int, int);   //黑骑士
bool Demon(int, int);         //妖精
bool FireDragon(int, int);    //火龙
bool Tyrant(int, int);        //魔王
bool Mimic(int, int);         //宝箱怪
bool Blade(int, int);         //魔剑
bool DragonCub(int, int);     //龙幼崽
bool DragonFlame(int, int);   //龙炎
bool DragonEgg(int, int);     //龙蛋
bool Slime(int, int);         //史莱姆
bool Bomb(int, int);          //炸弹怪
bool Bat(int, int);           //蝙蝠
bool Bone(int, int);          //白骨
bool Wolfman(int, int);       //狼人
bool HarleyQuinn(int, int);   //小丑女
bool Witch(int, int);         //女巫
bool Goblin(int, int);        //哥布林
bool Tauren(int, int);        //牛头人

bool GrailAI(int, int, int);         //圣杯
bool MinisterAI(int, int, int);      //大臣
bool PrincessAI(int, int, int);      //公主
bool Warrior_RoyalAI(int, int, int); //[皇]勇者
bool KingAI(int, int, int);          //国王
bool SaintSwordAI(int, int, int);    //圣剑
bool BlackSmithAI(int, int, int);    //铁匠
bool FighterAI(int, int, int);       //格斗家
bool MonkAI(int, int, int);          //僧侣
bool NunAI(int, int, int);           //修女
bool BusinessmanAI(int, int, int);   //商人
bool BabyCraneAI(int, int, int);     //送子鹤
bool DancerAI(int, int, int);        //舞娘
bool MasterAI(int, int, int);        //法师
bool GuardAI(int, int, int);         //卫兵
bool HolyKnightAI(int, int, int);    //圣骑士
bool BerserkerAI(int, int, int);     //狂战士
bool Warrior_Lv4AI(int, int, int);   //Lv.4勇者
bool Warrior_Lv5AI(int, int, int);   //Lv.5勇者
bool BlackKnightAI(int, int, int);   //黑骑士
bool DemonAI(int, int, int);         //妖精
bool FireDragonAI(int, int, int);    //火龙
bool TyrantAI(int, int, int);        //魔王
bool MimicAI(int, int, int);         //宝箱怪
bool BladeAI(int, int, int);         //魔剑
bool DragonCubAI(int, int, int);     //龙幼崽
bool DragonFlameAI(int, int, int);   //龙炎
bool DragonEggAI(int, int, int);     //龙蛋
bool SlimeAI(int, int, int);         //史莱姆
bool BombAI(int, int, int);          //炸弹怪
bool BatAI(int, int, int);           //蝙蝠
bool BoneAI(int, int, int);          //白骨
bool WolfmanAI(int, int, int);       //狼人
bool HarleyQuinnAI(int, int, int);   //小丑女
bool WitchAI(int, int, int);         //女巫
bool GoblinAI(int, int, int);        //哥布林
bool TaurenAI(int, int, int);        //牛头人

int CalcPref_Grail(int);         //圣杯
int CalcPref_Minister(int);      //大臣
int CalcPref_Princess(int);      //公主
int CalcPref_Warrior_Royal(int); //[皇]勇者
int CalcPref_King(int);          //国王
int CalcPref_SaintSword(int);    //圣剑
int CalcPref_BlackSmith(int);    //铁匠
int CalcPref_Fighter(int);       //格斗家
int CalcPref_Monk(int);          //僧侣
int CalcPref_Nun(int);           //修女
int CalcPref_Businessman(int);   //商人
int CalcPref_BabyCrane(int);     //送子鹤
int CalcPref_Dancer(int);        //舞娘
int CalcPref_Master(int);        //法师
int CalcPref_Guard(int);         //卫兵
int CalcPref_HolyKnight(int);    //圣骑士
int CalcPref_Berserker(int);     //狂战士
int CalcPref_Warrior_Lv4(int);   //Lv.4勇者
int CalcPref_Warrior_Lv5(int);   //Lv.5勇者
int CalcPref_BlackKnight(int);   //黑骑士
int CalcPref_Demon(int);         //妖精
int CalcPref_FireDragon(int);    //火龙
int CalcPref_Tyrant(int);        //魔王
int CalcPref_Mimic(int);         //宝箱怪
int CalcPref_Blade(int);         //魔剑
int CalcPref_DragonCub(int);     //龙幼崽
int CalcPref_DragonFlame(int);   //龙炎
int CalcPref_DragonEgg(int);     //龙蛋
int CalcPref_Slime(int);         //史莱姆
int CalcPref_Bomb(int);          //炸弹怪
int CalcPref_Bat(int);           //蝙蝠
int CalcPref_Bone(int);          //白骨
int CalcPref_Wolfman(int);       //狼人
int CalcPref_HarleyQuinn(int);   //小丑女
int CalcPref_Witch(int);         //女巫
int CalcPref_Goblin(int);        //哥布林
int CalcPref_Tauren(int);        //牛头人

#endif //HTCARDS_H
