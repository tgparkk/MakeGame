#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <vector>

struct Cell {
    bool isMine = false;       // ���� ����
    bool isRevealed = false;   // ���� ����
    bool isFlagged = false;    // ��� ǥ�� ����
    int surroundingMines = 0;  // �ֺ� ���� ����
};

class Minesweeper {
public:
    Minesweeper(int rows, int cols, int mineCount);
    void revealCell(int row, int col); // �� ����
    void toggleFlag(int row, int col); // ��� ǥ��/����
    bool checkWin() const;             // �¸� ���� Ȯ��
    const std::vector<std::vector<Cell>>& getBoard() const; // ���� ��ȯ
    void revealAllCells();            // ��� �� ���� (���� ����)
    void revealNearbyCells(int row, int col); // �ֺ� �� ����

private:
    int rows, cols, mineCount;
    std::vector<std::vector<Cell>> board;

    void placeMines();                // ���� ��ġ
    void calculateSurroundingMines(); // �ֺ� ���� ���� ���
    void floodFill(int row, int col); // �� ���� Ȯ��
};

#endif
