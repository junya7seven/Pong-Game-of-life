#include <stdio.h>

void printScreen(int racketPosition1, int racketPosition2, int ballPositionX, int ballPositionY) {
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            if (y == 0 || y == 24) {
                printf("-");
            } else if (x == 0 || x == 78 || x == 79) {
                printf("|");
            } else {
                if (x == ballPositionX && y == ballPositionY) {
                    printf("*");
                } else if ((x == 7 && (y == racketPosition1 - 1 || y == racketPosition1 ||
                                      y == racketPosition1 + 1)) ||
                           (x == 71 && (y == racketPosition2 - 1 || y == racketPosition2 ||
                                       y == racketPosition2 + 1)) ||
                           x == 39) {
                    printf("|");
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

void printScore(int score1, int score2) {
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            if (y == 0 || y == 24) {
                printf("-");
            } else if (x == 0 || x == 78 || x == 79) {
                printf("|");
            } else {
                if (x == 35 && y == 3) {
                    printf("%d", score1);
                } else if (x == 43 && y == 3) {
                    printf("%d", score2);
                } else if ((x == 7 && (y == 11 || y == 12 || y == 13)) ||
                           (x == 71 && (y == 11 || y == 12 || y == 13)) || x == 39) {
                    printf("|");
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

int roundOfGame(int turnPlayer) {
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
    while (scanf("%c", &ch) == 1 && ch != '\n') {
    }

    while (ballPositionX > 1 && ballPositionX < 77) {
        printScreen(racketPosition1, racketPosition2, ballPositionX, ballPositionY);
        char turn;
        char enter;

        // correct input
        while (1) {
            scanf("%c", &turn);
            if (turn == '\n') {
                turn = 'p';
                break;
            } else {
                scanf("%c", &enter);
                if (enter != '\n') {
                    while (scanf("%c", &enter) == 1 && enter != '\n') {
                    }
                } else if (turn == 'A' || turn == 'a' || turn == 'Z' || turn == 'z' || turn == 'K' ||
                           turn == 'k' || turn == 'M' || turn == 'm') {
                    break;
                }
            }
        }

        char vectorRacket;

        // determinate vector of racket
        if (turn == 'A' || turn == 'a') {
            if (racketPosition1 != 2) {
                vectorRacket = 'u';
            } else {
                vectorRacket = 'p';
            }
        } else if (turn == 'Z' || turn == 'z') {
            if (racketPosition1 != 22) {
                vectorRacket = 'd';
            } else {
                vectorRacket = 'p';
            }
        } else if (turn == 'K' || turn == 'k') {
            if (racketPosition2 != 2) {
                vectorRacket = 'u';
            } else {
                vectorRacket = 'p';
            }
        } else if (turn == 'M' || turn == 'm') {
            if (racketPosition2 != 22) {
                vectorRacket = 'd';
            } else {
                vectorRacket = 'p';
            }
        } else if (turn == 'p') {
            vectorRacket = 'p';
        }

        // turn of racket
        if (ballPositionX == 8) {
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
                } else if (vectorRacket == 'p') {
                    vectorBallX = 1;
                }
            } else if ((ballPositionY == racketPosition1 - 2) && vectorRacket == 'u') {
                vectorBallY = -1;
                vectorBallX = 1;
            } else if ((ballPositionY == racketPosition1 + 2) && vectorRacket == 'd') {
                vectorBallY = 1;
                vectorBallX = 1;
            } else if (vectorRacket == 'p' && (ballPositionY == racketPosition1 + 2) && vectorBallY == -1) {
                vectorBallY = 1;
                vectorBallX = 1;
            } else if (vectorRacket == 'p' && (ballPositionY == racketPosition1 - 2) && vectorBallY == 1) {
                vectorBallY = -1;
                vectorBallX = 1;
            }
        } else if (ballPositionX == 70) {
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
                if (vectorRacket == 'p') {
                    vectorBallX = -1;
                }
            } else if ((ballPositionY == racketPosition2 - 2) && vectorRacket == 'u') {
                vectorBallY = -1;
                vectorBallX = -1;
            } else if ((ballPositionY == racketPosition2 + 2) && vectorRacket == 'd') {
                vectorBallY = 1;
                vectorBallX = -1;
            } else if (vectorRacket == 'p' && (ballPositionY == racketPosition2 + 2) && vectorBallY == -1) {
                vectorBallY = 1;
                vectorBallX = -1;
            } else if (vectorRacket == 'p' && (ballPositionY == racketPosition2 - 2) && vectorBallY == 1) {
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
        }

        // change ball position
        ballPositionX += vectorBallX;
        ballPositionY += vectorBallY;
    }

    if (ballPositionX <= 5) {
        return 2;
    } else {
        return 1;
    }
}

int game(int scoreToWin, int startTurnPlayer) {
    int score1 = 0;
    int score2 = 0;
    int winner;
    int turnPlayer = startTurnPlayer;

    while (score1 < scoreToWin && score2 < scoreToWin) {
        printScore(score1, score2);
        winner = roundOfGame(turnPlayer);
        if (winner == 1) {
            score1++;
            turnPlayer = 2;
        } else if (winner == 2) {
            score2++;
            turnPlayer = 1;
        } else {
            return 1;
        }
    }

    printScore(score1, score2);

    if (score1 == scoreToWin) {
        printf("The winner is the left player!");
    } else {
        printf("The winner is the rigth player!");
    }
    return 0;
}

int main() {
    int scoreToWin = 21;
    int startTurnPlayer = 1;
    return game(scoreToWin, startTurnPlayer);
}
