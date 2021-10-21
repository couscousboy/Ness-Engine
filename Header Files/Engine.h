#ifndef ENGINE_H
#define ENGINE_H
#include "MoveGenerator.h"
#include "Evaluation.h"
#include "Search.h"

class Engine
{
public:
	Engine(Board*, MoveGenerator*, Search*);
	void run();
private:
	uint64_t perft(int);
	void testGenSpeed();

	Board* board;
	MoveGenerator* moveGen;
	Search* search;
};

#endif ENGINE_H