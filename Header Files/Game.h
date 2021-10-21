#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include "Pieces.h"
#include "MoveGenerator.h"
#include "Evaluation.h"
#include "Search.h"
#include "Engine.h"

class Game
{
public:
	Game();
	~Game();

	void run();
	void update();
	void render();

private:
	sf::RenderWindow* window;
	Board* board;
	MoveGenerator* moveGen;
	Evaluation* eval;
	Search* search;
	Engine* engine;

	BoardSquare* clickedSquare;
	Piece* clickedPiece;
	std::vector<Move> curPossibleMoves;
	BoardSquare* prevMoveSquare;
	bool movingPiece;
	void initWindow();
	void initBoard();
};

#endif