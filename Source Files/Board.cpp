#include <fstream>
#include <iostream>
#include <algorithm>
#include "Board.h"

Board::~Board() {
}

Board::Board(const std::string& FEN) {
	parseFEN(FEN);
	createBoardSquares();
	for (auto const& i : pieceList) {
		i->initTexture();
		i->initSprite();
	}
	generateBitboards();
}

void Board::parseFEN(const std::string& FEN) {
	int rank = 7;
	int file = 0;
	int index;
	int team;
	char id;
	for (char const& c : FEN) {
		if (c == '/') {
			rank -= 1;
			file = 0;
		} else {
			index = rank * 8 + file;
			if (isalpha(c)) {
				isupper(c) ? team = 0 : team = 1;
				id = toupper(c);
				switch (c) {
					case 'K':
						wkings[index] = 1;
						pieceList.push_back(new King(rank, file, team, id));
						break;
					case 'Q':
						wqueens[index] = 1;
						pieceList.push_back(new Queen(rank, file, team, id));
						break;
					case 'R':
						wrooks[index] = 1;
						pieceList.push_back(new Rook(rank, file, team, id));
						break;
					case 'B':
						wbishops[index] = 1;
						pieceList.push_back(new Bishop(rank, file, team, id));
						break;
					case 'N':
						wknights[index] = 1;
						pieceList.push_back(new Knight(rank, file, team, id));
						break;
					case 'P':
						wpawns[index] = 1;
						pieceList.push_back(new Pawn(rank, file, team, id));
						break;
					case 'k':
						bkings[index] = 1;
						pieceList.push_back(new King(rank, file, team, id));
						break;
					case 'q':
						bqueens[index] = 1;
						pieceList.push_back(new Queen(rank, file, team, id));
						break;
					case 'r':
						brooks[index] = 1;
						pieceList.push_back(new Rook(rank, file, team, id));
						break;
					case 'b':
						bbishops[index] = 1;
						pieceList.push_back(new Bishop(rank, file, team, id));
						break;
					case 'n':
						bknights[index] = 1;
						pieceList.push_back(new Knight(rank, file, team, id));
						break;
					case 'p':
						id = 'p';
						bpawns[index] = 1;
						pieceList.push_back(new Pawn(rank, file, team, id));
						break;
				}
				file += 1;
			} else {
				file += c - '0';
			}
		}
	}
}

void Board::generateBitboards() {
	white = 0;
	black = 0;
	occupied = 0;
	empty = 0;
	generateWhite();
	generateBlack();
	generateOccupied();
	generateEmpty();
}

void Board::updateCastlingFlags(Move& move) {
	if (move.originBoardList == 5) {		 //White king
		wKiCastle = 0;
		wQuCastle = 0;
	} else if (move.originBoardList == 11) { //Black king
		bKiCastle = 0;
		bQuCastle = 0;
	} else if (move.originBoardList == 3) {  //White rooks
		if (move.origin == 0 && wQuCastle)
			wQuCastle = 0;
		else if (move.origin == 7 && wKiCastle)
			wKiCastle = 0;
	} else if (move.originBoardList == 9) {  //Black rooks
		if (move.origin == 56 && bQuCastle)
			bQuCastle = 0;
		else if (move.origin == 63 && bKiCastle)
			bKiCastle = 0;
	}
}

void Board::makeMove(Move& move) {
	//Update castling flags
	updateCastlingFlags(move);

	boardList[move.originBoardList]->reset(move.origin);    //Remove origin square
	boardList[move.originBoardList]->set(move.destination); //Add destination square

	if (move.moveType == 1) //Move captures
		boardList[move.destinationBoardList]->reset(move.destination); //Capture destination piece

	turn ^= 1;
	generateBitboards();
}

void Board::unmakeMove(Move& move) {
	boardList[move.originBoardList]->set(move.origin);		   //Add origin square
	boardList[move.originBoardList]->reset(move.destination);  //Remove destination square

	if (move.moveType == 1) //Move captures
		boardList[move.destinationBoardList]->set(move.destination);
	//else if (move.moveType == 2) //Castling

	turn ^= 1;
	generateBitboards();
}

void Board::makeMoveGraphic(Move& move) {
	int i = 0;
	int found = 0;
	for (Piece* piece : pieceList) {
		if (piece->getIndex() == move.origin) {
			piece->setIndex(move.destination);
			piece->updateGraphic();
		} else if (piece->getIndex() == move.destination && move.moveType == 1) {
			found = i;
		}
		i++;
	}
	if (move.moveType == 1)
		pieceList.erase(pieceList.begin() + found);
	makeMove(move);
}

// Getters ///////////////////

void Board::generateWhite() {
	for (int i = 0; i < 6; i++) {
		white |= *boardList[i];
	}
}

void Board::generateBlack() {
	for (int i = 6; i < 12; i++) {
		black |= *boardList[i];
	}
}

void Board::generateOccupied() {
	occupied = white | black;
}

void Board::generateEmpty() {
	empty = ~occupied;
}

//Graphic methods
void Board::createBoardSquares() {
	BoardSquare square;
	int i = 0;
	for (int col = 0; col < 8; col++) {
		for (int row = 0; row < 8; row++) {
			if ((col + row) % 2 == 0) {
				squareList[i] = new BoardSquare(0, row * 80.f, col * 80.f);
			} 			else {
				BoardSquare square(1, row * 80.0f, col * 80.0f);
				squareList[i] = new BoardSquare(1, row * 80.f, col * 80.f);
			}
			i++;
		}
	}
}

Piece* Board::getPieceFromCoords(int index) {
	for (Piece* piece : pieceList) {
		if (piece->getIndex() == index && piece->getColor() == this->turn){
			return piece;
		}
	}
	return nullptr;
}

BoardSquare* Board::getSquareFromCoords(int x, int y) {
	for (BoardSquare* square : squareList) {
		if (square->x == x && square->y == y) {
			return square;
		}
	}
	return nullptr;
}

Move Board::checkValidMove(std::vector<Move>& legalMoves, int origin, int destination) {
	for (const auto& move : legalMoves) {
		if (move.origin == origin && move.destination == destination)
			return move;
	}
}

void Board::renderPieces(sf::RenderWindow& window) {
	for (Piece* piece: pieceList) {
		piece->render(window);
	}
}

void Board::renderBoard(sf::RenderWindow& window) {
	for (BoardSquare* square : squareList) {
		square->render(window);
	}
}

void Board::updatePiecePositions() {
	for (Piece* piece : pieceList) {
		piece->updateGraphic();
	}
}

//Testing methods
void Board::printBitboard(Bitboard bitboard) {
	for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file < 8; file++) {
			std::cout << bitboard[file + 8 * rank];
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}