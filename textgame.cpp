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
#include <stack>
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
	/*TODO:
	-hashing (x+y)% prime
	-check size
	-*/
public:
	static std::stack<Chunk*>* hashmap;
	static const int length = 16;
	static Chunk *head;
	static int bounds[4];
	int x, y;
	std::vector<std::vector<WorldNode *>> tempField;
	Chunk() {
		if (head == nullptr) {
			head = this;
			Chunk(0, 0);
			for (auto i: bounds) {
				i = 0;
			}
		} 
	}
	Chunk(int xpos, int ypos) {
		x = xpos;
		y = ypos;
		if (x > bounds[right]) {
			bounds[right] = x;
		}
		else if(x < bounds[left]) {
			bounds[left] = x;
		}
		if (y > bounds[down]) {
			bounds[down] = y;
		}
		else if (y < bounds[up]) {
			bounds[up] = y;
		}
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
		//make sure they dont have same number
	}
	static Chunk* travel(Chunk *c, int xpos, int ypos, int xdist, int ydist) {
		if (c != nullptr) {
			//Recursion goes here
		}
		else { //This is probably wrong 
			Chunk temp(nodetochunk(xpos), nodetochunk(ypos));
			generate(&temp);
			return &temp;
		}
	}
	static int nodetochunk(int i) {
		if (i < 0) {
			return (i - length) / length;
		}
		else {
			return i / length;
		}
	}
	static void repair() {
		//fix the node linkage;
		//make sure they dont have same number
	}
	static int prime(int prev) {	//Prime num calc
		for (int i = prev; i< INT_MAX; i++)
			for (int j = 2; j*j <= i; j++)
			{
				if (i % j == 0)
					break;
				else if (j + 1 > sqrt(i)) {
					return i;

				}
			}
	}
	static void resize(int size) {//resize hashmap
		std::stack<Chunk*> *newHashmap = new std::stack<Chunk*>[size];
		//Copy
		delete[]hashmap;
		hashmap = newHashmap;
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
std::stack<Chunk*>* Chunk::hashmap = new std::stack<Chunk*>[137];
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

