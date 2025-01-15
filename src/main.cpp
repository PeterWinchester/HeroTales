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
		printf("|开始游戏 按 1|\n");
		printf("|游戏帮助 按 2|\n");
		printf("|退出程序 按 3|\n");
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
	printf("已退出小传说。\n");
	return 0;
}

void Init() {
	SetConsoleTitleA("小传说 1.06");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
	system("cls");
	srand(time(0));
	g_nGameStatus = 0;
	printf("小传说(Hero Tales) v1.06\n");
	printf("Developed by Peter Winchester, 2024.\n\n");

	printf("Loading...");
	int iCurrentCard = 0;
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "在你的准备阶段，若只差 1 金币获胜，则直接获胜。";
		card[iCurrentCard].strName = "圣杯";
		card[iCurrentCard].pfnOpt = Grail;
		card[iCurrentCard].pfnOptAI = GrailAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "当你被[攻击]卡牌指定时，待[攻击]卡牌结算后，若本牌在你场上，可横置本牌，你抽一张牌。";
		card[iCurrentCard].strName = "大臣";
		card[iCurrentCard].pfnOpt = Minister;
		card[iCurrentCard].pfnOptAI = MinisterAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "打出本牌不消耗行动点，若本牌在你场上，在你的准备阶段，你 +1 金币。";
		card[iCurrentCard].strName = "公主";
		card[iCurrentCard].pfnOpt = Princess;
		card[iCurrentCard].pfnOptAI = PrincessAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "将一名对手的一张非黄色手牌置于其场上，若该手牌带有 Lv 或 [魔]，你 +1 金币。";
		card[iCurrentCard].strName = "勇者";
		card[iCurrentCard].pfnOpt = Warrior_Royal;
		card[iCurrentCard].pfnOptAI = Warrior_RoyalAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "横置，将弃牌区的一张 Lv.4 勇者加入手牌。";
		card[iCurrentCard].strName = "国王";
		card[iCurrentCard].pfnOpt = King;
		card[iCurrentCard].pfnOptAI = KingAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "当你使用勇者时，横置本牌，将勇者的效果改为丢弃对手场上的一张牌，若丢弃的牌 Lv 低于勇者，你 +1 金币。";
		card[iCurrentCard].strName = "圣剑";
		card[iCurrentCard].pfnOpt = SaintSword;
		card[iCurrentCard].pfnOptAI = SaintSwordAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 0;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "将两张手牌放置在你场上，若因此放置的手牌 Lv 或纹章相同，你 +1 金币。";
		card[iCurrentCard].strName = "铁匠";
		card[iCurrentCard].pfnOpt = BlackSmith;
		card[iCurrentCard].pfnOptAI = BlackSmithAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 0;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "将你的全部手牌放置在场上，若因此放置的手牌 Lv 之和为偶数且不存在纹章，你 +1 金币。";
		card[iCurrentCard].strName = "格斗家";
		card[iCurrentCard].pfnOpt = Fighter;
		card[iCurrentCard].pfnOptAI = FighterAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 1;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "从弃牌区选 1 张 Lv.1 以外的黄色手牌不消耗行动点打出，或将其加入手牌。";
		card[iCurrentCard].strName = "僧侣";
		card[iCurrentCard].pfnOpt = Monk;
		card[iCurrentCard].pfnOptAI = MonkAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 1;
		card[iCurrentCard].nType = TRIGGER;
		card[iCurrentCard].strDescription = "对手有 Lv.1 以外的卡牌进入弃牌区时，将要丢弃的卡牌加入你的手牌。";
		card[iCurrentCard].strName = "修女";
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
		card[iCurrentCard].strDescription = "将你场上的一张牌加入对手的手牌，那名对手向你支付 1 金币。";
		card[iCurrentCard].strName = "商人";
		card[iCurrentCard].pfnOpt = Businessman;
		card[iCurrentCard].pfnOptAI = BusinessmanAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 2;
		card[iCurrentCard].nType = TRIGGER;
		card[iCurrentCard].strDescription = "当你的准备阶段时，若有 2 张及以上的其它卡牌回到手中，你 +1 金币。";
		card[iCurrentCard].strName = "送子鹤";
		card[iCurrentCard].pfnOpt = BabyCrane;
		card[iCurrentCard].pfnOptAI = BabyCraneAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 2;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "打出后放置在你场上。若本牌在你场上，对手获得金币后，你可以丢弃此牌，并 +1 金币。";
		card[iCurrentCard].strName = "舞娘";
		card[iCurrentCard].pfnOpt = Dancer;
		card[iCurrentCard].pfnOptAI = DancerAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 2;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "向一名有手牌的对手宣言 1/2/3/4 中的一个数，他必须放置此等级的牌到其场上，否则你 +1 金币。";
		card[iCurrentCard].strName = "法师";
		card[iCurrentCard].pfnOpt = Master;
		card[iCurrentCard].pfnOptAI = MasterAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 3;
		card[iCurrentCard].nType = TRIGGER;
		card[iCurrentCard].strDescription = "当年你被[攻击]卡牌指定时，令那张牌无效，并将其放置在出牌者场上，之后你抽一张牌。";
		card[iCurrentCard].strName = "卫兵";
		card[iCurrentCard].pfnOpt = Guard;
		card[iCurrentCard].pfnOptAI = GuardAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 3;
		card[iCurrentCard].nType = TRIGGER;
		card[iCurrentCard].strDescription = "当你被[攻击]卡牌指定时，你抽 2 张牌；或者，令该[攻击]卡牌无效并返回拥有者手中，你抽 1 张牌。";
		card[iCurrentCard].strName = "圣骑士";
		card[iCurrentCard].pfnOpt = HolyKnight;
		card[iCurrentCard].pfnOptAI = HolyKnightAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 4;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "指定一名对手的全部非黄色手牌放置于其场上，若均带有 Lv，你 +1 金币，本牌名称也视作“勇者”。";
		card[iCurrentCard].strName = "狂战士";
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
		card[iCurrentCard].strDescription = "指定一名对手的一张非黄色手牌放置于其场上，若本牌 Lv 更高，你 +1 金币。";
		card[iCurrentCard].strName = "勇者";
		card[iCurrentCard].pfnOpt = Warrior_Lv4;
		card[iCurrentCard].pfnOptAI = Warrior_Lv4AI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = YELLOW;
		card[iCurrentCard].nLevel = 5;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "指定一名对手的一张非黄色手牌放置于其场上，若本牌 Lv 更高，你 +1 金币。";
		card[iCurrentCard].strName = "勇者";
		card[iCurrentCard].pfnOpt = Warrior_Lv5;
		card[iCurrentCard].pfnOptAI = Warrior_Lv5AI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = ROYAL_AND_DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "放置一张对手手牌于其场上，若带有 Lv, 你 +1 金币；若带有纹章，则从任意处将“魔剑”加入手牌。";
		card[iCurrentCard].strName = "黑骑士";
		card[iCurrentCard].pfnOpt = BlackKnight;
		card[iCurrentCard].pfnOptAI = BlackKnightAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "横置，将你场上的一张牌加入手牌。";
		card[iCurrentCard].strName = "妖精";
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
		card[iCurrentCard].strDescription = "横置，将弃牌区的一张[火]不消耗行动点打出，或将其加入手牌。";
		card[iCurrentCard].strName = "火龙";
		card[iCurrentCard].pfnOpt = FireDragon;
		card[iCurrentCard].pfnOptAI = FireDragonAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "当你打出一张带有 Lv 的紫色牌后，横置本牌，你 +1 金币。";
		card[iCurrentCard].strName = "魔王";
		card[iCurrentCard].pfnOpt = Tyrant;
		card[iCurrentCard].pfnOptAI = TyrantAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = TRIGGER;
		card[iCurrentCard].strDescription = "当对手获得金币后，你抽两张牌；或者，当对手 2 行动点均抽牌后，你 +1 金币。";
		card[iCurrentCard].strName = "宝箱怪";
		card[iCurrentCard].pfnOpt = Mimic;
		card[iCurrentCard].pfnOptAI = MimicAI;
	}
	for (int i = 1; i <= 1; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = DAEMON;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = NO_LEVEL;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "你打出“勇者”时，横置本牌，该勇者打出后放置于你的场上无需丢弃，且“勇者”的效果可指定任意一种颜色。";
		card[iCurrentCard].strName = "魔剑";
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
		card[iCurrentCard].strDescription = "选择每个场上有牌的对手，令他们场上的一张牌返回手牌。若那些牌均带有 Lv，你 +1 金币。";
		card[iCurrentCard].strName = "龙幼崽";
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
		card[iCurrentCard].strDescription = "丢弃一名对手场上的一张牌，若那张牌带有 Lv，你 +1 金币。";
		card[iCurrentCard].strName = "龙炎";
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
		card[iCurrentCard].strDescription = "从紫色牌库或弃牌区选一张[火]加入手牌。";
		card[iCurrentCard].strName = "龙蛋";
		card[iCurrentCard].pfnOpt = DragonEgg;
		card[iCurrentCard].pfnOptAI = DragonEggAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 0;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "将弃牌区的一张 Lv.1 牌或者 Lv.2 牌加入你的手牌。";
		card[iCurrentCard].strName = "史莱姆";
		card[iCurrentCard].pfnOpt = Slime;
		card[iCurrentCard].pfnOptAI = SlimeAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 1;
		card[iCurrentCard].nType = CONSISTENT;
		card[iCurrentCard].strDescription = "放置到对手场上成为他的牌。若你的场上放置了本牌，在准备阶段前，需选择你场上另一张牌返回手中。任何情况使你手牌数不小于5时，可丢弃本牌。";
		card[iCurrentCard].strName = "炸弹怪";
		card[iCurrentCard].pfnOpt = Bomb;
		card[iCurrentCard].pfnOptAI = BombAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 1;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "秘密查看一名对手的全部手牌，之后令他场上的牌全部回到手牌。";
		card[iCurrentCard].strName = "蝙蝠";
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
		card[iCurrentCard].strDescription = "从紫色弃牌区选择一张牌，放置到你的场上。";
		card[iCurrentCard].strName = "白骨";
		card[iCurrentCard].pfnOpt = Bone;
		card[iCurrentCard].pfnOptAI = BoneAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 2;
		card[iCurrentCard].nType = ORDINARY;
		card[iCurrentCard].strDescription = "从紫色弃牌区选择一张带有 Lv 的牌放置到场上；或者，从任意弃牌区选一张带有纹章的牌加入手牌。";
		card[iCurrentCard].strName = "狼人";
		card[iCurrentCard].pfnOpt = Wolfman;
		card[iCurrentCard].pfnOptAI = WolfmanAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 3;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "指定一名对手，向他宣言一张牌的名称，若对手的手牌拥有该牌，将其丢弃；若带有 Lv，你 +1 金币。";
		card[iCurrentCard].strName = "小丑女";
		card[iCurrentCard].pfnOpt = HarleyQuinn;
		card[iCurrentCard].pfnOptAI = HarleyQuinnAI;
	}
	for (int i = 1; i <= 2; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 3;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "指定一名对手，向他宣言一张牌的名称，若他的手牌或场上拥有该牌，必须将一张交付到你的手牌。";
		card[iCurrentCard].strName = "女巫";
		card[iCurrentCard].pfnOpt = Witch;
		card[iCurrentCard].pfnOptAI = WitchAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 4;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "指定一名金币比你多的对手，向你支付 1 金币。";
		card[iCurrentCard].strName = "哥布林";
		card[iCurrentCard].pfnOpt = Goblin;
		card[iCurrentCard].pfnOptAI = GoblinAI;
	}
	for (int i = 1; i <= 4; i++) {
		iCurrentCard++;
		card[iCurrentCard].nArmory = NO_ARMORY;
		card[iCurrentCard].nColor = PURPLE;
		card[iCurrentCard].nLevel = 4;
		card[iCurrentCard].nType = ATTACK;
		card[iCurrentCard].strDescription = "指定一名对手场上所有带纹章的卡牌，由他决定交付那些卡牌到你手牌，或向你支付 1 金币。";
		card[iCurrentCard].strName = "牛头人";
		card[iCurrentCard].pfnOpt = Tauren;
		card[iCurrentCard].pfnOptAI = TaurenAI;
	}
	printf(".Done!\n");
}

void Game() {
	system("cls");
	printf("（输入 0 返回首页）\n");
	printf("玩家数量应在 2~10 范围内。\n");
	while (1) {
		printf("请输入玩家数量（输入后请按 Enter）：");
		g_nNumPlayer = InputIntSafely();
		if (g_nNumPlayer == 0) return;
		else if (g_nNumPlayer < 2 || g_nNumPlayer > 10) printf("输入的数字超出了范围！\n");
		else break;
	}
	printf("获胜金币数应不少于 1。\n");
	while (1) {
		printf("请输入获胜金币数：");
		g_nStdCoinNumber = InputIntSafely();
		if (g_nStdCoinNumber == 0) return;
		else if (g_nStdCoinNumber < 1) printf("获胜金币数应不少于 1。\n");
		else break;
	}
	/* 初始化玩家数据。 */
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
	cardAbandoned.clear(); //清空弃牌区
	g_nAbandoned = 0;
	g_bMonkMode = false;
	g_bDragonMode = false;
	/* 游戏过程在这里。 */
	DealCards();
	g_nGameStatus = 1;
	printf("正在随机决定第一回合玩家......");
	int iCurrentPlayer = rand() % g_nNumPlayer + 1;
	printf("Done!\n");
	printf("第一回合为玩家 %d。\n", iCurrentPlayer);
	printf("游戏正式开始！\n");
	system("pause");
	while (g_nGameStatus) {
		DisplayCards();
		player[iCurrentPlayer].nActionPoint = 2;
		if (iCurrentPlayer != 1) {
			printf("玩家 %d 的回合。\n", iCurrentPlayer);
			ReadyStageAI(iCurrentPlayer);
			if (CheckGameFinish()) return;
			OperationStageAI(iCurrentPlayer);
			if (CheckGameFinish()) return;
		}
		else {
			printf("你的回合。\n");
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
		printf("（按 0 返回首页）\n");
		printf("+----------+\n");
		printf("|1.卡牌说明|\n");
		printf("|2.游戏规则|\n");
		printf("|3.关于游戏|\n");
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
			printf("小传说(Hero Tales) v1.06\n");
			printf("Developed by Peter Winchester, 2024.\n\n");
			system("pause");
		}
	}
}
