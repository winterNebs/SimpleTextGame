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
	Tile t;
	int x;
	int y;
	WorldNode(int xpos, int ypos) {
		t = Tile(rand() * 223 + 32, xpos, ypos);
	}
	WorldNode() {
		WorldNode(NULL, NULL);
	}
};
class Chunk {
public:
	static const int length = 16;
	static Chunk *head;
	int x, y;
	//WorldNode *tempField[length][length];
	std::vector<std::vector<WorldNode *>> tempField;
	Chunk *adj[] = { nullptr,nullptr,nullptr,nullptr};
	Chunk() {
		if (head == nullptr) {
			head = this;
			Chunk(0, 0);
		} 
	}
	Chunk(int xpos, int ypos) {
		x = xpos;
		y = ypos;
		generate(this);
	}
	static void generate(Chunk *c) {
		for (int i = 0; i < c->tempField.size(); i++) {
			for (int j = 0; j < c->tempField.at(i).size(); j++) {
				WorldNode temp(i + length * c->x, j + length * c->y);
				c->tempField.at(i).at(j) = &temp;
			}
		}
	}
	static std::vector<std::vector<WorldNode *>> clip(int x, int y) {
		//Return a little square portion of the array
	}
	static bool square(Chunk *c) {
		// maybe
		return false;
	}
	static void verifiy(int xpos, int pos, int r) {

	}
	static Chunk* travel(Chunk *c, int xpos, int ypos, int xdist, int ydist) {
		if (c != nullptr) {
			//Recursion goes here
		}
		else { //This is probably wrong 
			int realx, realy;
			if (xpos < 0) {
				realx = (xpos - length) / length;
			}
			else {
				realx = xpos / length;
			}
			if (ypos < 0) {
				realy = (ypos - length) / length;
			}
			else {
				realy = ypos / length;	
			}
			Chunk temp(realx, realy);
			generate(&temp);
			return &temp;
		}
	}
	static void repair() {
		//fix the node linkage;
	}

	//traversing array and infinite generation
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

