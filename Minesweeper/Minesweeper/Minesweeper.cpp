#include "Minesweeper.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

Minesweeper::Minesweeper(int rows, int cols, int mineCount)
    : rows(rows), cols(cols), mineCount(mineCount) {
    srand(static_cast<unsigned int>(time(0)));
    board.resize(rows, std::vector<Cell>(cols));

    placeMines();
    calculateSurroundingMines();
}

void Minesweeper::placeMines() {
    int placedMines = 0;
    while (placedMines < mineCount) {
        int row = rand() % rows;
        int col = rand() % cols;

        if (!board[row][col].isMine) {
            board[row][col].isMine = true;
            placedMines++;
        }
    }
}

void Minesweeper::calculateSurroundingMines() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board[r][c].isMine) continue;

            int surroundingMines = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && board[nr][nc].isMine) {
                        surroundingMines++;
                    }
                }
            }
            board[r][c].surroundingMines = surroundingMines;
        }
    }
}

void Minesweeper::toggleFlag(int row, int col) {
    if (!board[row][col].isRevealed) {
        board[row][col].isFlagged = !board[row][col].isFlagged;
    }
}

void Minesweeper::revealCell(int row, int col) {
    if (board[row][col].isRevealed || board[row][col].isFlagged) {
        return;
    }

    board[row][col].isRevealed = true;
    if (board[row][col].isMine) {
        gameOver = true;
    }
    else if (board[row][col].surroundingMines == 0) {
        floodFill(row, col);
    }
}

void Minesweeper::floodFill(int row, int col) {
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int nr = row + dr, nc = col + dc;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !board[nr][nc].isRevealed && !board[nr][nc].isMine) {
                revealCell(nr, nc);
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

void Minesweeper::checkForWrongFlags() {
    if (!gameOver) return;

    // 게임이 끝난 후 잘못된 플래그를 빨간색으로 표시
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!board[r][c].isMine && board[r][c].isFlagged) {
                // 잘못된 플래그 처리 (배경색 빨간색으로)
                // 렌더링 시 처리
            }
        }
    }
}

bool Minesweeper::checkWin() const {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if ((!board[r][c].isMine && !board[r][c].isRevealed) ||
                (board[r][c].isMine && !board[r][c].isFlagged)) {
                return false;
            }
        }
    }
    return true;
}

const std::vector<std::vector<Cell>>& Minesweeper::getBoard() const {
    return board;
}
