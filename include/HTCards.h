/* HTCards.h
   Created - 2024/08/04
   Written by Peter Winchester.
*/

#ifndef HTCARDS_H
#define HTCARDS_H

#include "HeroTales.h"

bool Grail(int, int);         //ʥ��
bool Minister(int, int);      //��
bool Princess(int, int);      //����
bool Warrior_Royal(int, int); //[��]����
bool King(int, int);          //����
bool SaintSword(int, int);    //ʥ��
bool BlackSmith(int, int);    //����
bool Fighter(int, int);       //�񶷼�
bool Monk(int, int);          //ɮ��
bool Nun(int, int);           //��Ů
bool Businessman(int, int);   //����
bool BabyCrane(int, int);     //���Ӻ�
bool Dancer(int, int);        //����
bool Master(int, int);        //��ʦ
bool Guard(int, int);         //����
bool HolyKnight(int, int);    //ʥ��ʿ
bool Berserker(int, int);     //��սʿ
bool Warrior_Lv4(int, int);   //Lv.4����
bool Warrior_Lv5(int, int);   //Lv.5����
bool BlackKnight(int, int);   //����ʿ
bool Demon(int, int);         //����
bool FireDragon(int, int);    //����
bool Tyrant(int, int);        //ħ��
bool Mimic(int, int);         //�����
bool Blade(int, int);         //ħ��
bool DragonCub(int, int);     //������
bool DragonFlame(int, int);   //����
bool DragonEgg(int, int);     //����
bool Slime(int, int);         //ʷ��ķ
bool Bomb(int, int);          //ը����
bool Bat(int, int);           //����
bool Bone(int, int);          //�׹�
bool Wolfman(int, int);       //����
bool HarleyQuinn(int, int);   //С��Ů
bool Witch(int, int);         //Ů��
bool Goblin(int, int);        //�粼��
bool Tauren(int, int);        //ţͷ��

bool GrailAI(int, int, int);         //ʥ��
bool MinisterAI(int, int, int);      //��
bool PrincessAI(int, int, int);      //����
bool Warrior_RoyalAI(int, int, int); //[��]����
bool KingAI(int, int, int);          //����
bool SaintSwordAI(int, int, int);    //ʥ��
bool BlackSmithAI(int, int, int);    //����
bool FighterAI(int, int, int);       //�񶷼�
bool MonkAI(int, int, int);          //ɮ��
bool NunAI(int, int, int);           //��Ů
bool BusinessmanAI(int, int, int);   //����
bool BabyCraneAI(int, int, int);     //���Ӻ�
bool DancerAI(int, int, int);        //����
bool MasterAI(int, int, int);        //��ʦ
bool GuardAI(int, int, int);         //����
bool HolyKnightAI(int, int, int);    //ʥ��ʿ
bool BerserkerAI(int, int, int);     //��սʿ
bool Warrior_Lv4AI(int, int, int);   //Lv.4����
bool Warrior_Lv5AI(int, int, int);   //Lv.5����
bool BlackKnightAI(int, int, int);   //����ʿ
bool DemonAI(int, int, int);         //����
bool FireDragonAI(int, int, int);    //����
bool TyrantAI(int, int, int);        //ħ��
bool MimicAI(int, int, int);         //�����
bool BladeAI(int, int, int);         //ħ��
bool DragonCubAI(int, int, int);     //������
bool DragonFlameAI(int, int, int);   //����
bool DragonEggAI(int, int, int);     //����
bool SlimeAI(int, int, int);         //ʷ��ķ
bool BombAI(int, int, int);          //ը����
bool BatAI(int, int, int);           //����
bool BoneAI(int, int, int);          //�׹�
bool WolfmanAI(int, int, int);       //����
bool HarleyQuinnAI(int, int, int);   //С��Ů
bool WitchAI(int, int, int);         //Ů��
bool GoblinAI(int, int, int);        //�粼��
bool TaurenAI(int, int, int);        //ţͷ��

int CalcPref_Grail(int);         //ʥ��
int CalcPref_Minister(int);      //��
int CalcPref_Princess(int);      //����
int CalcPref_Warrior_Royal(int); //[��]����
int CalcPref_King(int);          //����
int CalcPref_SaintSword(int);    //ʥ��
int CalcPref_BlackSmith(int);    //����
int CalcPref_Fighter(int);       //�񶷼�
int CalcPref_Monk(int);          //ɮ��
int CalcPref_Nun(int);           //��Ů
int CalcPref_Businessman(int);   //����
int CalcPref_BabyCrane(int);     //���Ӻ�
int CalcPref_Dancer(int);        //����
int CalcPref_Master(int);        //��ʦ
int CalcPref_Guard(int);         //����
int CalcPref_HolyKnight(int);    //ʥ��ʿ
int CalcPref_Berserker(int);     //��սʿ
int CalcPref_Warrior_Lv4(int);   //Lv.4����
int CalcPref_Warrior_Lv5(int);   //Lv.5����
int CalcPref_BlackKnight(int);   //����ʿ
int CalcPref_Demon(int);         //����
int CalcPref_FireDragon(int);    //����
int CalcPref_Tyrant(int);        //ħ��
int CalcPref_Mimic(int);         //�����
int CalcPref_Blade(int);         //ħ��
int CalcPref_DragonCub(int);     //������
int CalcPref_DragonFlame(int);   //����
int CalcPref_DragonEgg(int);     //����
int CalcPref_Slime(int);         //ʷ��ķ
int CalcPref_Bomb(int);          //ը����
int CalcPref_Bat(int);           //����
int CalcPref_Bone(int);          //�׹�
int CalcPref_Wolfman(int);       //����
int CalcPref_HarleyQuinn(int);   //С��Ů
int CalcPref_Witch(int);         //Ů��
int CalcPref_Goblin(int);        //�粼��
int CalcPref_Tauren(int);        //ţͷ��

#endif //HTCARDS_H
