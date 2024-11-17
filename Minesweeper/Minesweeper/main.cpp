#include <GLFW/glfw3.h>
#include <iostream>
#include "Minesweeper.h"
#include "Renderer.h"
#include <map>

Minesweeper game(10, 10, 20);  // 10x10 ����, 20���� ����
float cellSize = 30.0f;
float timer = 0.0f; // ���� Ÿ�̸� ����

GLFWwindow* window;  // window ������ ����

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderBoard(game, cellSize);
    
    // ���� ���� �ؽ�Ʈ ǥ��
    //renderGameState();

    glfwSwapBuffers(window);
}

// ���� �ؽ�ó �� (��Ʈ �ؽ�ó�� �����ϱ� ���� ��)
std::map<char, GLuint> charTextures;

GLuint loadTexture(const char* filepath); // �ؽ�ó �ε� �Լ� (��Ʈ �̹��� �ؽ�ó �ε�)

void renderText(const std::string& text, float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);  // �ؽ�Ʈ�� ��ġ ����

    for (char c : text) {
        // �ؽ�Ʈ�� �ش��ϴ� ���� �ؽ�ó�� ã�Ƽ� �׸���
        GLuint texture = charTextures[c];

        // �ؽ�ó�� ȭ�鿡 ����ϴ� �ڵ�
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);  // ���� �Ʒ�
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 0.0f);  // ������ �Ʒ�
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);  // ������ ��
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 1.0f);  // ���� ��
        glEnd();

        glTranslatef(1.0f, 0.0f, 0.0f);  // �� ���� ���� ���� ����
    }

    glPopMatrix();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int col = static_cast<int>(xpos / cellSize);
        int row = static_cast<int>(ypos / cellSize);

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            game.revealCell(row, col);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            game.toggleFlag(row, col);
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_F5) {
        // F5 Ű�� ������ �� ���� �����
        game = Minesweeper(10, 10, 20);  // ���ο� ���� ����
    }
}

void updateTimer(float deltaTime) {
    // Ÿ�̸Ӹ� ������Ʈ
    timer += deltaTime;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW Initialization failed!" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(800, 600, "Minesweeper", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    initOpenGL(800, 600);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);  // Ű �ݹ� ����

    while (!glfwWindowShouldClose(window)) {
        display();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

GLuint loadTexture(const char* filepath) {
    // �̹��� ������ OpenGL �ؽ�ó�� �ε��ϴ� �ڵ� (��: STB �̹��� ���̺귯�� ���)
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, channels;
    //unsigned char* data = stbi_load(filepath, &width, &height, &channels, 0);
    //if (data) {
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //stbi_image_free(data);

    return textureID;
}