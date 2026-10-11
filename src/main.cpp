#include "SDL3/SDL.h" // IWYU pragma: export

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Physics Sandbox", WIDTH, HEIGHT, SDL_WINDOW_VULKAN);

    // Check that the window was successfully created
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

  bool done = false;
  uint8_t green = 255;
  while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                done = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, green--, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
