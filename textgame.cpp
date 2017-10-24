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
#include "PerlinNoise.hpp"
#include <iomanip>
//Garbage collection when.

int seed = 420;
enum direction {up, left, down, right};
enum wallstype {solid = 219, bottomw = 220, leftw = 221, rightw = 222, topw = 223};
enum groundtype { blank = 32, light = 176, med = 177, dark = 178 };
enum fencetype {svert = 179, shor = 196, scross = 197};
enum alttype{hash = 35, water = 126, doublewater = 247, tree = 140};
template <class Type>
inline constexpr const Type& Clamp(const Type& x, const Type& min, const Type& max){
	return (x < min) ? min : ((max < x) ? max : x);
}
//Todo Noise:
//Distance from 00 can increase intensity of noise
//I guess each structure/tile can have differrent noise value;
//Structures
//Create array of tilesets or something. ikd about biomes or something
class Point {// END MY SUFFERING
public:
	int x, y;
	Point(int xd = NULL, int yd = NULL) : x{ xd }, y{ yd } {	}
};
class Tile{//ASCII starts at 128-254
public:
	char display;
	int x;
	int y;
	bool walkable;
	Tile(double noise, int xpos = 5, int ypos = 5) : x{ xpos }, y{ ypos } {
		interpret(noise);
	}
	Tile(char c = '#', int xpos = 5, int ypos = 5) : walkable{ true }, display{ c }, x{ xpos }, y{ ypos } {	}
	void interpret(double noise);
};
struct TileSet {
	double max;
	int tile;
	bool walkable;
	int range;
	TileSet(double ma = 1, int t = 35, bool w = true, int r = 0) :range{ r }, max { ma }, tile{ t }, walkable{ w } {	}
}; 
const std::vector<TileSet*> plains = { new TileSet(0.30,blank,true,1000), new TileSet(0.60,light),new TileSet(0.70,med),new TileSet(0.80,dark,false),new TileSet(1,water,false) };
const std::vector<TileSet*> swamp = { new TileSet(.3,doublewater,false,400), new TileSet(.5,water),new TileSet(.6,light),new TileSet(.8,tree,false), new TileSet(.8,tree,false) };
const std::vector<std::vector<TileSet*>> tilesets = {plains, swamp };///
void Tile::interpret(double noise) {///what hte fuck
	int rad = sqrt((x*x) + (y*y));
	for (int i = 0; i < tilesets.size(); i++) {
		if ( rad < tilesets[i][0]->range) { // eAA
			for (int j = tilesets[i].size(); j < 0; j--) {
				if (noise < tilesets[i][j]->max) {
					display = (int)tilesets[i][j]->tile;
					walkable = tilesets[i][j]->walkable;
					return;
				}
			}
			return;
		}
	}/**/
		/*
		if (noise < .1) { // clean ground
			display = blank;
		}
		else if (noise < .2) {
			display = light;
		}
		else if (noise < .3) {
			display = med;
		}
		else if (noise < .4) {
			display = dark;
		}
		else if (noise < .5) {
			display = solid;
		}
		else if (noise < .6) {
			display = 126;
		}
		else if (noise < .7) {
			display = 35;
		}*/
		///display = (int)(noise * 10)+48;

}
class Entity : public Tile{
public:
	Entity() {	}
	void move(direction dir);
};
class Player : public Entity{
public:
	Player(int xpos = 0, int ypos = 0){
		x = xpos;
		y = ypos;
		display = 135;
		walkable = false;
	}
};
class Chunk { 
	static const int viewdisty = 13;//number of squares away from player not including player  (17x17 for 8)
	static const int viewdistx = 50;
	static int primenum;
	static std::stack<Chunk*>* hashmap;
	static const int length = 16;
	static std::vector<Entity*> ents;
public:
	int tbounds[4];
	int x, y;
	static Player p;
	Tile* tempField[length][length];
	Chunk() : Chunk(0, 0){
		p = Player(0,0);
		ents.push_back(&p);
	} 
	Chunk(int xpos, int ypos) {
		x = xpos;
		y = ypos;
		tbounds[up] = chunktonode(y, 0);
		tbounds[left] = chunktonode(x, 0);
		tbounds[down] = chunktonode(y, length - 1);
		tbounds[right] = chunktonode(x, length - 1);
		hash();
		generate(this);
	}
	void hash() {
		if (abs(x + y) > primenum) {
			prime();
			resize();
		}
		int index = (abs(x + y)) % primenum;
		hashmap[index].push(this);
	}
	static void generate(Chunk *c) {//Freq = number, octave = variation
		//double frequency = sqrt((c->x * c->x) + (c->y * c->y))/2;
		double frequency = 1;
		frequency = Clamp(frequency, 0.1, 4.0);
		//int octaves = sqrt((c->x * c->x) + (c->y * c->y))/2;
		int octaves = 1;
		octaves = Clamp(octaves, 1, 16);
		const siv::PerlinNoise perlin(seed);
		const double fx = length / frequency;
		const double fy = length / frequency;
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				int actualx = i + length * c->x;
				int acutaly = j + length * c->y;
				double noise = perlin.octaveNoise0_1(actualx / fx, acutaly / fy, octaves);
				c->tempField[i][j] = new Tile(noise, actualx, acutaly);
			}
		}
	}
	static int nodetochunk(int i) {
		if (i >= 0) {
			return i / length;
		}
		return (i - length + 1) / length;
	}
	static int chunktonode(int c, int r) {//(c)hunk val; (r)elative tile val
		return (r + c * length);
	}
	static int relativenode(int c, int a) {
		return (a - c*length);
	}
	static void prime() {	//Prime num calc
		for (int i = primenum ; i< INT_MAX; i++)
			for (int j = 2; j*j <= i; j++)
			{
				if (i % j == 0)
					break;
				else if (j + 1 > sqrt(i)) {
					primenum = i;
					return;
				}
			}
	}
	static void resize() {//resize hashmap
		std::stack<Chunk*> *newHashmap = new std::stack<Chunk*>[primenum];
		for (int i = 0; i < primenum; i++) {
			newHashmap[i] = hashmap[i];
		}
		delete[]hashmap;
		hashmap = newHashmap;
	}
	static Chunk* get(int chunkx, int chunky) {
		if (!hashmap[(abs(chunkx) + abs(chunky)) % primenum].empty()) {
			for (std::stack<Chunk*> dump = hashmap[(abs(chunkx + chunky)) % primenum]; !dump.empty(); dump.pop()) {
				if (dump.top()->x == chunkx && dump.top()->y == chunky) {
					return dump.top();
				}
			}
		}
		return nullptr;
	}
	static Tile* getTile(int tilex, int tiley) {
		Chunk* c = get(nodetochunk(tilex), nodetochunk(tiley));
		if (c == nullptr) {
			c = new Chunk(nodetochunk(tilex), nodetochunk(tiley));
		}
		return c->tempField[relativenode(c->x, tilex)][relativenode(c->y, tiley)];

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
		int loaddir[]{
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
		std::string output = "";
		for (int i = 0; i < ents.size(); i++) {
			viewField[ents[i]->x - dist[left]][ents[i]->y - dist[up]] = ents[i];
		}
		for (int i = 0; i < (2 * viewdisty) + 1; i++) {
			for (int j = 0; j < (2 * viewdistx) + 1; j++) {
				output += viewField[j][i]->display;
			}
			output += "\n";
		}
		system("CLS");
		std::cout << output << "coords: (" << p.x << ", " << p.y << ")" << std::endl;
	} 
};
int Chunk::primenum = 137;
std::stack<Chunk*>* Chunk::hashmap = new std::stack<Chunk*>[primenum];
Player Chunk::p = Player();
std::vector<Entity*> Chunk::ents = std::vector<Entity*>();
void input() {
	if (_kbhit()) {
		if (_getch() == 224) {
			switch (_getch()) {
			case 72: {	//up
					Chunk::p.move(up);
				break; 
			}
			case 75: {	//left
					Chunk::p.move(left);
				break; 
			}
			case 80: {	//down
					Chunk::p.move(down);
				break;
			}
			case 77: {	//right
				Chunk::p.move(right);	
				break;
			}
			}
		}
	}
}
void Entity::move(direction dir) {
	switch (dir) {
	case up: {
		if (Chunk::getTile(x, y - 1)->walkable) {
			y -= 1;
		}
		break;
	}
	case left: {
		if (Chunk::getTile(x - 1, y)->walkable) {
			x -= 1;
		}
		break;
	}
	case down: {
		if (Chunk::getTile(x, y + 1)->walkable) {
			y += 1;
		}
		break;
	}
	case right: {
		if (Chunk::getTile(x + 1, y)->walkable) {
			x += 1;
		}
		break;
	}
	}
	//Chunk::draw();
}
int main(){

	Chunk first;
	srand(time(NULL));	

	while (1) {
		input();
		Chunk::draw();
		Sleep(100);
	}
    return 0;
}

