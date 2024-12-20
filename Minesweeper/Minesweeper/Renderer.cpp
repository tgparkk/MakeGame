#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <string>
#include "pch.h"

// 정적 변수 초기화
std::map<char, GLuint> Render::charTextures;

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
    float scoreBoardY = row_num * cellSize;

    // 스코어보드 배경 색 설정 및 영역 렌더링
    glColor3f(0.8f, 0.8f, 0.8f);  // 밝은 회색
    glBegin(GL_QUADS);
    glVertex2f(0.0f, scoreBoardY);                      // 왼쪽 아래
    glVertex2f(col_num * cellSize, scoreBoardY);        // 오른쪽 아래
    glVertex2f(col_num * cellSize, scoreBoardY + score_board_height);  // 오른쪽 위
    glVertex2f(0.0f, scoreBoardY + score_board_height); // 왼쪽 위
    glEnd();

    // 스코어보드 텍스트 표시
    glColor3f(0.0f, 0.0f, 0.0f);  // 검은색 텍스트

    // 남은 지뢰 개수
    std::string minesLeftText
        = "Mines Left: ";
        //= "Mines Left: " + std::to_string(game.getMineCount() - game.getFlagCount());
    renderText(minesLeftText, 10.0f, scoreBoardY + 10.0f);

    // 타이머
    std::string timerText 
        = "Time: " + std::to_string(static_cast<int>(timer));
    renderText(timerText, 150.0f, scoreBoardY + 10.0f);

    // 게임 상태
    std::string statusText
        = "Status: ";
        //= "Status: " + game.getGameState();
    renderText(statusText, 300.0f, scoreBoardY + 10.0f);
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

void Render::renderText(const std::string& text, float x, float y) {
    glEnable(GL_TEXTURE_2D);

    for (char c : text) {
        if (charTextures.find(c) == charTextures.end()) continue; // 텍스처 없는 문자는 무시

        GLuint texture = charTextures[c];
        glBindTexture(GL_TEXTURE_2D, texture);

        const uint16_t scale = 1;

        float charSize = 16.0f * scale; // 문자 크기 조정
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + charSize, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + charSize, y + charSize);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + charSize);
        glEnd();

        x += charSize; // 문자 간격
    }

    glDisable(GL_TEXTURE_2D);
}

void Render::initCharTextures(const std::string& fontTexturePath, int charWidth, int charHeight) {
    GLuint textureID = Render::loadTexture(fontTexturePath.c_str());

    if (textureID == 0) {
        //throw std::runtime_error("Failed to load font texture");
    }

    // 텍스처 ID를 저장하고 각 문자의 텍스처 좌표 매핑
    charTextures.clear();
    for (char c = 32; c < 127; ++c) { // ASCII 32~126 문자
        charTextures[c] = textureID;
    }
}

GLuint Render::loadTexture(const char* filepath) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, channels;
    unsigned char* data = stbi_load(filepath, &width, &height, &channels, 0);
    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        glDeleteTextures(1, &textureID);
        textureID = 0;  // 로드 실패 시 0 반환
    }

    stbi_image_free(data);
    return textureID;
}