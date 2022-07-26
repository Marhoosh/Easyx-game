#include <graphics.h>		// ����ͼ�ο�ͷ�ļ�
#include <conio.h>
#include<time.h>
#include<Windows.h>
#include<stdio.h>

#define GRAPHHEIGHT		20	//��λ������  
#define GRAPHWIDTH		16	//ͼ�ν����16����Ϊ�����ұ���������Ϊ6�Ĵ��������
#define PLAYHEIGHT		20  //��Ϸ����߶�
#define PLAYWIDTH		10	//��Ϸ������
#define BLOCKSIZE		30	//��λ������
#define BLOCKCOLOR      BLUE

int blockexist[PLAYHEIGHT][PLAYWIDTH];//0Ϊ�޷��飬1Ϊ�̶����飬2Ϊ�ƶ�����
int statunum;//��һ��״̬��6Ϊ������
int nextstatunum;
bool gameover;//��Ϸ�Ƿ����
int score;//�÷�
bool pause;//�Ƿ���ͣ

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

void updateblocks() {//�����������ƶ������λ��
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

void initblocks(struct block (*tmpblocks)[4],int *tmpstatunum) {//��ʼ�����״���������м�λ��,tmpblocksΪ��������
	//�����ȡһ����̬
	srand((unsigned)time(NULL));//������һ����Ϊ�˷�ֹrand()��������ɻ�
	*tmpstatunum = rand() % 7;
	for (int i = 0;i < 4;i++) {
		//blocks[i].x = statu[statunum][i] / 2 ;
		//blocks[i].y = statu[statunum][i] % 2 + PLAYWIDTH / 2;
		
		(*tmpblocks)[i].x = statu[*tmpstatunum][i] / 2 - 4;
		(*tmpblocks)[i].y = statu[*tmpstatunum][i] % 2 + PLAYWIDTH / 2;
	}
	/*for (int i = 0;i < 4;i++) {
		printf("blocks���� %d %d\n", blocks[i].x, blocks[i].y);
	}*/

	//����̬�����ת����
	
	srand((unsigned)time(NULL));
	int rotatenum = rand() % 4;
	for (int i = 0;i < rotatenum;i++) {
		if (*tmpstatunum == 6)  break;  //�����β�����ת
		block p = (*tmpblocks)[1];  //��ת����
		for (int j = 0;j < 4;j++) {
			int x = (*tmpblocks)[j].y - p.y;
			int y = (*tmpblocks)[j].x - p.x;
			(*tmpblocks)[j].x = p.x - x;
			(*tmpblocks)[j].y = p.y + y;
		}
	}
	
	//�������·ŵ��������Ϸ�
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

	//����Ƿ�ɷ��÷���
	for (int i = 0;i < 4;i++) {
		if (blockexist[blocks[i].x][blocks[i].y]) {
			gameover = true;
			break;
		}
	}
}


bool checkblock(int x,int y){//x,yΪ�������꣬��鷽��λ���Ƿ����
	//����Ƿ�Խ��
	if (x>=PLAYHEIGHT || y<0 || y>=PLAYWIDTH) { return false; }
	//���˴��Ƿ��Ѿ��з���
	if (blockexist[x][y] == 1) { return false; }
	return true;
}

bool rotate() {//��ת�ɹ�������true�����򷵻�false;
	if (statunum == 6)  return true;
	block p = blocks[1];  //��ת����
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

bool checkgodown() {//�ж��ܷ�����һ����λ
	//����·��Ƿ���ڹ̶�����
	for (int i = 0;i < 4;i++) {
		if (blockexist[blocks[i].x + 1][blocks[i].y] == 1) {
			return false;
		}
	}

	//����Ƿ񵽵�
	for (int i = 0;i < 4;i++) {
		if (blocks[i].x + 1 == PLAYHEIGHT) {
			return false;
		}
	}
	return true;
}

bool godown() {//����ɹ�������true�����򷵻�false;

	if (!checkgodown())  return false;

	for (int i = 0;i < 4;i++) {
		blocks[i].x++;
	}

	updateblocks();
	return true;
}

bool goleft() {//�����ƶ��ɹ�������true�����򷵻�false��
	for (int i = 0;i < 4;i++) {
		if (!checkblock(blocks[i].x, blocks[i].y - 1)) { return false; }
	}
	for (int i = 0;i < 4;i++) {
		blocks[i].y--;
	}
	updateblocks();
	return true;
}

bool goright() {//�����ƶ��ɹ�������true�����򷵻�false��
	for (int i = 0;i < 4;i++) {
		if (!checkblock(blocks[i].x, blocks[i].y + 1)) { return false; }
	}
	for (int i = 0;i < 4;i++) {
		blocks[i].y++;
	}
	updateblocks();
	return true;
}

void stickblocks() {//���ƶ�����̶�
	for (int i = 0;i < 4;i++) {
		blockexist[blocks[i].x][blocks[i].y] = 1;
	}
}

void DrawPlayGraph() {//�滭ͼ�ν���
	
	//�����Ļ
	cleardevice();
	//��������
	setlinecolor(WHITE);
	for (int i = 0;i < PLAYWIDTH+1;i++) {//������
		line(i* BLOCKSIZE, 0, i * BLOCKSIZE, PLAYHEIGHT * BLOCKSIZE);
	}
	for (int i = 0;i < PLAYHEIGHT + 1;i++) {//������
		line(0, i * BLOCKSIZE, PLAYWIDTH * BLOCKSIZE,i*BLOCKSIZE );
	}
	
	//blockexist[1][1] = true;

	//�滭�����ϵķ���
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

	//���ƼƷ���
	settextcolor(WHITE);
	TCHAR text[11] = _T("�÷�");
	outtextxy(PLAYWIDTH*BLOCKSIZE + 50, GRAPHHEIGHT*BLOCKSIZE / 2, text);
	_stprintf_s(text, _T("%d"), score);
	outtextxy(PLAYWIDTH * BLOCKSIZE + 50, GRAPHHEIGHT * BLOCKSIZE / 2+20, text);

	//������һ����Ҫ���ֵĶ���˹����
	setfillcolor(BLOCKCOLOR);
	for (int i = 0;i < 4;i++) {
		int x = nextblocks[i].x;
		int y = nextblocks[i].y;
		//printf("nextblocks���� %d %d\n", x, y);
		fillrectangle(y * BLOCKSIZE, x * BLOCKSIZE, (y + 1) * BLOCKSIZE, (x + 1) * BLOCKSIZE);
	}

	FlushBatchDraw();
}

void deleterow() {//�������в����·���
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
	score = score + count * (20 + 10 * (count - 1));//һ������������Խ�࣬�÷�Խ��
}

void mergerow() {//���������к���кϲ�
	for (int i = PLAYHEIGHT - 1;i >= 0;i--) {
		int rownum = i;
		int j = 0;
		//���������û�з��飬��û�з��飬���úϲ�
		j = 0;
		while (j < PLAYWIDTH && blockexist[rownum][j] == 0) { j++; }
		if (j >= PLAYWIDTH)  continue;

		//�ж����и����䵽��λ��
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
	initgraph(GRAPHWIDTH*BLOCKSIZE, GRAPHHEIGHT*BLOCKSIZE,EW_SHOWCONSOLE);	// ������ͼ���ڣ���СΪ 640x480 ����
	BeginBatchDraw();
	DrawPlayGraph();//��������
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
		DrawPlayGraph();//�滭ҳ��
		long starttime = 0;
		long predownkeytime = -1e9;//����Ϊ����С���������������������С

		starttime = clock();
		while(1){
			long endtime = clock();
			//������Ȼ����������ٶ�����
			if (!pause && (double)(endtime - starttime)/CLOCKS_PER_SEC > max(0.1,0.5-(score/100)*0.1)) {
				if (godown()) {
					DrawPlayGraph();
				}
				else {
					break;
				}
				starttime = endtime;
			}
			if (!pause && GetAsyncKeyState(VK_UP)) {//�����ϼ�
				if (rotate()) {
					DrawPlayGraph();
				}
			}
			if (!pause && GetAsyncKeyState(VK_DOWN)) {//�����¼�
				
				//�ж��Ƿ�Ϊ˫��
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
			if (!pause && GetAsyncKeyState(VK_LEFT)) {//�������
				if (goleft()) {
					DrawPlayGraph();
				}
			}
			if (!pause && GetAsyncKeyState(VK_RIGHT)) {//�����Ҽ�
				if (goright()) {
					DrawPlayGraph();
				}
			}
			/*
			if (GetAsyncKeyState(VK_SPACE)) {//�ո����ͣ
				pause ^= 1;
			}
			//��GetAsyncKeyState��ʵ�ֻ�ȡ������Ϣ��
			�����¿ո����������������ֽ����ͣ��˲���ƶ���bug
			*/
			
			Sleep(150);//����GetAsyncKeyState��Ե�ʣ���ֹ��һ�μ���ɰ���ε�Ч��
		}
		firstblocks = false;
		stickblocks();//�̶��ƶ�����
		deleterow();//������
		mergerow();//�ϲ���
		DrawPlayGraph();
	}
	
	_getch();				// �����������
	EndBatchDraw();
	closegraph();			// �رջ�ͼ����
	return 0;
}