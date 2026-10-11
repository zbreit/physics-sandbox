#include "SDL3/SDL.h"// IWYU pragma: export

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Physics Sandbox",
        640,
        480,
        SDL_WINDOW_OPENGL
    );

    // Check that the window was successfully created
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    bool done = false;
    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }

        // Do game logic, present a frame, etc.
    }

    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
