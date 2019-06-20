#pragma once
#include <QThread>
#include <set>

class Game : public QThread
{
	Q_OBJECT

	static const char AI_piece = 'x';
	static const char player_piece = 'o';

	private:

		char **board;
		bool _playerTurn;
		bool _isThinking;
		std::list<int> freeCells;

		int utility();
		int maxMove();
		int maxVal();
		int minVal();

		void run();

	public:

		Game(bool playerTurn = false);
		~Game();

		bool playerTurn() { return _playerTurn; }
		bool isThinking() { return _isThinking; }
		bool move(int x, int y);
		bool canMove() { return !freeCells.empty(); }
		bool wins(bool player, int & x, int & y);
		bool ended();

	signals:
			
		void notifyAIMove(int x, int y);
};