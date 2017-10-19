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
#include <string>
//Garbage collection when.
enum direction {
	up, left, down, right
};
class Point {// END MY SUFFERING
public:
	int x, y;
	Point() : Point(NULL,NULL){
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
	Tile() : Tile(0,0){	}
	Tile(int xpos, int ypos) : Tile(rand() * 223 + 32, xpos, ypos) {	}
	Tile(char c, int xpos, int ypos) : walkable{ true }, display{ c }, x{ xpos }, y{ ypos } {	}
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
	Player() : Player(0,0){	}
	Player(int xpos, int ypos){
		x = xpos;
		y = ypos;
		display = 135;
		walkable = false;
	}
};
class Chunk { 
public:
	static const int viewdisty = 8;//number of squares away from player not including player  (17x17 for 8)
	static const int viewdistx = 16;
	static int primenum;
	static std::stack<Chunk*>* hashmap;
	static const int length = 16;
	int tbounds[4];
	int x, y;
	static Player p;
	//std::vector<std::vector<Tile *>> tempField;
	Tile* tempField[length][length];
	Chunk() : Chunk(0, 0){
		p = Player(0,0);
	} 
	Chunk(int xpos, int ypos) {
		//tempField = std::vector<std::vector<Tile *>>();
		x = xpos;
		y = ypos;
		tbounds[up] = chunktonode(y, 0);
		tbounds[left] = chunktonode(x, 0);
		tbounds[down] = chunktonode(y, length - 1);
		tbounds[right] = chunktonode(x, length - 1);
		hash();
		generate(this);
		//std::cout << "rows: " << sizeof(tempField) / sizeof(tempField[0]) << std::endl;
		//std::cout << "col: " << sizeof(tempField[0]) / sizeof(Tile*) << std::endl;

	}
	void hash() {
		int index = (abs(x) + abs(y)) % primenum;
		hashmap[index].push(this);
		//std::cout << "ADDED AT " << x << y;
	}
	static void generate(Chunk *c) {
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				c->tempField[i][j] = new Tile(i + length * c->x, j + length * c->y);
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
		if (!hashmap[(abs(chunkx) + abs(chunky)) % primenum].empty()) {
			for (std::stack<Chunk*> dump = hashmap[(abs(chunkx) + abs(chunky)) % primenum]; !dump.empty(); dump.pop()) {
				if (dump.top()->x == chunkx && dump.top()->y == chunky) {
					return dump.top();
				}
			}
		}
		return nullptr;
	}
	static void draw() {
		//First get player position
		//Calculate radius for terrain genertation (Chunks only)
		Tile *viewField[2*viewdistx+1][2*viewdisty+1];
		int dist[]{ p.y - viewdisty,
			p.x - viewdistx,
			p.y + viewdisty,
			p.x + viewdistx
		};
		Point cornerCoords[]{ Point(dist[left], dist[up]),		//Top left
			Point(dist[left],dist[down]),						//Bottom left
			Point(dist[right],dist[down]),						//Bottom right
			Point(dist[right],dist[up])							//Top right
		};
		int loaddir[]{//PROBLEM
			-ceil((float)(abs(relativenode(nodetochunk(p.y), p.y) - viewdisty)) / length),	//up 
			-ceil((float)(abs(relativenode(nodetochunk(p.x), p.x) - viewdistx)) / length),	//left
			ceil((float)(relativenode(nodetochunk(p.y), p.y) + viewdisty) / length),			//down
			ceil((float)(relativenode(nodetochunk(p.x), p.x) + viewdistx) / length)			//right
		};//Coordinates are (left + up, left + down, right + down, right + up)
	
		  //Calculate view distance and required chunks
		for (int i = loaddir[left]; i <= loaddir[right]; i++) {
			for (int j = loaddir[up]; j <= loaddir[down]; j++) {
				Chunk* c = get(nodetochunk(p.x) + i, nodetochunk(p.y) + j);
				if (c == nullptr) {
					c = new Chunk((nodetochunk(p.x) + i), (nodetochunk(p.y) + j));
				}
				//Grab parts of the array and add to big boy array
				for (int a = 0; a < length; a++) {
					for (int b = 0; b <length; b++) {
						if (c->tempField[a][b]->x >= dist[left] && 
							c->tempField[a][b]->x <= dist[right] &&
							c->tempField[a][b]->y >= dist[up] &&
							c->tempField[a][b]->y <= dist[down]) {
							viewField[c->tempField[a][b]->x - dist[left]]
								[c->tempField[a][b]->y - dist[up]] = c->tempField[a][b];
						}
					}
				}
			}
		}
		system("CLS");
		std::string output = "";
		for (int i = 0; i < (2 * viewdisty) + 1; i++) {
			for (int j = 0; j < (2 * viewdistx) + 1; j++) {
				output += viewField[j][i]->display;
			}
			output += "\n";
		}
		std::cout << output;
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
	Chunk first(0,0);
	srand(time(NULL));	
	while (1) {
		Chunk::draw();
		Sleep(1000);
	}
    return 0;
}

