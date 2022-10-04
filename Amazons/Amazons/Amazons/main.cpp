//-/----- AMAZING AMAZONS -----/-//
/* Notes: Introduction included in README.txt */
/* Author: 1900013010  Vegetable exploded. */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <easyx.h>
#include <graphics.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
using namespace std;

//----- CONSTANTS -----//
int field[8][8]; //真实棋盘,2为黑棋,3为白棋
int field_backup[8][8]; //棋盘备份

struct pos { int x; int y; };
int ais, pls; //ai与玩家的color
pos ai[4], pl[4]; //双方Queen位置
pos pl_cursor; pos pl_st, pl_ed, pl_ob; //玩家使用

int dx[8] = { 1,0,-1,0,1,1,-1,-1 };
int dy[8] = { 0,1,0,-1,1,-1,1,-1 };
int legality[8][8]; //合法的走棋位置
int cycles = 0; //循环数

//Weights for Evaluation
const double priorAdvantage = 0.2;
const double w_t1[23] = { 0.1080,0.1080,0.1235,0.1332,0.1400,0.1468,0.1565,0.1720,0.1949,0.2217,0.2476,0.2680,0.2800,0.2884,0.3000,0.3208,0.3535,0.4000,0.4613,0.5350,0.6181,0.7075,0.8000 };
const double w_t2[23] = { 0.3940,0.3940,0.3826,0.3753,0.3700,0.3647,0.3574,0.3460,0.3294,0.3098,0.2903,0.2740,0.2631,0.2559,0.2500,0.2430,0.2334,0.2200,0.2020,0.1800,0.1550,0.1280,0.1000 };
//const double w_p1[23] = { 0.1160,0.1160,0.1224,0.1267,0.1300,0.1333,0.1376,0.1440,0.1531,0.1640,0.1754,0.1860,0.1944,0.1995,0.2000,0.1950,0.1849,0.1700,0.1510,0.1287,0.1038,0.0773,0.0500 };
//const double w_p2[23] = { 0.1160,0.1160,0.1224,0.1267,0.1300,0.1333,0.1376,0.1440,0.1531,0.1640,0.1754,0.1860,0.1944,0.1995,0.2000,0.1950,0.1849,0.1700,0.1510,0.1287,0.1038,0.0773,0.0500 };
//const double w_m[23] = { 0.2300,0.2300,0.2159,0.2067,0.2000,0.1933,0.1841,0.1700,0.1496,0.1254,0.1010,0.0800,0.0652,0.0557,0.0500,0.0464,0.0436,0.0400,0.0346,0.0274,0.0190,0.0097,0 };

double t_lim = 1; //AI单步限时(单位为秒)

//Mouse
int msx, msy; int msx_grid, msy_grid; //鼠标位置与所在格子

//Images
IMAGE menupic, bkpic, whitepic, blackpic; //背景图片
COLORREF DARK = RGB(50, 50, 50);
COLORREF ICY = RGB(248, 248, 255);
COLORREF SKY = RGB(30, 144, 235);

//Files
char address[50] = "D:/Amazons/Saves/save0.txt";
int flag_load = 0;

//Graphic Sizes
const int canvas_width = 960, canvas_height = 540; //窗口尺寸
const int text_height = 16;
const int butth = 40, buttw = 200; //按键大小
const int bpoint1 = 54, bpoint2 = 486; //棋盘顶点坐标
const int center = 270;
const int gridsiz = 54;

//Discriptions
char dis_menu[4][10] = { "新游戏", "载入存档", "设置", "退出" };
char dis_select[50] = { "请选择执白或执黑." };
char dis_pause[20] = { "点击以继续游戏." };
char dis_quit[2][50] = { "确认要退出吗?您的进度将不会被保存.", "确定" };
char dis_board[3][15] = { "保存游戏", "暂停", "退出至主菜单" };
char dis_result[2][15] = { "你赢了!", "很遗憾,你输了." };
char dis_back[5] = { "返回" };
char dis_savesuc[20] = { "保存成功!" };
char dis_color[30] = { "你执子的颜色" };
char dis_yourturn[10] = { "你的回合" };
char dis_thinking[50] = { "人工智障思考中..." };
char dis_option[10] = { "选  项" };
char dis_lang[10] = { "语言" };
char dis_cg[20] = { "播放开场动画" };
char dis_true[5] = { "是" }, dis_false[5] = { "否" };
char dis_diff[20] = { "难度" };
char dis_diffi[3][10] = { "简单", "普通", "困难" };
char dis_restart[70] = { "此界面的一切更改会在重新启动后生效." };
char dis_apply[20] = { "应用更改并返回" };

//Character Drawings
const int amazing[7][41] = {
{0,1,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,0,1,0,0,1,1,1,1},
{1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,0},
{1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,0},
{1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,1,0},
{1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1},
{1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1},
{1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,1,1,0,0,1,1,1,0}
};
const int amazons[7][41] = {
{0,1,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,0,1,1,0,0,1,0,0,1,1,1,1},
{1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0},
{1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0},
{1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,1,1,1,0},
{1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,1},
{1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,1},
{1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,0,0,1,0,0,1,1,0,1,1,1,1,0}
};

//Settings
int lang = 0; //语言
int cg = 1; //是否播放开始CG
int diff = 2; //难度

//----- FUNCTIONS -----//
void illus_set() { //绘图界面初始化
	initgraph(canvas_width, canvas_height);
	settextstyle(text_height, 0, 0);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	loadimage(&menupic, _T("D:/Amazons/Resources/MenuPic.png"), canvas_width, canvas_height, FALSE);
	loadimage(&bkpic, _T("D:/Amazons/Resources/BkPic.png"), canvas_width, canvas_height, FALSE);
	loadimage(&whitepic, _T("D:/Amazons/Resources/White.png"), 0.5*canvas_width, canvas_height, FALSE);
	loadimage(&blackpic, _T("D:/Amazons/Resources/Black.png"), 0.5*canvas_width, canvas_height, FALSE);
	//loadimage(&easyx, _T("D:/Amazons/Resources/Easyx.png"), 240, 100, FALSE);
}
void language_set(int language = 0) { //语言, 默认为简体中文
	LOGFONT word;
	if (language == 0) { //简体中文
		settextstyle(text_height, 0, _T("幼圆"), 0, 0, 600, false, false, false);
		gettextstyle(&word);
		word.lfQuality = ANTIALIASED_QUALITY;
		settextstyle(&word);
	}
	else if (language == 1) { //English
		ifstream lan("D:/Amazons/Localization/English.txt");
		if (!lan) { return; }
		char c;
		for (int i = 0; i < 4; ++i) { lan.getline(dis_menu[i], 10); c = lan.get(); }
		lan.getline(dis_select, 50); c = lan.get();
		lan.getline(dis_pause, 20); c = lan.get();
		for (int i = 0; i < 2; ++i) { lan.getline(dis_quit[i], 50); c = lan.get(); }
		for (int i = 0; i < 3; ++i) { lan.getline(dis_board[i], 15); c = lan.get(); }
		for (int i = 0; i < 2; ++i) { lan.getline(dis_result[i], 15); c = lan.get(); }
		lan.getline(dis_back, 5); c = lan.get();
		lan.getline(dis_savesuc, 20); c = lan.get();
		lan.getline(dis_color, 30); c = lan.get();
		lan.getline(dis_yourturn, 10); c = lan.get();
		lan.getline(dis_thinking, 50); c = lan.get();
		lan.getline(dis_option, 10); c = lan.get();
		lan.getline(dis_lang, 10); c = lan.get();
		lan.getline(dis_cg, 20); c = lan.get();
		lan.getline(dis_true, 5); c = lan.get();
		lan.getline(dis_false, 5); c = lan.get();
		lan.getline(dis_diff, 20); c = lan.get();
		for (int i = 0; i < 3; ++i) { lan.getline(dis_diffi[i], 10); c = lan.get(); }
		lan.getline(dis_restart, 70); c = lan.get();
		lan.getline(dis_apply, 20); c = lan.get();
		lan.close();
	}
}
void global_set() { //读取设置文件
	ifstream settings("D:/Amazons/GlobalSettings.txt");
	if (!settings) { return; }
	char cinset[100];
	//Language
	settings >> cinset;
	if (strstr(cinset, "English")) { lang = 1; }
	else { lang = 0; }
	language_set(lang);
	//PlayCG
	settings >> cinset;
	if (strstr(cinset, "false")) { cg = 0; }
	//Difficulty
	settings >> cinset;
	if (strstr(cinset, "easy")) { diff = 0;  t_lim = 0.3; }
	else if (strstr(cinset, "normal")) { diff = 1;  t_lim = 0.9; }
	else if (strstr(cinset, "hard")) { diff = 2; t_lim = 1.5; }
	settings.close();
}
void initialSet() { //程序初始化
	illus_set();
	global_set();
}

//---UI Illustration---//
void illus_amaz_lines() { //给开场CG画线
	for (int i = 0; i < 42; ++i) {
		line(70 + 20 * i, 200, 70 + 20 * i, 340);
	}
	for (int i = 0; i < 8; ++i) {
		line(70, 200 + 20 * i, 890, 200 + 20 * i);
	}
}
void illus_amaz(int flag) { //拙劣的开场CG
	//70,150,890,390
	if (!flag) { return; }
	setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 1);
	setlinecolor(DARK);
	for (int i = 0; i < 42; ++i) {
		line(70 + 20 * i, 200, 70 + 20 * i, 340);
		Sleep(10);
	}
	for (int i = 0; i < 8; ++i) {
		line(70, 200 + 20 * i, 890, 200 + 20 * i);
		Sleep(30);
	}
	Sleep(100);
	setfillcolor(WHITE);
	for (int i = 0; i < 41; ++i) {
		for (int j = 0; j < 7; ++j) {
			if (amazing[j][i]) {
				fillcircle(80 + 20 * i, 210 + 20 * j, 8);
			}
		}
		Sleep(25);
	}
	Sleep(1000);
	for (int q = 0; q < 7; ++q) {
		Sleep(100);
		cleardevice();
		for (int i = 0; i < 7; ++i) {
			for (int j = 0; j < 41; ++j) {
				if (amazing[i][j] && (j<24 || i>q)) {
					fillcircle(80 + 20 * j, 210 + 20 * i, 8);
				}
			}
		}
		illus_amaz_lines();
	}
	Sleep(150);
	for (int q = 0; q < 7; ++q) {
		Sleep(100);
		cleardevice();
		for (int i = 0; i < 7; ++i) {
			for (int j = 0; j < 41; ++j) {
				if (amazons[i][j] && (j < 24 || i <= q)) {
					fillcircle(80 + 20 * j, 210 + 20 * i, 8);
				}
			}
		}
		illus_amaz_lines();
	}
	Sleep(1000);
	COLORREF TEMP = RGB(0, 0, 0);
	int light = 0;
	while (light < 255) {
		cleardevice();
		light += 15;
		TEMP = RGB(light, light, light);
		setlinecolor(TEMP);
		setbkcolor(TEMP);
		for (int i = 0; i < 7; ++i) {
			for (int j = 0; j < 41; ++j) {
				if (amazons[i][j]) { fillcircle(80 + 20 * j, 210 + 20 * i, 8); }
			}
		}
		Sleep(60);
	}
	setfillcolor(WHITE);
	fillrectangle(0, 0, 960, 540);
	Sleep(600);
	setlinecolor(ICY);
}
void illus_main() { //显示主菜单
	cleardevice();
	putimage(0, 0, &menupic);
	setfillcolor(ICY);
	/*
	fillrectangle(100, 105, 350, 145);
	fillrectangle(100, 250, 350, 290);
	fillrectangle(100, 395, 350, 435);
	*/
	int sta[4] = { -250,-350,-450,-550 };
	while (1) { //拙劣的动画效果
		cleardevice();
		putimage(0, 0, &menupic);
		fillrectangle(sta[0], 85, sta[0] + 250, 125);
		fillrectangle(sta[1], 195, sta[1] + 250, 235);
		fillrectangle(sta[2], 305, sta[2] + 250, 345);
		fillrectangle(sta[3], 415, sta[3] + 250, 455);
		if (sta[3] == 100) { break; }
		for (int i = 0; i < 4; ++i) {
			if (sta[i] < 100) { sta[i] += 50; }
		}
		Sleep(30);
	}
	outtextxy(120, 95, dis_menu[0]);
	outtextxy(120, 205, dis_menu[1]);
	outtextxy(120, 315, dis_menu[2]);
	outtextxy(120, 425, dis_menu[3]);
}
void illus_settings() { //显示设置画面
	cleardevice();
	putimage(0, 0, &bkpic);
	line(10, 130, 950, 130);
	LOGFONT t;
	gettextstyle(&t);
	t.lfHeight = 50;
	settextstyle(&t);
	outtextxy(400, 40, dis_option);
	t.lfHeight = text_height;
	settextstyle(&t);
	//language
	outtextxy(200, 190, dis_lang);
	if (lang == 1) {
		setfillcolor(WHITE); fillcircle(400, 200, 10);
		setfillcolor(YELLOW); fillcircle(600, 200, 10);
	}
	else {
		setfillcolor(YELLOW); fillcircle(400, 200, 10);
		setfillcolor(WHITE); fillcircle(600, 200, 10);
	}
	outtextxy(430, 190, "简体中文"); outtextxy(630, 190, "English");
	//difficulty
	outtextxy(200, 270, dis_diff);
	for (int i = 0; i < 3; ++i) {
		if (i == diff) { setfillcolor(YELLOW); }
		else { setfillcolor(WHITE); }
		fillcircle(150 * i + 400, 280, 10);
		outtextxy(150 * i + 430, 270, dis_diffi[i]);
	}
	//palycg
	outtextxy(200, 350, dis_cg);
	if (cg == 0) {
		setfillcolor(WHITE); fillcircle(400, 360, 10);
		setfillcolor(YELLOW); fillcircle(600, 360, 10);
	}
	else {
		setfillcolor(YELLOW); fillcircle(400, 360, 10);
		setfillcolor(WHITE); fillcircle(600, 360, 10);
	}
	outtextxy(430, 350, dis_true); outtextxy(630, 350, dis_false);
	outtextxy(200, 450, dis_restart);
	setfillcolor(ICY);
	fillrectangle(560, 440, 760, 480);
	outtextxy(590, 450, dis_apply);
}
void illus_saveload() { //显示存档画面
	cleardevice();
	putimage(0, 0, &bkpic);
	setfillcolor(ICY);
	char sv[6] = "0";
	for (int i = 1; i < 6; ++i) {
		fillrectangle(180, 90 * i - 20, 380, 90 * i + 20);
		sv[0] = i + '0';
		outtextxy(200, 90 * i - 10, sv);
	}
	for (int i = 6; i < 10; ++i) {
		fillrectangle(580, 90 * i - 470, 780, 90 * i - 430);
		sv[0] = i + '0';
		outtextxy(600, 90 * i - 460, sv);
	}
	fillrectangle(580, 430, 780, 470);
	outtextxy(600, 440, dis_back);
}
void illus_sideselect() { //显示选边画面
	cleardevice();
	putimage(0, 0, &whitepic);
	putimage(0.5*canvas_width, 0, &blackpic);
	outtext(dis_select);
}
void illus_pause() { //显示暂停画面
	putimage(0, 0, &bkpic);
	outtext(dis_pause);
}
void illus_quit() { //显示退出时提醒
	cleardevice();
	putimage(0, 0, &bkpic);
	outtextxy(345, 240, dis_quit[0]);
	setfillcolor(ICY);
	fillrectangle(440, 280, 520, 320); outtextxy(455, 290, dis_quit[1]);
}
void illus_chessboard(int isturn = 1, int f = 0) { //绘制游戏界面
	cleardevice();
	putimage(0, 0, &bkpic);
	int sleeptime = 30;
	for (int i = 0; i < 9; ++i) {
		line(bpoint1, bpoint1 + gridsiz * i, bpoint2, bpoint1 + gridsiz * i);
		if (f) { Sleep(sleeptime); }
	}
	for (int i = 0; i < 9; ++i) {
		line(bpoint1 + gridsiz * i, bpoint1, bpoint1 + gridsiz * i, bpoint2);
		if (f) { Sleep(sleeptime); }
	}
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (field[i][j] == 1) {
				setfillcolor(SKY);
				fillcircle(bpoint1 + gridsiz * 0.5 + gridsiz * i, bpoint1 + gridsiz * 0.5 + gridsiz * j, 23);
				if (f) { Sleep(sleeptime); }
			}
			if (field[i][j] == 2) {
				setfillcolor(BLACK);
				fillcircle(bpoint1 + gridsiz * 0.5 + gridsiz * i, bpoint1 + gridsiz * 0.5 + gridsiz * j, 23);
				if (f) { Sleep(sleeptime); }
			}
			if (field[i][j] == 3) {
				setfillcolor(WHITE);
				fillcircle(bpoint1 + gridsiz * 0.5 + gridsiz * i, bpoint1 + gridsiz * 0.5 + gridsiz * j, 23);
				if (f) { Sleep(sleeptime); }
			}
		}
	}
	if (pls == 2) { setfillcolor(BLACK); }
	else { setfillcolor(WHITE); }
	fillcircle(570, 168, 15);
	outtextxy(610, 158, dis_color);
	if (isturn) {
		setfillcolor(YELLOW);
		outtextxy(610, 362, dis_yourturn);
	}
	else {
		setfillcolor(SKY);
		outtextxy(610, 362, dis_thinking);
	}
	fillcircle(570, 372, 15);
	setfillcolor(ICY);
	int dist = (bpoint2 - bpoint1 - 3 * butth) / 2;
	int btlft = 630;
	fillrectangle(btlft, bpoint1, btlft + buttw, bpoint1 + butth);
	outtextxy(btlft + 20, bpoint1 + 10, dis_board[0]);
	fillrectangle(btlft, bpoint1 + butth + dist, btlft + buttw, bpoint2 - butth - dist);
	outtextxy(btlft + 20, bpoint1 + butth + dist + 10, dis_board[1]);
	fillrectangle(630, bpoint2 - butth, btlft + buttw, bpoint2);
	outtextxy(btlft + 20, bpoint2 - butth + 10, dis_board[2]);
}
void illus_final(int feature) { //显示终局胜负
	setfillcolor(ICY);
	fillrectangle(center - 100, center - 20, center + 100, center + 20);
	if (feature) { outtextxy(center - 80, center - 10, dis_result[0]); }
	else { outtextxy(center - 80, center - 10, dis_result[1]); }
}
void illus_legality() { //标记合法棋步
	illus_chessboard();
	setfillcolor(YELLOW);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (legality[i][j] == 1) {
				fillrectangle(bpoint1 + gridsiz * i, bpoint1 + gridsiz * j, bpoint1 + gridsiz * i + gridsiz, bpoint1 + gridsiz * j + gridsiz);
			}
		}
	}
}

//---Mouse Control---//
void get_mouse() { //获取鼠标位置
	while (1) {
		MOUSEMSG msp = GetMouseMsg();
		if (msp.mkLButton) {
			msx = msp.x; msy = msp.y;
			FlushMouseMsgBuffer();
			return;
		}
	}
}
void get_mouse_grid() { //计算鼠标所在格子
	msx_grid = (msx - bpoint1) / gridsiz;
	msy_grid = (msy - bpoint1) / gridsiz;
}

//---File Service---//
int get_saveid() { //获取存档编号
	illus_saveload();
	get_mouse();
	if (msx > 180 && msx < 380 && msy > 70 && msy < 110) { return 1; }
	if (msx > 180 && msx < 380 && msy > 160 && msy < 200) { return 2; }
	if (msx > 180 && msx < 380 && msy > 250 && msy < 290) { return 3; }
	if (msx > 180 && msx < 380 && msy > 340 && msy < 380) { return 4; }
	if (msx > 180 && msx < 380 && msy > 430 && msy < 470) { return 5; }
	if (msx > 580 && msx < 780 && msy > 70 && msy < 110) { return 6; }
	if (msx > 580 && msx < 780 && msy > 160 && msy < 200) { return 7; }
	if (msx > 580 && msx < 780 && msy > 250 && msy < 290) { return 8; }
	if (msx > 580 && msx < 780 && msy > 340 && msy < 380) { return 9; }
	if (msx > 580 && msx < 780 && msy > 430 && msy < 470) { return 0; }
	return get_saveid();
}
void save() {
	int id = get_saveid();
	if (id) {
		address[21] = id + '0';
		ofstream f0(address);
		f0 << pls << endl; ais = 5 - pls;
		f0 << cycles - 1 << endl;
		for (int i = 0; i < 4; ++i) {
			f0 << pl[i].x << ' ' << pl[i].y << endl;
		}
		for (int i = 0; i < 4; ++i) {
			f0 << ai[i].x << ' ' << ai[i].y << endl;
		}
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				f0 << field[i][j] << ' ';
			}
		}
		f0.close();
		outtext(dis_savesuc);
		Sleep(1000);
	}
}
int load() {
	int id = get_saveid();
	if (id) {
		address[21] = id + '0';
		ifstream f0(address);
		if (!f0) { return 0; }
		f0 >> pls; ais = 5 - pls;
		f0 >> cycles;
		for (int i = 0; i < 4; ++i) {
			f0 >> pl[i].x; f0 >> pl[i].y;
		}
		for (int i = 0; i < 4; ++i) {
			f0 >> ai[i].x; f0 >> ai[i].y;
		}
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				f0 >> field[i][j];
			}
		}
		f0.close(); return 1;
	}
	return 0;
}

//---Options---//
void options_apply() {
	ofstream sett("D:/Amazons/GlobalSettings.txt");
	sett << "Language=";
	if (lang == 1) { sett << "English"; }
	else { sett << "Simplified_Chinese"; }
	sett << endl;
	sett << "PlayCG=";
	if (cg) { sett << "true"; }
	else { sett << "false"; }
	sett << endl;
	sett << "Difficulty=";
	if (diff == 2) { sett << "hard"; }
	else if (diff == 1) { sett << "normal"; }
	else { sett << "easy"; }
	sett.close();
}
void options_set() {
	while (true) {
		illus_settings();
		get_mouse();
		if (msx > 560 && msx < 760 && msy > 440 && msy < 480) {
			options_apply(); return;
		}
		else if (fabs(msx - 400) < 10 && fabs(msy - 200) < 10) { lang = 0; }
		else if (fabs(msx - 600) < 10 && fabs(msy - 200) < 10) { lang = 1; }
		else if (fabs(msx - 400) < 10 && fabs(msy - 280) < 10) { diff = 0; }
		else if (fabs(msx - 550) < 10 && fabs(msy - 280) < 10) { diff = 1; }
		else if (fabs(msx - 700) < 10 && fabs(msy - 280) < 10) { diff = 2; }
		else if (fabs(msx - 400) < 10 && fabs(msy - 360) < 10) { cg = 1; }
		else if (fabs(msx - 600) < 10 && fabs(msy - 360) < 10) { cg = 0; }
	}
}

//---Other Functions---//
void initialize() { //开始新游戏
	memset(field, 0, sizeof(field));
	field[0][2] = 2; field[2][0] = 2;
	field[5][0] = 2; field[7][2] = 2;
	field[0][5] = 3; field[2][7] = 3;
	field[5][7] = 3; field[7][5] = 3;
	if (ais == 2) {
		ai[0].x = 0; ai[0].y = 2; ai[1].x = 2; ai[1].y = 0;
		ai[2].x = 5; ai[2].y = 0; ai[3].x = 7; ai[3].y = 2;
		pl[0].x = 0; pl[0].y = 5; pl[1].x = 2; pl[1].y = 7;
		pl[2].x = 5; pl[2].y = 7; pl[3].x = 7; pl[3].y = 5;
	}
	else {
		pl[0].x = 0; pl[0].y = 2; pl[1].x = 2; pl[1].y = 0;
		pl[2].x = 5; pl[2].y = 0; pl[3].x = 7; pl[3].y = 2;
		ai[0].x = 0; ai[0].y = 5; ai[1].x = 2; ai[1].y = 7;
		ai[2].x = 5; ai[2].y = 7; ai[3].x = 7; ai[3].y = 5;
	}
	cycles = 0;
}
void backup() {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			field_backup[i][j] = field[i][j];
		}
	}
}
void upback() {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			field[i][j] = field_backup[i][j];
		}
	}
}

bool lose(int side) { //判断游戏是否结束
	if (side == pls) {
		for (int q = 0; q < 4; ++q) {
			for (int i = 0; i < 8; ++i) {
				int cx = pl[q].x + dx[i], cy = pl[q].y + dy[i];
				if (field[cx][cy] == 0 && cx >= 0 && cy >= 0 && cx < 8 && cy < 8) { return false; }
			}
		}
	}
	else if (side == ais) {
		for (int q = 0; q < 4; ++q) {
			for (int i = 0; i < 8; ++i) {
				int cx = ai[q].x + dx[i], cy = ai[q].y + dy[i];
				if (field[cx][cy] == 0 && cx >= 0 && cy >= 0 && cx < 8 && cy < 8) { return false; }
			}
		}
	}
	return true;
}

void refresh_queen() { //基于field刷新Queen列表
	int seq = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (field[i][j] == pls) {
				pl[seq].x = i; pl[seq].y = j;
				seq++;
			}
		}
	}
	seq = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (field[i][j] == ais) {
				ai[seq].x = i; ai[seq].y = j;
				seq++;
			}
		}
	}
}
void refresh_field() {
	for (int q = 0; q < 4; ++q) {
		field[ai[q].x][ai[q].y] = ais;
		field[pl[q].x][pl[q].y] = pls;
	}
}
int refresh_legality(int x, int y) { //获取合法下棋位置
	memset(legality, 0, sizeof(legality));
	int cx = x; int cy = y;
	int freedom = 0;
	for (int q = 0; q < 8; ++q) {
		cx = x; cy = y;
		while ((field[cx][cy] == 0 || (cx == x && cy == y)) && cx >= 0 && cy >= 0 && cx < 8 && cy < 8) {
			legality[cx][cy] = 1; freedom++;
			cx += dx[q], cy += dy[q];
		}
	}
	legality[x][y] = 0;
	return freedom;
}

//---Basic Functions for AI---//
void QueenMove(int Q[8][8], int field[8][8], int side) { //计算Queenmove
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			Q[i][j] = 32767;
			if (field[i][j] == side) {
				Q[i][j] = 0;
			}
		}
	}
	for (int l = 0; l < 7; ++l) {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (Q[i][j] == l) {
					for (int k = 0; k < 8; ++k) {
						int cx = i + dx[k]; int cy = j + dy[k];
						while (field[cx][cy] == 0 && cx >= 0 && cy >= 0 && cx < 8 && cy < 8) {
							if (Q[cx][cy] == 32767) {
								Q[cx][cy] = l + 1;
							}
							cx += dx[k], cy += dy[k];
						}
					}
				}
			}
		}
	}
}
void KingMove(int K[8][8], int field[8][8], int side) { //计算Kingmove
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			K[i][j] = 32767;
			if (field[i][j] == side) {
				K[i][j] = 0;
			}
		}
	}
	for (int l = 0; l < 13; ++l) {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (K[i][j] == l) {
					for (int k = 0; k < 8; ++k) {
						int cx = i + dx[k]; int cy = j + dy[k];
						if (field[cx][cy] == 0 && cx >= 0 && cy >= 0 && cx < 8 && cy < 8) {
							if (K[cx][cy] == 32767) { K[cx][cy] = l + 1; }
						}
					}
				}
			}
		}
	}
}

int Mobility(int field[8][8], int side) { //计算Mobility(已弃置)
	int m = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (field[i][j] == side) {
				for (int q = 0; q < 8; ++q) {
					int cx = i + dx[q]; int cy = j + dy[q];
					while (field[cx][cy] == 0 && cx >= 0 && cy >= 0 && cx < 8 && cy < 8) {
						m++;
						cx += dx[q], cy += dy[q];
					}
				}
			}
		}
	} return m;
}

//---Evaluation---//
inline double Min(double a, double b) {
	return a > b ? b : a;
}
inline double Max(double a, double b) {
	return a > b ? a : b;
}

inline double DeltaT(int me, int rival) {
	if (me == 32767 && rival == 32767) { return 0; }
	if (me == rival) { return -priorAdvantage; }
	if (rival == 32767) { return 1.3; }
	if (me == 32767) { return -1.3; }
	if (me < rival) { return 1; }
	if (me > rival) { return -1; }
	return 0;
}
inline int Twice(int n) {
	if (n < 20) { return 1 << n; }
	return 1048576;
}

double Evaluation(int field[8][8], int side, int stn) { //评估函数(只考虑Territory)
	stn += cycles;
	if (stn > 22) { stn = 22; }
	//Prepare Q&K Move
	int myQ[8][8], riQ[8][8];
	int myK[8][8], riK[8][8];
	QueenMove(myQ, field, side); KingMove(myK, field, side);
	QueenMove(riQ, field, 5 - side); KingMove(riK, field, 5 - side);
	//Territory and Position
	double tq = 0, tk = 0;
	//double pq=0, pk=0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			tq += DeltaT(myQ[i][j], riQ[i][j]);
			tk += DeltaT(myK[i][j], riK[i][j]);
			//pq += 2.0/Twice(myQ[i][j]) -2.0/Twice(riQ[i][j]);
			//pk += Min(1.0, Max(-1.0, 1.0*(riK[i][j]-myK[i][j])/6));
		}
	}
	//double m = Mobility(field, side)-Mobility(field, 5-side);
	double val = w_t1[stn] * tq + w_t2[stn] * tk;// + w_p1[stn]*pq + w_p2[stn]*pk + w_m[stn]*m;
	return val;
}

//---Tree Search---//
clock_t start;
struct ACT {
	int x0; int y0;
	int x1; int y1;
	int x2; int y2;
	double eva;
}; //将(x0,y0)处的棋子移到(x1,y1), 并在(x2,y2)处放置障碍
bool ActCompare(ACT a, ACT b) { //用于sort函数
	return (a.eva > b.eva);
}

inline void Execute(int field[8][8], ACT a, int side) { //执行一个Act
	field[a.x0][a.y0] = 0;
	field[a.x1][a.y1] = side;
	field[a.x2][a.y2] = 1;
}
inline void Rewind(int field[8][8], ACT a, int side) { //撤销一个Act
	field[a.x2][a.y2] = 0;
	field[a.x1][a.y1] = 0;
	field[a.x0][a.y0] = side;
}

double TreeSearch(int field[8][8], int side, double alpha, double beta, int depth, int plan) {
	if (depth == 0 || clock() - start > CLOCKS_PER_SEC*t_lim) {
		return Evaluation(field, side, (plan - depth) / 2);
	}
	ACT moves[2000] = { 0 };
	//Get Positions of Queens
	pos me[4];
	int q1 = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (field[i][j] == side) {
				me[q1].x = i; me[q1].y = j;
				q1++;
			}
			if (q1 == 4) {
				i = 8; break;
			}
		}
	}
	//Generate Legal Moves
	int tot = 0;
	for (int p = 0; p < 4; ++p) {
		int stx = me[p].x, sty = me[p].y;
		for (int q = 0; q < 8; ++q) {
			int cx = stx + dx[q]; int cy = sty + dy[q];
			while (field[cx][cy] == 0 && cx >= 0 && cy >= 0 && cx < 8 && cy < 8) {
				for (int r = 0; r < 8; ++r) {
					int ox = cx + dx[r]; int oy = cy + dy[r];
					while ((field[ox][oy] == 0 && ox >= 0 && oy >= 0 && ox < 8 && oy < 8) || (ox == stx && oy == sty)) {
						moves[tot].x0 = stx; moves[tot].y0 = sty;
						moves[tot].x1 = cx; moves[tot].y1 = cy;
						moves[tot].x2 = ox; moves[tot].y2 = oy;
						tot++;
						ox += dx[r]; oy += dy[r];
					}
				}
				cx += dx[q], cy += dy[q];
			}
		}
	}
	if (tot == 0) { return -32766; }
	//Search
	double opt = -32767; double val;
	for (int i = 0; i < tot; ++i) {
		Execute(field, moves[i], side);
		val = -TreeSearch(field, 5 - side, -beta, -alpha, depth - 1, plan);
		if (val > opt) {
			opt = val;
		}
		Rewind(field, moves[i], side);
		//Alpha-Beta
		if (opt > alpha) {
			alpha = opt;
		}
		if (alpha >= beta) { break; }
	}
	return opt;
}

ACT Heuristic(int field[8][8], int side) { //First Layer
	ACT strategy[2000] = { 0 };
	//Get Positions of Queens
	pos me[4];
	int q1 = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (field[i][j] == side) {
				me[q1].x = i; me[q1].y = j;
				q1++;
			}
			if (q1 == 4) {
				i = 8; break;
			}
		}
	}
	//Generate Legal Moves
	int tot = 0;
	for (int p = 0; p < 4; ++p) {
		int stx = me[p].x, sty = me[p].y;
		for (int q = 0; q < 8; ++q) {
			int cx = stx + dx[q]; int cy = sty + dy[q];
			while (field[cx][cy] == 0 && cx >= 0 && cy >= 0 && cx < 8 && cy < 8) {
				for (int r = 0; r < 8; ++r) {
					int ox = cx + dx[r]; int oy = cy + dy[r];
					while ((field[ox][oy] == 0 && ox >= 0 && oy >= 0 && ox < 8 && oy < 8) || (ox == stx && oy == sty)) {
						strategy[tot].x0 = stx; strategy[tot].y0 = sty;
						strategy[tot].x1 = cx; strategy[tot].y1 = cy;
						strategy[tot].x2 = ox; strategy[tot].y2 = oy;
						Execute(field, strategy[tot], side);
						strategy[tot].eva = Evaluation(field, side, 0);
						Rewind(field, strategy[tot], side);
						tot++;
						ox += dx[r]; oy += dy[r];
					}
				}
				cx += dx[q], cy += dy[q];
			}
		}
	}
	sort(strategy, strategy + tot, ActCompare);
	//Iterative Deepening & History Heuristic Strategy
	int layer = 0, i = 0;
	while (true) {
		layer++;
		for (i = 0; i < tot; ++i) {
			if (clock() - start > CLOCKS_PER_SEC*t_lim) { break; }
			Execute(field, strategy[i], side);
			strategy[i].eva = -TreeSearch(field, 5 - side, -32767, 32767, layer, layer);
			Rewind(field, strategy[i], side);
		}
		sort(strategy, strategy + i, ActCompare);
		if (clock() - start > CLOCKS_PER_SEC*t_lim) { break; }
	}
	return strategy[0];
}

void ArtificialAmentia(int field[8][8], int side) { //执行AI的决策
	ACT ai = Heuristic(field, side);
	Execute(field, ai, side);
}

void FirstStrategy() { //黑棋第一步的快速落子方案
	int seed = rand();
	if (seed < 16300) { field[2][0] = 0; field[2][6] = 2; field[5][3] = 1; }
	else { field[5][0] = 0; field[5][6] = 2; field[2][3] = 1; }
	refresh_queen();
}

//----- MAIN PROGRAM -----//
int main() {
	initialSet();
	illus_amaz(cg);
	srand(time(NULL));
	//---Main Menu---//
start: illus_main();
	//main menu options
start1: get_mouse();
	if (msx > 100 && msx < 350 && msy > 85 && msy < 125) { //New game
		illus_sideselect();
		get_mouse();
		if (msx < 0.5 * canvas_width) { ais = 2; }
		else { ais = 3; }
		pls = 5 - ais;
		initialize();
		if (ais == 2) {
			illus_chessboard(0, 1);
			Sleep(1000); FirstStrategy();
		}
		else { illus_chessboard(1, 1); }
	}
	else if (msx > 100 && msx < 350 && msy > 195 && msy < 235) { //Load game
		int load_suc = load();
		if (load_suc == 0) { goto start; }
		else { illus_chessboard(1, 1); flag_load = 1; }
	}
	else if (msx > 100 && msx < 350 && msy > 305 && msy < 345) { //Options
		options_set();
		goto start;
	}
	else if (msx > 100 && msx < 350 && msy > 415 && msy < 455) { //Quit
		return 0;
	}
	else { goto start1; }
	//--- In Game---//
	while (1) {
		if (flag_load) { flag_load = 0; }
		cycles++;
		//---Player Decision Phase---//
	layer1: illus_chessboard();
		backup();
		get_mouse(); //select queen
		if (msx > bpoint1 && msx < bpoint2 && msy > bpoint1 && msy < bpoint2) { //chessboard
			get_mouse_grid();
			if (field[msx_grid][msy_grid] == pls) {
				pl_cursor.x = msx_grid; pl_cursor.y = msy_grid;
				pl_st.x = msx_grid; pl_st.y = msy_grid;
				refresh_legality(msx_grid, msy_grid);
				illus_legality();
			layer2: get_mouse(); //select destination
				if (msx > bpoint1 && msx < bpoint2 && msy > bpoint1 && msy < bpoint2) {
					get_mouse_grid();
					if (legality[msx_grid][msy_grid]) {
						pl_cursor.x = msx_grid; pl_cursor.y = msy_grid;
						pl_ed.x = msx_grid; pl_ed.y = msy_grid;
						field[pl_st.x][pl_st.y] = 0; field[pl_ed.x][pl_ed.y] = pls;
						refresh_legality(msx_grid, msy_grid);
						illus_legality();
					layer3: get_mouse(); //select obstacle
						if (msx > bpoint1 && msx < bpoint2 && msy > bpoint1 && msy < bpoint2) {
							get_mouse_grid();
							if (legality[msx_grid][msy_grid]) {
								pl_ob.x = msx_grid; pl_ob.y = msy_grid;
								//realize orders
								field[pl_ob.x][pl_ob.y] = 1;
								refresh_queen();
								illus_chessboard();
							}
							else if (msx_grid == pl_ed.x && msy_grid == pl_ed.y) { upback();  goto layer1; } //cancel input
							else { goto layer3; } //illegal input
						}
						else { goto layer3; } //out of chessboard
					}
					else if (msx_grid == pl_st.x && msy_grid == pl_st.y) { upback();  goto layer1; } //cancel input
					else { goto layer2; } //illegal input
				}
				else { goto layer2; } //out of chessboard
			}
			else { goto layer1; } //not selecting player's entity
		}
		else if (msx > 630 && msx < 630 + buttw && msy > bpoint1 && msy < bpoint1 + butth) { //save
			save();
			goto layer1;
		}
		else if (msx > 630 && msx < 630 + buttw && msy > center - 0.5*butth && msy < center + 0.5*butth) { //pause
			illus_pause();
			get_mouse();
			goto layer1;
		}
		else if (msx > 630 && msx < 630 + buttw && msy > bpoint2 - butth && msy < bpoint2) { //quit
			illus_quit();
			get_mouse();
			if (msx > 440 && msx < 520 && msy > 280 && msy < 320) { goto start; }
			else { goto layer1; }
		}
		else { goto layer1; } //out of chessboard and buttons
		if (lose(ais)) { //you win!
			illus_final(1);
			get_mouse(); goto layer1;
		}
		refresh_queen();
		//---AI Decision Phase---//
		illus_chessboard(0);
		start = clock();
		ArtificialAmentia(field, ais);
		refresh_queen();
		illus_chessboard();
		if (lose(pls)) { //you lose!
			illus_final(0);
			get_mouse(); goto layer1;
		}
	}
}