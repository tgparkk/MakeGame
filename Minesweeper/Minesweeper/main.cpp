#include <GLFW/glfw3.h>
#include <iostream>
#include "Minesweeper.h"
#include "Renderer.h"

Minesweeper game(10, 10, 20);  // 10x10 보드, 20개의 지뢰
float cellSize = 30.0f;

GLFWwindow* window;  // window 포인터 선언

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderBoard(game, cellSize);
    glfwSwapBuffers(window);  // window 포인터 사용
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

    while (!glfwWindowShouldClose(window)) {
        display();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
