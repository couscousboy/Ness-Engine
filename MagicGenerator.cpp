#include "MagicGenerator.h"

MagicGenerator::MagicGenerator() {
	generateNorth();
	generateSouth();
	generateEast();
	generateWest();
	generateNoEa();
	generateNoWe();
	generateSoEa();
	generateSoWe();
	generateRookAttacks();
	generateBishopAttacks();
	generateQueenAttacks();
	generateBlockers();
	generateAttackSets();
}


int MagicGenerator::bitScanForward(uint64_t bitboard) {
	return index64[((bitboard ^ (bitboard - 1))* 0x03f79d71b4cb0a89ULL) >> 58];
}

int MagicGenerator::bitScanReverse(uint64_t bitboard) {
	bitboard |= bitboard >> 1;
	bitboard |= bitboard >> 2;
	bitboard |= bitboard >> 4;
	bitboard |= bitboard >> 8;
	bitboard |= bitboard >> 16;
	bitboard |= bitboard >> 32;
	return index64[(bitboard * 0x03f79d71b4cb0a89ULL) >> 58];
}

void MagicGenerator::generateNorth() {
	moveSet temp;
	std::bitset<64> north(0x0101010101010100);
	for (int i = 0; i < 64; i++, north <<= 1) {
		temp.push_back(north);
	}
	rayAttacks[North] = temp;
}

void MagicGenerator::generateSouth() {
	moveSet temp;
	std::bitset<64> south(0x0080808080808080);
	for (int i = 63; i >= 0; i--, south >>= 1) {
		temp.push_back(south);
	}
	std::reverse(temp.begin(), temp.end());
	rayAttacks[South] = temp;
}

void MagicGenerator::generateEast() {
	moveSet temp;
	std::bitset<64> firstRank(0x00000000000000FF);
	std::bitset<64> east(0x00000000000000FE);
	for (int i = 0; i < 64; i++, east <<= 1) {
		if (i % 8 == 0) {
			firstRank <<= 8;
		}
		temp.push_back(east & ~firstRank);
	}
	rayAttacks[East] = temp;
}

void MagicGenerator::generateWest() {
	moveSet temp;
	std::bitset<64> eightRank(0xFF00000000000000);
	std::bitset<64> west(0x7F00000000000000);
	for (int i = 63; i >= 0; i--, west >>= 1) {
		if ((i+1) % 8 == 0) {
			eightRank >>= 8;
		}
		temp.push_back(west & ~eightRank);
	}
	std::reverse(temp.begin(), temp.end());
	rayAttacks[West] = temp;
}

void MagicGenerator::generateNoEa() {
	moveSet temp;
	std::bitset<64> mask = ~empty;
	std::bitset<64> noea(0x8040201008040200);
	int wrap = 1;
	int file = 0;
	for (int i = 0; i < 64; i++, noea <<= 1) {
		if (i % 8 == 0) {
			wrap++;
			mask = ~empty;
			file = 0;
		}
		if (file >= wrap) {
			mask >>= 8;
		}
		temp.push_back(mask & noea);
		file++;
	}
	rayAttacks[NoEa] = temp;
}

void MagicGenerator::generateNoWe() {
	moveSet temp;
	std::bitset<64> mask(0x0101010101010101);
	std::bitset<64> nowe(0x102040810204000 >> 7);
	for (int i = 0; i < 64; i++, nowe <<= 1) {
		if (i % 8 == 0) {
			mask = 0x0101010101010101;
		}
		temp.push_back(mask & nowe);
		mask = mask | mask << 1;

	}
	rayAttacks[NoWe] = temp;
}

void MagicGenerator::generateSoWe() {
	moveSet temp;
	int file = 7;
	std::bitset<64> mask(0x8080808080808080);
	std::bitset<64> sowe(0x40201008040201);
	for (int i = 63; i >= 0; i--, sowe >>= 1) {
		if ((i + 1) % 8 == 0) {
			file = 7;
		}
		if (file == 7) {
			mask = empty;
		} else if (file == 6) {
			mask = 0x8080808080808080;
		}
		temp.push_back(~mask & sowe);
		mask = mask | mask >> 1;
		file--;
	}
	std::reverse(temp.begin(), temp.end());
	rayAttacks[SoWe] = temp;
}

void MagicGenerator::generateSoEa() {
	moveSet temp;
	int file = 7;
	std::bitset<64> mask(0x8080808080808080);
	std::bitset<64> soea(0x2040810204080 << 7);

	for (int i = 63; i >= 0; i--, soea >>= 1) {
		if ((i + 1) % 8 == 0) {
			file = 7;
		}
		if (file == 7) {
			mask = empty;
		} else if (file == 6) {
			mask = 0x8080808080808080;
		}
		temp.push_back(mask & soea);
		mask = mask | mask >> 1;
		file--;
	}
	std::reverse(temp.begin(), temp.end());
	rayAttacks[SoEa] = temp;
}

void MagicGenerator::generateRookAttacks() {
	for (int i = 0; i < 64; i++) {
		rookAttacks.push_back(rayAttacks[North][i] | rayAttacks[East][i] | rayAttacks[South][i] | rayAttacks[West][i]);
	}
}

void MagicGenerator::generateBishopAttacks() {
	for (int i = 0; i < 64; i++) {
		bishopAttacks.push_back(rayAttacks[NoEa][i] | rayAttacks[NoWe][i] | rayAttacks[SoEa][i] | rayAttacks[SoWe][i]);
	}
}

void MagicGenerator::generateQueenAttacks() {
	for (int i = 0; i < 64; i++) {
		queenAttacks.push_back(rookAttacks[i] | bishopAttacks[i]);
	}
}

std::bitset<64> MagicGenerator::generateBlockerBoard(int sq, std::bitset<64> blockerMask) {
	std::bitset<64> blockerBoard = blockerMask;
	int bitInd = 0;
	for (int i = 0; i < 64; i++) {
		if (blockerMask[i] == 1) {
			if (!(sq & (1 << bitInd))) {
				blockerBoard &= ~(1ULL << i);
			}
			bitInd++;
		}
	}
	return blockerBoard;
}

void MagicGenerator::generateBlockers() {
	std::bitset<64> aFile(0x0101010101010101);
	std::bitset<64> hFile(0x8080808080808080);
	std::bitset<64> r1(0x00000000000000FF);
	std::bitset<64> r8(0xFF00000000000000);
	std::bitset<64> edgeMask(~(aFile | hFile | r1 | r8));
	std::bitset<64> corners(0x8100000000000081);
	int file = -1;
	int rank = 0;
	
	for (int i = 0; i < 64; i++) {
		rank = i & 7;
		if (i % 8 == 0)
			file++;

		if (file == 0) {
			rookBlockerMasks.push_back(rookAttacks[i] & ~(corners | r8));
		} else if (file == 7) {
			rookBlockerMasks.push_back(rookAttacks[i] & ~(corners | r1));
		} else if (rank == 0) {
			rookBlockerMasks.push_back(rookAttacks[i] & ~(corners | hFile));
		} else if (rank == 7) {
			rookBlockerMasks.push_back(rookAttacks[i] & ~(corners | aFile));
		} else {
			rookBlockerMasks.push_back(rookAttacks[i] & edgeMask);
		}
		bishopBlockerMasks.push_back(bishopAttacks[i] & edgeMask);
	}

	for (int sq = 0; sq < 64; sq++) {
		int bits = rookBlockerMasks[sq].count();
		for (int i = 0; i < (1 << bits); i++) {
			rookBlockerBoards[sq].push_back(generateBlockerBoard(i, rookBlockerMasks[sq]));
		}
	}

	for (int sq = 0; sq < 64; sq++) {
		int bits = bishopBlockerMasks[sq].count();
		for (int i = 0; i < (1 << bits); i++) {
			bishopBlockerBoards[sq].push_back(generateBlockerBoard(i, bishopBlockerMasks[sq]));
		}
	}
}

std::bitset<64> MagicGenerator::generateBishopMoveBoard(int sq, std::bitset<64> blockers) {
	std::bitset<64> moveBoard = empty;
	moveBoard |= rayAttacks[NoEa][sq];
	uint64_t NoEaMasked = (rayAttacks[NoEa][sq] & blockers).to_ullong();
	int NoEaBlocked = bitScanForward(NoEaMasked);
	moveBoard &= ~rayAttacks[NoEa][NoEaBlocked];

	moveBoard |= rayAttacks[NoWe][sq];
	uint64_t NoWeMasked = (rayAttacks[NoWe][sq] & blockers).to_ullong();
	int NoWeBlocked = bitScanForward(NoWeMasked);
	moveBoard &= ~rayAttacks[NoWe][NoWeBlocked];

	moveBoard |= rayAttacks[SoEa][sq];
	uint64_t SoEaMasked = (rayAttacks[SoEa][sq] & blockers).to_ullong();
	int SoEaBlocked = bitScanReverse(SoEaMasked);
	moveBoard &= ~rayAttacks[SoEa][SoEaBlocked];
	

	moveBoard |= rayAttacks[SoWe][sq];
	uint64_t SoWeMasked = (rayAttacks[SoWe][sq] & blockers).to_ullong();
	int SoWeBlocked = bitScanReverse(SoWeMasked);
	moveBoard &= ~rayAttacks[SoWe][SoWeBlocked];

	return moveBoard;
}

std::bitset<64> MagicGenerator::generateRookMoveBoard(int sq, std::bitset<64> blockers) {
	std::bitset<64> moveBoard = empty;

	moveBoard |= rayAttacks[North][sq];
	uint64_t NoMasked = (rayAttacks[North][sq] & blockers).to_ullong();
	int NoBlocked = bitScanForward(NoMasked);
	moveBoard &= ~rayAttacks[North][NoBlocked];

	moveBoard |= rayAttacks[East][sq];
	uint64_t EaMasked = (rayAttacks[East][sq] & blockers).to_ullong();
	int EaBlocked = bitScanForward(EaMasked);
	moveBoard &= ~rayAttacks[East][EaBlocked];

	moveBoard |= rayAttacks[South][sq];
	uint64_t SoMasked = (rayAttacks[South][sq] & blockers).to_ullong();
	int SoBlocked = bitScanReverse(SoMasked);
	moveBoard &= ~rayAttacks[South][SoBlocked];

	moveBoard |= rayAttacks[West][sq];
	uint64_t WeMasked = (rayAttacks[West][sq] & blockers).to_ullong();
	int WeBlocked = bitScanReverse(WeMasked);
	moveBoard &= ~rayAttacks[West][WeBlocked];

	return moveBoard;
}

void MagicGenerator::generateAttackSets() {
	//Loop through all possible bishop squares
	unsigned long long key;
	int test = 0;
	for (int i = 0; i < 64; i++) {
		//For every square, loop through all possible blockers for that square
		for (std::bitset<64> blockers : bishopBlockerBoards[i]) {
			key = ((blockers.to_ullong()) * bishopMagics[i]) >> (64 - bishopBlockerMasks[i].count());
			bishopAttackSet[i][key] = (generateBishopMoveBoard(i, blockers));
		}
	}

	for (int i = 0; i < 64; i++) {
		for (std::bitset<64> blockers : rookBlockerBoards[i]) {
			key = ((blockers.to_ullong()) * rookMagics[i]) >> (64 - rookBlockerMasks[i].count());
			rookAttackSet[i][key] = (generateRookMoveBoard(i, blockers));
		}
	}
}

std::bitset<64> MagicGenerator::getBishopAttacks(int sq, std::bitset<64> blockers) {
	blockers &= bishopBlockerMasks[sq];
	unsigned long long key = (blockers.to_ullong() * bishopMagics[sq]) >> (64 - bishopBlockerMasks[sq].count());
	return bishopAttackSet[sq][key];
}

std::bitset<64> MagicGenerator::getRookAttacks(int sq, std::bitset<64> blockers) {
	blockers &= rookBlockerMasks[sq];
	unsigned long long key = (blockers.to_ullong() * rookMagics[sq]) >> (64 - rookBlockerMasks[sq].count());
	return rookAttackSet[sq][key];
}

std::bitset<64> MagicGenerator::getQueenAttacks(int sq, std::bitset<64> blockers) {
	return getRookAttacks(sq, blockers) | getBishopAttacks(sq, blockers);
}

std::bitset<64> MagicGenerator::getRayToSquare(int attackSq, int targetSq, char attackType) {
	std::bitset<64> targetBitboard;
	std::bitset<64> testRay;
	std::bitset<64> ray;
	int iStart = 0;
	int iEnd = 8;

	targetBitboard[targetSq] = 1;
	//Do rays and check overlap, if yes then correct ray
	switch (attackType) {
		case 'B':
			ray = getBishopAttacks(attackSq, targetBitboard);
			break;
		case 'R':
			ray = getRookAttacks(attackSq, targetBitboard);
			break;
		case 'Q':
			ray = getQueenAttacks(attackSq, targetBitboard);
			break;
	}

	switch (attackType) {
		case 'R':
			iStart = 0;
			iEnd = 4;
			break;
		case 'B':
			iStart = 4;
			iEnd = 8;
	}

	for (int i = iStart; i < iEnd; i++) {
		testRay = rayAttacks[i][attackSq];
		if ( (testRay & targetBitboard) != 0) {
			return ((testRay & ray) & ~targetBitboard);
		}
	}

	return ray;

}

std::bitset<64> MagicGenerator::getRayOverlaps(int attackSq, int kingSq, char attackType) {
	std::bitset<64> testRay;
	std::bitset<64> testKingRay;
	std::bitset<64> rayOverlap;
	std::bitset<64> pieceBitboard;
	std::bitset<64> kingBitboard;
	int iStart = 0;
	int iEnd = 8;
	pieceBitboard[attackSq] = 1;
	kingBitboard[kingSq] = 1;

	switch (attackType) {
		case 'R':
			iStart = 0;
			iEnd = 4;
			break;
		case 'B':
			iStart = 4;
			iEnd = 8;
	}

	for (int i = iStart; i < iEnd; i++) {
		testKingRay = rayAttacks[oppositeRay[i]][kingSq];
		testRay = rayAttacks[i][attackSq] | pieceBitboard;
		rayOverlap = testRay & testKingRay;
		if (rayOverlap != 0)
			return rayOverlap;
	}
	return empty;
}

std::bitset<64> MagicGenerator::xrayRookAttacks(std::bitset<64> occupied, std::bitset<64> blockers, int sq) {
	std::bitset<64> attacks = getRookAttacks(sq, occupied);
	blockers &= attacks;
	return attacks ^ getRookAttacks(sq, (occupied ^ blockers));
}

std::bitset<64> MagicGenerator::xrayBishopAttacks(std::bitset<64> occupied, std::bitset<64> blockers, int sq) {
	std::bitset<64> attacks = getBishopAttacks(sq, occupied);
	blockers &= attacks;
	return attacks ^ getBishopAttacks(sq, (occupied ^ blockers));
}