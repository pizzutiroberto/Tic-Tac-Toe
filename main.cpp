#include <QApplication>
#include "Board.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	(new Board())->show();

	return a.exec();
}
