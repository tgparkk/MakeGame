#include "pch.h"
#include "FiveInARow.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Omok Game", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // OpenGL 초기화
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(1.0f, 1.0f, 0.8f, 1.0f); // 황색 배경 (R=1.0, G=1.0, B=0.8)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FiveInARow fiveInARow(WINDOW_WIDTH, WINDOW_HEIGHT);
    FiveInARow::setInstance(&fiveInARow);

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
        double xpos, ypos;
        glfwGetCursorPos(win, &xpos, &ypos);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            FiveInARow::onMouseClick((int)xpos, (int)ypos);
        }
        });

    while (!glfwWindowShouldClose(window)) {
        //std::cout << "Rendering frame..." << std::endl;

        glClear(GL_COLOR_BUFFER_BIT);
        fiveInARow.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
