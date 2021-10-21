#ifndef BOARDSQUARE_H
#define BOARDSQUARE_H
#include <SFML/Graphics.hpp>

class BoardSquare
{
public:
	BoardSquare();
	BoardSquare(int, float, float);
	void update();
	void render(sf::RenderTarget&);
	void initSprite(int, float, float);
	void setColor(sf::Color);
	void resetColor();

	int x;
	int y;
	int color;

private:
	sf::RectangleShape sprite;
};

#endif