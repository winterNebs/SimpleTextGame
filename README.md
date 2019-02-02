# SimpleTextGame
A simple c++ text based game

Built Release (May need to instal c++ libraries):
https://github.com/winterNebs/SimpleTextGame/raw/Noise/textgame.exe

Simple Text Game is a simple, infinite, open world, top-down, text-based game.
This was a 1 month project, completed in October, to help familiarize myself with C++.
---
The first 14 days were spent on getting the basics of the tile engine to work properly, including actually getting output at all, storing data properly, etc.
The next 2 days were spent on setting up hashing for the data storage (16x16 tiles are stored in a Chunk, all the chunks are stored in the hash).
2 more days were spent on getting the draw function to properly read from the hash map (stitching the chunks together).
Days 19-24 were spent on making tiles generate randomly using a perlin noise function.
Days 25-30 were spent on implementing enemies.

I learned a lot during the development of this game. In addition to just learning the syntax of C++, I learned a lot more about pointers, which don't really exist in Java. I also learned more about vectors, which exist in Java, however I have not used them much before. I would have liked to continue developing this game, however due to the flickering caused by the console, I decided it was not worth it to do another month.

