#include "Renderer.h"
#include <GLFW/glfw3.h>

void initOpenGL(int windowWidth, int windowHeight) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);  // 2D ��ǥ�� ����
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // ���� ����
}

void renderCell(int row, int col, const Cell& cell, float cellSize) {
    float x = col * cellSize;
    float y = row * cellSize;

    if (cell.isRevealed) {
        glColor3f(cell.isMine ? 1.0f : 0.8f, 0.8f, 0.8f); // ���ڸ� ������, �ƴϸ� ȸ��
    }
    else {
        glColor3f(cell.isFlagged ? (cell.isMine ? 1.0f : 0.8f) : 0.5f,  // �߸��� �÷����� ��� ������
                  cell.isFlagged ? (cell.isMine ? 0.0f : 0.0f) : 0.5f,
                  0.5f);
    }

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + cellSize, y);
    glVertex2f(x + cellSize, y + cellSize);
    glVertex2f(x, y + cellSize);
    glEnd();

    // ���빰 �׸���
    if (cell.isRevealed) {
        if (cell.isMine) {
            drawFlag(x, y, cellSize); // ���ڴ� ��߷� ǥ��
        }
        else {
            drawNumber(cell.surroundingMines, x, y, cellSize * 0.6f); // ���� �׸���
        }
    }
    else if (cell.isFlagged) {
        drawFlag(x, y, cellSize); // ���
    }
}

void renderBoard(const Minesweeper& game, float cellSize) {
    const auto& board = game.getBoard();
    for (int r = 0; r < board.size(); ++r) {
        for (int c = 0; c < board[r].size(); ++c) {
            renderCell(r, c, board[r][c], cellSize);
        }
    }
}

void drawNumber(int number, float x, float y, float size) {
    glColor3f(0.0f, 0.0f, 0.0f); // ���� ����: ������
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    // ���� ���� ����
    switch (number) {
    case 0: // ���� 0
        glVertex2f(x, y);                     // ���� �ϴ�
        glVertex2f(x, y + size);              // ���� ���
        glVertex2f(x, y + size);              // ��� ���μ�
        glVertex2f(x + size, y + size);
        glVertex2f(x + size, y + size);       // ���� ���
        glVertex2f(x + size, y);
        glVertex2f(x + size, y);              // ���� �ϴ�
        glVertex2f(x, y);
        break;
    case 1: // ���� 1
        glVertex2f(x + size * 0.5f, y);       // �߾� ���μ�
        glVertex2f(x + size * 0.5f, y + size);
        break;
    case 2: // ���� 2 (������)
        glVertex2f(x, y);                     // �ϴ� ���μ�
        glVertex2f(x + size, y);
        glVertex2f(x + size, y);              // ������ �ϴ�
        glVertex2f(x + size, y + size * 0.5f);
        glVertex2f(x + size, y + size * 0.5f);// �߰� ���μ�
        glVertex2f(x, y + size * 0.5f);
        glVertex2f(x, y + size * 0.5f);       // ���� ���
        glVertex2f(x, y + size);
        glVertex2f(x, y + size);              // ��� ���μ�
        glVertex2f(x + size, y + size);
        break;
    case 3: // ���� 3
        glVertex2f(x, y + size);              // ��� ���μ�
        glVertex2f(x + size, y + size);
        glVertex2f(x + size, y + size);       // ������ ���μ� (��)
        glVertex2f(x + size, y);
        glVertex2f(x, y + size * 0.5f);       // �߰� ���μ�
        glVertex2f(x + size, y + size * 0.5f);
        glVertex2f(x, y);                     // �ϴ� ���μ�
        glVertex2f(x + size, y);
        break;
    case 4:
        // ���� ���μ�
        glVertex2f(x, y + size);                // ���� ��
        glVertex2f(x, y);                       // ���� �Ʒ�

        // ���μ� (�߾�)
        glVertex2f(x, y + size * 0.5f);         // ���� �߰�
        glVertex2f(x + size, y + size * 0.5f);   // ������ �߰�

        // ������ ���μ�
        glVertex2f(x + size, y + size * 0.5f);   // ������ �߰�
        glVertex2f(x + size, y);                 // ������ �Ʒ�
        break;


    case 5: // ���� 5
        glVertex2f(x + size, y + size);       // ��� ���μ�
        glVertex2f(x, y + size);
        glVertex2f(x, y + size);              // ���� ���μ� (��)
        glVertex2f(x, y + size * 0.5f);
        glVertex2f(x, y + size * 0.5f);       // �߰� ���μ�
        glVertex2f(x + size, y + size * 0.5f);
        glVertex2f(x + size, y + size * 0.5f);// ������ ���μ� (�Ʒ�)
        glVertex2f(x + size, y);
        glVertex2f(x + size, y);              // �ϴ� ���μ�
        glVertex2f(x, y);
        break;
    case 6: // ���� 6
        glVertex2f(x + size, y + size);       // ��� ���μ�
        glVertex2f(x, y + size);
        glVertex2f(x, y + size);              // ���� ���μ� (��ü)
        glVertex2f(x, y);
        glVertex2f(x, y);                     // �ϴ� ���μ�
        glVertex2f(x + size, y);
        glVertex2f(x + size, y);              // ������ ���μ� (�Ʒ�)
        glVertex2f(x + size, y + size * 0.5f);
        glVertex2f(x + size, y + size * 0.5f);// �߰� ���μ�
        glVertex2f(x, y + size * 0.5f);
        break;
    case 7: // ���� 7
        glVertex2f(x, y + size);              // ��� ���μ�
        glVertex2f(x + size, y + size);
        glVertex2f(x + size, y + size);       // ������ ���μ�
        glVertex2f(x + size, y);
        break;
    case 8: // ���� 8
        glVertex2f(x, y);                     // ���� �ϴ�
        glVertex2f(x, y + size);
        glVertex2f(x, y + size);              // ���� ���
        glVertex2f(x + size, y + size);
        glVertex2f(x + size, y + size);       // ���� ���
        glVertex2f(x + size, y);
        glVertex2f(x + size, y);              // ���� �ϴ�
        glVertex2f(x, y);
        glVertex2f(x, y + size * 0.5f);       // �߰� ���μ�
        glVertex2f(x + size, y + size * 0.5f);
        break;
    }
    glEnd();
}


void drawFlag(float x, float y, float size) {
    // ��� õ (������)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + size * 0.3f, y);             // ��� ��� (���� ��)
    glVertex2f(x + size * 0.3f, y + size * 0.5f); // �߰� ���� (�Ʒ��� ���� ��)
    glVertex2f(x + size * 0.7f, y + size * 0.5f); // �߰� ���� (�Ʒ��� ������ ��)
    glEnd();

    // ��ߴ� (������)
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(x + size * 0.3f, y);              // ��ߴ� ���� (����)
    glVertex2f(x + size * 0.3f, y + size);       // ��ߴ� ������ (�Ʒ���)
    glEnd();
}

