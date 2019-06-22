/**
 * Главный модуль.
 *
 * @author Андрей Савельев
 * @author Дмитрий Шатов
 */

#include "logic.h"
#include "render.h"

//----------------------------------------------------------------------------------------------------------------------

/**
 * Обработать все накопившиеся события SDL.
 * 
 * @param shouldExit true, если следует завершить работу программы.
 */
static void pollSdlEvents(bool *const shouldExit) {
    SDL_Event ev;

    *shouldExit = false;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_QUIT: { // завершать работу, если была попытка закрыть окно.
                *shouldExit = true;
                break;
            }
            case SDL_KEYDOWN: {
                if (SDLK_ESCAPE == ev.key.keysym.sym) { // завершать работу, если был нажат ESCAPE.
                    *shouldExit = true;
                }
                break;
            }
            default: {
                break;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

// TODO: Документация к методу.

static void gameLoop(const RenderObjects *const ro, GameState *const gs) {

    Uint32 time = SDL_GetTicks();
    Uint32 dt = 1;

    while (true) {
        bool shouldExit;
        pollSdlEvents(&shouldExit);
        if (shouldExit) {
            return;
        }
        redraw(ro, gs);
        updateGameState(dt, gs);

        Uint32 nextTime = SDL_GetTicks();
        dt = nextTime - time;
        time = nextTime;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Точка входа.
 *
 * @param argc количество аргументов.
 * @param argv агрументы.
 *
 * @return код завершения процесса.
 */
int main(const int argc, char **const args) {
    RenderObjects ro;
    if (initRenderObjects(&ro)) {
        freeRenderObjects(&ro);
        return 1;
    }

    GameState gs;
    initGameState(&gs);

    gameLoop(&ro, &gs);

    freeRenderObjects(&ro);

    return 0;
}
