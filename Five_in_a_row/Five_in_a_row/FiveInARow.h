#pragma once
class FiveInARow
{
private:
    static FiveInARow* instance;
    static const int BOARD_SIZE = 15;
    static const int EMPTY = 0;
    static const int BLACK = 1;
    static const int WHITE = 2;

    int board[BOARD_SIZE][BOARD_SIZE];
    int currentPlayer;
    float cellSize;
    int windowWidth, windowHeight;

    void drawGrid() const;
    void drawCircle(float x, float y, float radius, bool isBlack) const;
    bool checkWin(int x, int y, int player) const;
    //void drawText(float x, float y, const char* text) const;


public:
    FiveInARow(int windowWidth, int windowHeight);
    static void setInstance(FiveInARow* game); // 인스턴스 설정
    void render() const;
    static void onMouseClick(int mouseX, int mouseY);
};

