#ifndef RENDERER_H
#define RENDERER_H


#include "Minesweeper.h"
#include "define.h"
#include "pch.h"

// 글자 데이터를 저장할 구조체
struct Character {
	GLuint TextureID;   // 글자 텍스처 ID
	int SizeX, SizeY;   // 글자 크기
	int BearingX, BearingY; // 글자 위치 조정
	GLuint Advance;     // 다음 글자로 이동할 거리
};



class Render {
public:
	// OpenGL 초기화 함수
	static void initOpenGL(int windowWidth, int windowHeight);
	static void renderCell(int row, int col, const Cell& cell, float cellSize);

	// 보드 렌더링 함수
	static void renderBoard(const Minesweeper& game, float cellSize);
	static void renderScoreBoard(const Minesweeper& game, float cellSize
		, uint16_t row_num, uint16_t col_num, float timer);

	static void drawNumber(int number, float x, float y, float size);

	static void drawFlag(float x, float y, float size);

	static void RenderText(const std::string& text, float x, float y, float scale);



	static void LoadFont(const char* fontPath);

	static std::map<char, Character> Characters; // 글자 데이터 맵

};



#endif
