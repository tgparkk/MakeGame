#include "Minesweeper.h"
#include <cstdlib>
#include <ctime>

Minesweeper::Minesweeper(int rows, int cols, int mineCount)
    : rows(rows), cols(cols), mineCount(mineCount), board(rows, std::vector<Cell>(cols)) {
    srand(static_cast<unsigned>(time(0)));
    placeMines();
    calculateSurroundingMines();
}

void Minesweeper::placeMines() {
    int placed = 0;
    while (placed < mineCount) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (!board[r][c].isMine) {
            board[r][c].isMine = true;
            placed++;
        }
    }
}

void Minesweeper::calculateSurroundingMines() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board[r][c].isMine) continue;

            int count = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && board[nr][nc].isMine) {
                        count++;
                    }
                }
            }
            board[r][c].surroundingMines = count;
        }
    }
}

void Minesweeper::revealCell(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols || board[row][col].isRevealed) return;

    board[row][col].isRevealed = true;

    if (board[row][col].isMine) {
        revealAllCells(); // 지뢰 클릭 시 모든 셀을 표시
    }
    else {
        if (board[row][col].surroundingMines == 0) {
            floodFill(row, col); // 주변 셀들을 확장
        }
    }
}

void Minesweeper::toggleFlag(int row, int col) {
    if (row >= 0 && row < rows && col >= 0 && col < cols && !board[row][col].isRevealed) {
        board[row][col].isFlagged = !board[row][col].isFlagged;
    }
}

void Minesweeper::revealNearbyCells(int row, int col) {
    // 주변 8칸을 열어줍니다.
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int nr = row + dr, nc = col + dc;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !board[nr][nc].isRevealed) {
                board[nr][nc].isRevealed = true;
                if (board[nr][nc].surroundingMines == 0) {
                    floodFill(nr, nc); // 주변에 지뢰가 없다면 더 확장
                }
            }
        }
    }
}

void Minesweeper::revealAllCells() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            board[r][c].isRevealed = true;
        }
    }
}

void Minesweeper::floodFill(int row, int col) {
    // 주변 셀을 모두 열어줍니다.
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int nr = row + dr, nc = col + dc;
            revealCell(nr, nc); // 주변 셀을 모두 열어줌
        }
    }
}

bool Minesweeper::checkWin() const {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            if (!cell.isRevealed && !cell.isMine) return false;
        }
    }
    return true;
}

const std::vector<std::vector<Cell>>& Minesweeper::getBoard() const {
    return board;
}
