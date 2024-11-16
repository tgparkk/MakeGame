#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <vector>

struct Cell {
    bool isMine = false;       // 지뢰 여부
    bool isRevealed = false;   // 열림 여부
    bool isFlagged = false;    // 깃발 표시 여부
    int surroundingMines = 0;  // 주변 지뢰 개수
};

class Minesweeper {
public:
    Minesweeper(int rows, int cols, int mineCount);
    void revealCell(int row, int col); // 셀 열기
    void toggleFlag(int row, int col); // 깃발 표시/해제
    bool checkWin() const;             // 승리 조건 확인
    const std::vector<std::vector<Cell>>& getBoard() const; // 보드 반환
    void revealAllCells();            // 모든 셀 열기 (지뢰 포함)
    void revealNearbyCells(int row, int col); // 주변 셀 열기

private:
    int rows, cols, mineCount;
    std::vector<std::vector<Cell>> board;

    void placeMines();                // 지뢰 배치
    void calculateSurroundingMines(); // 주변 지뢰 개수 계산
    void floodFill(int row, int col); // 빈 공간 확장
};

#endif
