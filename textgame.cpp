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

class Tile{
public:
	char display;
	int x;
	int y;
	bool walkable;
	Tile() {
		
	}
	Tile(char c, int xpos, int ypos) {
		display = c;
		x = xpos;
		y = ypos;
		walkable = true;
	}
	Tile(int xpos, int ypos) {
		Tile(rand() * 223 + 32, xpos, ypos);
	}
};
class Entity : public Tile{
public:
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
class Player : public Entity{
public:
	Player() {
		x = 0;
		y = 0;
		display = 135;
		walkable = false;
	}
};
class Chunk { 
public:
	static const int viewdist = 8;//number of squares away from player not including player  (17x17 for 8)
	static int primenum;
	static std::stack<Chunk*>* hashmap;
	static const int length = 16;
	static int bounds[4];
	int x, y;
	static Player p;
	std::vector<std::vector<Tile *>> tempField;
	Chunk() {
			Chunk(0, 0);
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
		hash();
		generate(this);
	}
	void hash() {
		
		int index = (abs(x) + abs(y)) % primenum;
		hashmap[index].push(this);
	}
	static void generate(Chunk *c) {
		for (int i = 0; i < c->tempField.size(); i++) {
			for (int j = 0; j < c->tempField.at(i).size(); j++) {
				Tile temp(i + length * c->x, j + length * c->y);
				c->tempField.at(i).at(j) = &temp;
			}
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
		for (int i = 0; i < primenum; i++) {
			newHashmap[i] = hashmap[i];
		}
		delete[]hashmap;
		hashmap = newHashmap;
	}
	static void draw() {
		//First get player position
		//Calculate radius for terrain genertation (Chunks only)
		int chunkx = nodetochunk(p.x);
		int chunky = nodetochunk(p.y);
		std::vector<std::vector<Chunk>> needtoload;
		
		//probablyt can make better

		//Calculate view distance and required chunks
		//Make a sectioned array
			//Find top left most chunk, take part
			//etc etc
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
int Chunk::primenum = 137;
std::stack<Chunk*>* Chunk::hashmap = new std::stack<Chunk*>[primenum];
Player Chunk::p = Player();
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

