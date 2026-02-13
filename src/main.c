#include "raylib.h"
#include "resource_dir.h"
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 1000
#define HEIGHT 1000

int grid_start_x;
int grid_start_y;
int grid_end_x;
int grid_end_y;
int index = 0;

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

letter letters[] = {
    {symbol : 'T', squares : {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}},
    {symbol : 'J', squares : {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}}},
    {symbol : 'L', squares : {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}},
    {symbol : 'S', squares : {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}}},
    {symbol : 'Z', squares : {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}}};

typedef struct shape
{
  Color color;
  coordinates c;
  bool placed;
  letter l;
  bool started;
} shape;

void drawShape(shape *s, Color c);
shape shapesGenerated[100000];
int generateRandomValue(int start, int end, int step_size)
{
  return end - start + GetRandomValue(0, 10) * step_size;
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

    if (!s->placed)
    {
      s->c.starty += 1;
    }
}

void drawShape(shape *s, Color c)
{
  printf(" startx: %d, starty:%d\n", s->c.startx, s->c.starty);
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
      .l = letters[GetRandomValue(0,5)],
  };
  shapesGenerated[index] = s;
  index++;
  drawShape(&s, color);
}

int main()
{

  InitWindow(WIDTH, HEIGHT, "tetris");
  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
    BeginDrawing();
    drawGrid();
    if (index == 0 || shapesGenerated[index - 1].placed == true){
      generateRandomBlocks();
    }
    updateShape(&shapesGenerated[index-1]);
    EndDrawing();
  }
  return 0;
}
