#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <string>
#include "pch.h"

// ���� ���� �ʱ�ȭ
std::map<char, GLuint> Render::charTextures;

void Render::initOpenGL(int windowWidth, int windowHeight) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);  // 2D ��ǥ�� ����
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // ���� ����
}

void Render::renderCell(int row, int col, const Cell& cell, float cellSize) {
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

void Render::renderBoard(const Minesweeper& game, float cellSize) {
    const auto& board = game.getBoard();
    for (int r = 0; r < board.size(); ++r) {
        for (int c = 0; c < board[r].size(); ++c) {
            renderCell(r, c, board[r][c], cellSize);

            // 2. �� ĭ�� ��輱�� �׸���
            float x = c * cellSize;
            float y = r * cellSize;

            glColor3f(0.0f, 0.0f, 0.0f); // ��輱�� �� (������)
            glBegin(GL_LINE_LOOP);      // �簢�� ��輱�� �׸��� ���
            glVertex2f(x, y);                   // ���� �Ʒ�
            glVertex2f(x + cellSize, y);        // ������ �Ʒ�
            glVertex2f(x + cellSize, y + cellSize); // ������ ��
            glVertex2f(x, y + cellSize);        // ���� ��
            glEnd();
        }
    }
}

void Render::renderScoreBoard(const Minesweeper& game, float cellSize, uint16_t row_num, uint16_t col_num, float timer) {
    float scoreBoardY = row_num * cellSize;

    // ���ھ�� ��� �� ���� �� ���� ������
    glColor3f(0.8f, 0.8f, 0.8f);  // ���� ȸ��
    glBegin(GL_QUADS);
    glVertex2f(0.0f, scoreBoardY);                      // ���� �Ʒ�
    glVertex2f(col_num * cellSize, scoreBoardY);        // ������ �Ʒ�
    glVertex2f(col_num * cellSize, scoreBoardY + score_board_height);  // ������ ��
    glVertex2f(0.0f, scoreBoardY + score_board_height); // ���� ��
    glEnd();

    // ���ھ�� �ؽ�Ʈ ǥ��
    glColor3f(0.0f, 0.0f, 0.0f);  // ������ �ؽ�Ʈ

    // ���� ���� ����
    std::string minesLeftText
        = "Mines Left: ";
        //= "Mines Left: " + std::to_string(game.getMineCount() - game.getFlagCount());
    renderText(minesLeftText, 10.0f, scoreBoardY + 10.0f);

    // Ÿ�̸�
    std::string timerText 
        = "Time: " + std::to_string(static_cast<int>(timer));
    renderText(timerText, 150.0f, scoreBoardY + 10.0f);

    // ���� ����
    std::string statusText
        = "Status: ";
        //= "Status: " + game.getGameState();
    renderText(statusText, 300.0f, scoreBoardY + 10.0f);
}

void Render::drawNumber(int number, float x, float y, float size) {
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


void Render::drawFlag(float x, float y, float size) {
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

void Render::renderText(const std::string& text, float x, float y) {
    glEnable(GL_TEXTURE_2D);

    for (char c : text) {
        if (charTextures.find(c) == charTextures.end()) continue; // �ؽ�ó ���� ���ڴ� ����

        GLuint texture = charTextures[c];
        glBindTexture(GL_TEXTURE_2D, texture);

        const uint16_t scale = 1;

        float charSize = 16.0f * scale; // ���� ũ�� ����
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + charSize, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + charSize, y + charSize);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + charSize);
        glEnd();

        x += charSize; // ���� ����
    }

    glDisable(GL_TEXTURE_2D);
}

void Render::initCharTextures(const std::string& fontTexturePath, int charWidth, int charHeight) {
    GLuint textureID = Render::loadTexture(fontTexturePath.c_str());

    if (textureID == 0) {
        //throw std::runtime_error("Failed to load font texture");
    }

    // �ؽ�ó ID�� �����ϰ� �� ������ �ؽ�ó ��ǥ ����
    charTextures.clear();
    for (char c = 32; c < 127; ++c) { // ASCII 32~126 ����
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
        textureID = 0;  // �ε� ���� �� 0 ��ȯ
    }

    stbi_image_free(data);
    return textureID;
}