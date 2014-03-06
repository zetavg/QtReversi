#include <stdio.h>
#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 8 // 棋盤格數
#define STONE_SIZE 40 // 棋子顯示大小

#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QToolButton>
#include <QPushButton>
#include <QIcon>
#include <QSignalMapper>
#include <QObject>

typedef enum { null, black, white } piece;

class Game : public QObject {
	Q_OBJECT
	public:
		Game(QLabel **guiBoard_i,
				QPixmap *guiImgStoneW_i, QPixmap *guiImgStoneB_i,
				QPixmap *guiImgStoneWa_i, QPixmap *guiImgStoneBa_i,
				QPixmap *guiImgStoneWs_i, QPixmap *guiImgStoneBs_i,
				QPixmap *guiImgStoneN_i) {
			/* 存圖片 */
			guiBoard = guiBoard_i;
			guiImgStoneW = guiImgStoneW_i;
			guiImgStoneB = guiImgStoneB_i;
			guiImgStoneWa = guiImgStoneWa_i;
			guiImgStoneBa = guiImgStoneBa_i;
			guiImgStoneWs = guiImgStoneWs_i;
			guiImgStoneBs = guiImgStoneBs_i;
			guiImgStoneN = guiImgStoneN_i;
			/* 棋盤歸零 */
			for (int i=0; i<BOARD_SIZE*BOARD_SIZE; i++) {
				boardData[i] = null;
			}
		}

		/* 取得棋盤某格的資料 */
		piece getBoardData(int x, int y) {
			if (x > BOARD_SIZE-1 || y > BOARD_SIZE-1 || x < 0 || y < 0) return null;
			return boardData[x+y*BOARD_SIZE];
		}

		/* User 落棋 */
		void drop(int x, int y) {

			if (moves[x+y*BOARD_SIZE][1][1] > 0) {
				printf("(%d %d)\n", x, y);
				doDrop(x, y, nxtMove);
				for (int i=-1; i<2; i++) {
					for (int j=-1; j<2; j++) { // 九宮格吃棋子
						if (!(i == 0 && j == 0)) { // 跳過中間
							for (int d=1; d<=moves[x+y*BOARD_SIZE][1+i][1+j]; d++) {
								doDrop(x+i*d, y+j*d, nxtMove);
							}
						}
					}
				}
				next();
			} else {
				printf("Illegal move\n");
			}
		}

		/* 換人 */
		void next() {
			if (nxtMove == black) {
				nxtMove = white;
				printf("Next: white\n");
			} else {
				nxtMove = black;
				printf("Next: black\n");
			}
			calcMoves(nxtMove);
		}

		/* 執行落棋, 同時處理內部資料和 ui */
		void doDrop(int x, int y, piece p) {
			printf("doDrop: (%d, %d)\n", x, y);
			boardData[x+y*BOARD_SIZE] = p;
			switch (p) {
				case null:
					guiBoard[x+y*BOARD_SIZE]->setPixmap(*guiImgStoneN);
					break;
				case white:
					guiBoard[x+y*BOARD_SIZE]->setPixmap(*guiImgStoneW);
					break;
				case black:
					guiBoard[x+y*BOARD_SIZE]->setPixmap(*guiImgStoneB);
					break;
			}
		}

		/* 重繪棋盤 */
		void drawBoard() {
			for (int x=0; x<BOARD_SIZE; x++) {
				for (int y=0; y<BOARD_SIZE; y++) {
					int p = getBoardData(x, y);
					switch (p) {
						case null:
							guiBoard[x+y*BOARD_SIZE]->setPixmap(*guiImgStoneN);
							break;
						case white:
							guiBoard[x+y*BOARD_SIZE]->setPixmap(*guiImgStoneW);
							break;
						case black:
							guiBoard[x+y*BOARD_SIZE]->setPixmap(*guiImgStoneB);
							break;
					}
				}
			}
		}

		/* 計算落棋表 */
		void calcMoves(piece who) {
			totalMoves = 0;
			/* 清空表格, 負一代表未計算 */
			for (int i=0; i<BOARD_SIZE*BOARD_SIZE; i++) {
				moves[i][1][1] = -1;
			}
			printf("Avail. moves for ");
			(who == black)? printf("black: ") : printf("white: ");
			for (int x=0; x<BOARD_SIZE; x++) {
				for (int y=0; y<BOARD_SIZE; y++) { // 造訪每一格
					if (getBoardData(x, y) != null && getBoardData(x, y) != who) {  // 若此格的棋色與給定的不同
						calcMoveAround(x, y, who); // 計算周圍是否可以落棋
					}
				}
			}
			printf(" [total:%d]\n", totalMoves);
		}

		void calcMoveAround(int x, int y, piece who) {
			for (int i=-1; i<2; i++) {
				for (int j=-1; j<2; j++) { // 九宮格造訪
					if (getBoardData((x+i), (y+j)) == null && moves[(x+i)+(y+j)*BOARD_SIZE][1][1] == -1) { // 若此格為空, 且未計算過
						calcMoveAt((x+i), (y+j), who); // 算他
					}
				}
			}

		}

		void calcMoveAt(int x, int y, piece who) {
			if (x > BOARD_SIZE-1 || y > BOARD_SIZE-1 || x < 0 || y < 0) return;
			int d;
			piece p;
			moves[x+y*BOARD_SIZE][1][1] = 0;
			for (int i=-1; i<2; i++) {
				for (int j=-1; j<2; j++) { // 九宮格
					if (!(i == 0 && j == 0)) { // 跳過中間
						moves[x+y*BOARD_SIZE][1+i][1+j] = 0; // 計數重設
						d = 0; // 計數器
						p = getBoardData(x+i*(d+1), y+j*(d+1)); // 下一顆考慮棋
						while (p != null) { // 往下尋找同顏色, 同時記錄
							if (p == who) { // 找到同色
								moves[x+y*BOARD_SIZE][1+i][1+j] = d; // 寫入計數
								moves[x+y*BOARD_SIZE][1][1] += d;
								break;
							}
							d++;
							p = getBoardData(x+i*(d+1), y+j*(d+1));
						}
					}
				}
			}
			if (moves[x+y*BOARD_SIZE][1][1] > 0) {
				totalMoves++;
				printf("(%d, %d) ", x, y);
			}
		}

		/* 顯示提示 */
		void doHint(int x, int y) {
			drawBoard();

			QPixmap *aPix, *sPix;

			if (nxtMove == white) {
				aPix = guiImgStoneWa;
				sPix = guiImgStoneWs;
			} else {
				aPix = guiImgStoneBa;
				sPix = guiImgStoneBs;
			}

			if (x < BOARD_SIZE && y < BOARD_SIZE && x >= 0 && y >= 0) {
				if (moves[x+y*BOARD_SIZE][1][1] > 0) { // 可以下
					guiBoard[x+y*BOARD_SIZE]->setPixmap(*aPix);
					for (int i=-1; i<2; i++) {
						for (int j=-1; j<2; j++) { // 九宮格
							if (!(i == 0 && j == 0)) { // 跳過中間
								for (int d=1; d<=moves[x+y*BOARD_SIZE][1+i][1+j]; d++) {
									guiBoard[(x+i*d)+(y+j*d)*BOARD_SIZE]->setPixmap(*sPix);
								}
							}
						}
					}
				}
			}

		}

		bool hint = false; // 是否顯示提示

	public slots:
		void reset() {
			/* Clear the board */
			for (int x=0; x<BOARD_SIZE; x++) {
				for (int y=0; y<BOARD_SIZE; y++) {
					doDrop(x, y, null);
				}
			}

			/* Init */
			doDrop(BOARD_SIZE/2, BOARD_SIZE/2, white);
			doDrop(BOARD_SIZE/2-1, BOARD_SIZE/2-1, white);
			doDrop(BOARD_SIZE/2-1, BOARD_SIZE/2, black);
			doDrop(BOARD_SIZE/2, BOARD_SIZE/2-1, black);
			this->nxtMove = white;
			calcMoves(nxtMove);

			printf("Restarted!\n");
			return;
		}

		void hintSwitch(int t) {
			if (t) hint = true;
			else hint = false;
			drawBoard();
			printf("hintSwitch: %d\n", t);
			return;
		}

		void update();


	signals:
		void updateWScore(int); // 黑分數更新
		void updateBScore(int); // 白分數更新

	private:
		piece boardData[BOARD_SIZE*BOARD_SIZE]; // 棋盤資料, 老師說二維陣列太貴
		piece nxtMove; // 輪到誰
		int totalMoves;
		int moves[BOARD_SIZE*BOARD_SIZE][3][3]; // 落棋表, 每個位置都是一組二維陣列:
		QLabel **guiBoard; // 棋盤 ui              +--------------+--------------+--------------+
		QPixmap *guiImgStoneW; // 白棋子圖         |往左上可吃幾顆| 往上可吃幾顆 |往右上可吃幾顆|
		QPixmap *guiImgStoneB; // 黑棋子圖         +--------------+--------------+--------------+
		QPixmap *guiImgStoneWs; // 白棋子小圖      | 往左可吃幾顆 | 總共可吃幾顆 | 往右可吃幾顆 |
		QPixmap *guiImgStoneBs; // 黑棋子小圖      +--------------+--------------+--------------+
		QPixmap *guiImgStoneWa; // 白棋子半透明圖  |往左下可吃幾顆| 往下可吃幾顆 |往右下可吃幾顆|
		QPixmap *guiImgStoneBa; // 黑棋子半透明圖  +--------------+--------------+--------------+
		QPixmap *guiImgStoneN; // 沒棋子圖
};

#endif
