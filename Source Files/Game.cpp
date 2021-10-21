#include "Game.h"
#include <chrono>
#include <thread>

Game::Game() {
	this->initWindow();
	this->initBoard();
}

Game::~Game() {
	delete this->window;
	delete this->board;
}

void Game::initWindow() {
	this->window = new sf::RenderWindow(sf::VideoMode(640, 640), "Chess", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(30);
	this->window->setVerticalSyncEnabled(false);
}

void Game::initBoard() {
	this->board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
	//this->board = new Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1");
	//this->board = new Board("5k2/8/2q1R1pp/8/8/8/8/1K6");
	//this->board = new Board("1k6/pppq4/8/8/8/8/P7/1K3R2");
	//this->board = new Board("5r2/8/8/pRk3p1/2N3P1/P4b2/1K6/5B2");
	this->moveGen = new MoveGenerator(this->board);
	this->eval = new Evaluation(this->board);
	this->search = new Search(this->board, this->moveGen, this->eval);
	this->engine = new Engine(this->board, this->moveGen, this->search);
	this->engine->run();
	curPossibleMoves = moveGen->getLegalMoves();
}

void Game::run() {
	while (this->window->isOpen()) {
		this->render();
		this->update();
	}
}

void Game::update() {
	sf::Event e;
	while (this->window->pollEvent(e)) {
		if (e.Event::type == sf::Event::Closed) {
			this->window->close();

		} else if (e.Event::type == sf::Event::MouseButtonPressed) {

			if (e.mouseButton.button == sf::Mouse::Left) {
				int x = sf::Mouse::getPosition(*window).x;
				int y = sf::Mouse::getPosition(*window).y;

				int roundedx = (x / 80);
				int roundedy = (y / 80);
			    roundedy = -roundedy + 7;
				int index = ((roundedy * 8) + roundedx);
				
				clickedPiece = this->board->getPieceFromCoords(index);
				if (clickedPiece) { //Checks that clicked piece is not nullptr (movable piece clicked)
					movingPiece = true;
					if (clickedSquare) //Checks that clicked square is not nullptr
						clickedSquare->resetColor();
					clickedSquare = this->board->getSquareFromCoords((x / 80) * 80, (y / 80) * 80);
					clickedSquare->setColor(sf::Color(186, 202, 68));
				}
			}

		} else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			int x = sf::Mouse::getPosition(*window).x;
			int y = sf::Mouse::getPosition(*window).y;
			if (clickedPiece && movingPiece)
				clickedPiece->setPiecePickedPosition(x, y);

		} else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			int x = sf::Mouse::getPosition(*window).x;
			int y = sf::Mouse::getPosition(*window).y;

			int roundedx = (x / 80);
			int roundedy = (y / 80);
			roundedy = -roundedy + 7;
			int index = ((roundedy * 8) + roundedx);

			

			if (clickedPiece && movingPiece) {
				Move move = board->checkValidMove(curPossibleMoves, clickedPiece->getIndex(), index);
				std::cout << move.destination << " " << move.origin << "\n";
				try { //Making a move
					board->makeMoveGraphic(move);
					if (prevMoveSquare)
						prevMoveSquare->resetColor();
					clickedSquare->resetColor();
					curPossibleMoves = moveGen->getLegalMoves();

					//Engine Run Move
					int best = this->search->search(6, -99999, 99999);
					std::cout << best << "\n";
					Move bestMove = this->search->bestMoveMap[6];
					std::cout << bestMove.origin << " " << bestMove.destination << "\n";
					this->board->makeMoveGraphic(bestMove);
					this->search->bestMoveMap.clear();

					roundedx = bestMove.destination >> 3;
					roundedy = bestMove.destination & 7;
					roundedx = -roundedx + 7;

					std::cout << roundedx << "---" << roundedy;

					prevMoveSquare = this->board->getSquareFromCoords(roundedy * 80, roundedx * 80);
					prevMoveSquare->setColor(sf::Color(246, 246, 105));
					curPossibleMoves = moveGen->getLegalMoves();
				} catch (std::out_of_range) {} //If attempted move is not possible
				clickedPiece->setPiecePosition();
			}
			movingPiece = false;
		}
	}
}

void Game::render() {
	this->window->clear();
	this->board->renderBoard(*this->window);
	this->board->renderPieces(*this->window);
	this->window->display();
}

