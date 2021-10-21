#include "BoardSquare.h"
#include <iostream>

BoardSquare::BoardSquare() {
}

BoardSquare::BoardSquare(int color, float x, float y) {
	this->x = x;
	this->y = y;
	this->color = color;
	this->initSprite(color, x, y);
}

void BoardSquare::initSprite(int color, float x, float y) {
	this->sprite.setSize(sf::Vector2f(80, 80));
	this->resetColor();
	this->sprite.setPosition(sf::Vector2f(x, y));
}

void BoardSquare::render(sf::RenderTarget& target) {
	target.draw(this->sprite);
}

void BoardSquare::setColor(sf::Color color) {
	this->sprite.setFillColor(color);
}

void BoardSquare::resetColor() {
	if (color == 0) {
		this->sprite.setFillColor(sf::Color(238, 238, 210));
	} else if (color == 1) {
		this->sprite.setFillColor(sf::Color(118, 150, 86));
	}
}

void BoardSquare::update() {

}