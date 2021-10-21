#ifndef EVALUATE_H
#define EVALUATE_H
#include "Board.h"
#include "Evaluation.h"
#include <iostream>
class Evaluation
{
public:
	Evaluation(Board*);
	int Evaluate();
private:
	static const int pawnVal = 100;
	static const int knightVal = 300;
	static const int bishopVal = 300;
	static const int rookVal = 500;
	static const int queenVal = 900;

	int countMaterial(int);

	Board* board;
};

#endif