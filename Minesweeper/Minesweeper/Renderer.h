#ifndef RENDERER_H
#define RENDERER_H


#include "Minesweeper.h"
#include "define.h"
#include "pch.h"

// ���� �����͸� ������ ����ü
struct Character {
	GLuint TextureID;   // ���� �ؽ�ó ID
	int SizeX, SizeY;   // ���� ũ��
	int BearingX, BearingY; // ���� ��ġ ����
	GLuint Advance;     // ���� ���ڷ� �̵��� �Ÿ�
};



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

	static void RenderText(const std::string& text, float x, float y, float scale);



	static void LoadFont(const char* fontPath);

	static void drawDigit(int digit, float x, float y, float size);
	static void renderTimer(int time, float x, float y, float size);

	static std::map<char, Character> Characters; // ���� ������ ��

	static void renderTestDigits(float x, float y, float size) {
		float digitSpacing = size * 1.5f; // ���� ����

		for (int i = 0; i < 10; ++i) {
			drawDigit(i, x + (digitSpacing * i), y, size);
		}
	}
};



#endif
