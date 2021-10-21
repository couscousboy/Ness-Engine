#include "Search.h"

Search::Search(Board* board, MoveGenerator* moveGen, Evaluation* evaluation) {
	this->board = board;
	this->moveGen = moveGen;
	this->eval = evaluation;
}

int Search::search(int depth, int alpha, int beta) {
	if (depth == 0) {
		return eval->Evaluate();
	}

	std::vector<Move> moves = moveGen->getLegalMoves();
	if (moves.size() == 0) {
		if (moveGen->getCheck()) { //Checkmate
			return -999999;
		}
		return 0; //Stalemate
	}

	for (Move& move : moves) {
		board->makeMove(move);
		int evaluation = -search(depth - 1, -beta, -alpha);
//std::cout << move.origin << " " << move.destination << " " << evaluation << "\n";
		board->unmakeMove(move);

		if (evaluation >= beta) 
			return beta;

		if (alpha < evaluation) {
			alpha = evaluation;
			bestMoveMap[depth] = move;
		}
			
		alpha = std::max(alpha, evaluation);
	}
	return alpha;
}

int Search::search(int depth) {
	if (depth == 0) {
		return eval->Evaluate();
	}

	std::vector<Move> moves = moveGen->getLegalMoves();
	if (moves.size() == 0) {
		if (moveGen->getCheck()) { //Checkmate
			return -999999;
		}
		return 0; //Stalemate
	}

	int bestEval = -999999;

	for (Move& move : moves) {
		board->makeMove(move);
		int evaluation = -search(depth - 1);
		bestEval = std::max(evaluation, bestEval);
		board->unmakeMove(move);
		bestMoveMap.insert({ evaluation, move });
	}
	return bestEval;
}

Move Search::rootSearch(int bestEval) {
	Move bestMove = bestMoveMap.find(bestEval)->second;
	return bestMove;
}