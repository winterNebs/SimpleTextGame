// textgame.cpp : Defines the entry point for the console application.
//have some pride.
#include "stdafx.h"
#include "vector"
#include "iostream"
#include <initializer_list>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
enum direction {
	up, left, down, right
};
class Entity {
public:
	char display;
	int x;
	int y;
	void move(int dir) {
		switch (dir) {
		case up: y -= 1;
			break;
		case left: x -= 1;
			break;
		case down: y += 1;
			break;
		case right: x += 1;
			break;

		}
	}
};
class Tile : public Entity {
public:
	bool walkable;
	Tile() {
		Tile(rand() * 223 + 32, 0, 0);
	}
	Tile(char c, int xpos, int ypos) {
		display = c;
		x = xpos;
		y = ypos;
	}
};
class WorldNode {
public:
	static WorldNode *head;
	WorldNode *dirs[4];
	Tile t;
	int x;
	int y;
	WorldNode(int xpos, int ypos) {
		for (auto i : dirs) {
			i = nullptr;
		}
		t = Tile(rand() * 223 + 32, xpos, ypos);
	}
	WorldNode() {
		WorldNode(NULL, NULL);
	}
	void addNode(int dir, WorldNode& node) {
		*dirs[dir] = node;
	}
	WorldNode(WorldNode *dir[4]) {
		for (int i = 0; i < 4; i++) {
			dirs[i] = dir[i];
		}
	}
};
class Chunk {
	//Array to graph
public:
	static const int length = 16;
	int x, y;
	WorldNode *tempField[length][length];
	Chunk *adj[4];
	Chunk() {

	}
	Chunk(int xpos, int ypos) {
		x = xpos;
		y = ypos;
		//Generate(xpos, ypos);
	}
	/*void Generate(int xpos, int ypos) {
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				WorldNode temp(i+xpos,j+ypos);
				tempField[i][j] = &temp;
				if (i == 0) {
					tempField[i][j]->dirs[3] = nullptr;
				}
				else {
					tempField[i][j]->dirs[3] = tempField[i - 1][j];
					if (i == length - 1) {
						tempField[i][j]->dirs[1] = nullptr;
					}
					tempField[i - 1][j]->dirs[1] = tempField[i][j];
				}
				if (j == 0) {
					tempField[i][j]->dirs[0] = nullptr;
				}
				else{
					tempField[i][j]->dirs[0] = tempField[i][j - 1];
					if (j == length - 1) {
						tempField[i][j]->dirs[2] = nullptr;
					}
					tempField[i][j - 1]->dirs[2] = tempField[i][j];
				}
			}
		}
	}*/
};
void input() {
	if (_kbhit()) {
		if (_getch() == 224) {
			switch (_getch()) {
			case 72:	//up
				break;
			case 75:	//left
				break;
			case 80:	//down
				break;
			case 77:	//right
				break;
			}
		}
	}
}
int main(){
	srand(time(NULL));
	while (1) {
		Sleep(1);
	}
    return 0;
}

