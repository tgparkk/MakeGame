#include <GLFW/glfw3.h>
#include <iostream>
#include "Minesweeper.h"
#include "Renderer.h"
#include <map>

Minesweeper game(10, 10, 20);  // 10x10 보드, 20개의 지뢰
float cellSize = 30.0f;
float timer = 0.0f; // 게임 타이머 변수

GLFWwindow* window;  // window 포인터 선언

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderBoard(game, cellSize);
    
    // 게임 상태 텍스트 표시
    //renderGameState();

    glfwSwapBuffers(window);
}

// 문자 텍스처 맵 (폰트 텍스처를 관리하기 위한 맵)
std::map<char, GLuint> charTextures;

GLuint loadTexture(const char* filepath); // 텍스처 로딩 함수 (폰트 이미지 텍스처 로딩)

void renderText(const std::string& text, float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);  // 텍스트의 위치 설정

    for (char c : text) {
        // 텍스트에 해당하는 문자 텍스처를 찾아서 그린다
        GLuint texture = charTextures[c];

        // 텍스처를 화면에 출력하는 코드
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);  // 왼쪽 아래
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 0.0f);  // 오른쪽 아래
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);  // 오른쪽 위
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 1.0f);  // 왼쪽 위
        glEnd();

        glTranslatef(1.0f, 0.0f, 0.0f);  // 각 문자 간의 간격 조정
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
        // F5 키가 눌렸을 때 게임 재시작
        game = Minesweeper(10, 10, 20);  // 새로운 게임 생성
    }
}

void updateTimer(float deltaTime) {
    // 타이머를 업데이트
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
    glfwSetKeyCallback(window, keyCallback);  // 키 콜백 설정

    while (!glfwWindowShouldClose(window)) {
        display();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

GLuint loadTexture(const char* filepath) {
    // 이미지 파일을 OpenGL 텍스처로 로드하는 코드 (예: STB 이미지 라이브러리 사용)
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