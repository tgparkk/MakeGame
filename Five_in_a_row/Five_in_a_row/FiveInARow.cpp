#include "FiveInARow.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 정적 멤버 변수 정의
FiveInARow* FiveInARow::instance = nullptr; // 반드시 초기화 필요

// 생성자
FiveInARow::FiveInARow(int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight), currentPlayer(BLACK) {
    cellSize = static_cast<float>(windowWidth) / BOARD_SIZE;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = EMPTY;
        }
    }
}

// 정적 인스턴스 설정
void FiveInARow::setInstance(FiveInARow* game) {
    instance = game;
}

// 격자 그리기
void FiveInARow::drawGrid() const {
    glColor3f(0.0f, 0.0f, 0.0f); // 검은색
    glBegin(GL_LINES);
    for (int i = 0; i <= BOARD_SIZE; ++i) {
        glVertex2f(i * cellSize, 0);
        glVertex2f(i * cellSize, windowHeight);

        glVertex2f(0, i * cellSize);
        glVertex2f(windowWidth, i * cellSize);
    }
    glEnd();
}


// 원 그리기
void FiveInARow::drawCircle(float x, float y, float radius, bool isBlack) const {
    glColor3f(isBlack ? 0.0f : 1.0f, isBlack ? 0.0f : 1.0f, isBlack ? 0.0f : 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; ++i) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

// 렌더링
void FiveInARow::render() const {
    drawGrid();

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == BLACK) {
                drawCircle((j + 0.5f) * cellSize, (i + 0.5f) * cellSize, cellSize / 3, true);
            }
            else if (board[i][j] == WHITE) {
                drawCircle((j + 0.5f) * cellSize, (i + 0.5f) * cellSize, cellSize / 3, false);
            }
        }
    }
}

// 정적 메서드에서 인스턴스를 통해 접근
void FiveInARow::onMouseClick(int mouseX, int mouseY) {
    if (!instance) return; // 인스턴스가 설정되지 않은 경우 무시
    int col = mouseX / (instance->windowWidth / BOARD_SIZE);
    int row = mouseY / (instance->windowHeight / BOARD_SIZE);

    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && instance->board[row][col] == EMPTY) {
        instance->board[row][col] = instance->currentPlayer;
        if (instance->checkWin(row, col, instance->currentPlayer)) {
            std::cout << (instance->currentPlayer == BLACK ? "Black" : "White") << " wins!" << std::endl;
        }
        instance->currentPlayer = (instance->currentPlayer == BLACK) ? WHITE : BLACK;
    }
}

// 승리 조건 확인
bool FiveInARow::checkWin(int x, int y, int player) const {
    const int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
    for (auto& dir : directions) {
        int count = 1;
        for (int step = 1; step <= 4; ++step) {
            int nx = x + step * dir[0];
            int ny = y + step * dir[1];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == player) {
                count++;
            }
            else {
                break;
            }
        }
        for (int step = 1; step <= 4; ++step) {
            int nx = x - step * dir[0];
            int ny = y - step * dir[1];
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == player) {
                count++;
            }
            else {
                break;
            }
        }
        if (count >= 5) return true;
    }
    return false;
}