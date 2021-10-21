#include <fstream>
#include <string>
#include "MoveGenerator.h"
#include "MagicGenerator.h"

MoveGenerator::MoveGenerator(Board* board) {
	this->board = board;
	getPregeneratedMoves("Static Bitboards/knightMoves.txt", knightMoves);
	getPregeneratedMoves("Static Bitboards/kingMoves.txt", kingMoves);
	getPregeneratedMoves("Static Bitboards/whitePawnPushes.txt", whitePawnPushes);
	getPregeneratedMoves("Static Bitboards/whitePawnAttacks.txt", whitePawnAttacks);
	getPregeneratedMoves("Static Bitboards/blackPawnPushes.txt", blackPawnPushes);
	getPregeneratedMoves("Static Bitboards/blackPawnAttacks.txt", blackPawnAttacks);
}

void MoveGenerator::getPregeneratedMoves(std::string file, Bitboard* arr) {
	std::ifstream moveFile;
	moveFile.open(file);
	std::string line;
	int i = 0;
	while (std::getline(moveFile, line)) {
		Bitboard move(line);
		arr[i] = move;
		i++;
	}
}

int MoveGenerator::getCheck() {
	return check;
}

int MoveGenerator::getNextSetBit(Bitboard* bits) {
	int bitInd = magicGen.bitScanForward(bits->to_ullong());
	bits->reset(bitInd);
	return bitInd;
}

void MoveGenerator::removeEnemyKing(int turn) {
	if (turn) {
		board->tempKing = board->bkings;
		board->bkings = 0;
	} else {
		board->tempKing = board->wkings;
		board->wkings = 0;
	}
	board->generateBitboards();
}

void MoveGenerator::addEnemyKing(int turn) {
	if (turn)
		board->bkings = board->tempKing;
	else
		board->wkings = board->tempKing;
	board->tempKing = 0;
	board->generateBitboards();
}

bool MoveGenerator::isSlidingPiece(char type) {
	if (type == 'R' || type == 'B' || type == 'Q')
		return true;
	return false;
}

char MoveGenerator::getPieceType(int sq, int color) { //0 = white, 1 == black
	int iStart = 0;
	int iEnd = 6;

	if (color) {
		iStart = 6;
		iEnd = 12;
	}

	for (iStart; iStart < iEnd; iStart++) {
		if (board->boardList[iStart]->test(sq))
			return board->boardListIntToType[iStart];
	}

	return NULL;
}

int MoveGenerator::getBoardList(int sq, int color) {
	int iStart = 0;
	int iEnd = 6;

	if (color) {
		iStart = 6;
		iEnd = 12;
	}

	for (int i = 0; i < 12; i++) {
		if (board->boardList[i]->test(sq))
			return i;
	}

	return NULL;
}

Bitboard MoveGenerator::getPossibleMoves(int sq, int color, char pieceType) {
	Bitboard moves;
	Bitboard self = (color == 0) ? board->white : board->black;
	switch (pieceType) {
		case 'P':
			moves |= ((whitePawnPushes[sq] & ~board->occupied) | (whitePawnAttacks[sq] & board->black));
			break;
		case 'p':
			moves |= (blackPawnPushes[sq] | (blackPawnAttacks[sq] & board->white));
			break;
		case 'N':
			moves |= (knightMoves[sq]);
			break;
		case 'B':
			moves |= magicGen.getBishopAttacks(sq, board->occupied);
			break;
		case 'R':
			moves |= magicGen.getRookAttacks(sq, board->occupied);
			break;
		case 'Q':
			moves |= magicGen.getQueenAttacks(sq, board->occupied);
			break;
		case 'K':
			moves |= (kingMoves[sq] & ~kingDangerSquares);
			break;
	}
	return (moves & ~self);
}

Bitboard MoveGenerator::getPawnAttacks(int sq, int color, char pieceType) {
	Bitboard moves;
	Bitboard self = (color == 0) ? board->white : board->black;
	switch (pieceType) {
		case 'P':
			moves |= whitePawnAttacks[sq];
			break;
		case 'p':
			moves |= blackPawnAttacks[sq];
			break;
	}
	return (moves & ~self);
}

std::vector<Move> MoveGenerator::getLegalMoves() {
	std::vector<Move> legalMoves;
	legalMoves.reserve(218);
	Bitboard kingTurn = board->turn ? board->bkings : board->wkings;
	Bitboard colorTurn = board->turn ? board->white : board->black;
	Bitboard notColorTurn = board->turn ? board->black : board->white;
	Bitboard colorTurnCopy = colorTurn;
	int kingSquare = magicGen.bitScanForward(kingTurn.to_ullong());

	if (board->turn)
		toCalculate = board->black;
	else
		toCalculate = board->white;

	kingDangerSquares = 0;
	attackers = 0;

	getEvasions(kingSquare);
	setCheck(); // 0 = No check | 1 = Single check | 2 = Double check

	if (check != 2) {
		getPinnedPieces(kingSquare, kingTurn, notColorTurn);
		toCalculate &= ~pinnedPieces;
		getPinnedMoves(kingSquare, colorTurn, legalMoves);
	}

	while (colorTurnCopy != 0) {
		int piece = getNextSetBit(&colorTurnCopy);
		char pieceType = getPieceType(piece, board->turn ^ 1);
		getKingDangerSquares(piece, pieceType);
	}

	if (check == 1) { //Only some squares legal if single check
		getCheckMask(check, kingSquare);
	} else if (check == 2) { //Only king moves legal if double check
		getKingMoves(kingSquare, colorTurn, legalMoves);
		return legalMoves;
	}

	getKingMoves(kingSquare, colorTurn, legalMoves);
	toCalculate &= ~kingTurn;
	getPieceMoves(check, colorTurn, legalMoves);
//	board->printBitboard(pinnedPieces);
//	board->printBitboard(pinningPieces);
	return legalMoves;
}

std::vector<Move> MoveGenerator::test() {
	std::vector<Move> legalMoves;
	legalMoves.reserve(218);
	Bitboard colorTurn = board->turn ? board->white : board->black;
	if (board->turn)
		toCalculate = board->black;
	else
		toCalculate = board->white;

	while (toCalculate != 0) {
		int piece = getNextSetBit(&toCalculate);
		Bitboard moves = getPossibleMoves(piece, board->turn, getPieceType(piece, board->turn));
		moves &= (pushMask | captureMask);
		while (moves != 0) {
			int i = getNextSetBit(&moves);
			int moveType = 0;
			if (colorTurn.test(i))
				moveType = 1;
			legalMoves.push_back(Move{ i, piece, moveType, getBoardList(piece, board->turn), getBoardList(i, board->turn ^ 1) });
		}
	}
	return legalMoves;
}

void MoveGenerator::getKingDangerSquares(int index, char pieceType) {
	removeEnemyKing(board->turn);
	if (pieceType != 'P' && pieceType != 'p')
		kingDangerSquares |= getPossibleMoves(index, board->turn, pieceType); //Pretend enemy piece can capture its own colour to prevent evasion to attacked square
	else
		kingDangerSquares |= getPawnAttacks(index, board->turn, pieceType);
	addEnemyKing(board->turn);
}

void MoveGenerator::getEvasions(int kingSquare) {
	Bitboard knightTurn = !board->turn ? board->bknights : board->wknights;
	Bitboard bishopTurn = !board->turn ? board->bbishops : board->wbishops;
	Bitboard rookTurn = !board->turn ? board->brooks : board->wrooks;
	Bitboard queenTurn = !board->turn ? board->bqueens : board->wqueens;
	Bitboard pawnTurn = !board->turn ? board->bpawns : board->wpawns;

	attackers |= knightMoves[kingSquare] & knightTurn;
	attackers |= magicGen.getBishopAttacks(kingSquare, board->occupied) & bishopTurn;
	attackers |= magicGen.getRookAttacks(kingSquare, board->occupied) & rookTurn;
	attackers |= magicGen.getQueenAttacks(kingSquare, board->occupied) & queenTurn;
}

void MoveGenerator::setCheck() {
	int numAttackers = attackers.count();
	check = (numAttackers < 2) ? numAttackers : 2;
}

void MoveGenerator::getCheckMask(int checkType, int kingSquare) {
	pushMask = 0xFFFFFFFFFFFFFFFF;
	captureMask = attackers;

	int checkSquare = magicGen.bitScanForward(attackers.to_ullong());
	char pieceType = getPieceType(checkSquare, board->turn ^ 1);
	if (isSlidingPiece(pieceType)) {
		pushMask = magicGen.getRayToSquare(checkSquare, kingSquare, pieceType);
	} else {
		pushMask = 0;
	}
}

void MoveGenerator::getPinnedPieces(int kingSquare, Bitboard& kingTurn, Bitboard& colorTurn) { //Only consider some pieces
	pinnedPieces = 0;
	pinningPieces = 0;
	Bitboard opRQ;
	Bitboard opBQ;
	Bitboard pinRays;
	if (!board->turn) {
		opRQ = (board->brooks | board->bqueens);
		opBQ = (board->bbishops | board->bqueens);
	} else {
		opRQ = (board->wrooks | board->wqueens);
		opBQ = (board->wbishops | board->wqueens);
	}

	Bitboard pinner = magicGen.xrayRookAttacks(board->occupied, colorTurn, kingSquare) & opRQ;
	pinningPieces |= pinner;
	while (pinner != 0) {
		int sq = getNextSetBit(&pinner);
		pinnedPieces |= (magicGen.getRayOverlaps(sq, kingSquare, 'R') & colorTurn);
		//pinnedPieces.set(getNextSetBit(&pinRays));
	}

	pinner = magicGen.xrayBishopAttacks(board->occupied, colorTurn, kingSquare) & opBQ;
	pinningPieces |= pinner;
	while (pinner != 0) {
		int sq = getNextSetBit(&pinner);
		pinnedPieces |= (magicGen.getRayOverlaps(sq, kingSquare, 'B') & colorTurn);
		//pinnedPieces.set(getNextSetBit(&pinRays));
	}
}

void MoveGenerator::getPinnedMoves(int kingSquare, Bitboard& colorTurn, std::vector<Move>& legalMoves) {
	while (pinningPieces != 0) {
		int pinningSq = getNextSetBit(&pinningPieces);
		Bitboard rayToPinner = magicGen.getRayToSquare(kingSquare, pinningSq, getPieceType(pinningSq, board->turn ^ 1));
		rayToPinner.set(pinningSq);
		Bitboard pinnedToCalculate = rayToPinner & pinnedPieces;

		while (pinnedToCalculate != 0) {
			int pinnedSq = getNextSetBit(&pinnedToCalculate);
			char pinnedType = getPieceType(pinnedSq, board->turn);

			if (pinnedType != 'N') {
				Bitboard moves = getPossibleMoves(pinnedSq, board->turn, pinnedType);
				moves &= rayToPinner;

				while (moves != 0) {
					int moveSq = getNextSetBit(&moves);
					int moveType = 0;
					if (colorTurn.test(moveSq))
						moveType = 1;

					legalMoves.push_back(Move{ moveSq, pinnedSq, moveType,
						getBoardList(pinnedSq, board->turn), getBoardList(moveSq, board->turn ^ 1) });
				}
			}

		}
	}
}

void MoveGenerator::getKingMoves(int kingSquare, Bitboard& colorTurn, std::vector<Move>& legalMoves) {
	Bitboard kingMoves = getPossibleMoves(kingSquare, board->turn, 'K');
	while (kingMoves != 0) {
		int i = getNextSetBit(&kingMoves);
		int moveType = 0;
		if (colorTurn.test(i))
			moveType = 1;
		legalMoves.push_back(Move{ i, kingSquare, moveType, getBoardList(kingSquare, board->turn), getBoardList(i,board->turn ^ 1) });
	}
}

void MoveGenerator::getPieceMoves(int check, Bitboard& colorTurn, std::vector<Move>& legalMoves) {
	while (toCalculate != 0) {
		int piece = getNextSetBit(&toCalculate);
		Bitboard moves = getPossibleMoves(piece, board->turn, getPieceType(piece, board->turn));
		if (check != 0)
			moves &= (pushMask | captureMask);
		while (moves != 0) {
			int i = getNextSetBit(&moves);
			int moveType = 0;
			if (colorTurn.test(i))
				moveType = 1;
			legalMoves.push_back(Move{ i, piece, moveType, getBoardList(piece, board->turn), getBoardList(i, board->turn ^ 1) });
		}
	}
}