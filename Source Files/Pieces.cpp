#include "Pieces.h"

King::King(int rank, int file, int color, char id) : Piece(rank, file, color, id) {
	if (this->color == 0) {
		this->textureDir = "Pieces/wking.png";
	} else {
		this->textureDir = "Pieces/bking.png";
	}
}

Queen::Queen(int rank, int file, int color, char id) : Piece(rank, file, color, id) {
	if (this->color == 0) {
		this->textureDir = "Pieces/wqueen.png";
	} else {
		this->textureDir = "Pieces/bqueen.png";
	}
}

Rook::Rook(int rank, int file, int color, char id) : Piece(rank, file, color, id) {
	if (this->color == 0) {
		this->textureDir = "Pieces/wrook.png";
	} else {
		this->textureDir = "Pieces/brook.png";
	}
}

Bishop::Bishop(int rank, int file, int color, char id) : Piece(rank, file, color, id) {
	if (this->color == 0) {
		this->textureDir = "Pieces/wbishop.png";
	} else {
		this->textureDir = "Pieces/bbishop.png";
	}
}

Knight::Knight(int rank, int file, int color, char id) : Piece(rank, file, color, id) {
	if (this->color == 0) {
		this->textureDir = "Pieces/wknight.png";
	} else {
		this->textureDir = "Pieces/bknight.png";
	}
}

Pawn::Pawn(int rank, int file, int color, char id) : Piece(rank, file, color, id) {
	if (this->color == 0) {
		this->textureDir = "Pieces/wpawn.png";
	} else {
		this->textureDir = "Pieces/bpawn.png";
	}
}