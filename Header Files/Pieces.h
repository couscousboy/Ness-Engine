#include <iostream>
#ifndef PIECES_H
#define PIECES_H
#include "Piece.h"
#include <SFML/Graphics.hpp>

class King : public Piece
{
public:
	King(int, int, int, char);
protected:
};

class Queen : public Piece
{
public:
	Queen(int, int, int, char);
protected:
	//const int startDir = 0;
	//const int endDir = 8;
};

class Rook : public Piece
{
public:
	Rook(int, int, int, char);
protected:
	//const int startDir = 0;
	//const int endDir = 4;
};

class Bishop : public Piece
{
public:
	Bishop(int, int, int, char);
protected:
	//const int startDir = 4;
	//const int endDir = 8;
};

class Knight : public Piece
{
public:
	Knight(int, int, int, char);
protected:
};

class Pawn : public Piece
{
public:
	Pawn(int, int, int, char);
protected:
};

#endif