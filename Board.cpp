#include "Board.h"
#include <QMessageBox>

Board::Board() : QGraphicsView()
{
	cells = 0;
	game = 0;
	counter_draws = 0;
	counter_o_wins = 0;
	counter_x_wins = 0;

	background_color = Qt::black;
	grid_color = Qt::magenta;
	circle_color = Qt::cyan;
	cross_color = Qt::yellow;
	line_color = Qt::white;
	text_color = Qt::white;

	scene = new QGraphicsScene();
	scene->setBackgroundBrush(QBrush(QColor(background_color)));
	setScene(scene);
	setInteractive(true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scene->setSceneRect(0, 0, Cell::cellsize * 3, Cell::cellsize * 3);

	_menuBar = new QMenuBar(this);
	_newGame = new QAction("New Game", 0);
	_gameMode = new QMenu("Game Mode");
	_pve = new QAction("Player VS AI", 0);
	_pve->setCheckable(true);
	_pve->setChecked(true);
	_pvp = new QAction("Player VS Player", 0);
	_pvp->setCheckable(true);
	_pvp->setChecked(false);
	_pieceSelection = new QMenu("Piece");
	_o_piece = new QAction("Play as O", 0);
	_o_piece->setCheckable(true);
	_o_piece->setChecked(true);
	_x_piece = new QAction("Play as X", 0);
	_x_piece->setCheckable(true);
	_x_piece->setChecked(false);
	_themeSelection = new QMenu("Theme");
	_theme1 = new QAction("Neon");
	_theme1->setCheckable(true);
	_theme1->setChecked(true);
	_theme2 = new QAction("Simple");
	_theme2->setCheckable(true);
	_theme2->setChecked(false);

	_menuBar->addAction(_newGame);
	_menuBar->addMenu(_gameMode);
	_gameMode->addAction(_pve);
	_gameMode->addAction(_pvp);
	_menuBar->addMenu(_pieceSelection);
	_pieceSelection->addAction(_o_piece);
	_pieceSelection->addAction(_x_piece);
	_menuBar->addMenu(_themeSelection);
	_themeSelection->addAction(_theme1);
	_themeSelection->addAction(_theme2);

	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	connect(_newGame, SIGNAL(triggered()), this, SLOT(reset()));
	connect(_pve, SIGNAL(triggered()), this, SLOT(pveOn()));
	connect(_pvp, SIGNAL(triggered()), this, SLOT(pvpOn()));
	connect(_o_piece, SIGNAL(triggered()), this, SLOT(humanWantO()));
	connect(_x_piece, SIGNAL(triggered()), this, SLOT(humanWantX()));
	connect(_theme1, SIGNAL(triggered()), this, SLOT(select_theme1()));
	connect(_theme2, SIGNAL(triggered()), this, SLOT(select_theme2()));

	reset();
	resizeEvent(0);
}

void Board::reset()
{
	if (cells)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				scene->removeItem(cells[i][j]);
				delete cells[i][j];
			}
			delete cells[i];
		}
		delete cells;
	}

	if (game)
		delete game;

	scene->clear();

	cells = new Cell**[3];
	for (int i = 0; i < 3; i++)
	{
		cells[i] = new Cell*[3];
		for (int j = 0; j < 3; j++)
		{
			cells[i][j] = new Cell(this, j, i);
			scene->addItem(cells[i][j]);
		}
	}

	game = new Game(true);
	timer->start(1000);
	drawGrid();
	drawHUD();

	connect(game, SIGNAL(notifyAIMove(int, int)), this, SLOT(moveAt(int, int)));

	if (!game->playerTurn() && _pve->isChecked())
		game->start();
}

void Board::update() { }

void Board::resizeEvent(QResizeEvent *e)
{
	fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void Board::moveAt(int x, int y)
{
	bool playerTurn = game->playerTurn();

	if (game->isThinking())
		return;

	if (!game->move(x, y))
		return;

	bool piece_selection;
	if (playerTurn)
		piece_selection = _o_piece->isChecked() ? false : true;
	else
		piece_selection = _o_piece->isChecked() ? true : false;

	cells[y][x]->setContent(piece_selection);
	scene->update();

	int xw;
	int yw;
	if (game->wins(playerTurn, xw, yw))
	{
		int line_width = 10;
		if (xw >= 0 && yw < 0)
			scene->addLine(xw*Cell::cellsize + Cell::cellsize / 2, 0, xw*Cell::cellsize + Cell::cellsize / 2, Cell::cellsize * 3, QPen(QBrush(line_color), line_width));
		else if (yw >= 0 && xw < 0)
			scene->addLine(0, yw*Cell::cellsize + Cell::cellsize / 2, Cell::cellsize * 3, yw*Cell::cellsize + Cell::cellsize / 2, QPen(QBrush(line_color), line_width));
		else if (yw == 0 && xw == 0)
			scene->addLine(0, 0, Cell::cellsize * 3, Cell::cellsize * 3, QPen(QBrush(line_color), line_width));
		else if (yw == 1 && xw == 1)
			scene->addLine(Cell::cellsize * 3, 0, 0, Cell::cellsize * 3, QPen(QBrush(line_color), line_width));

		scene->update();

		if (playerTurn)
			_o_piece->isChecked() ? counter_o_wins++ : counter_x_wins++;
		else
			_o_piece->isChecked() ? counter_x_wins++ : counter_o_wins++;

		QString humanO = _o_piece->isChecked() ? "O" : "X";
		QString humanX = _o_piece->isChecked() ? "X" : "O";
		QMessageBox::information(this, "Game Over", QString("") + (playerTurn ? humanO : humanX) + " wins!");
		reset();
	}
	else if (game->ended())
	{
		counter_draws++;
		QMessageBox::information(this, "Game Over", "This is a draw!");
		reset();
	}

	if (!game->playerTurn() && _pve->isChecked())
		game->start();
}

void Board::humanWantO()
{
	_o_piece->setChecked(true);
	_x_piece->setChecked(false);

	int tmp_counter = counter_o_wins;
	counter_o_wins = counter_x_wins;
	counter_x_wins = tmp_counter;

	reset();
}

void Board::humanWantX()
{
	_x_piece->setChecked(true);
	_o_piece->setChecked(false);

	int tmp_counter = counter_o_wins;
	counter_o_wins = counter_x_wins;
	counter_x_wins = tmp_counter;

	reset();
}

void Board::pveOn()
{
    _pve->setChecked(true);
    _pvp->setChecked(false);
    
    counter_draws = 0;
    counter_o_wins = 0;
    counter_x_wins = 0;

    reset();
}

void Board::pvpOn()
{
    _pvp->setChecked(true);
    _pve->setChecked(false);
    
    counter_draws = 0;
    counter_o_wins = 0;
    counter_x_wins = 0;

    reset();
}

void Board::select_theme1()
{
	_theme1->setChecked(true);
	_theme2->setChecked(false);

	background_color = Qt::black;
	grid_color = Qt::magenta;
	circle_color = Qt::cyan;
	cross_color = Qt::yellow;
	line_color = Qt::white;
	text_color = Qt::white;

	scene->setBackgroundBrush(QBrush(QColor(background_color)));
	drawHUD();
	drawGrid();
	scene->update();
}

void Board::select_theme2()
{
	_theme2->setChecked(true);
	_theme1->setChecked(false);

	background_color = Qt::white;
	grid_color = Qt::black;
	circle_color = Qt::blue;
	cross_color = Qt::red;
	line_color = Qt::black;
	text_color = Qt::black;

	scene->setBackgroundBrush(QBrush(QColor(background_color)));
	drawHUD();
	drawGrid();
	scene->update();
}

void Board::drawHUD()
{
	O_Wins = scene->addText("O Wins: " + QString::number(counter_o_wins));
	O_Wins->setDefaultTextColor(text_color);
	O_Wins->setPos(15, 280);
	X_Wins = scene->addText("X Wins: " + QString::number(counter_x_wins));
	X_Wins->setDefaultTextColor(text_color);
	X_Wins->setPos(118, 280);
	Draws = scene->addText("Draws: " + QString::number(counter_draws));
	Draws->setDefaultTextColor(text_color);
	Draws->setPos(220, 280);
}

void Board::drawGrid()
{
	for (int i = 1; i < 3; i++)
	{
		scene->addLine(Cell::cellsize*i, 10, Cell::cellsize*i, Cell::cellsize * 3 - 10, QPen(QBrush(grid_color), 3));
		scene->addLine(10, Cell::cellsize*i, Cell::cellsize * 3 - 10, Cell::cellsize*i, QPen(QBrush(grid_color), 3));
	}
}
