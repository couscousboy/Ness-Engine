#include "Engine.h"
#include <chrono>

Engine::Engine(Board* board, MoveGenerator* moveGen, Search* search) {
	this->board = board;
	this->moveGen = moveGen;
	this->search = search;
}

void Engine::run() {
	//std::vector<Move> moves = this->moveGen->getLegalMoves();
	/*for (Move& move : moves) {
		std::cout << (move.origin & 7) << " " << (move.origin >> 3) << " | "
			<< (move.destination & 7) << " " << (move.destination >> 3) << "\n";
		std::cout << move.origin << " " << move.destination << "\n";
	}*/
	//std::cout << moves.size() << "\n";
	//testGenSpeed();

	
	/*for (int i = 0; i < 5; i++) {
		std::cout << search->search(i) << "\n";
	}*/

}

uint64_t Engine::perft(int depth) {
	if (depth == 0)
		return 1ull;

	std::vector<Move> legalMoves = moveGen->getLegalMoves();
	uint64_t nodes = 0;

	for (Move& move : legalMoves) {
		board->makeMove(move);
		nodes += perft(depth - 1);
		board->unmakeMove(move);
	}

	return nodes;
}

void Engine::testGenSpeed() {
	for (int i = 1; i < 10; i++) {
		auto t1 = std::chrono::high_resolution_clock::now();
		uint64_t nodes = perft(i);
		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		auto s = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
		std::cout << "Depth " << i << " - " << nodes << " in " << ms.count() << "ms";
		if (s.count() != 0)
			std::cout << " " << (nodes / s.count()) << "NPS";
		std::cout << "\n";
	}
}