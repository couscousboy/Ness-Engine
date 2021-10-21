#include <iostream>
#include "Piece.h"

#pragma warning(disable: 4244) //Disables float to int conversion warning for rendering

Piece::Piece(int rank, int file, int color, char id) {
	// 0 = White
	this->color = color;
	this->rank = rank;
	this->index = (rank * 8 + file);
	this->file = file;
	this->id = id;
}

int Piece::getIndex() {
	return index;
}

int Piece::getId() {
	return id;
}

int Piece::getColor() {
	return color;
}

int Piece::getCapturedIndex() {
	return capturedIndex;
}

void Piece::setIndex(int index) {
	this->index = index;
}

void Piece::setCapturedIndex(int index) {
	this->capturedIndex = index;
}

bool Piece::checkSameTeam(int pieceOne, int pieceTwo) {
	return (pieceOne >= 0) ^ (pieceTwo < 0);
}

bool Piece::isSlidingPiece() {
	return (id == 'R' || id == 'B' || id == 'Q') ? true : false;
}

//Graphic Methods

void Piece::setPiecePosition() {
	this->sprite.setPosition(sf::Vector2f((this->file * 80) + 40, (560 - this->rank * 80) + 40));
}

void Piece::setPiecePickedPosition(int x, int y) {
	this->sprite.setPosition(sf::Vector2f(x, y));
}

void Piece::updateGraphic() {
	this->file = this->index % 8;
	this->rank = this->index / 8;
	this->setPiecePosition();
}

void Piece::render(sf::RenderTarget& target) {
	target.draw(this->sprite);
}

void Piece::initTexture() {
	this->texture.loadFromFile(this->textureDir);
}

void Piece::initSprite() {
	this->sprite.setTexture(this->texture);
	this->sprite.setOrigin(sf::Vector2f(30.0f, 30.0f));
	this->setPiecePosition();
}
