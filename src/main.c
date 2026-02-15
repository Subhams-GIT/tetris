#include "raylib.h"
#include "resource_dir.h"
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 1000
#define HEIGHT 1000
#define CELL_SIZE 20
#define GRID_COLS 12
#define GRID_ROWS 21

typedef struct coordinates
{
  int startx;
  int starty;
} coordinates;

typedef struct letter
{
  char symbol;
  int squares[3][3];
} letter;

typedef struct shape
{
  Color color;
  coordinates c;
  bool placed;
  letter l;
} shape;

bool key_pressed = false;
int grid_start_x;
int grid_start_y;
int grid_end_x;
int grid_end_y;
int index = 0;
int grid[21][12];

letter letters[] = {
    {symbol : 'T', squares : {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}},
    {symbol : 'J', squares : {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}}},
    {symbol : 'L', squares : {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}},
    {symbol : 'S', squares : {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}}},
    {symbol : 'Z', squares : {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}}};

shape shapesGenerated[100];

void drawShape(shape *s);

int generateRandomValue(int start, int end, int step_size)
{
  int columns = (grid_end_x - grid_start_x) / 20;
  int randomCol = GetRandomValue(0, columns - 3);
  int startx = grid_start_x + randomCol * 20;
  return startx;
}

void drawGrid()
{

  /* vertical lines */
  for (int i = WIDTH / 4; i < WIDTH / (2); i += 20)
  {
    DrawLine(i, 80, i, HEIGHT / 2, WHITE);
  }

  /* horizontal lines */
  for (int i = 80; i < HEIGHT / 2 + 10; i += 20)
  {
    DrawLine(WIDTH / 4, i, WIDTH / 2 - 10, i, WHITE);
  }
  grid_start_x = WIDTH / 4;
  grid_start_y = 80;
  grid_end_x = WIDTH / 2;
  grid_end_y = HEIGHT / 2;
}

Color RandomColor()
{
  return (Color){
      GetRandomValue(0, 255),
      GetRandomValue(0, 255),
      GetRandomValue(0, 255),
      255,
  };
}

void updateShape(shape *s)
{
  int baseCol = (s->c.startx - grid_start_x) / CELL_SIZE;
  int baseRow = (s->c.starty - grid_start_y) / CELL_SIZE;

  // Check if moving down would collide
  for (int row = 0; row < 3; row++)
  {
    for (int col = 0; col < 3; col++)
    {
      if (s->l.squares[row][col] == 1)
      {
        int nextRow = baseRow + row + 1;
        int nextCol = baseCol + col;

        // Bottom boundary
        if (nextRow >= GRID_ROWS)
        {
          s->placed = true;
          goto place_block;
        }

        // Collision with placed block
        if (grid[nextRow][nextCol] == 1)
        {
          s->placed = true;
          goto place_block;
        }
      }
    }
  }

  // No collision → move down
  s->c.starty += CELL_SIZE;
  return;

place_block:

  // Lock shape into grid
  for (int row = 0; row < 3; row++)
  {
    for (int col = 0; col < 3; col++)
    {
      if (s->l.squares[row][col] == 1)
      {
        int gridRow = baseRow + row;
        int gridCol = baseCol + col;

        if (gridRow >= 0 && gridRow < GRID_ROWS &&
            gridCol >= 0 && gridCol < GRID_COLS)
        {
          grid[gridRow][gridCol] = 1;
        }
      }
    }
  }
}

void moveShapeRight(shape *s)
{
  if (s->c.startx + 80 >= grid_end_x)
    return;
  s->c.startx += 20;
}

void moveShapeleft(shape *s)
{
  if (s->c.startx <= grid_start_x)
    return;
  s->c.startx -= 20;
}

void drawShape(shape *s)
{
  for (int row = 0; row < 3; row++)
  {
    for (int col = 0; col < 3; col++)
    {
      if (s->l.squares[row][col] == 1)
      {
        DrawRectangle(
            s->c.startx + col * 20,
            s->c.starty + row * 20,
            20,
            20,
            s->color);
        DrawRectangleLines(
            s->c.startx + col * 20,
            s->c.starty + row * 20,
            20,
            20,
            WHITE);
      }
    }
  }
}

void generateRandomBlocks()
{
  Color color = RandomColor();
  int startx = generateRandomValue(grid_start_x, grid_end_x, 20);
  coordinates c = {
      .startx = startx,
      .starty = 80,
  };

  shape s = {
      .color = color,
      .c = c,
      .placed = false,
      .l = letters[GetRandomValue(0, 4)],
  };
  shapesGenerated[index] = s;
  index++;
}

int main()
{
  InitWindow(WIDTH, HEIGHT, "tetris");
  SetTargetFPS(10);
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(BLACK);
    drawGrid();
    if (index == 0 || shapesGenerated[index - 1].placed == true)
    {
      generateRandomBlocks();
    }
    if (index > 0)
    {

      if (IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_LEFT))
        key_pressed = false;
      if (IsKeyDown(KEY_RIGHT))
      {
        key_pressed = true;
        moveShapeRight(&shapesGenerated[index - 1]);
      }

      if (IsKeyDown(KEY_LEFT))
      {
        key_pressed = true;
        moveShapeleft(&shapesGenerated[index - 1]);
      }
    }
    if (index > 0 && shapesGenerated[index - 1].placed == false)
    {
      WaitTime(1);
      if (!key_pressed)
        updateShape(&shapesGenerated[index - 1]);
    }
    for (int i = 0; i < index; i++)
    {
      drawShape(&shapesGenerated[i]);
    }
    EndDrawing();
  }
  return 0;
}
