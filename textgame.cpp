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
class Point {// END MY SUFFERING
public:
	int x, y;
	Point() {
		Point(NULL, NULL);
	}
	Point(int xd, int yd) {
		x = xd;
		y = yd;
	}
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
	int tbounds[4];
	int x, y;
	static Player p;
	std::vector<std::vector<Tile *>> tempField;
	Chunk() {
			Chunk(0, 0);
	} 
	Chunk(int xpos, int ypos) {
		x = xpos;
		y = ypos;
		
		tbounds[up] = chunktonode(y, 0);
		tbounds[left] = chunktonode(x, 0);
		tbounds[down] = chunktonode(y, length - 1);
		tbounds[right] = chunktonode(x, length - 1);
		/*
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
		*/
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
		return (i - length + 1) / length;
	}
	static int chunktonode(int c, int r) {//(c)hunk val; (r)elative tile val
		return (r + c * length);
	}
	static int relativenode(int c, int a) {
		return (a - c*length);
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
	static Chunk* get(int chunkx, int chunky) {
		for (int i = 0; i < primenum; i++) { //why are you like this
			if (!hashmap[i].empty()) {
				for (std::stack<Chunk*> dump = hashmap[i]; !dump.empty(); dump.pop()) {
					if (dump.top()->x == chunkx && dump.top()->y == chunky){
						return dump.top();
					}
				}
			}
		}
		return nullptr;
	}
	static void draw() {
		//First get player position
		//Calculate radius for terrain genertation (Chunks only)
		Tile *viewField[2*viewdist+1][2*viewdist+1];
		int dist[]{ p.y - viewdist,
			p.x - viewdist,
			p.y + viewdist,
			p.x + viewdist
		};
		Point cornerCoords[]{ Point(dist[left], dist[up]),		//Top left
			Point(dist[left],dist[down]),						//Bottom left
			Point(dist[right],dist[down]),						//Bottom right
			Point(dist[right],dist[up])							//Top right
		};
		int loaddir[]{
			ceil(abs(relativenode(nodetochunk(p.y), p.y) - viewdist) / length), //up 
			ceil(abs(relativenode(nodetochunk(p.x), p.x) - viewdist) / length),	//left
			ceil((relativenode(nodetochunk(p.y), p.y) + viewdist) / length),	//down
			ceil((relativenode(nodetochunk(p.x), p.x) + viewdist) / length)		//right
		};//Coordinates are (left + up, left + down, right + down, right + up)
		//Calculate view distance and required chunks
		for (int i = loaddir[left]; i <= loaddir[right]; i++) {
			for (int j = loaddir[up]; j <= loaddir[down]; i++) {
				Chunk* c = get(nodetochunk(p.x) + i, nodetochunk(p.y) + j);
				if (c == nullptr) {
					generate(c);
				}
				//Grab parts of the array and add to big boy array
				for (int a = 0; a < c->tempField.size(); a++) {
					for (int b = 0; b < c->tempField.at(a).size(); b++) {
						if (c->tempField.at(a).at(b)->x > dist[left] && 
							c->tempField.at(a).at(b)->x < dist[right] &&
							c->tempField.at(a).at(b)->y > dist[up] &&
							c->tempField.at(a).at(b)->y < dist[down]) {
							viewField[c->tempField.at(a).at(b)->x - dist[left]]
								[c->tempField.at(a).at(b)->y - dist[up]] = c->tempField.at(a).at(b);
						}
					}
				}
			}
		}
	} 
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
	Chunk first();
	srand(time(NULL));	
	while (1) {
		Chunk::draw();
		Sleep(1);
	}
    return 0;
}

