#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

void printScreen(int racketPosition1, int racketPosition2, int ballPositionX, int ballPositionY, int score1,
                 int score2) {
    system("clear");
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            if (y == 0 || y == 24) {
                printf("-");
            } else if (x == 0 || x == 79) {
                printf("|");
            } else {
                if (x == ballPositionX && y == ballPositionY) {
                    printf("*");
                } else if (y == 5 && x == 38) {
                    printf("%d", score1);
                } else if (y == 5 && x == 42) {
                    printf("%d", score2);
                } else if ((x == 7 &&
                            (y == racketPosition1 - 1 || y == racketPosition1 || y == racketPosition1 + 1)) ||
                           (x == 71 &&
                            (y == racketPosition2 - 1 || y == racketPosition2 || y == racketPosition2 + 1)) ||
                           x == 40) {
                    printf("|");
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

struct TermSettings {
    struct termios oldt;
    struct termios newt;
    int oldf;
};

struct TermSettings setTmode() {
    struct TermSettings settings;

    tcgetattr(STDIN_FILENO, &settings.oldt);  // Получение текущих настроек терминала
    settings.newt = settings.oldt;            // Копирование настроек
    settings.newt.c_lflag &=
        ~(ICANON | ECHO);  // Отключение флагов для моментального считывания нажатия клавиши без enter
    tcsetattr(STDIN_FILENO, TCSANOW, &settings.newt);  // Применение новых настроек
    settings.oldf = fcntl(STDIN_FILENO, F_GETFL, 0);  // Получение текущих флагов стандартного ввода
    fcntl(STDIN_FILENO, F_SETFL,
          settings.oldf | O_NONBLOCK);  // Перевод ввода для неблокирующего ввода для getchar

    return settings;
}

void reset_terminal_mode(struct TermSettings settings) {
    tcsetattr(STDIN_FILENO, TCSANOW, &settings.oldt);  // Восстановление исходных настроек
    fcntl(STDIN_FILENO, F_SETFL, settings.oldf);  // Восстановление старых флагов
}

int keystroke() {
    int ch;  // Переменная для считывания символа

    struct TermSettings settings = setTmode();

    ch = getchar();  // Считывание символа, если символа нет то вернет EOF

    reset_terminal_mode(settings);

    if (ch != EOF) {        // Проверка на считывание символа
        ungetc(ch, stdin);  // Возврат символа для следующего вызова getchar
        return 1;
    }

    return 0;
}

int roundOfGame(int turnPlayer, int speedOfGame, int boost, int score1, int score2) {
    int racketPosition1 = 12;
    int racketPosition2 = 12;
    int ballPositionX;
    int ballPositionY = 12;

    int vectorBallX;
    int vectorBallY = 0;

    if (turnPlayer == 1) {
        ballPositionX = 8;
        vectorBallX = -1;
    } else if (turnPlayer == 2) {
        ballPositionX = 70;
        vectorBallX = 1;
    }

    char ch;
    while (keystroke() && scanf("%c", &ch) == 1 && ch != '\n') {
    }
    int boostValue = 0;
    while (ballPositionX > 1 && ballPositionX < 77) {
        printScreen(racketPosition1, racketPosition2, ballPositionX, ballPositionY, score1, score2);
        char turn;

        // correct input
        if (keystroke()) {
            scanf("%c", &turn);
            if (turn == 'Q' || turn == 'q') {
                return -1;
            } else if (turn == 'P' || turn == 'p') {
                while (1) {
                    if (keystroke() && scanf("%c", &turn) && (turn == 'p' || turn == 'P')) {
                        turn = ' ';
                        break;
                    }
                    usleep(100000);
                }
            } else if (!(turn == 'A' || turn == 'a' || turn == 'Z' || turn == 'z' || turn == 'K' ||
                         turn == 'k' || turn == 'M' || turn == 'm')) {
                turn = ' ';
            }
        } else {
            turn = ' ';
        }

        char vectorRacket;

        // determinate vector of racket
        if (turn == 'A' || turn == 'a') {
            if (racketPosition1 != 2) {
                vectorRacket = 'u';
            } else {
                vectorRacket = ' ';
            }
        } else if (turn == 'Z' || turn == 'z') {
            if (racketPosition1 != 22) {
                vectorRacket = 'd';
            } else {
                vectorRacket = ' ';
            }
        } else if (turn == 'K' || turn == 'k') {
            if (racketPosition2 != 2) {
                vectorRacket = 'u';
            } else {
                vectorRacket = ' ';
            }
        } else if (turn == 'M' || turn == 'm') {
            if (racketPosition2 != 22) {
                vectorRacket = 'd';
            } else {
                vectorRacket = ' ';
            }
        } else if (turn == ' ') {
            vectorRacket = ' ';
        }

        // turn of racket
        if (ballPositionX == 8) {
            boostValue += boost;
            if ((ballPositionY == racketPosition1 - 1) || (ballPositionY == racketPosition1) ||
                (ballPositionY == racketPosition1 + 1)) {
                if (vectorRacket == 'u') {
                    if (vectorBallY == 1) {
                        vectorBallY = 0;
                    } else if (vectorBallY == 0) {
                        vectorBallY = -1;
                    } else if (vectorBallY == -1) {
                        vectorBallY = -1;
                    }
                    vectorBallX = 1;
                } else if (vectorRacket == 'd') {
                    if (vectorBallY == 1) {
                        vectorBallY = 1;
                    } else if (vectorBallY == 0) {
                        vectorBallY = 1;
                    } else if (vectorBallY == -1) {
                        vectorBallY = 0;
                    }
                    vectorBallX = 1;
                } else if (vectorRacket == ' ') {
                    vectorBallX = 1;
                }
            } else if ((ballPositionY == racketPosition1 - 2) && vectorRacket == 'u') {
                vectorBallY = -1;
                vectorBallX = 1;
            } else if ((ballPositionY == racketPosition1 + 2) && vectorRacket == 'd') {
                vectorBallY = 1;
                vectorBallX = 1;
            } else if (vectorRacket == ' ' && (ballPositionY == racketPosition1 + 2) && vectorBallY == -1) {
                vectorBallY = 1;
                vectorBallX = 1;
            } else if (vectorRacket == ' ' && (ballPositionY == racketPosition1 - 2) && vectorBallY == 1) {
                vectorBallY = -1;
                vectorBallX = 1;
            }
        } else if (ballPositionX == 70) {
            boostValue += boost;
            if ((ballPositionY == racketPosition2 - 1) || (ballPositionY == racketPosition2) ||
                (ballPositionY == racketPosition2 + 1)) {
                if (vectorRacket == 'u') {
                    if (vectorBallY == 1) {
                        vectorBallY = 0;
                    } else if (vectorBallY == 0) {
                        vectorBallY = -1;
                    } else if (vectorBallY == -1) {
                        vectorBallY = -1;
                    }
                    vectorBallX = -1;
                } else if (vectorRacket == 'd') {
                    if (vectorBallY == 1) {
                        vectorBallY = 1;
                    } else if (vectorBallY == 0) {
                        vectorBallY = 1;
                    } else if (vectorBallY == -1) {
                        vectorBallY = 0;
                    }
                    vectorBallX = -1;
                }
                if (vectorRacket == ' ') {
                    vectorBallX = -1;
                }
            } else if ((ballPositionY == racketPosition2 - 2) && vectorRacket == 'u') {
                vectorBallY = -1;
                vectorBallX = -1;
            } else if ((ballPositionY == racketPosition2 + 2) && vectorRacket == 'd') {
                vectorBallY = 1;
                vectorBallX = -1;
            } else if (vectorRacket == ' ' && (ballPositionY == racketPosition2 + 2) && vectorBallY == -1) {
                vectorBallY = 1;
                vectorBallX = -1;
            } else if (vectorRacket == ' ' && (ballPositionY == racketPosition2 - 2) && vectorBallY == 1) {
                vectorBallY = -1;
                vectorBallX = -1;
            }
        }

        // turn of walls
        if (ballPositionY == 1 || ballPositionY == 23) {
            vectorBallY = -vectorBallY;
        }

        // change racket position
        if (turn == 'A' || turn == 'a') {
            if (racketPosition1 != 2) {
                racketPosition1--;
            }
        } else if (turn == 'Z' || turn == 'z') {
            if (racketPosition1 != 22) {
                racketPosition1++;
            }
        } else if (turn == 'K' || turn == 'k') {
            if (racketPosition2 != 2) {
                racketPosition2--;
            }
        } else if (turn == 'M' || turn == 'm') {
            if (racketPosition2 != 22) {
                racketPosition2++;
            }
        } else

            // change ball position
            ballPositionX += vectorBallX;
        ballPositionY += vectorBallY;
        if (speedOfGame - boostValue < 20000) {
            usleep(20000);
        } else {
            usleep(speedOfGame - boost);
        }
    }

    if (ballPositionX <= 5) {
        return 2;
    } else {
        return 1;
    }
}

int game(int scoreToWin, int startTurnPlayer, int speedOfGame, int boost) {
    int winner;
    int turnPlayer = startTurnPlayer;
    int score1 = 0;
    int score2 = 0;

    while (score1 < scoreToWin && score2 < scoreToWin) {
        winner = roundOfGame(turnPlayer, speedOfGame, boost, score1, score2);
        if (winner == 1) {
            score1++;
            turnPlayer = 2;
        } else if (winner == 2) {
            score2++;
            turnPlayer = 1;
        } else {
            printf("The score: \nThe left player (%d : %d) The rigth player\n", score1, score2);
            return 0;
        }
    }

    if (score1 == scoreToWin) {
        printf("The winner %d-%d is the left player!", score1, score2);
    } else {
        printf("The winner %d-%d is the rigth player!", score1, score2);
    }
    return 0;
}

int main() {
    int scoreToWin = 21;
    int startTurnPlayer = 1;
    int speedOfGame = 45000;
    int boost = 2000;
    return game(scoreToWin, startTurnPlayer, speedOfGame, boost);
}