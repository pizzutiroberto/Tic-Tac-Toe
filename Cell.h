#pragma once
#include <QGraphicsRectItem>
#include <QColor>

class Board;

class Cell : public QGraphicsRectItem
{
	enum CellContent {EMPTY, CIRCLE, CROSS};

	static const int cellsize = 100;
	static const int pen_width = 5;

	protected:
		
		Board *board;
		int x;
		int y;
		int offset;
		CellContent content;

		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *e);

	public:

		Cell(Board *_board, int _x, int _y);
		
		bool empty() { return content == EMPTY; }
		void setContent(bool cross) { content = cross ? CROSS : CIRCLE; }

		friend class Board;
};