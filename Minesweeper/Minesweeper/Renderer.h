#ifndef RENDERER_H
#define RENDERER_H

#include "Minesweeper.h"

// OpenGL 초기화 함수
void initOpenGL(int windowWidth, int windowHeight);

// 보드 렌더링 함수
void renderBoard(const Minesweeper& game, float cellSize);

void drawNumber(int number, float x, float y, float size);

void drawFlag(float x, float y, float size);

#endif
