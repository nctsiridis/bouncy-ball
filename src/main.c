#include <stdio.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <math.h>

#define WINDOW_NAME "Circle Physics"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int close(int x, char msg[]) {
  if (strcmp(msg, "") != 0) {
    if (x == 1) printf("ERROR: ");
    printf("%s\n", msg);
  }
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return x;
}

struct Color {
  int r, g, b, a;
};

struct Color RED = {150, 0, 0, 255};
struct Color GREEN = {0, 150, 0, 255};
struct Color BLUE = {0, 0, 150, 255};

struct Ball {
  float x, y;
  int rad;
  float vx, vy;
  struct Color color;
};

struct Force {
  float fx;
  float fy;
};

void render_circle(int x, int y, int rad, struct Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  int start_outer_x = x - rad;
  int start_outer_y = y - rad;
  int diam = 2 * rad;

  int inner_side_len = sqrt((diam * diam) / 2);
  int chunk = (diam - inner_side_len) / 2;
  int start_inner_x = start_outer_x + chunk;
  int start_inner_y = start_outer_y + chunk;
  SDL_Rect inner_rect = {start_inner_x, start_inner_y, inner_side_len, inner_side_len};
  SDL_RenderFillRect(renderer, &inner_rect);

  for (int i = start_outer_x; i < start_outer_x + diam; i++) {
    for (int j = start_outer_y; j < start_outer_y + diam; j++) {
      int flag_hori = i > start_inner_x && i < start_inner_x + inner_side_len;
      int flag_vert = j > start_inner_y && j < start_inner_y + inner_side_len;
      if (!flag_hori || !flag_vert) {
        int euc_to_center = sqrt(((i - x) * (i - x)) + ((j - y) * (j - y)));
        if (euc_to_center < rad) {
          SDL_RenderDrawPoint(renderer, i, j);
        }
      }
    }
  }
  SDL_RenderPresent(renderer);
}

void update_ball(struct Ball* ball) {
  ball->x += ball->vx;
  ball->y += ball->vy;

  if (ball->y + ball->rad >= WINDOW_HEIGHT) {
    ball->vy = -ball->vy;
  }

  if (ball->x - ball->rad <= 0 || ball->x + ball->rad >= WINDOW_WIDTH) {
    ball->vx = -ball->vx;
  }
}

void apply_force(struct Force* f, struct Ball* b) {
  b->vx += f->fx;
  b->vy += f->fy; 
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) return close(1, "Failed to initialize SDL2 Video.");

  window = SDL_CreateWindow(
      WINDOW_NAME,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_SHOWN
  );

  if (!window) return close(1, "Failed to initialize SDL window");

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  int running = 1;
  SDL_Event e;

  struct Ball ball = (struct Ball) {50, 150, 50, 0.1f, 0.f, BLUE};

  struct Force gravity = (struct Force) {0.f, 0.0002f};
  
  while(running) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) running = 0;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    update_ball(&ball);
    apply_force(&gravity, &ball);
    render_circle((int) ball.x, (int) ball.y, ball.rad, ball.color);
  }

  return close(0, "");
}
