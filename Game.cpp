#include "Game.h"

Game::Game(bool playerTurn)
{
	_playerTurn = playerTurn;
	_isThinking = false;

	board = new char*[3];
	for (int i = 0; i < 3; i++)
	{
		board[i] = new char[3];
		for (int j = 0; j < 3; j++)
			board[i][j] = ' ';
	}

	for (int i = 0; i < 3 * 3; i++)
		freeCells.push_back(i);
}

Game::~Game()
{
	if (board)
	{
		for (int i = 0; i < 3; i++)
			delete[] board[i];
		delete[] board;
	}
}

bool Game::move(int x, int y)
{
	if (x > 3 || y > 3 || x < 0 || y < 0 || board[y][x] != ' ')
		return false;

	board[y][x] = _playerTurn ? player_piece : AI_piece;

	freeCells.remove(y * 3 + x);

	_playerTurn = !_playerTurn;

	return true;
}

bool Game::wins(bool player, int & x, int & y)
{
	char piece = player ? player_piece : AI_piece;
	x = y = -1;

	for (int i = 0; i < 3; i++)
	{
		bool won = true;
		for (int j = 0; j < 3 && won; j++)
			if (board[i][j] != piece)
				won = false;
		if (won)
		{
			y = i;
			return true;
		}
	}

	for (int j = 0; j < 3; j++)
	{
		bool won = true;
		for (int i = 0; i < 3 && won; i++)
			if (board[i][j] != piece)
				won = false;
		if (won)
		{
			x = j;
			return true;
		}
	}

	bool won = true;
	for (int j = 0; j < 3 && won; j++)
		if (board[j][j] != piece)
			won = false;
	if (won)
	{
		x = y = 0;
		return true;
	}

	won = true;
	for (int j = 0; j < 3 && won; j++)
		if (board[j][3 - 1 - j] != piece)
			won = false;
	if (won)
	{
		x = y = 1;
		return true;
	}

	return false;
}

bool Game::ended()
{
	int x;
	int y;
	return !canMove() || wins(true, x, y) || wins(false, x, y);
}

int Game::utility()
{
	int x;
	int y;
	if (wins(true, x, y))
		return -1;
	else if (wins(false, x, y))
		return 1;
	else
		return 0;
}

int Game::maxVal()
{
	if (ended())
		return utility();

	int fu = std::numeric_limits<int>::min();

	auto freeCellsCopy = freeCells;
	for (auto & v : freeCellsCopy)
	{
		board[v / 3][v % 3] = 'x';
		freeCells.remove(v);

		fu = std::max(fu, minVal());

		board[v / 3][v % 3] = ' ';
		freeCells.push_front(v);
	}
	
	return fu;
}

int Game::minVal()
{
	if (ended())
		return utility();

	int fu = std::numeric_limits<int>::max();

	auto freeCellsCopy = freeCells;
	for (auto & v : freeCellsCopy)
	{
		board[v / 3][v % 3] = 'o';
		freeCells.remove(v);

		fu = std::min(fu, maxVal());

		board[v / 3][v % 3] = ' ';
		freeCells.push_front(v);
	}

	return fu;
}

int Game::maxMove()
{
	if (ended())
		return - 1;

	int fu = std::numeric_limits<int>::min();
	int best_move = -1;

	auto freeCellsCopy = freeCells;
	for (auto & v : freeCellsCopy)
	{
		board[v / 3][v % 3] = 'x';
		freeCells.remove(v);

		int fv = minVal();
		if (fv > fu)
		{
			best_move = v;
			fu = fv;
		}

		board[v / 3][v % 3] = ' ';
		freeCells.push_front(v);
	}

	return best_move;
}

void Game::run()
{
	if (ended())
		return;

	_isThinking = true;
	int best_move = maxMove();
	_isThinking = false;
	emit notifyAIMove(best_move % 3, best_move / 3);
}
