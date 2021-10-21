#ifndef SEARCH_H
#define SEARCH_H
#include "MoveGenerator.h"
#include "Evaluation.h"

class Search
{
public:
	Search(Board*, MoveGenerator*, Evaluation*);
	int search(int, int, int);
	int search(int);
	Move rootSearch(int);
	std::vector<Move> bestMoves;
	std::map<int, Move> bestMoveMap;

private:
	Board* board;
	MoveGenerator* moveGen;
	Evaluation* eval;
	
	
};

#endif