#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "MagicGenerator.h"
#include "Board.h"
#include <bitset>
#include <vector>

class Board;

typedef std::bitset<64> Bitboard;
class MoveGenerator
{
public:
	MoveGenerator(Board*);
	std::vector<Move> getLegalMoves();
	void getPregeneratedMoves(std::string, Bitboard*);

	int getCheck();

private:
	//Helper methods
	int getNextSetBit(Bitboard*);
	void removeEnemyKing(int);
	void addEnemyKing(int);
	bool isSlidingPiece(char);
	char getPieceType(int, int);
	int  getBoardList(int, int);

	//Move generation
	Bitboard getPossibleMoves(int, int, char);
	Bitboard getPawnAttacks(int, int, char);
	std::vector<Move> test();
	void getKingDangerSquares(int, char);
	void getEvasions(int);
	void setCheck();
	void getCheckMask(int, int);
	void getPinnedPieces(int, Bitboard&, Bitboard&);
	void getPinnedMoves(int, Bitboard&, std::vector<Move>&);
	void getKingMoves(int, Bitboard&, std::vector<Move>&);
	void getPieceMoves(int, Bitboard&, std::vector<Move>&);

	MagicGenerator magicGen;
	Board *board;

	Bitboard knightMoves[64];
	Bitboard kingMoves[64];
	Bitboard whitePawnPushes[64];
	Bitboard whitePawnAttacks[64];
	Bitboard blackPawnPushes[64];
	Bitboard blackPawnAttacks[64];

	Bitboard toCalculate;
	Bitboard attackedSquares;
	Bitboard kingDangerSquares;
	Bitboard attackers;
	Bitboard pinningPieces;
	Bitboard pinnedPieces;

	Bitboard captureMask = 0xFFFFFFFFFFFFFFFF;
	Bitboard pushMask = 0xFFFFFFFFFFFFFFFF;

	int check = 0;
};

#endif