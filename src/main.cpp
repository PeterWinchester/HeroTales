/* HeroTales.cpp
   2024/6/23
   Written by Peter Winchester.
*/

#include "HeroTales.h"

void Init();
void Game();
void Help();

int main() {
	Init();
	int iProgramRunning = 1, iFirstTime = 1;
	while (iProgramRunning) {
		if (!iFirstTime) system("cls");
		else iFirstTime = 0;
		printf("+-------------+\n");
		printf("|��ʼ��Ϸ �� 1|\n");
		printf("|��Ϸ���� �� 2|\n");
		printf("|�˳����� �� 3|\n");
		printf("+-------------+\n");
		while (1) {
			char ch = _getch();
			if (ch == '1') {
				Game();
				break;
			}
			else if (ch == '2') {
				Help();
				break;
			}
			else if (ch == '3') {
				iProgramRunning = 0;
				break;
			}
		}
	}
	printf("���˳�С��˵��\n");
	return 0;
}

void Init() {
	SetConsoleTitleA("С��˵ 1.06");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
	system("cls");
	srand(time(0));
	g_nGameStatus = 0;
	printf("С��˵(Hero Tales) v1.06\n");
	printf("Developed by Peter Winchester, 2024.\n\n");

	printf("Loading...");
	int iCurrentCard = 0;
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "�����׼���׶Σ���ֻ�� 1 ��һ�ʤ����ֱ�ӻ�ʤ��";
		card[iCurrentCard].strName = "ʥ��";
		card[iCurrentCard].pfnOpt = Grail;
		card[iCurrentCard].pfnOptAI = GrailAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "���㱻[����]����ָ��ʱ����[����]���ƽ�������������㳡�ϣ��ɺ��ñ��ƣ����һ���ơ�";
		card[iCurrentCard].strName = "��";
		card[iCurrentCard].pfnOpt = Minister;
		card[iCurrentCard].pfnOptAI = MinisterAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "������Ʋ������ж��㣬���������㳡�ϣ������׼���׶Σ��� +1 ��ҡ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = Princess;
		card[iCurrentCard].pfnOptAI = PrincessAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "��һ�����ֵ�һ�ŷǻ�ɫ���������䳡�ϣ��������ƴ��� Lv �� [ħ]���� +1 ��ҡ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = Warrior_Royal;
		card[iCurrentCard].pfnOptAI = Warrior_RoyalAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "���ã�����������һ�� Lv.4 ���߼������ơ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = King;
		card[iCurrentCard].pfnOptAI = KingAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "����ʹ������ʱ�����ñ��ƣ������ߵ�Ч����Ϊ�������ֳ��ϵ�һ���ƣ����������� Lv �������ߣ��� +1 ��ҡ�";
		card[iCurrentCard].strName = "ʥ��";
		card[iCurrentCard].pfnOpt = SaintSword;
		card[iCurrentCard].pfnOptAI = SaintSwordAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 0;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "���������Ʒ������㳡�ϣ�����˷��õ����� Lv ��������ͬ���� +1 ��ҡ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = BlackSmith;
		card[iCurrentCard].pfnOptAI = BlackSmithAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 0;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "�����ȫ�����Ʒ����ڳ��ϣ�����˷��õ����� Lv ֮��Ϊż���Ҳ��������£��� +1 ��ҡ�";
		card[iCurrentCard].strName = "�񶷼�";
		card[iCurrentCard].pfnOpt = Fighter;
		card[iCurrentCard].pfnOptAI = FighterAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 1;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "��������ѡ 1 �� Lv.1 ����Ļ�ɫ���Ʋ������ж�����������������ơ�";
		card[iCurrentCard].strName = "ɮ��";
		card[iCurrentCard].pfnOpt = Monk;
		card[iCurrentCard].pfnOptAI = MonkAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 1;
		card[iCurrentCard].nType = TRIGGER;
		card[iCurrentCard].strDescription = "������ Lv.1 ����Ŀ��ƽ���������ʱ����Ҫ�����Ŀ��Ƽ���������ơ�";
		card[iCurrentCard].strName = "��Ů";
		card[iCurrentCard].pfnOpt = Nun;
		card[iCurrentCard].pfnOptAI = NunAI;
	}
	printf(".");
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 1;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "���㳡�ϵ�һ���Ƽ�����ֵ����ƣ�������������֧�� 1 ��ҡ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = Businessman;
		card[iCurrentCard].pfnOptAI = BusinessmanAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 2;
		card[iCurrentCard].nType = TRIGGER;
		card[iCurrentCard].strDescription = "�����׼���׶�ʱ������ 2 �ż����ϵ��������ƻص����У��� +1 ��ҡ�";
		card[iCurrentCard].strName = "���Ӻ�";
		card[iCurrentCard].pfnOpt = BabyCrane;
		card[iCurrentCard].pfnOptAI = BabyCraneAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 2;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "�����������㳡�ϡ����������㳡�ϣ����ֻ�ý�Һ�����Զ������ƣ��� +1 ��ҡ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = Dancer;
		card[iCurrentCard].pfnOptAI = DancerAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 2;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "��һ�������ƵĶ������� 1/2/3/4 �е�һ��������������ô˵ȼ����Ƶ��䳡�ϣ������� +1 ��ҡ�";
		card[iCurrentCard].strName = "��ʦ";
		card[iCurrentCard].pfnOpt = Master;
		card[iCurrentCard].pfnOptAI = MasterAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 3;
		card[iCurrentCard].nType = TRIGGER;
		card[iCurrentCard].strDescription = "�����㱻[����]����ָ��ʱ������������Ч������������ڳ����߳��ϣ�֮�����һ���ơ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = Guard;
		card[iCurrentCard].pfnOptAI = GuardAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 3;
		card[iCurrentCard].nType = TRIGGER;
		card[iCurrentCard].strDescription = "���㱻[����]����ָ��ʱ����� 2 ���ƣ����ߣ����[����]������Ч������ӵ�������У���� 1 ���ơ�";
		card[iCurrentCard].strName = "ʥ��ʿ";
		card[iCurrentCard].pfnOpt = HolyKnight;
		card[iCurrentCard].pfnOptAI = HolyKnightAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 4;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "ָ��һ�����ֵ�ȫ���ǻ�ɫ���Ʒ������䳡�ϣ��������� Lv���� +1 ��ң���������Ҳ���������ߡ���";
		card[iCurrentCard].strName = "��սʿ";
		card[iCurrentCard].pfnOpt = Berserker;
		card[iCurrentCard].pfnOptAI = BerserkerAI;
	}
	printf(".");
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 4;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "ָ��һ�����ֵ�һ�ŷǻ�ɫ���Ʒ������䳡�ϣ������� Lv ���ߣ��� +1 ��ҡ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = Warrior_Lv4;
		card[iCurrentCard].pfnOptAI = Warrior_Lv4AI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 5;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "ָ��һ�����ֵ�һ�ŷǻ�ɫ���Ʒ������䳡�ϣ������� Lv ���ߣ��� +1 ��ҡ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = Warrior_Lv5;
		card[iCurrentCard].pfnOptAI = Warrior_Lv5AI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL_AND_DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "����һ�Ŷ����������䳡�ϣ������� Lv, �� +1 ��ң����������£�������⴦����ħ�����������ơ�";
		card[iCurrentCard].strName = "����ʿ";
		card[iCurrentCard].pfnOpt = BlackKnight;
		card[iCurrentCard].pfnOptAI = BlackKnightAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "���ã����㳡�ϵ�һ���Ƽ������ơ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = Demon;
		card[iCurrentCard].pfnOptAI = DemonAI;
	}
	printf(".");
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nFire = FIRE;
		card[iCurrentCard].nArmory = DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "���ã�����������һ��[��]�������ж�����������������ơ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = FireDragon;
		card[iCurrentCard].pfnOptAI = FireDragonAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "������һ�Ŵ��� Lv ����ɫ�ƺ󣬺��ñ��ƣ��� +1 ��ҡ�";
		card[iCurrentCard].strName = "ħ��";
		card[iCurrentCard].pfnOpt = Tyrant;
		card[iCurrentCard].pfnOptAI = TyrantAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = TRIGGER;
		card[iCurrentCard].strDescription = "�����ֻ�ý�Һ���������ƣ����ߣ������� 2 �ж�������ƺ��� +1 ��ҡ�";
		card[iCurrentCard].strName = "�����";
		card[iCurrentCard].pfnOpt = Mimic;
		card[iCurrentCard].pfnOptAI = MimicAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "���������ߡ�ʱ�����ñ��ƣ������ߴ�����������ĳ������趪�����ҡ����ߡ���Ч����ָ������һ����ɫ��";
		card[iCurrentCard].strName = "ħ��";
		card[iCurrentCard].pfnOpt = Blade;
		card[iCurrentCard].pfnOptAI = BladeAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nFire = FIRE;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "ѡ��ÿ���������ƵĶ��֣������ǳ��ϵ�һ���Ʒ������ơ�����Щ�ƾ����� Lv���� +1 ��ҡ�";
		card[iCurrentCard].strName = "������";
		card[iCurrentCard].pfnOpt = DragonCub;
		card[iCurrentCard].pfnOptAI = DragonCubAI;
	}
	printf(".");
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nFire = FIRE;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "����һ�����ֳ��ϵ�һ���ƣ��������ƴ��� Lv���� +1 ��ҡ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = DragonFlame;
		card[iCurrentCard].pfnOptAI = DragonFlameAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nFire = FIRE;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "����ɫ�ƿ��������ѡһ��[��]�������ơ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = DragonEgg;
		card[iCurrentCard].pfnOptAI = DragonEggAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 0;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "����������һ�� Lv.1 �ƻ��� Lv.2 �Ƽ���������ơ�";
		card[iCurrentCard].strName = "ʷ��ķ";
		card[iCurrentCard].pfnOpt = Slime;
		card[iCurrentCard].pfnOptAI = SlimeAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 1;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "���õ����ֳ��ϳ�Ϊ�����ơ�����ĳ��Ϸ����˱��ƣ���׼���׶�ǰ����ѡ���㳡����һ���Ʒ������С��κ����ʹ����������С��5ʱ���ɶ������ơ�";
		card[iCurrentCard].strName = "ը����";
		card[iCurrentCard].pfnOpt = Bomb;
		card[iCurrentCard].pfnOptAI = BombAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 1;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "���ܲ鿴һ�����ֵ�ȫ�����ƣ�֮���������ϵ���ȫ���ص����ơ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = Bat;
		card[iCurrentCard].pfnOptAI = BatAI;
	}
	printf(".");
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 2;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "����ɫ������ѡ��һ���ƣ����õ���ĳ��ϡ�";
		card[iCurrentCard].strName = "�׹�";
		card[iCurrentCard].pfnOpt = Bone;
		card[iCurrentCard].pfnOptAI = BoneAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 2;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "����ɫ������ѡ��һ�Ŵ��� Lv ���Ʒ��õ����ϣ����ߣ�������������ѡһ�Ŵ������µ��Ƽ������ơ�";
		card[iCurrentCard].strName = "����";
		card[iCurrentCard].pfnOpt = Wolfman;
		card[iCurrentCard].pfnOptAI = WolfmanAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 3;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "ָ��һ�����֣���������һ���Ƶ����ƣ������ֵ�����ӵ�и��ƣ����䶪���������� Lv���� +1 ��ҡ�";
		card[iCurrentCard].strName = "С��Ů";
		card[iCurrentCard].pfnOpt = HarleyQuinn;
		card[iCurrentCard].pfnOptAI = HarleyQuinnAI;
	}
	for (int i = 1; i <= 2; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 3;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "ָ��һ�����֣���������һ���Ƶ����ƣ����������ƻ���ӵ�и��ƣ����뽫һ�Ž�����������ơ�";
		card[iCurrentCard].strName = "Ů��";
		card[iCurrentCard].pfnOpt = Witch;
		card[iCurrentCard].pfnOptAI = WitchAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 4;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "ָ��һ����ұ����Ķ��֣�����֧�� 1 ��ҡ�";
		card[iCurrentCard].strName = "�粼��";
		card[iCurrentCard].pfnOpt = Goblin;
		card[iCurrentCard].pfnOptAI = GoblinAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 4;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "ָ��һ�����ֳ������д����µĿ��ƣ���������������Щ���Ƶ������ƣ�������֧�� 1 ��ҡ�";
		card[iCurrentCard].strName = "ţͷ��";
		card[iCurrentCard].pfnOpt = Tauren;
		card[iCurrentCard].pfnOptAI = TaurenAI;
	}
	printf(".Done!\n");
}

void Game() {
	system("cls");
	printf("������ 0 ������ҳ��\n");
	printf("�������Ӧ�� 2~10 ��Χ�ڡ�\n");
	while (1) {
		printf("���������������������밴 Enter����");
		g_nNumPlayer = InputIntSafely();
		if (g_nNumPlayer == 0) return;
		else if (g_nNumPlayer < 2 || g_nNumPlayer > 10) printf("��������ֳ����˷�Χ��\n");
		else break;
	}
	printf("��ʤ�����Ӧ������ 1��\n");
	while (1) {
		printf("�������ʤ�������");
		g_nStdCoinNumber = InputIntSafely();
		if (g_nStdCoinNumber == 0) return;
		else if (g_nStdCoinNumber < 1) printf("��ʤ�����Ӧ������ 1��\n");
		else break;
	}
	/* ��ʼ��������ݡ� */
	for (int i = 1; i <= g_nNumPlayer; i++) {
		player[i].nNumCoin = 0;
		player[i].nActionPoint = 2;
		if (player[i].nInHand) {
			player[i].nInHand = 0;
			player[i].cardInHand.clear();
		}
		if (player[i].nOnField) {
			player[i].nOnField = 0;
			player[i].cardOnField.clear();
		}
	}
	cardAbandoned.clear(); //���������
	g_nAbandoned = 0;
	g_bMonkMode = false;
	g_bDragonMode = false;
	/* ��Ϸ��������� */
	DealCards();
	g_nGameStatus = 1;
	printf("�������������һ�غ����......");
	int iCurrentPlayer = rand() % g_nNumPlayer + 1;
	printf("Done!\n");
	printf("��һ�غ�Ϊ��� %d��\n", iCurrentPlayer);
	printf("��Ϸ��ʽ��ʼ��\n");
	system("pause");
	while (g_nGameStatus) {
		DisplayCards();
		player[iCurrentPlayer].nActionPoint = 2;
		if (iCurrentPlayer != 1) {
			printf("��� %d �Ļغϡ�\n", iCurrentPlayer);
			ReadyStageAI(iCurrentPlayer);
			if (CheckGameFinish()) return;
			OperationStageAI(iCurrentPlayer);
			if (CheckGameFinish()) return;
		}
		else {
			printf("��Ļغϡ�\n");
			ReadyStage();
			if (CheckGameFinish()) return;
			OperationStage();
			if (CheckGameFinish()) return;
		}
		iCurrentPlayer++;
		if (iCurrentPlayer > g_nNumPlayer) iCurrentPlayer = 1;
	}
}

void Help() {
	while (1) {
		system("cls");
		printf("���� 0 ������ҳ��\n");
		printf("+----------+\n");
		printf("|1.����˵��|\n");
		printf("|2.��Ϸ����|\n");
		printf("|3.������Ϸ|\n");
		printf("+----------+\n");
		char ch = _getch();
		if (ch == '0') {
			break;
		}
		else if (ch == '1') {
			DescribeCards();
		}
		else if (ch == '2') {
			DisplayRules();
		}
		else if (ch == '3') {
			printf("С��˵(Hero Tales) v1.06\n");
			printf("Developed by Peter Winchester, 2024.\n\n");
			system("pause");
		}
	}
}
