#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <string>
#include "pch.h"

// 정적 변수 초기화
std::map<char, Character> Render::Characters;

void Render::initOpenGL(int windowWidth, int windowHeight) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);  // 2D 좌표계 설정
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // 배경색 설정
}

void Render::renderCell(int row, int col, const Cell& cell, float cellSize) {
    float x = col * cellSize;
    float y = row * cellSize;

    if (cell.isRevealed) {
        glColor3f(cell.isMine ? 1.0f : 0.8f, 0.8f, 0.8f); // 지뢰면 빨간색, 아니면 회색
    }
    else {
        glColor3f(cell.isFlagged ? (cell.isMine ? 1.0f : 0.8f) : 0.5f,  // 잘못된 플래그일 경우 빨간색
                  cell.isFlagged ? (cell.isMine ? 0.0f : 0.0f) : 0.5f,
                  0.5f);
    }

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + cellSize, y);
    glVertex2f(x + cellSize, y + cellSize);
    glVertex2f(x, y + cellSize);
    glEnd();

    // 내용물 그리기
    if (cell.isRevealed) {
        if (cell.isMine) {
            drawFlag(x, y, cellSize); // 지뢰는 깃발로 표시
        }
        else {
            drawNumber(cell.surroundingMines, x, y, cellSize * 0.6f); // 숫자 그리기
        }
    }
    else if (cell.isFlagged) {
        drawFlag(x, y, cellSize); // 깃발
    }
}

void Render::renderBoard(const Minesweeper& game, float cellSize) {
    const auto& board = game.getBoard();
    for (int r = 0; r < board.size(); ++r) {
        for (int c = 0; c < board[r].size(); ++c) {
            renderCell(r, c, board[r][c], cellSize);

            // 2. 각 칸의 경계선을 그리기
            float x = c * cellSize;
            float y = r * cellSize;

            glColor3f(0.0f, 0.0f, 0.0f); // 경계선의 색 (검정색)
            glBegin(GL_LINE_LOOP);      // 사각형 경계선을 그리는 모드
            glVertex2f(x, y);                   // 왼쪽 아래
            glVertex2f(x + cellSize, y);        // 오른쪽 아래
            glVertex2f(x + cellSize, y + cellSize); // 오른쪽 위
            glVertex2f(x, y + cellSize);        // 왼쪽 위
            glEnd();
        }
    }
}

void Render::renderScoreBoard(const Minesweeper& game, float cellSize, uint16_t row_num, uint16_t col_num, float timer) {
    float scoreBoardY = score_board_height / 2.0f;

    // 스코어보드 배경
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(col_num * cellSize, 0.0f);
    glVertex2f(col_num * cellSize, score_board_height);
    glVertex2f(0.0f, score_board_height);
    glEnd();

    float timerX = (col_num * cellSize) - 50.0f; // 오른쪽 끝으로 정렬
    float timerY = scoreBoardY - 10.0f;
    float digitSize = 20.0f;

    // 숫자 테스트
    float startX = 20.0f; // 시작 x 좌표
    float startY = scoreBoardY - 10.0f; // 시작 y 좌표
    renderTestDigits(startX, startY, digitSize);

    //renderTimer(static_cast<int>(timer), timerX, timerY, digitSize);
}

void Render::drawNumber(int number, float x, float y, float size) {
    glColor3f(0.0f, 0.0f, 0.0f); // 숫자 색상: 검정색
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    // 숫자 도형 정의
    switch (number) {
    case 0: // 숫자 0
        glVertex2f(x, y);                     // 좌측 하단
        glVertex2f(x, y + size);              // 좌측 상단
        glVertex2f(x, y + size);              // 상단 가로선
        glVertex2f(x + size, y + size);
        glVertex2f(x + size, y + size);       // 우측 상단
        glVertex2f(x + size, y);
        glVertex2f(x + size, y);              // 우측 하단
        glVertex2f(x, y);
        break;
    case 1: // 숫자 1
        glVertex2f(x + size * 0.5f, y);       // 중앙 세로선
        glVertex2f(x + size * 0.5f, y + size);
        break;
    case 2: // 숫자 2 (뒤집힘)
        glVertex2f(x, y);                     // 하단 가로선
        glVertex2f(x + size, y);
        glVertex2f(x + size, y);              // 오른쪽 하단
        glVertex2f(x + size, y + size * 0.5f);
        glVertex2f(x + size, y + size * 0.5f);// 중간 가로선
        glVertex2f(x, y + size * 0.5f);
        glVertex2f(x, y + size * 0.5f);       // 왼쪽 상단
        glVertex2f(x, y + size);
        glVertex2f(x, y + size);              // 상단 가로선
        glVertex2f(x + size, y + size);
        break;
    case 3: // 숫자 3
        glVertex2f(x, y + size);              // 상단 가로선
        glVertex2f(x + size, y + size);
        glVertex2f(x + size, y + size);       // 오른쪽 세로선 (위)
        glVertex2f(x + size, y);
        glVertex2f(x, y + size * 0.5f);       // 중간 가로선
        glVertex2f(x + size, y + size * 0.5f);
        glVertex2f(x, y);                     // 하단 가로선
        glVertex2f(x + size, y);
        break;
    case 4:
        // 왼쪽 세로선
        glVertex2f(x, y + size);                // 왼쪽 위
        glVertex2f(x, y);                       // 왼쪽 아래

        // 가로선 (중앙)
        glVertex2f(x, y + size * 0.5f);         // 왼쪽 중간
        glVertex2f(x + size, y + size * 0.5f);   // 오른쪽 중간

        // 오른쪽 세로선
        glVertex2f(x + size, y + size * 0.5f);   // 오른쪽 중간
        glVertex2f(x + size, y);                 // 오른쪽 아래
        break;


    case 5: // 숫자 5
        glVertex2f(x + size, y + size);       // 상단 가로선
        glVertex2f(x, y + size);
        glVertex2f(x, y + size);              // 왼쪽 세로선 (위)
        glVertex2f(x, y + size * 0.5f);
        glVertex2f(x, y + size * 0.5f);       // 중간 가로선
        glVertex2f(x + size, y + size * 0.5f);
        glVertex2f(x + size, y + size * 0.5f);// 오른쪽 세로선 (아래)
        glVertex2f(x + size, y);
        glVertex2f(x + size, y);              // 하단 가로선
        glVertex2f(x, y);
        break;
    case 6: // 숫자 6
        glVertex2f(x + size, y + size);       // 상단 가로선
        glVertex2f(x, y + size);
        glVertex2f(x, y + size);              // 왼쪽 세로선 (전체)
        glVertex2f(x, y);
        glVertex2f(x, y);                     // 하단 가로선
        glVertex2f(x + size, y);
        glVertex2f(x + size, y);              // 오른쪽 세로선 (아래)
        glVertex2f(x + size, y + size * 0.5f);
        glVertex2f(x + size, y + size * 0.5f);// 중간 가로선
        glVertex2f(x, y + size * 0.5f);
        break;
    case 7: // 숫자 7
        glVertex2f(x, y + size);              // 상단 가로선
        glVertex2f(x + size, y + size);
        glVertex2f(x + size, y + size);       // 오른쪽 세로선
        glVertex2f(x + size, y);
        break;
    case 8: // 숫자 8
        glVertex2f(x, y);                     // 좌측 하단
        glVertex2f(x, y + size);
        glVertex2f(x, y + size);              // 좌측 상단
        glVertex2f(x + size, y + size);
        glVertex2f(x + size, y + size);       // 우측 상단
        glVertex2f(x + size, y);
        glVertex2f(x + size, y);              // 우측 하단
        glVertex2f(x, y);
        glVertex2f(x, y + size * 0.5f);       // 중간 가로선
        glVertex2f(x + size, y + size * 0.5f);
        break;
    }
    glEnd();
}


void Render::drawFlag(float x, float y, float size) {
    // 깃발 천 (빨간색)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + size * 0.3f, y);             // 깃발 상단 (위쪽 점)
    glVertex2f(x + size * 0.3f, y + size * 0.5f); // 중간 지점 (아래쪽 왼쪽 점)
    glVertex2f(x + size * 0.7f, y + size * 0.5f); // 중간 지점 (아래쪽 오른쪽 점)
    glEnd();

    // 깃발대 (검정색)
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(x + size * 0.3f, y);              // 깃발대 왼쪽 (위쪽)
    glVertex2f(x + size * 0.3f, y + size);       // 깃발대 오른쪽 (아래쪽)
    glEnd();
}

void Render::RenderText(const std::string& text, float x, float y, float scale) {
    glEnable(GL_BLEND); // 알파 블렌딩 활성화
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D); // 텍스처 활성화
    for (auto c : text) {

        if (Characters.find(c) == Characters.end()) {
            std::cerr << "Character not loaded: " << c << std::endl;
            continue; // 문자가 로드되지 않았다면 건너뛰기
        }

        Character ch = Characters[c];

        float xpos = x + ch.BearingX * scale;
        float ypos = y - (ch.SizeY - ch.BearingY) * scale;

        float w = ch.SizeX * scale;
        float h = ch.SizeY * scale;

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos, ypos);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + w, ypos);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + w, ypos + h);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos, ypos + h);
        glEnd();

        x += (ch.Advance >> 6) * scale;
    }

    glDisable(GL_BLEND); // 블렌딩 비활성화
    glDisable(GL_TEXTURE_2D); // 텍스처 비활성화
}

// FreeType 초기화 및 글자 텍스처 생성
void Render::LoadFont(const char* fontPath) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not initialize FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48); // 폰트 크기 설정

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 텍스처 정렬

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph: " << c << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);//GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            static_cast<GLuint>(face->glyph->advance.x >> 6)
        };

        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Render::drawDigit(int digit, float x, float y, float size) {
    glColor3f(1.0f, 0.0f, 0.0f); // 빨간색

    /*
     -- 0 --
    |       |
    5       1
    |       |
     -- 6 --
    |       |
    4       2
    |       |
     -- 3 --
    */

    // 숫자에 따른 세그먼트 활성화 (정확한 패턴으로 수정)
    static bool segments[10][7] = {
        {1, 1, 1, 1, 1, 1, 0}, // 0
        {0, 1, 1, 0, 0, 0, 0}, // 1
        {1, 1, 0, 1, 1, 0, 1}, // 2
        {1, 1, 1, 1, 0, 0, 1}, // 3
        {0, 1, 1, 0, 0, 1, 1}, // 4
        {1, 0, 1, 1, 0, 1, 1}, // 5
        {1, 0, 1, 1, 1, 1, 1}, // 6
        {1, 1, 1, 0, 0, 0, 0}, // 7
        {1, 1, 1, 1, 1, 1, 1}, // 8
        {1, 1, 1, 1, 0, 1, 1}  // 9
    };


    // 각 세그먼트의 위치와 크기
    static struct Segment {
        float x1, y1, x2, y2;
    };

    static Segment seg[7] = {
        {x, y + size, x + size, y + size * 0.9f},   // Top
        {x + size * 0.9f, y + size, x + size, y},  // Top-right
        {x + size * 0.9f, y, x + size, y - size},  // Bottom-right
        {x, y - size, x + size, y - size * 1.1f},  // Bottom
        {x, y, x + size * 0.1f, y - size},         // Bottom-left
        {x, y + size, x + size * 0.1f, y},         // Top-left
        {x, y, x + size, y - size * 0.1f}          // Middle
    };

    // 배경을 옅은 빨간색으로 채우기
    glColor3f(0.3f, 0.0f, 0.0f); // 옅은 빨간색
    glBegin(GL_QUADS);
    glVertex2f(x, y + size);
    glVertex2f(x + size, y + size);
    glVertex2f(x + size, y - size);
    glVertex2f(x, y - size);
    glEnd();

    // 활성화된 세그먼트 진한 빨간색으로 채우기
    glColor3f(1.0f, 0.0f, 0.0f); // 진한 빨간색
    for (int i = 0; i < 7; ++i) {
        if (segments[digit][i]) {
            glBegin(GL_QUADS);
            glVertex2f(seg[i].x1, seg[i].y1);
            glVertex2f(seg[i].x2, seg[i].y1);
            glVertex2f(seg[i].x2, seg[i].y2);
            glVertex2f(seg[i].x1, seg[i].y2);
            glEnd();
        }
    }
}

void Render::renderTimer(int time, float x, float y, float size) {
    // 최대 3자리 숫자로 표현 (예: 999, 045 등)
    time = time % 1000; // 1000 이상은 999로 제한

    int hundreds = time / 100;
    int tens = (time / 10) % 10;
    int units = time % 10;

    // 숫자 간격 조정
    float digitSpacing = size * 1.5f; // 숫자 간격을 넓힘

    // 오른쪽부터 숫자를 표시
    drawDigit(units, x, y, size);                   // 일의 자리
    drawDigit(tens, x - digitSpacing, y, size);     // 십의 자리
    drawDigit(hundreds, x - digitSpacing * 2, y, size); // 백의 자리
}
