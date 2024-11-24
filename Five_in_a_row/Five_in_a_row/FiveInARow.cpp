#include "FiveInARow.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ���� ��� ���� ����
FiveInARow* FiveInARow::instance = nullptr; // �ݵ�� �ʱ�ȭ �ʿ�

// ������
FiveInARow::FiveInARow(int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight), currentPlayer(BLACK) {
    cellSize = static_cast<float>(windowWidth) / BOARD_SIZE;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = EMPTY;
        }
    }
}

// ���� �ν��Ͻ� ����
void FiveInARow::setInstance(FiveInARow* game) {
    instance = game;
}

// ���� �׸���
void FiveInARow::drawGrid() const {
    glColor3f(0.0f, 0.0f, 0.0f); // ������
    glBegin(GL_LINES);
    for (int i = 0; i <= BOARD_SIZE; ++i) {
        glVertex2f(i * cellSize, 0);
        glVertex2f(i * cellSize, windowHeight);

        glVertex2f(0, i * cellSize);
        glVertex2f(windowWidth, i * cellSize);
    }
    glEnd();
}


// �� �׸���
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

// ������
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

// ���� �޼��忡�� �ν��Ͻ��� ���� ����
void FiveInARow::onMouseClick(int mouseX, int mouseY) {
    if (!instance) return; // �ν��Ͻ��� �������� ���� ��� ����
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

// �¸� ���� Ȯ��
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