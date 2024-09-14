#pragma once

#ifndef TEMPLE_H
#define TEMPLE_H

#include "Player.h"
#include <vector>
#include <algorithm>
#include <queue>
#include <utility>

const int MAXROWS = 18;
const int MAXCOLS = 70;

// Define a struct to represent a room
struct Room {
	int startRow;
	int startCol;
	int width;
	int height;
};

struct coord {
	int x;
	int y;
	
	int height;
	int heuristic;

	int parentX;
	int parentY;


	coord(int xVal, int yVal, int h = 0, int heur = 0, int pX = -1, int pY = -1) {
		x = xVal;
		y = yVal;		
		height = h;
		heuristic = heur;
		parentX = pX;
		parentY = pY;
	}

	bool operator<(const coord& c) const {
		if ((height + heuristic) < (c.height + c.heuristic)) {
			return true;
		}
		else
			return false;
	}

	bool operator==(const coord& c) const {
		if (x == c.x && y == c.y) {
			return true;
		}
		else
			return false;
	}

};

struct pathCoord {
	vector<coord> totalPath;

	void addCoordinate(coord c) {
		totalPath.push_back(c);
	}

};

// each position on the map is represented by a cell, which tells if they are a monster, scroll, weapon, stair, idol, etc.
struct Cell {
	char display;
	bool isMonster;
	bool isScroll;
	bool isWeapon;
	bool isStair;
	bool isIdol;

	Weapon* w;
	Scroll* s;
	Monster* m;

	Cell(char c) {
		w = nullptr;
		s = nullptr;
		m = nullptr;
		display = c;
		isMonster = false;
		isScroll = false;
		isWeapon = false;
		isStair = false;
		isIdol = false;
	}
};


class Temple {
public:
	Temple();
	Temple(int level);
	~Temple();
	void display() const;
	char getCell(int row, int col) const;
	void setCell(int row, int col, char contents);
	vector<int> getRandomEmptyCell();

	int objectPos[3][3]; // keep track of the positions of objects on this level

	void placeWeapons(int row, int col);
	void placeScrolls(int row, int col);
	void placeBogeyman(int row, int col);
	void placeSnakewoman(int row, int col);
	void placeGoblin(int row, int col);
	void placeDragon(int row, int col);

	void placeIdol(int row, int col);

	Monster* getMonster(int row, int col);
	void removeMonster(int row, int col);

	Weapon* getWeapon(int row, int col);
	Scroll* getScroll(int row, int col);

	void removeWeapon(int row, int col);
	void removeScroll(int row, int col);

	void moveMonsters(Player* p, int goblinSmellDistance);
	void moveBogeyman(Monster* m, Player* p);
	void moveSnakewoman(Monster* m, Player* p);
	void moveGoblinRecursive(Monster* m, Player* p, int goblinSmellDistance);

	int calculateHeuristic(Monster* m, Player* p);
	int calculateHeuristic(int x, int y, Player* p);
	int calculateHeuristicRecursion(int x, int y, int pX, int pY);
	pathCoord findDistance(Monster* m, Player* p, int max);

	pathCoord findDistanceRecursive(Monster* m, Player* p, int maxDistance);
	bool findPathRecursive(int x, int y, int playerX, int playerY, int maxDistance, int currentDistance, vector<coord>& path, vector<vector<bool>>& visited);
	bool isValidMove(int x, int y);

	bool isAdjacent(int x1, int y1, int x2, int y2);

	vector<Monster*> returnMonsters() {
		return monsters;
	}

private:
	void generateRandomBoxes();
	bool isAreaClear(int startRow, int startCol, int boxWidth, int boxHeight) const;
	void placeBox(int startRow, int startCol, int boxWidth, int boxHeight);
	void placeStairs(int row, int col);
	void connectBoxes(const vector<Room>& boxes);
	void replaceHashBetweenRooms();

	int m_level;
	int m_rows = MAXROWS;
	int m_cols = MAXCOLS;

	vector<Room> rooms; // Vector to store rooms and their coordinates
	vector<vector<Cell>> temple;

	vector<Monster*> monsters; // used to keep track of all monsters in current level


};

#endif

