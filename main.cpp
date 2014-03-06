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

Game *game;

/* 可以偵測滑鼠事件的視窗 */
class QMouseEventWindow : public QWidget {
	protected:
		void mouseReleaseEvent(QMouseEvent *event);
};

/* 在視窗上釋放滑鼠左鍵 */
void QMouseEventWindow::mouseReleaseEvent(QMouseEvent *event) {
	printf("Mouse click on: (%d, %d)\n", event->x(), event->y());
	int x = event->x();
	int y = event->y();
	x /= STONE_SIZE;
	y /= STONE_SIZE;
	if (x < BOARD_SIZE && y < BOARD_SIZE) {
		printf("Drop: (%d, %d)\n", x, y);
		game->drop(x, y);
	}
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
	QPixmap *stoneWPix = new QPixmap(":/img/stoneW.png");
	QPixmap *stoneWPixS = new QPixmap;
	*stoneWPixS = stoneWPix->scaled(stoneSize, stoneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap *stoneBPix = new QPixmap(":/img/stoneB.png");
	QPixmap *stoneBPixS = new QPixmap;
	*stoneBPixS = stoneBPix->scaled(stoneSize, stoneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap *stoneWaPix = new QPixmap(":/img/stoneWa.png");
	QPixmap *stoneWaPixS = new QPixmap;
	*stoneWaPixS = stoneWaPix->scaled(stoneSize, stoneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap *stoneBaPix = new QPixmap(":/img/stoneBa.png");
	QPixmap *stoneBaPixS = new QPixmap;
	*stoneBaPixS = stoneBaPix->scaled(stoneSize, stoneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap *stoneWsPix = new QPixmap(":/img/stoneWs.png");
	QPixmap *stoneWsPixS = new QPixmap;
	*stoneWsPixS = stoneWsPix->scaled(stoneSize, stoneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap *stoneBsPix = new QPixmap(":/img/stoneBs.png");
	QPixmap *stoneBsPixS = new QPixmap;
	*stoneBsPixS = stoneBsPix->scaled(stoneSize, stoneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap *nullPix = new QPixmap(":/img/null.png");

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
	game = new Game(square,
		stoneWPixS, stoneBPixS,
		stoneWaPixS, stoneBaPixS,
		stoneWsPixS, stoneBsPixS,
		nullPix);
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
