#pragma once
#include <QGraphicsView>
#include <QTimer>
#include <QMenuBar>
#include <QAction>
#include "Cell.h"
#include "Game.h"

class Board : public QGraphicsView
{
	Q_OBJECT

	private:

		QGraphicsScene *scene;
		Cell ***cells;
		Game *game;
		QTimer *timer = new QTimer(this);

		QGraphicsTextItem *O_Wins;
		QGraphicsTextItem *X_Wins;
		QGraphicsTextItem *Draws;

		QMenuBar* _menuBar;
		QAction* _newGame;
		QMenu* _pieceSelection;
		QAction* _o_piece;
		QAction* _x_piece;
		QMenu* _gameMode;
		QAction* _pve;
		QAction* _pvp;
		QMenu* _themeSelection;
		QAction* _theme1;
		QAction* _theme2;

		int counter_o_wins;
		int counter_x_wins;
		int counter_draws;

		QColor background_color;
		QColor circle_color;
		QColor cross_color;
		QColor line_color;
		QColor grid_color;
		QColor text_color;

		void drawHUD();
		void drawGrid();

	protected:
		
		virtual void resizeEvent(QResizeEvent *e);

	public:

		Board();

		QColor getCircleColor() { return circle_color; }
		QColor getCrossColor() { return cross_color; }

	public slots:

		void reset();
		void update();
		void moveAt(int x, int y);
		void humanWantO();
		void humanWantX();
		void select_theme1();
		void select_theme2();
        void pveOn();
        void pvpOn();
};
