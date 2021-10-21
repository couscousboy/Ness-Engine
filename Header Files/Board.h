#ifndef BOARD_H
#define BOARD_H
#include <map>
#include <vector>
#include <bitset>
#include <SFML/graphics.hpp>
#include "BoardSquare.h"
#include "Pieces.h"


class MagicGenerator;
class BoardSquare;
class Piece;

struct Move {
	int destination;
	int origin;
	int moveType;
	int originBoardList;
	int destinationBoardList;
};

typedef std::bitset<64> Bitboard;

class Board
{
public:
	~Board();
	Board(const std::string&);
	void generateBitboards();
	void makeMove(Move&);
	void unmakeMove(Move&);
	void makeMoveGraphic(Move&);

	Piece* getPieceFromCoords(int);
	BoardSquare* getSquareFromCoords(int, int);
	Move checkValidMove(std::vector<Move>&, int, int);

	void renderPieces(sf::RenderWindow&);
	void renderBoard(sf::RenderWindow&);
	void updatePiecePositions();

	Bitboard wpawns;
	Bitboard bpawns;
	Bitboard wknights;
	Bitboard bknights;
	Bitboard wbishops;
	Bitboard bbishops;
	Bitboard wrooks;
	Bitboard brooks;
	Bitboard wqueens;
	Bitboard bqueens;
	Bitboard wkings;
	Bitboard bkings;

	Bitboard occupied;
	Bitboard empty;
	Bitboard white;
	Bitboard black;

	void printBitboard(Bitboard);

	Bitboard tempKing;
	std::vector<Bitboard*> boardList{ &wpawns, &wknights, &wbishops, &wrooks, &wqueens, &wkings, &bpawns, &bknights, &bbishops, &brooks, &bqueens, &bkings };
	char boardListIntToType[12] = { 'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'N', 'B', 'R', 'Q', 'K' };

	int turn = 1;

private:
	BoardSquare* squareList[64];
	std::vector<Piece*> pieceList;

	void parseFEN(const std::string&);

	//Bitboard operations
	void generateWhite();
	void generateBlack();
	void generateOccupied();
	void generateEmpty();

	void updateCastlingFlags(Move&);

	//Graphic methods
	void createBoardSquares();

	/////////

	int wKiCastle = 1;
	int bKiCastle = 1;
	int wQuCastle = 1;
	int bQuCastle = 1;
};

#endif