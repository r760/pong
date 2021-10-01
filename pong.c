#include "pong.h"

/**
 * movement
 */

void up(struct Coordinate *paddle[]) {
  int i;
  if (paddle[0]->y - 1 >= 0) {
    for (i = 0; i < PADDLE_LENGTH; i++) {
      int y = paddle[i]->y;
      paddle[i]->y = y - 1;
    }
  }
}

void down(struct Coordinate *paddle[]) {
  int i;
  if (paddle[PADDLE_LENGTH - 1]->y + 1 <= HEIGHT - 1) {
    for (i = 0; i < PADDLE_LENGTH; i++) {
      int y = paddle[i]->y;
      paddle[i]->y = y + 1;
    }
  }
}

void ai() {
  int x = ball->x;
  int y = ball->y;

  srand(time(NULL));
  int r0 = rand(), r1 = rand(), r2 = rand();

  if ((yDirection != 0 && x <= WIDTH * LOOKAHEAD) ||
      (yDirection == 0 && x <= WIDTH * LOOKAHEAD * 0.5)) {
    if (aiPaddle[0]->y < y && aiPaddle[1]->y < y && aiPaddle[2]->y < y) {
      if ((r0 % 2 == 0 && r1 % 2 == 0 && r2 % 2 == 0) ||
          (r0 % 2 != 0 && r1 % 2 != 0 && r2 % 2 != 0)) {
        up(aiPaddle);
      } else {
        down(aiPaddle);
      }
    } else if (aiPaddle[0]->y > y && aiPaddle[1]->y > y && aiPaddle[2]->y > y) {
      if ((r0 % 2 == 0 && r1 % 2 == 0 && r2 % 2 == 0) ||
          (r0 % 2 != 0 && r1 % 2 != 0 && r2 % 2 != 0)) {
        down(aiPaddle);
      } else {
        up(aiPaddle);
      }
    }
  }
}

/**
 * move the ball
 */
void moveBall() {
  int y = ball->y + yDirection;
  int x = ball->x + xDirection;

  if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
    ball->y = y;
    ball->x = x;
  } else if ((ball->y == playerPaddle[0]->y && ball->x == playerPaddle[0]->x) ||
             (ball->y == aiPaddle[0]->y && ball->x == aiPaddle[0]->x)) {
    // top of paddle
    yDirection = -1;
    xDirection *= (-1);
    if (ball->y == 0) {
      yDirection = 1;
    }
  } else if ((ball->y == playerPaddle[1]->y && ball->x == playerPaddle[1]->x) ||
             (ball->y == aiPaddle[1]->y && ball->x == aiPaddle[1]->x)) {
    // middle of paddle
    yDirection *= 0;
    xDirection *= (-1);
  } else if ((ball->y == playerPaddle[2]->y && ball->x == playerPaddle[2]->x) ||
             (ball->y == aiPaddle[2]->y && ball->x == aiPaddle[2]->x)) {
    // bottom of paddle
    yDirection = 1;
    xDirection *= (-1);
    if (ball->y == HEIGHT - 1) {
      yDirection = -1;
    }
  } else if (ball->x == WIDTH - 1) {
    // ai player scored
    xDirection *= (-1);
    if (ball->y == 0 || ball->y == HEIGHT - 1) {
      yDirection *= (-1);
    }
    aiPlayerScoreCount++;
  } else if (ball->x == 0) {
    // real player scored
    xDirection *= (-1);
    if (ball->y == 0 || ball->y == HEIGHT - 1) {
      yDirection *= (-1);
    }
    realPlayerScoreCount++;
  } else if (ball->y == 0 || ball->y == HEIGHT - 1) {
    // top and bottom collision
    yDirection *= (-1);
  }
}

int main() {
  newGame();
  initscr();

  while (stay) {
    timeout(70);
    clear();
    ai();
    moveBall();
    writeToBoard();
    printBoard();
    printScore();
    printBindings();
    move(2, 108);
    refresh();

    if (stay) {
      char c = getch();
      if (c == 'q') {
        stay = false;
      } else if (DOWN_KEY(c)) {
        down(playerPaddle);
      } else if (UP_KEY(c)) {
        up(playerPaddle);
      }
      if (aiPlayerScoreCount >= 10 || realPlayerScoreCount >= 10) {
        gameOverHandler();
      }
    }
  }

  endwin();
  return 0;
}