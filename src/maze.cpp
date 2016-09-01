#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>

#include "maze.h"

Maze::Maze(int width, int height)
{
	Maze::width = width;
	Maze::height = height;
	labyrinth.resize(height);
	for (int i = 0; i < height; i++) {
		labyrinth[i].resize(width);
		for (int j = 0; j < width; j++) {
			labyrinth[i][j].x = i;
			labyrinth[i][j].y = j;
			labyrinth[i][j].Visited = false;
		}
	}
}

void Maze::Draw2D(int argc, char** argv)
{
	GLUTBackendInit(argc, argv, false, false);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 21")) {
		return;
	}

	GUI* pApp = new GUI();

	if (!pApp->Init()) {
		return;
	}

	pApp->Run();

	delete pApp;

	return;
}