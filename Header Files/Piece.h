#ifndef PIECE_H
#define PIECE_H
#include <vector>
#include <SFML/Graphics.hpp>

class Piece
{
public: 
	Piece(int, int, int, char);

	//Move Handling
	int getIndex();
	int getId();
	int getColor();
	int getCapturedIndex();
	void setIndex(int);
	void setCapturedIndex(int);
	bool checkSameTeam(int, int);
	bool isSlidingPiece();

	//Graphics
	void setPiecePosition();
	void setPiecePickedPosition(int, int);
	void updateGraphic();
	void render(sf::RenderTarget&);
	void initSprite();
	void initTexture();

protected:
	int color = 0;
	int rank = 0;
	int index = 0;
	int capturedIndex = 0;
	int file = 0;
	int startDir = 0;
	int endDir = 8;
	char id = 0;

	sf::Sprite sprite;
	sf::Texture texture;
	std::string textureDir = "Pieces/wqueen.png";
};

#endif