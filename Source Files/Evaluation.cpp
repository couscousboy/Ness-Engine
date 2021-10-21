#include "Evaluation.h"

Evaluation::Evaluation(Board* board) {
	this->board = board;
}

int Evaluation::Evaluate() {
	int whiteEval = countMaterial(0);
	int blackEval = countMaterial(1);

	int eval = whiteEval - blackEval;
	int colorPerspective = board->turn ? 1 : -1;
	return eval * colorPerspective;
}


int Evaluation::countMaterial(int turn) {
	Bitboard knightTurn = !turn ? board->bknights : board->wknights;
	Bitboard bishopTurn = !turn ? board->bbishops : board->wbishops;
	Bitboard rookTurn = !turn ? board->brooks : board->wrooks;
	Bitboard queenTurn = !turn ? board->bqueens : board->wqueens;
	Bitboard pawnTurn = !turn ? board->bpawns : board->wpawns;

	int material = 0;
	material += knightTurn.count() * knightVal;
	material += bishopTurn.count() * bishopVal;
	material += rookTurn.count() * rookVal;
	material += queenTurn.count() * queenVal;
	material += pawnTurn.count() * pawnVal;
	return material;
}