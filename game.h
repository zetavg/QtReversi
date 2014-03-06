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

class Game : public QObject {
	Q_OBJECT
	public:
		Game(QLabel **guiBoard_i, QPixmap *guiImgStoneW_i, QPixmap *guiImgStoneB_i, QPixmap *guiImgStoneN_i) {
			guiBoard = guiBoard_i;
			guiImgStoneW = guiImgStoneW_i;
			guiImgStoneB = guiImgStoneB_i;
			guiImgStoneN = guiImgStoneN_i;
			for (int i=0; i<BOARD_SIZE*BOARD_SIZE; i++) {
				boardData[i] = 0;
			}
		}

		/* 取得棋盤某格的資料 */
		int getBoardData(int i, int j) {
			return boardData[i*BOARD_SIZE+j];
		}
		/* 落棋, 同時處理內部資料和 ui */
		int drop(int i, int j, int p) {
			boardData[i*BOARD_SIZE+j] = p;
			switch (p) {
				case 0:
					guiBoard[i*BOARD_SIZE+j]->setPixmap(*guiImgStoneN);
					break;
				case 1:
					guiBoard[i*BOARD_SIZE+j]->setPixmap(*guiImgStoneW);
					break;
				case 2:
					guiBoard[i*BOARD_SIZE+j]->setPixmap(*guiImgStoneB);
					break;
			}
		}

	public slots:
		void reset();

	signals:
		void valueChanged();

	private:
		int boardData[BOARD_SIZE*BOARD_SIZE]; // 棋盤資料, 老師說二維陣列太貴, 0 無 1 白 2 黑
		QLabel **guiBoard; // 指向棋盤 ui
		QPixmap *guiImgStoneW; // 指向白棋子圖
		QPixmap *guiImgStoneB; // 指向黑棋子圖
		QPixmap *guiImgStoneN; // 指向沒棋子圖
};

#endif
