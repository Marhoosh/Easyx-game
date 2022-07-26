#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
#include<time.h>
#include<Windows.h>
#include<stdio.h>

#define GRAPHHEIGHT		20	//单位：棋子  
#define GRAPHWIDTH		16	//图形界面宽16，是为了在右边留出长度为6的待填充区域
#define PLAYHEIGHT		20  //游戏界面高度
#define PLAYWIDTH		10	//游戏界面宽度
#define BLOCKSIZE		30	//单位：像素
#define BLOCKCOLOR      BLUE

int blockexist[PLAYHEIGHT][PLAYWIDTH];//0为无方块，1为固定方块，2为移动方块
int statunum;//哪一种状态，6为正方形
int nextstatunum;
bool gameover;//游戏是否结束
int score;//得分
bool pause;//是否暂停

struct block {
	int x;
	int y;
}blocks[4];

struct block nextblocks[4];

int statu[7][4] = {
	1,3,5,7,
	2,4,5,7,
	3,5,4,6,
	3,5,4,7,
	2,3,5,7,
	3,5,7,6,
	2,3,4,5,
};

void updateblocks() {//更新棋盘中移动方块的位置
	for (int i = 0;i < PLAYHEIGHT;i++) {
		for (int j = 0;j < PLAYWIDTH;j++) {
			if (blockexist[i][j] == 2) {
				blockexist[i][j] = 0;
			}
		}
	}
	for (int i = 0;i < 4;i++) {
		if (blocks[i].y >= 0 && blocks[i].x >= 0) {
			blockexist[blocks[i].x][blocks[i].y] = 2;
		}
	}
}

void initblocks(struct block (*tmpblocks)[4],int *tmpstatunum) {//初始随机形状并设置在中间位置,tmpblocks为传出参数
	//随机获取一种形态
	srand((unsigned)time(NULL));//加上这一行是为了防止rand()的随机规律化
	*tmpstatunum = rand() % 7;
	for (int i = 0;i < 4;i++) {
		//blocks[i].x = statu[statunum][i] / 2 ;
		//blocks[i].y = statu[statunum][i] % 2 + PLAYWIDTH / 2;
		
		(*tmpblocks)[i].x = statu[*tmpstatunum][i] / 2 - 4;
		(*tmpblocks)[i].y = statu[*tmpstatunum][i] % 2 + PLAYWIDTH / 2;
	}
	/*for (int i = 0;i < 4;i++) {
		printf("blocks坐标 %d %d\n", blocks[i].x, blocks[i].y);
	}*/

	//将形态随机旋转几次
	
	srand((unsigned)time(NULL));
	int rotatenum = rand() % 4;
	for (int i = 0;i < rotatenum;i++) {
		if (*tmpstatunum == 6)  break;  //正方形不用旋转
		block p = (*tmpblocks)[1];  //旋转中心
		for (int j = 0;j < 4;j++) {
			int x = (*tmpblocks)[j].y - p.y;
			int y = (*tmpblocks)[j].x - p.x;
			(*tmpblocks)[j].x = p.x - x;
			(*tmpblocks)[j].y = p.y + y;
		}
	}
	
	//将方块下放到棋盘正上方
	while (1) {
		int i = 0;
		while (i < 4 && (*tmpblocks)[i].x < 0) { i++; }
		if (i < 4) { break; }
		for (i = 0;i < 4;i++) { (*tmpblocks)[i].x++; }
	}
	
}

void setnextblocks() {
	for (int i = 0;i < 4;i++) {
		nextblocks[i].x += PLAYHEIGHT /2 - 3;
		nextblocks[i].y += PLAYWIDTH / 2 +1;
	}
}

void truntonextblocks() {
	statunum = nextstatunum;
	for (int i = 0;i < 4;i++) {
		blocks[i].x = nextblocks[i].x+3-PLAYHEIGHT/2;
		blocks[i].y = nextblocks[i].y - 1 - PLAYWIDTH / 2;
	}

	//检查是否可放置方块
	for (int i = 0;i < 4;i++) {
		if (blockexist[blocks[i].x][blocks[i].y]) {
			gameover = true;
			break;
		}
	}
}


bool checkblock(int x,int y){//x,y为方块坐标，检查方块位置是否合理
	//检查是否越界
	if (x>=PLAYHEIGHT || y<0 || y>=PLAYWIDTH) { return false; }
	//检查此处是否已经有方块
	if (blockexist[x][y] == 1) { return false; }
	return true;
}

bool rotate() {//旋转成功，返回true，否则返回false;
	if (statunum == 6)  return true;
	block p = blocks[1];  //旋转中心
	for (int i = 0;i < 4;i++) {
		int x = blocks[i].y - p.y;
		int y = blocks[i].x - p.x;
		x = p.x - x;
		y = p.y + y;
		if (!checkblock(x, y)) { return false; }
	}
	for (int i = 0;i < 4;i++) {
		int x = blocks[i].y - p.y;
		int y = blocks[i].x - p.x;
		x = p.x - x;
		y = p.y + y;
		blocks[i] = { x,y };
	}
	updateblocks();
	return true;
}

bool checkgodown() {//判断能否下落一个单位
	//检查下方是否存在固定方块
	for (int i = 0;i < 4;i++) {
		if (blockexist[blocks[i].x + 1][blocks[i].y] == 1) {
			return false;
		}
	}

	//检查是否到底
	for (int i = 0;i < 4;i++) {
		if (blocks[i].x + 1 == PLAYHEIGHT) {
			return false;
		}
	}
	return true;
}

bool godown() {//下落成功，返回true，否则返回false;

	if (!checkgodown())  return false;

	for (int i = 0;i < 4;i++) {
		blocks[i].x++;
	}

	updateblocks();
	return true;
}

bool goleft() {//向左移动成功，返回true，否则返回false；
	for (int i = 0;i < 4;i++) {
		if (!checkblock(blocks[i].x, blocks[i].y - 1)) { return false; }
	}
	for (int i = 0;i < 4;i++) {
		blocks[i].y--;
	}
	updateblocks();
	return true;
}

bool goright() {//向右移动成功，返回true，否则返回false；
	for (int i = 0;i < 4;i++) {
		if (!checkblock(blocks[i].x, blocks[i].y + 1)) { return false; }
	}
	for (int i = 0;i < 4;i++) {
		blocks[i].y++;
	}
	updateblocks();
	return true;
}

void stickblocks() {//将移动方块固定
	for (int i = 0;i < 4;i++) {
		blockexist[blocks[i].x][blocks[i].y] = 1;
	}
}

void DrawPlayGraph() {//绘画图形界面
	
	//清空屏幕
	cleardevice();
	//绘制棋盘
	setlinecolor(WHITE);
	for (int i = 0;i < PLAYWIDTH+1;i++) {//画竖线
		line(i* BLOCKSIZE, 0, i * BLOCKSIZE, PLAYHEIGHT * BLOCKSIZE);
	}
	for (int i = 0;i < PLAYHEIGHT + 1;i++) {//画横线
		line(0, i * BLOCKSIZE, PLAYWIDTH * BLOCKSIZE,i*BLOCKSIZE );
	}
	
	//blockexist[1][1] = true;

	//绘画棋盘上的方块
	for (int i = 0;i < PLAYHEIGHT;i++) {
		for (int j = 0;j < PLAYWIDTH;j++) {
			if (blockexist[i][j]) {
				setfillcolor(BLOCKCOLOR);
				fillrectangle(j * BLOCKSIZE, i * BLOCKSIZE, (j + 1) * BLOCKSIZE, (i + 1) * BLOCKSIZE);
			}
			/*
			else {
				clearrectangle(j * BLOCKSIZE, i * BLOCKSIZE, (j + 1) * BLOCKSIZE, (i + 1) * BLOCKSIZE);
			}
			*/
			
		}
	}

	//绘制计分区
	settextcolor(WHITE);
	TCHAR text[11] = _T("得分");
	outtextxy(PLAYWIDTH*BLOCKSIZE + 50, GRAPHHEIGHT*BLOCKSIZE / 2, text);
	_stprintf_s(text, _T("%d"), score);
	outtextxy(PLAYWIDTH * BLOCKSIZE + 50, GRAPHHEIGHT * BLOCKSIZE / 2+20, text);

	//绘制下一个将要出现的俄罗斯方块
	setfillcolor(BLOCKCOLOR);
	for (int i = 0;i < 4;i++) {
		int x = nextblocks[i].x;
		int y = nextblocks[i].y;
		//printf("nextblocks坐标 %d %d\n", x, y);
		fillrectangle(y * BLOCKSIZE, x * BLOCKSIZE, (y + 1) * BLOCKSIZE, (x + 1) * BLOCKSIZE);
	}

	FlushBatchDraw();
}

void deleterow() {//消除多行并更新分数
	int count = 0;
	for (int i = 0;i < PLAYHEIGHT;i++) {
		int j = 0;
		while (j<PLAYWIDTH && blockexist[i][j] == 1)  j++;
		if (j < PLAYWIDTH)  continue;
		count++;
		for (j = 0;j < PLAYWIDTH;j++) {
			blockexist[i][j] = 0;
		}
	}
	score = score + count * (20 + 10 * (count - 1));//一次性消除行数越多，得分越多
}

void mergerow() {//将消除多行后的行合并
	for (int i = PLAYHEIGHT - 1;i >= 0;i--) {
		int rownum = i;
		int j = 0;
		//检查这行有没有方块，若没有方块，则不用合并
		j = 0;
		while (j < PLAYWIDTH && blockexist[rownum][j] == 0) { j++; }
		if (j >= PLAYWIDTH)  continue;

		//判断这行该下落到的位置
		while (rownum + 1< PLAYHEIGHT) {
			j = 0;
			while (j < PLAYWIDTH && blockexist[rownum+1][j] == 0) { j++; }
			if (j < PLAYWIDTH) { break; }
			rownum++;
		}
		if (rownum == i)  continue;

		for (j = 0;j < PLAYWIDTH;j++) {
			blockexist[rownum][j] = blockexist[i][j];
			blockexist[i][j] = 0;
		}
	}
}

int main()
{
	bool firstblocks = true;
	pause = false;
	gameover = false;
	initgraph(GRAPHWIDTH*BLOCKSIZE, GRAPHHEIGHT*BLOCKSIZE,EW_SHOWCONSOLE);	// 创建绘图窗口，大小为 640x480 像素
	BeginBatchDraw();
	DrawPlayGraph();//画出棋盘
	while (!gameover) {
		if (firstblocks) {
			initblocks(&blocks,&statunum);
			updateblocks();
			initblocks(&nextblocks,&nextstatunum);
			setnextblocks();
		}
		else {
			truntonextblocks();
			updateblocks();
			initblocks(&nextblocks,&nextstatunum);
			setnextblocks();
		}
		DrawPlayGraph();//绘画页面
		long starttime = 0;
		long predownkeytime = -1e9;//设置为无穷小，但不是真正意义的无穷小

		starttime = clock();
		while(1){
			long endtime = clock();
			//方块自然下落和下落速度设置
			if (!pause && (double)(endtime - starttime)/CLOCKS_PER_SEC > max(0.1,0.5-(score/100)*0.1)) {
				if (godown()) {
					DrawPlayGraph();
				}
				else {
					break;
				}
				starttime = endtime;
			}
			if (!pause && GetAsyncKeyState(VK_UP)) {//按下上键
				if (rotate()) {
					DrawPlayGraph();
				}
			}
			if (!pause && GetAsyncKeyState(VK_DOWN)) {//按下下键
				
				//判断是否为双击
				long nowdownkeytime = clock();
				if ((double)(nowdownkeytime - predownkeytime) / CLOCKS_PER_SEC < 0.2) {
					while (godown()){}
					DrawPlayGraph();
					break;
				}
				predownkeytime = nowdownkeytime;
				if (godown()) {
					DrawPlayGraph();
				}
				else {
					break;
				}
			}
			if (!pause && GetAsyncKeyState(VK_LEFT)) {//按下左键
				if (goleft()) {
					DrawPlayGraph();
				}
			}
			if (!pause && GetAsyncKeyState(VK_RIGHT)) {//按下右键
				if (goright()) {
					DrawPlayGraph();
				}
			}
			/*
			if (GetAsyncKeyState(VK_SPACE)) {//空格键暂停
				pause ^= 1;
			}
			//用GetAsyncKeyState来实现获取键盘消息，
			当按下空格后继续按方向键会出现解除暂停后瞬间移动的bug
			*/
			
			Sleep(150);//由于GetAsyncKeyState的缘故，防止按一次键变成按多次的效果
		}
		firstblocks = false;
		stickblocks();//固定移动方块
		deleterow();//消除行
		mergerow();//合并行
		DrawPlayGraph();
	}
	
	_getch();				// 按任意键继续
	EndBatchDraw();
	closegraph();			// 关闭绘图窗口
	return 0;
}