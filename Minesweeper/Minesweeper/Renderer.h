#ifndef RENDERER_H
#define RENDERER_H

#include "Minesweeper.h"

// OpenGL �ʱ�ȭ �Լ�
void initOpenGL(int windowWidth, int windowHeight);

// ���� ������ �Լ�
void renderBoard(const Minesweeper& game, float cellSize);

#endif
