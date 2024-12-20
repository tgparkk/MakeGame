#include <GLFW/glfw3.h>
#include <iostream>
#include "Minesweeper.h"
#include "Renderer.h"
#include <map>
#include "define.h"


uint16_t row_num = 10;
uint16_t col_num = 10;
uint16_t mine_num = 10;

Minesweeper game(row_num, col_num, mine_num);
float cellSize = 30.0f;
float timer = 0.0f; // ���� Ÿ�̸� ����

GLFWwindow* window;  // window ������ ����

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Render::renderScoreBoard(game, cellSize, row_num, col_num, timer);
    
    glPushMatrix();
    glTranslatef(0.0f, score_board_height, 0.0f);
    Render::renderBoard(game, cellSize);
    glPopMatrix();

    glfwSwapBuffers(window);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // ���ھ� ���� ������ Ŭ���ߴ��� Ȯ��
        if (ypos < score_board_height) {
            return; // ���ھ� ���� ������ ����
        }

        // ���ھ� ���� ���̸� ������ ���� ���� ��ǥ ���
        ypos -= score_board_height;

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
        game = Minesweeper(row_num, col_num, mine_num);  // ���ο� ���� ����
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

    // TODO
    // Implement a function to receive input(row,col) 

    uint32_t width = cellSize * col_num;
    uint32_t heigth = cellSize * row_num;

    window = glfwCreateWindow(width, heigth, "Minesweeper", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    Render::initOpenGL(width, heigth);

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
