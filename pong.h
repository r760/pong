#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 10
#define WIDTH 22
#define PADDLE_LENGTH 3
#define LOOKAHEAD 0.30

#define UP_KEY(c) (c == 'k' || c == 'w')
#define DOWN_KEY(c) (c == 'j' || c == 's')

void gameOverHandler();

struct Coordinate {
  int y;
  int x;
};

char board[HEIGHT][WIDTH];

struct Coordinate *playerPaddle[PADDLE_LENGTH], *aiPaddle[PADDLE_LENGTH];
struct Coordinate *ball;

char defaultCharacter = ' ';
bool stay = true;
int aiPlayerScoreCount = 0, realPlayerScoreCount = 0, yDirection = 0,
    xDirection = 1;

/*
 * Utility Functions
 */

/**
 * set all board pieces to c
 */
void set(char c) {
  int y, x;
  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++) {
      board[y][x] = c;
    }
  }
}

void writeToBoard() {
  set(defaultCharacter);
  int i;
  for (i = 0; i < PADDLE_LENGTH; i++) {
    int y = playerPaddle[i]->y;
    int x = playerPaddle[i]->x;
    board[y][x] = '+';
  }

  for (i = 0; i < PADDLE_LENGTH; i++) {
    int y = aiPaddle[i]->y;
    int x = aiPaddle[i]->x;
    board[y][x] = '+';
  }

  board[ball->y][ball->x] = '0';
}

void newGame() {
  set(defaultCharacter);

  struct Coordinate *c0 =
      (struct Coordinate *)malloc(sizeof(struct Coordinate));
  c0->y = (HEIGHT / 2) - 1;
  c0->x = WIDTH - 1;

  struct Coordinate *c1 =
      (struct Coordinate *)malloc(sizeof(struct Coordinate));
  c1->y = (HEIGHT / 2);
  c1->x = WIDTH - 1;

  struct Coordinate *c2 =
      (struct Coordinate *)malloc(sizeof(struct Coordinate));
  c2->y = (HEIGHT / 2) + 1;
  c2->x = WIDTH - 1;
  playerPaddle[0] = c0;
  playerPaddle[1] = c1;
  playerPaddle[2] = c2;

  struct Coordinate *c3 =
      (struct Coordinate *)malloc(sizeof(struct Coordinate));
  c3->y = (HEIGHT / 2) - 1;
  c3->x = 0;

  struct Coordinate *c4 =
      (struct Coordinate *)malloc(sizeof(struct Coordinate));
  c4->y = (HEIGHT / 2);
  c4->x = 0;

  struct Coordinate *c5 =
      (struct Coordinate *)malloc(sizeof(struct Coordinate));
  c5->y = (HEIGHT / 2) + 1;
  c5->x = 0;
  aiPaddle[0] = c3;
  aiPaddle[1] = c4;
  aiPaddle[2] = c5;

  ball = (struct Coordinate *)malloc(sizeof(struct Coordinate));
  ball->y = HEIGHT / 2;
  ball->x = WIDTH / 2;

  aiPlayerScoreCount = 0, realPlayerScoreCount = 0, yDirection = 0,
  xDirection = 1;
}

/*
 * IO Functions
 */

void print(const char *temp, int y, int x) {
  int i = 0;
  while (temp[i] != '\0') {
    mvaddch(y, x++, temp[i++]);
  }
}

void printBindings() {
  const char *bindings[4] = {"Commands:", "q - Quit\n",
                             "j (or s) - Move down\n", "k (or w) - Move up\n"};

  int i, y = 2, x = 99;
  for (i = 0; i < 4; i++) {
    if (i == 1) {
      x++;
    }
    print(bindings[i], y++, x);
  }
}

void printBoard() {
  int y, x, temp;
  temp = 2;
  for (y = 0; y < HEIGHT; y++) {
    move(temp, 5);
    printw("+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+--"
           "-+---+---+---+---+---+\n");
    move(temp + 1, 5);
    for (x = 0; x < WIDTH; x++) {
      printw("| %c ", board[y][x]);
    }
    printw("|\n");
    temp += 2;
  }
  move(temp, 5);
  printw("+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+"
         "---+---+---+---+---+\n");
}

void printScore() {
  move(1, 42);
  printw("AI (%d) - YOU (%d)\n", aiPlayerScoreCount, realPlayerScoreCount);
}

void gameOverHandler() {
  char c;
  while (true) {
    clear();
    printBoard();
    printScore();
    print("Commands:\n", 2, 99);
    print("q - Quit\n", 3, 100);
    print("n - New game\n", 4, 100);
    print("Game Over!\n", 24, 45);
    move(2, 108);
    refresh();

    c = getch();
    if (c == 'q') {
      stay = false;
      break;
    } else if (c == 'n') {
      newGame();
      break;
    }
  }
}