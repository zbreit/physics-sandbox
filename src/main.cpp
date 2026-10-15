#include "SDL3/SDL.h" // IWYU pragma: export
#include "vec.hpp"
#include <chrono>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

// Units in meters
constexpr SDL_FRect WORLD_BBOX = {
    .x = 0,
    .y = 0,
    .w = 20,
    .h = 20.0 / SCREEN_WIDTH * SCREEN_HEIGHT,
};

sndbx::vec<2> world_point_to_screen(sndbx::vec<2> point) {
  auto x2 = (point.x() - WORLD_BBOX.x) / WORLD_BBOX.w * SCREEN_WIDTH;
  // Y-axis is flipped between screen and world space.
  auto y2 = SCREEN_HEIGHT -
            ((point.y() - WORLD_BBOX.y) / WORLD_BBOX.h * SCREEN_HEIGHT);

  return sndbx::vec<2>{{x2, y2}};
}

float world_scalar_to_screen(float point) {
  return point / WORLD_BBOX.w * SCREEN_WIDTH;
}

// Source - https://stackoverflow.com/a/48291620
// Posted by Scotty Stephens, modified by community. See post 'Timeline' for
// change history Retrieved 2026-10-11, License - CC BY-SA 4.0
void sdl_draw_circle(SDL_Renderer *renderer, float center_x, float center_y,
                     float radius) {
  const float diameter = (radius * 2);

  float x = (radius - 1);
  float y = 0;
  float tx = 1;
  float ty = 1;
  float error = (tx - diameter);

  while (x >= y) {
    //  Each of the following renders an octant of the circle
    SDL_RenderPoint(renderer, center_x + x, center_y - y);
    SDL_RenderPoint(renderer, center_x + x, center_y + y);
    SDL_RenderPoint(renderer, center_x - x, center_y - y);
    SDL_RenderPoint(renderer, center_x - x, center_y + y);
    SDL_RenderPoint(renderer, center_x + y, center_y - x);
    SDL_RenderPoint(renderer, center_x + y, center_y + x);
    SDL_RenderPoint(renderer, center_x - y, center_y - x);
    SDL_RenderPoint(renderer, center_x - y, center_y + x);

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

struct particle {
  // Position
  sndbx::vec<2> pos;
  // Velocity
  sndbx::vec<2> vel;
  // Acceleration
  sndbx::vec<2> acc;
  // Mass
  float mass;
  // Radius
  float rad;
};

void update_particle(particle &particle, float dt) {
  particle.pos += particle.vel * dt + particle.acc * (0.5F * dt * dt);
  particle.vel += particle.acc * dt;
}

bool are_colliding(const particle &p1, const particle &p2) {
  auto diff = p1.pos - p2.pos;
  auto square_mag = diff.square_mag();

  return square_mag <= p1.rad * p1.rad || square_mag <= p2.rad * p2.rad;
}

void fix_world_collisions(particle &particle) {
  constexpr float DRAG = 0.9;
  auto x = particle.pos.x();
  auto y = particle.pos.y();
  auto r = particle.rad;

  if (x - r < WORLD_BBOX.x) {
    particle.vel.x() *= -DRAG;
    particle.vel.y() *= DRAG;
    particle.pos.x() = WORLD_BBOX.x + r;
  } else if (x + r > WORLD_BBOX.x + WORLD_BBOX.w) {
    particle.vel.x() *= -DRAG;
    particle.vel.y() *= DRAG;
    particle.pos.x() = WORLD_BBOX.x + WORLD_BBOX.w - r;
  }

  if (y - r < WORLD_BBOX.y) {
    particle.vel.y() *= -DRAG;
    particle.vel.x() *= DRAG;
    particle.pos.y() = WORLD_BBOX.y + r;
  } else if (y + r > WORLD_BBOX.y + WORLD_BBOX.h) {
    particle.vel.y() *= -DRAG;
    particle.vel.x() *= DRAG;
    particle.pos.y() = WORLD_BBOX.y + WORLD_BBOX.h - r;
  }
}

auto main() -> int {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Physics Sandbox", SCREEN_WIDTH,
                                        SCREEN_HEIGHT, SDL_WINDOW_VULKAN);

  // Check that the window was successfully created
  if (window == nullptr) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg): Using SDL is fine
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n",
                 SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

  std::array<particle, 2> particles{{
      {
          .pos = {{WORLD_BBOX.w / 2, WORLD_BBOX.h / 2}},
          .vel = {{10, 20}},
          .acc = {{0, -9.8}},
          .mass = 1,
          .rad = 5,
      },
      {
          .pos = {{WORLD_BBOX.w / 2, WORLD_BBOX.h / 2}},
          .vel = {{10, 20}},
          .acc = {{0, -9.8}},
          .mass = 1,
          .rad = 2,
      },
  }};

  bool done = false;
  auto prev_timestamp = std::chrono::steady_clock::now();
  while (!done) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }

    // Render update
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 10, 127, 255, 255);
    for (auto &particle : particles) {
      auto center = world_point_to_screen(particle.pos);
      auto radius = world_scalar_to_screen(particle.rad);
      sdl_draw_circle(renderer, center.x(), center.y(), radius);
    }
    SDL_RenderPresent(renderer);

    // Physics update
    auto now = std::chrono::steady_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::duration<float>>(
                  now - prev_timestamp)
                  .count();
    prev_timestamp = now;
    for (auto &particle : particles) {
      update_particle(particle, dt);
      fix_world_collisions(particle);
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
