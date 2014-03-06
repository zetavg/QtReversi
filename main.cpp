#include <stdio.h>
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

#include "game.h"

void Game::reset() {

	/* Clear the board */
	for (int i=0; i<BOARD_SIZE; i++) {
		for (int j=0; j<BOARD_SIZE; j++) {
			drop(i, j, 0);
		}
	}

	/* Init */
	drop(BOARD_SIZE/2, BOARD_SIZE/2, 1);
	drop(BOARD_SIZE/2-1, BOARD_SIZE/2-1, 1);
	drop(BOARD_SIZE/2-1, BOARD_SIZE/2, 2);
	drop(BOARD_SIZE/2, BOARD_SIZE/2-1, 2);

	printf("Restarted!\n");
	return;
}

class QMouseEventWindow : public QWidget {
	protected:
		void mouseReleaseEvent(QMouseEvent *event);
};

void QMouseEventWindow::mouseReleaseEvent(QMouseEvent *event) {
	printf("Mouse: (%d, %d)\n", event->x(), event->y());
	QString msg;
	msg.sprintf("Mouse: (%d, %d)", event->x(), event->y());
	this->setWindowTitle(msg);
}

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	/* 常數們 */
	int boardSize = BOARD_SIZE;
	int stoneSize = STONE_SIZE;
	int boardWidth = stoneSize*boardSize;

	/* 新視窗 */
	QWidget *window = new QMouseEventWindow;
	window->setWindowTitle("ReversiGrid");
	window->setFixedSize(boardWidth+200, boardWidth);
	QWidget *board = new QWidget(window);
	board->setFixedSize(boardWidth, boardWidth);

	/* 棋盤網格排版 */
	QGridLayout *grid = new QGridLayout;
	grid->setSpacing(0);
	grid->setMargin(0);
	grid->setGeometry(QRect(0, 0, 10, 100));

	/* 載入棋子圖片 */
	QPixmap *stoneWPix = new QPixmap(":/stoneW.png");
	QPixmap *stoneWPixS = new QPixmap;
	*stoneWPixS = stoneWPix->scaled(stoneSize, stoneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap *stoneBPix = new QPixmap(":/stoneB.png");
	QPixmap *stoneBPixS = new QPixmap;
	*stoneBPixS = stoneBPix->scaled(stoneSize, stoneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap *nullPix = new QPixmap(":/null.png");

	/* 構建棋盤 */
	QLabel *square[BOARD_SIZE*BOARD_SIZE];
	for (int i=0; i<boardSize; i++) {
		for (int j=0; j<boardSize; j++) {
			square[i*BOARD_SIZE+j] = new QLabel;
			square[i*BOARD_SIZE+j]->setFrameStyle(QFrame::Panel + QFrame::Sunken);
			square[i*BOARD_SIZE+j]->setAlignment(Qt::AlignCenter);
			square[i*BOARD_SIZE+j]->setAttribute(Qt::WA_TranslucentBackground);
			// square[i*BOARD_SIZE+j]->setPixmap();
			grid->addWidget(square[i*BOARD_SIZE+j], i, j);
		}
	}

	/* 遊戲物件 */
	Game *game = new Game(square, stoneWPixS, stoneBPixS, nullPix);
	game->reset();

	/* 按鍵 */
	QPushButton *restartBtn = new QPushButton("Restart");
	restartBtn->setParent(window);
	restartBtn->setGeometry(boardWidth+50, 70, 100, 40);
	QObject::connect(restartBtn, SIGNAL(clicked()), game, SLOT(reset()));

	QPushButton *exitBtn = new QPushButton("Exit");
	exitBtn->setParent(window);
	exitBtn->setGeometry(boardWidth+50, 120, 100, 40);
	QObject::connect(exitBtn, SIGNAL(clicked()), &app, SLOT(quit()));

	board->setLayout(grid);

	window->show();

	return app.exec();
}
