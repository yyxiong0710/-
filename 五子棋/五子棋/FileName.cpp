#include <graphics.h>
#include <conio.h>
#include <algorithm>
#include <vector>
#include <math.h>
using namespace std;

#define FullScore 999999
#define Row 15
#define Col 15
//黑棋为1，白棋为-1
int chessboard[Row][Col];
int boardscore[Row][Col];
bool isblackplay = true;
int unitlength = 50;
int Width = Col * unitlength;
int Height = Row * unitlength;
int chess_size = unitlength/3;
const int startplace_x = (Width - (Col - 1) * unitlength) / 2;
const int startplace_y = (Height - (Row - 1) * unitlength) / 2;
bool isclicked = true;
bool blackwin = false;
bool whitewin = false;
bool draw = false;

int* min_c(int[][2], int, int);

void startup() {
	initgraph(Width, Height);
	setbkcolor(RGB(193, 209, 105));
	setbkmode(TRANSPARENT);
	cleardevice();
	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Col; j++) {
			chessboard[i][j] = 0;
			boardscore[i][j] = 0;
		}
	}
	BeginBatchDraw();
}

void show_chessboard() {
	setlinecolor(BLACK);
	int startplace_x = (Width - (Col - 1) * unitlength)/2;
	int startplace_y = (Height - (Row - 1) * unitlength) / 2;
	for (int i = 0; i < Col; i++) {
		line(startplace_x + i * unitlength, startplace_y, startplace_x + i * unitlength, Height - startplace_y);
	}
	for (int j = 0; j < Row; j++) {
		line(startplace_x, startplace_y + j * unitlength, Width - startplace_x, startplace_y + j * unitlength);
	}
}

void showscore() {
	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Col; j++) {
			if (chessboard[i][j] == 0) {
				if (isblackplay) {
					settextstyle(0.3 * unitlength, 0, _T("宋体"));
					settextcolor(RED);
					RECT r1 = { startplace_x + (j + 0.3) * unitlength,startplace_y+(i-0.5)*unitlength,startplace_x + (j - 0.3) * unitlength,startplace_y + (i - 0.2) * unitlength };
					TCHAR s[20];
					swprintf_s(s, _T("%d"), boardscore[i][j]);
					drawtext(s, &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else {
					settextstyle(0.3 * unitlength, 0, _T("宋体"));
					settextcolor(BLUE);
					RECT r1 = { startplace_x + (j + 0.3) * unitlength,startplace_y + (i - 0.5) * unitlength,startplace_x + (j - 0.3) * unitlength,startplace_y + (i - 0.2) * unitlength };
					TCHAR s[20];
					swprintf_s(s, _T("%d"), boardscore[i][j]);
					drawtext(s, &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
			}
		}
	}
}

void show() {
	cleardevice();

	show_chessboard();
	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Col; j++) {
			if (chessboard[i][j] != 0) {
				if (chessboard[i][j] == 1) {
					setlinecolor(BLACK);
					setfillcolor(BLACK);
				}
				else {
					setlinecolor(WHITE);
					setfillcolor(WHITE);
				}
				fillcircle(startplace_x + j * unitlength, startplace_y + i * unitlength,chess_size);
			}
		}
	}

	showscore();

	if (blackwin) {
		settextstyle(2*unitlength, 0, _T("宋体"));
		settextcolor(BLACK);
		RECT r1 = { 0,0,Width,Height };
		drawtext(_T("黑方胜利"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (whitewin) {
		settextstyle(2 * unitlength, 0, _T("宋体"));
		settextcolor(WHITE);
		RECT r1 = { 0,0,Width,Height };
		drawtext(_T("白方胜利"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (draw) {
		settextstyle(2 * unitlength, 0, _T("宋体"));
		settextcolor(GREEN);
		RECT r1 = { 0,0,Width,Height };
		drawtext(_T("平局"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	FlushBatchDraw();
}

void update() {
	MOUSEMSG m;
	if (MouseHit()) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			int jx = (m.x - startplace_x) / unitlength;
			int iy = (m.y - startplace_y) / unitlength;

			int near_coordinate[4][2];
			near_coordinate[0][0] = jx;
			near_coordinate[0][1] = iy;
			near_coordinate[1][0] = jx + 1;
			near_coordinate[1][1] = iy;
			near_coordinate[2][0] = jx;
			near_coordinate[2][1] = iy + 1;
			near_coordinate[3][0] = jx + 1;
			near_coordinate[3][1] = iy + 1;

			int *smallest_coordinate = min_c(near_coordinate,m.x,m.y);

			int d = (m.x - (smallest_coordinate[0] * unitlength + startplace_x)) * (m.x - (smallest_coordinate[0] * unitlength + startplace_x)) + (m.y - (smallest_coordinate[1] * unitlength + startplace_y)) * (m.y - (smallest_coordinate[1] * unitlength + startplace_y));
			if (d <= pow(chess_size,2)) {
				if (chessboard[smallest_coordinate[1]][smallest_coordinate[0]] == 0) {
					if (isblackplay) {
						chessboard[smallest_coordinate[1]][smallest_coordinate[0]] = 1;
					}
					else {
						chessboard[smallest_coordinate[1]][smallest_coordinate[0]] = -1;
					}
					isblackplay = !isblackplay;
					isclicked = true;
				}
			}
		}
	}
}

int* min_c(int coordinate[][2],int x,int y) {
	int d;
	int min;
	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			d = (x - (coordinate[i][0] * unitlength + startplace_x)) * (x - (coordinate[i][0] * unitlength + startplace_x)) + (y - (coordinate[i][1] * unitlength + startplace_y)) * (y - (coordinate[i][1] * unitlength + startplace_y));
			min = i;
		}
		else {
			if (d > (x - (coordinate[i][0] * unitlength + startplace_x)) * (x - (coordinate[i][0] * unitlength + startplace_x)) + (y - (coordinate[i][1] * unitlength + startplace_y)) * (y - (coordinate[i][1] * unitlength + startplace_y))) {
				d = (x - (coordinate[i][0] * unitlength + startplace_x)) * (x - (coordinate[i][0] * unitlength + startplace_x)) + (y - (coordinate[i][1] * unitlength + startplace_y)) * (y - (coordinate[i][1] * unitlength + startplace_y));
				min = i;
			}
		}
	}
	return coordinate[min];
}

int checkwin() {

	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Col; j++) {
			if (chessboard[i][j] == 0) continue;
			//黑方
			if (chessboard[i][j] == 1) {
				int m;
				//右边是否集齐5个
				for (m = 0; m < 5; m++) {
					if (chessboard[i][j + m] == 1) continue;
					else break;
				}
				if (m == 5) return 1;

				//下面是否集齐5个
				for (m = 0; m < 5; m++) {
					if (chessboard[i + m][j] == 1) continue;
					else break;
				}
				if (m == 5) return 1;

				//右下对角是否集齐五个
				for (m = 0; m < 5; m++) {
					if (chessboard[i + m][j + m] == 1) continue;
					else break;
				}
				if (m == 5) return 1;

				//左下对角是否集齐五个
				for (m = 0; m < 5; m++) {
					if (chessboard[i + m][j - m] == 1)continue;
					else break;
				}
				if (m == 5) return 1;
			}
			//白方
			else if (chessboard[i][j] == -1) {
				int m;
				//右边是否集齐5个
				for (m = 0; m < 5; m++) {
					if (chessboard[i][j + m] == -1) continue;
					else break;
				}
				if (m == 5) return -1;

				//下面是否集齐5个
				for (m = 0; m < 5; m++) {
					if (chessboard[i + m][j] == -1) continue;
					else break;
				}
				if (m == 5) return -1;

				//右下对角是否集齐五个
				for (m = 0; m < 5; m++) {
					if (chessboard[i + m][j + m] == -1) continue;
					else break;
				}
				if (m == 5) return -1;

				//左下对角是否集齐五个
				for (m = 0; m < 5; m++) {
					if (chessboard[i + m][j - m] == -1)continue;
					else break;
				}
				if (m == 5) return -1;
			}
		}
	}




	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Col; j++) {
			if (chessboard[i][j] == 0) {
				return 0;
			}
		}
	}

	return 2;
}

bool isend() {
	if (blackwin || whitewin || draw) {
		return true;
	}
	return false;
};

int onechess_score(bool who,int chessboard[][Col], int rowcoldiag) {
	int whoplay = who ? 1 : -1;
	int sum = 0;
	if (rowcoldiag == 1) {
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (chessboard[i][j] == whoplay) {
					if (j == 0 && chessboard[i][j + 1] != -whoplay) {
						sum += 1;
					}
					else if (j == Col - 1 && chessboard[i][j - 1] != -whoplay) {
						sum += 1;
					}
					else if (j > 0 && j < Col - 1 && chessboard[i][j - 1] != -whoplay && chessboard[i][j + 1] != -whoplay) {
						sum += 2;
					}
					else if (j > 0 && j < Col - 1 && (chessboard[i][j - 1] == -whoplay && chessboard[i][j + 1] != -whoplay || chessboard[i][j - 1] != -whoplay && chessboard[i][j + 1] == -whoplay)) {
						sum += 1;
					}
				}
			}
		}
	}
	else if (rowcoldiag == 2) {
		for (int j = 0; j < Col; j++) {
			for (int i = 0; i < Row; i++) {
				if (chessboard[i][j] == whoplay) {
					if (i == 0 && chessboard[i + 1][j] != -whoplay) {
						sum += 1;
					}
					else if (i == Row - 1 && chessboard[i - 1][j] != -whoplay) {
						sum += 1;
					}
					else if (i > 0 && i < Row - 1 && chessboard[i - 1][j] != -whoplay && chessboard[i + 1][j] != -whoplay) {
						sum += 2;
					}
					else if (i > 0 && i < Row - 1 && (chessboard[i - 1][j] == -whoplay && chessboard[i + 1][j] != -whoplay || chessboard[i - 1][j] != -whoplay && chessboard[i + 1][j] == -whoplay)) {
						sum += 1;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (chessboard[i][j] == whoplay) {
					if ((i == 0 || j == 0) && chessboard[i][j] == whoplay && chessboard[i + 1][j + 1] != -whoplay) {
						sum += 1;
					}
					else if ((i == Row - 1 || j == Col - 1) && chessboard[i][j] == whoplay && chessboard[i - 1][j - 1] != -whoplay) {
						sum += 1;
					}
					else if (i > 0 && j > 0 && i < Row - 1 && j < Col - 1 && chessboard[i + 1][j + 1] != -whoplay && chessboard[i - 1][j - 1] != -whoplay) {
						sum += 2;
					}
					else if (i > 0 && j > 0 && i < Row - 1 && j < Col - 1 && (chessboard[i + 1][j + 1] == -whoplay && chessboard[i - 1][j - 1] != -whoplay || chessboard[i + 1][j + 1] != -whoplay && chessboard[i - 1][j - 1] == -whoplay)) {
						sum += 1;
					}
				}
			}
		}

		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (chessboard[i][j] == whoplay) {
					if ((i == Row-1 || j == 0) && chessboard[i][j] == whoplay && chessboard[i - 1][j + 1] != -whoplay) {
						sum += 1;
					}
					else if ((i == 0 || j == Col-1) && chessboard[i][j] == whoplay && chessboard[i + 1][j - 1] != -whoplay) {
						sum += 1;
					}
					else if (i > 0 && j > 0 && i < Row - 1 && j < Col - 1 && chessboard[i + 1][j - 1] != -whoplay && chessboard[i - 1][j + 1] != -whoplay) {
						sum += 2;
					}
					else if (i > 0 && j > 0 && i < Row - 1 && j < Col - 1 && (chessboard[i + 1][j - 1] == -whoplay && chessboard[i - 1][j + 1] != -whoplay || chessboard[i + 1][j - 1] != -whoplay && chessboard[i - 1][j + 1] == -whoplay)) {
						sum += 1;
					}
				}
			}
		}
	}
	return sum;
}

int twochess_score(bool who, int chessboard[][Col], int rowcoldiag) {
	int whoplay = who ? 1 : -1;
	int sum = 0;
	if (rowcoldiag == 1) {
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (j < Col - 1 && chessboard[i][j] == whoplay && chessboard[i][j + 1] == whoplay) {
					if (j == 0 && chessboard[i][j + 2] != -whoplay) {
						sum += 5;
					}
					else if (j == Col - 2 && chessboard[i][j - 1] != -whoplay) {
						sum += 5;
					}
					else if (j > 0 && chessboard[i][j - 1] != -whoplay && chessboard[i][j + 2] != -whoplay) {
						sum += 10;
					}
					else if (j > 0 && (chessboard[i][j - 1] == -whoplay && chessboard[i][j + 2] != -whoplay || chessboard[i][j - 1] != -whoplay && chessboard[i][j + 2] == -whoplay)) {
						sum += 5;
					}
				}
			}
		}
	}
	else if (rowcoldiag == 2) {
		for (int j = 0; j < Col; j++) {
			for (int i = 0; i < Row; i++) {
				if (i < Row - 1 && chessboard[i][j] == whoplay && chessboard[i + 1][j] == whoplay) {
					if (i == 0 && chessboard[i + 2][j] != -whoplay) {
						sum += 5;
					}
					else if (i == Row - 2 && chessboard[i - 1][j] != -whoplay) {
						sum += 5;
					}
					else if (i > 0 && chessboard[i - 1][j] != -whoplay && chessboard[i + 2][j] != -whoplay) {
						sum += 10;
					}
					else if (i > 0 && (chessboard[i - 1][j] == -whoplay && chessboard[i + 2][j] != -whoplay || chessboard[i - 1][j] != -whoplay && chessboard[i + 2][j] == -whoplay)) {
						sum += 5;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (i<Row-1 && j< Col -1 && chessboard[i][j] == whoplay && chessboard[i+1][j+1] == whoplay) {
					if ((i == 0 || j == 0) && chessboard[i + 2][j + 2] != -whoplay) {
						sum += 5;
					}
					else if ((i == Row - 2 || j == Col - 2) && chessboard[i-1][j-1]!=-whoplay) {
						sum += 5;
					}
					else if (i > 0 && j > 0 && i < Row - 2 && j < Col - 2 && chessboard[i + 2][j + 2] != -whoplay && chessboard[i - 1][j - 1] != -whoplay) {
						sum += 10;
					}
					else if (i > 0 && j > 0 && i < Row - 2 && j < Col - 2 && (chessboard[i + 2][j + 2] == -whoplay && chessboard[i - 1][j - 1] != -whoplay || chessboard[i + 2][j + 2] != -whoplay && chessboard[i - 1][j - 1] == -whoplay)) {
						sum += 5;
					}


				}
			}
		}

		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (i <Row-1 && j >0 && chessboard[i][j] == whoplay && chessboard[i + 1][j - 1] == whoplay) {
					if ((i == Row-2 || (j == 1 && i!=0)) && chessboard[i][j] == whoplay && chessboard[i - 1][j + 1] != -whoplay) {
						sum += 5;
					}
					else if (((i == 0 && j>1) || (j == Col - 1 && i<Row-2)) && chessboard[i + 2][j - 2] != -whoplay) {
						sum += 5;
					}
					else if (i > 1 && j > 1 && i < Row - 2 && j < Col - 2 && chessboard[i + 2][j - 2] != -whoplay && chessboard[i - 1][j + 1] != -whoplay) {
						sum += 10;
					}
					else if (i > 1 && j > 1 && i < Row - 2 && j < Col - 2 && (chessboard[i + 2][j - 2] == -whoplay && chessboard[i - 1][j + 1] != -whoplay || chessboard[i + 2][j - 2] != -whoplay && chessboard[i - 1][j + 1] == -whoplay)) {
						sum += 5;
					}


				}
			}
		}
	}
	return sum;
}

int threechess_score(bool who, int chessboard[][Col],int rowcoldiag) {
	int whoplay = who ? 1 : -1;
	int sum = 0;
	if (rowcoldiag == 1) {
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (j < Col - 2 && chessboard[i][j] == whoplay && chessboard[i][j + 1] == whoplay && chessboard[i][j + 2] == whoplay) {
					if (j == 0 && chessboard[i][j + 3] != -whoplay) {
						sum += 20;
					}
					else if (j == Col - 3 && chessboard[i][j - 1] != -whoplay) {
						sum += 20;
					}
					else if (j > 0 && chessboard[i][j - 1] != -whoplay && chessboard[i][j + 3] != -whoplay) {
						sum += 40;
					}
					else if (j > 0 && (chessboard[i][j - 1] == -whoplay && chessboard[i][j + 3] != -whoplay || chessboard[i][j - 1] != -whoplay && chessboard[i][j + 3] == -whoplay)) {
						sum += 20;
					}
				}
			}
		}
	}
	else if (rowcoldiag == 2) {
		for (int j = 0; j < Col; j++) {
			for (int i = 0; i < Row; i++) {
				if (i < Row - 2 && chessboard[i][j] == whoplay && chessboard[i + 1][j] == whoplay && chessboard[i + 2][j] == whoplay) {
					if (i == 0 && chessboard[i + 3][j] != -whoplay) {
						sum += 20;
					}
					else if (i == Row - 3 && chessboard[i - 1][j] != -whoplay) {
						sum += 20;
					}
					else if (i > 0 && chessboard[i - 1][j] != -whoplay && chessboard[i + 3][j] != -whoplay) {
						sum += 40;
					}
					else if (i > 0 && (chessboard[i - 1][j] == -whoplay && chessboard[i + 3][j] != -whoplay || chessboard[i - 1][j] != -whoplay && chessboard[i + 3][j] == -whoplay)) {
						sum += 20;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (i < Row - 2 && j < Col - 2 && chessboard[i][j] == whoplay && chessboard[i + 1][j + 1] == whoplay && chessboard[i+2][j+2] == whoplay) {
					if ((i == 0 || j == 0) && chessboard[i][j] == whoplay && chessboard[i + 3][j + 3] != -whoplay) {
						sum += 20;
					}
					else if ((i == Row - 3 || j == Col - 3) && chessboard[i - 1][j - 1] != -whoplay) {
						sum += 20;
					}
					else if (i > 0 && j > 0 && i < Row - 3 && j < Col - 3 && chessboard[i + 3][j + 3] != -whoplay && chessboard[i - 1][j - 1] != -whoplay) {
						sum += 40;
					}
					else if (i > 0 && j > 0 && i < Row - 3 && j < Col - 3 && (chessboard[i + 3][j + 3] == -whoplay && chessboard[i - 1][j - 1] != -whoplay || chessboard[i + 3][j + 3] != -whoplay && chessboard[i - 1][j - 1] == -whoplay)) {
						sum += 20;
					}
				}
			}
		}

		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (i < Row-2 && j > 1 && chessboard[i][j] == whoplay && chessboard[i + 1][j - 1] == whoplay && chessboard[i+2][j-2] == whoplay) {
					if ((i == 0 || j == Row - 1) && chessboard[i][j] == whoplay && chessboard[i + 3][j - 3] != -whoplay) {
						sum += 20;
					}
					else if ((i == Row-3 || j == 2) && chessboard[i - 1][j + 1] != -whoplay) {
						sum += 20;
					}
					else if (i > 1 && j > 1 && i < Row - 2 && j < Col - 2 && chessboard[i + 3][j - 3] != -whoplay && chessboard[i - 1][j + 1] != -whoplay) {
						sum += 40;
					}
					else if (i > 1 && j > 1 && i < Row - 2 && j < Col - 2 && (chessboard[i + 3][j - 3] == -whoplay && chessboard[i - 1][j + 1] != -whoplay || chessboard[i + 3][j - 3] != -whoplay && chessboard[i - 1][j + 1] == -whoplay)) {
						sum += 20;
					}


				}
			}
		}
	}
	return sum;
}

int fourchess_score(bool who, int chessboard[][Col], int rowcoldiag) {
	int whoplay = who ? 1 : -1;
	int sum = 0;
	if (rowcoldiag == 1) {
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (j < Col - 3 && chessboard[i][j] == whoplay && chessboard[i][j + 1] == whoplay && chessboard[i][j + 2] == whoplay && chessboard[i][j + 3] == whoplay) {
					if (j == 0 && chessboard[i][j + 4] != -whoplay) {
						sum += 40;
					}
					else if (j == Col - 4 && chessboard[i][j - 1] != -whoplay) {
						sum += 40;
					}
					else if (j > 0 && chessboard[i][j - 1] != -whoplay && chessboard[i][j + 4] != -whoplay) {
						sum += 70;
					}
					else if (j > 0 && (chessboard[i][j - 1] == -whoplay && chessboard[i][j + 4] != -whoplay || chessboard[i][j - 1] != -whoplay && chessboard[i][j + 4] == -whoplay)) {
						sum += 40;
					}
				}
			}
		}
	}
	else if (rowcoldiag == 2) {
		for (int j = 0; j < Col; j++) {
			for (int i = 0; i < Row; i++) {
				if (j < Col - 3 && chessboard[i][j] == whoplay && chessboard[i + 1][j] == whoplay && chessboard[i + 2][j] == whoplay && chessboard[i + 3][j] == whoplay) {
					if (i == 0 && chessboard[i + 4][j] != -whoplay) {
						sum += 40;
					}
					else if (i == Row - 4 && chessboard[i - 1][j] != -whoplay) {
						sum += 40;
					}
					else if (i > 0 && chessboard[i - 1][j] != -whoplay && chessboard[i + 4][j] != -whoplay) {
						sum += 70;
					}
					else if (i > 0 && (chessboard[i - 1][j] == -whoplay && chessboard[i + 4][j] != -whoplay || chessboard[i - 1][j] != -whoplay && chessboard[i + 4][j] == -whoplay)) {
						sum += 40;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (i < Row - 3 && j < Col - 3 && chessboard[i][j] == whoplay && chessboard[i + 1][j + 1] == whoplay && chessboard[i + 2][j + 2] == whoplay && chessboard[i+3][j+3] == whoplay) {
					if ((i == 0 || j == 0) && chessboard[i][j] == whoplay && chessboard[i + 4][j + 4] != -whoplay) {
						sum += 40;
					}
					else if ((i == Row - 4 || j == Col - 4) && chessboard[i - 1][j - 1] != -whoplay) {
						sum += 40;
					}
					else if (i > 0 && j > 0 && i < Row - 4 && j < Col - 4 && chessboard[i + 4][j + 4] != -whoplay && chessboard[i - 1][j - 1] != -whoplay) {
						sum += 70;
					}
					else if (i > 0 && j > 0 && i < Row - 4 && j < Col - 4 && (chessboard[i + 4][j + 4] == -whoplay && chessboard[i - 1][j - 1] != -whoplay || chessboard[i + 4][j + 4] != -whoplay && chessboard[i - 1][j - 1] == -whoplay)) {
						sum += 40;
					}  
				}
			}
		}

		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				if (i < Row - 3 && j > 2 && chessboard[i][j] == whoplay && chessboard[i + 1][j - 1] == whoplay && chessboard[i + 2][j - 2] == whoplay && chessboard[i + 3][j - 3] == whoplay) {
					if ((i == 0 || j == Row - 1) && chessboard[i][j] == whoplay && chessboard[i + 4][j - 4] != -whoplay) {
						sum += 40;
					}
					else if ((i == Row - 4 || j == 3) && chessboard[i - 1][j + 1] != -whoplay) {
						sum += 40;
					}
					else if (i > 1 && j > 1 && i < Row - 2 && j < Col - 2 && chessboard[i + 4][j - 4] != -whoplay && chessboard[i - 1][j + 1] != -whoplay) {
						sum += 70;
					}
					else if (i > 1 && j > 1 && i < Row - 2 && j < Col - 2 && (chessboard[i + 4][j - 4] == -whoplay && chessboard[i - 1][j + 1] != -whoplay || chessboard[i + 4][j - 4] != -whoplay && chessboard[i - 1][j + 1] == -whoplay)) {
						sum += 40;
					}


				}
			}
		}


	}
	return sum;
}

void estimate_chessboard_score() {
	//bool blackplay = isblackplay;
	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Col; j++) {
			if (chessboard[i][j] == 0) {
				if (isblackplay) {
					chessboard[i][j] = 1;
				}
				else {
					chessboard[i][j] = -1;
				}
				int k = checkwin();
				if (k == 1 || k == -1) {
					boardscore[i][j] = k*FullScore;
				}
				else {
					int score1 = 0;
					int score2 = 0;
					for (int m = 1; m <= 3; m++) {
						score1 += onechess_score(isblackplay, chessboard, m) + twochess_score(isblackplay, chessboard, m) + threechess_score(isblackplay, chessboard, m) + fourchess_score(isblackplay, chessboard, m);
						score2 += onechess_score(!isblackplay, chessboard, m) + twochess_score(!isblackplay, chessboard, m) + threechess_score(!isblackplay, chessboard, m) + fourchess_score(!isblackplay, chessboard, m);
					}
					boardscore[i][j] = score1 - score2;
					if (!isblackplay) {
						boardscore[i][j] = -boardscore[i][j];
					}
				}
				chessboard[i][j] = 0;
			}
		}
	}

	return;
}

int main() {
	startup();
	while (1) {
		if (isclicked) {
			estimate_chessboard_score();
			int k = checkwin();
			if (k == 1) {
				blackwin = true;
			}
			else if (k == -1) {
				whitewin = true;
			}
			else if (k == 2) {
				draw = true;
			}
			show();
			if (isend()) {
				break;
			}
			isclicked = false;
		}
		update();
	}
	_getch();
	return 0;
}