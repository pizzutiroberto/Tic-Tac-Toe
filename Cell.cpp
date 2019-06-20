#include "Cell.h"
#include "Board.h"
#include <QPainter>
#include <string>

Cell::Cell(Board *_board, int _x, int _y) : QGraphicsRectItem()
{
	x = _x;
	y = _y;
	content = EMPTY;
	board = _board;
	offset = 15;

	setRect(cellsize * x + 0.1 * cellsize, cellsize * y + 0.1 * cellsize, cellsize - 0.2 * cellsize, cellsize - 0.2 * cellsize);
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing);

	if (content == EMPTY)
	{
		painter->setPen(Qt::NoPen);
		painter->setBrush(Qt::NoBrush);
	}
	else
	{
		if (content == CIRCLE)
		{
			painter->setPen(QPen(QBrush(board->getCircleColor()), pen_width));
			painter->drawEllipse(rect().x() + offset/2, rect().y() + offset/2, rect().width() - offset, rect().height() - offset);
		}
		else
		{
			painter->setPen(QPen(QBrush(board->getCrossColor()), pen_width));
			painter->drawLine(rect().x() + offset, rect().y() + offset, rect().x() + rect().width() - offset, rect().y() + rect().height() - offset);
			painter->drawLine(rect().x()+rect().width() - offset, rect().y() + offset, rect().x() + offset, rect().y()+rect().height() - offset);
		}
	}
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	board->moveAt(x, y);
}