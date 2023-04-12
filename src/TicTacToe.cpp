#include <iostream>
#include <conio.h>
#include <iomanip>
#include <thread>
#include <Windows.h>

using namespace std;

char boardCells[8]; // Every Cells contains a value represented by ( #=null, X=crosse, O=nought )
int cellSelector = 4;
bool whoPlayNow = true; // true = X, false = O;
bool win = false;

void printingBoard(bool stateForSelecting) {
    system("CLS");
    // Printing the board

    for (int i = 0; i <= 8; i++) {

        if (((i % 3) == 0) && (i != 0))
            cout << "\n---+---+---\n";

        if (((i % 3) != 0))
            cout << " | ";
        if (((i % 3) == 0) || i == 0)
            cout << " ";

        if (stateForSelecting == true) {
            cout << boardCells[i];
        }
        else {
            if (cellSelector == i) {
                cout << "\033[31m"; // change text color to red
                cout << "\033[4m"; // make text underlined
            }

            cout << boardCells[i];

            cout << "\033[0m"; // reset text color to default
            cout << "\033[0m"; // reset text attributes to default

        }
    }

    cout << endl << endl;
    if (whoPlayNow == true) {
        cout << "   [ X ]";
    }
    if (whoPlayNow == false) {
        cout << "   [ O ]";
    }
}

bool checkForWin(bool whoPlayNowInside) {
    char player;

    if (whoPlayNowInside == true) {
        player = 'X';
    }
    if (whoPlayNowInside == false) {
        player = 'O';
    }

    // Check for horizontal wins
    for (int i = 0; i < 9; i += 3) {
        if (boardCells[i] == player && boardCells[i + 1] == player && boardCells[i + 2] == player) {
            return true;
        }
    }

    // Check for vertical wins
    for (int i = 0; i < 3; i++) {
        if (boardCells[i] == player && boardCells[i + 3] == player && boardCells[i + 6] == player) {
            return true;
        }
    }

    // Check for diagonal wins
    if (boardCells[0] == player && boardCells[4] == player && boardCells[8] == player) {
        return true;
    }

    if (boardCells[2] == player && boardCells[4] == player && boardCells[6] == player) {
        return true;
    }

    return false;
}

void winScreen(char charOfPlayer) {
    system("CLS");
    cout << "\033[32m"; // Set text color to green
    cout << "    [" << charOfPlayer << "] \n  Winnerr!\n\n\nPlay Again?\n   -F1-";
    win = !win;
    char ch;
    bool wantToPlayAgain = true;
    while (wantToPlayAgain) {
        if (_kbhit()) { // check if key has been pressed
            ch = _getch();
            if ((int)ch == 59) {
                win = !win;
                system("CLS");
                cout << "\033[39m"; // Reset text color to default
                wantToPlayAgain = !wantToPlayAgain;
                cellSelector = 4;
                // filling the Cells with blanks
                for (int k = 0; k <= 8; k++) {
                    boardCells[k] = ' ';
                }
            }
        }
    }
}

void assignValueToCell() {

    if (boardCells[cellSelector] == ' ') {
        if (whoPlayNow == true) {
            boardCells[cellSelector] = 'X';
            if (checkForWin(true)) {
                winScreen('X');
            }
        }
        if (whoPlayNow == false) {
            boardCells[cellSelector] = 'O';
            if (checkForWin(false)) {
                winScreen('O');
            }
        }

        whoPlayNow = !whoPlayNow; // toggle who plays next
    }
    else {
        cout << "\033[31m"; // Set text color to red
        printingBoard(true);
        Sleep(200);
        system("CLS");
        cout << "\033[39m"; // Reset text color to default
    }
}

int main() {
    static CONSOLE_FONT_INFOEX fontex;
    fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetCurrentConsoleFontEx(hOut, 0, &fontex);
    fontex.FontWeight = 700;
    fontex.dwFontSize.X = 36;
    fontex.dwFontSize.Y = 36;
    SetCurrentConsoleFontEx(hOut, NULL, &fontex);

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {
      0,
      0,
      10,
      10
    };
    SetConsoleWindowInfo(consoleHandle, TRUE, &windowSize);
    COORD bufferSize = {
      11,
      11
    };
    SetConsoleScreenBufferSize(consoleHandle, bufferSize);

    // filling the Cells with blanks
    for (int k = 0; k <= 8; k++) {
        boardCells[k] = ' ';
    }

    printingBoard(false);

    char ch;
    while (true) {
        if (_kbhit()) { // check if key has been pressed
            ch = _getch();
            //cout << (int)ch;
            switch ((int)ch) {
            case 72: // up arrow
                if (cellSelector > 0 && cellSelector <= 8 && cellSelector - 3 >= 0)
                    cellSelector = cellSelector - 3;
                printingBoard(false);
                break;

            case 80: // down arrow
                if (cellSelector >= 0 && cellSelector < 8 && cellSelector + 3 <= 8)
                    cellSelector = cellSelector + 3;
                printingBoard(false);
                break;

            case 75: // left arrow
                if (cellSelector > 0 && cellSelector <= 8)
                    cellSelector = cellSelector - 1;
                printingBoard(false);
                break;

            case 77: // right arrow
                if (cellSelector >= 0 && cellSelector < 8)
                    cellSelector = cellSelector + 1;
                printingBoard(false);
                break;

            case 13: // enter key
                assignValueToCell();
                if (win) {
                    printingBoard(false);
                }
                break;
            }
        }
    }
}