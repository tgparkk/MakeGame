#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <string>
#include "pch.h"

// ���� ���� �ʱ�ȭ
std::map<char, Character> Render::Characters;

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
    float scoreBoardY = 0;

    // ���ھ�� ��� �� ���� �� ���� ������
    glColor3f(0.8f, 0.8f, 0.8f);  // ���� ȸ��
    glBegin(GL_QUADS);
    glVertex2f(0.0f, scoreBoardY);                      // ���� �Ʒ�
    glVertex2f(col_num * cellSize, scoreBoardY);        // ������ �Ʒ�
    glVertex2f(col_num * cellSize, scoreBoardY );  // ������ ��
    glVertex2f(0.0f, scoreBoardY ); // ���� ��
    glEnd();

    // �ؽ�Ʈ �������� ���� ���� ���� (���)
    glColor3f(1.0f, 1.0f, 1.0f);  // ��� �ؽ�Ʈ

    // �ؽ�Ʈ ��� ���� �ؽ�ó�� Ȱ��ȭ�ؾ� �մϴ�.
    glEnable(GL_TEXTURE_2D);

    // ���� ���� ����
    std::string minesLeftText
        = "Mines Left: ";
        //= "Mines Left: " + std::to_string(game.getMineCount() - game.getFlagCount());
    RenderText(minesLeftText, 10.0f, scoreBoardY + 10.0f, 1);

    // Ÿ�̸�
    std::string timerText 
        = "Time: " + std::to_string(static_cast<int>(timer));
    RenderText(timerText, 150.0f, scoreBoardY + 10.0f, 1);

    // ���� ����
    std::string statusText
        = "Status: ";
        //= "Status: " + game.getGameState();
    RenderText(statusText, 300.0f, scoreBoardY + 10.0f, 1);

    // �ؽ�ó ��Ȱ��ȭ
    glDisable(GL_TEXTURE_2D);
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

void Render::RenderText(const std::string& text, float x, float y, float scale) {
    for (auto c : text) {
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
}

// FreeType �ʱ�ȭ �� ���� �ؽ�ó ����
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

    FT_Set_Pixel_Sizes(face, 0, 48); // ��Ʈ ũ�� ����

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // �ؽ�ó ����

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