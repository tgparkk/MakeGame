#ifndef RENDERER_H
#define RENDERER_H

#include <GLFW/glfw3.h>
#include "Minesweeper.h"
#include "define.h"
#include <string>
#include <map>


class Render {
public:
	// OpenGL �ʱ�ȭ �Լ�
	static void initOpenGL(int windowWidth, int windowHeight);
	static void renderCell(int row, int col, const Cell& cell, float cellSize);

	// ���� ������ �Լ�
	static void renderBoard(const Minesweeper& game, float cellSize);
	static void renderScoreBoard(const Minesweeper& game, float cellSize
		, uint16_t row_num, uint16_t col_num, float timer);

	static void drawNumber(int number, float x, float y, float size);

	static void drawFlag(float x, float y, float size);

	static void renderText(const std::string& text, float x, float y);
	static GLuint loadTexture(const char* filepath); // �ؽ�ó �ε� �Լ� (��Ʈ �̹��� �ؽ�ó �ε�)
	

	// ���� �ؽ�ó �� (��Ʈ �ؽ�ó�� �����ϱ� ���� ��)
	static void initCharTextures(const std::string& fontTexturePath
		, int charWidth, int charHeight);
	static std::map<char, GLuint> charTextures;

};



#endif
