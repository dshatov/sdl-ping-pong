/**
 * Пинг-понг.
 *
 * @author Андрей Савельев
 * @author Дмитрий Шатов
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __unix__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#else

#include <SDL.h>
#include <SDL_ttf.h>

#endif

//----------------------------------------------------------------------------------------------------------------------

// TODO: вынести числовые константы из функций в именованные константы.

static const int WINDOW_WIDTH = 1000;
static const int WINDOW_HEIGHT = 600;

//----------------------------------------------------------------------------------------------------------------------

typedef struct {
    int c1;
    int c2;
    int bx;
    int by;
    bool l;
    SDL_Rect b;
    SDL_Rect p1;
    SDL_Rect p2;
} GameState;

typedef struct {
    SDL_Window *window;
    SDL_Rect t;
    TTF_Font *font;
    SDL_Color color;
    SDL_Renderer *renderer;
    SDL_Texture *backgroundTexture;
    SDL_Texture *whiteTexture;
} RenderObjects;

//----------------------------------------------------------------------------------------------------------------------

// TODO: Документация к методу.

static void redraw(const RenderObjects *const ro, const GameState *const gs) {
    SDL_RenderClear(ro->renderer);
    SDL_RenderCopy(ro->renderer, ro->backgroundTexture, NULL, NULL);
    SDL_RenderCopy(ro->renderer, ro->whiteTexture, NULL, &gs->p1);
    SDL_RenderCopy(ro->renderer, ro->whiteTexture, NULL, &gs->p2);
    SDL_RenderCopy(ro->renderer, ro->whiteTexture, NULL, &gs->b);

    char s[32];
    snprintf(s, 32, "%d   %d", gs->c1, gs->c2);
    SDL_Surface *score = TTF_RenderText_Solid(ro->font, s, ro->color);
    SDL_Texture *score2 = SDL_CreateTextureFromSurface(ro->renderer, score);
    SDL_RenderCopy(ro->renderer, score2, NULL, &ro->t);

    SDL_RenderPresent(ro->renderer);

    SDL_DestroyTexture(score2);
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Проинициализировать графику.
 * 
 * @return ноль в случае успеха; не ноль в случае ошибки.
 */
static int initRenderObjects(RenderObjects *const ro) {
    memset(ro, 0, sizeof(*ro));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    ro->window = SDL_CreateWindow(
        "PING PONG",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (NULL == ro->window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    ro->renderer = SDL_CreateRenderer(ro->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (NULL == ro->renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface *surface;

    surface = SDL_LoadBMP("./background.bmp");
    if (NULL == surface) {
        printf("Background surface could not be loaded! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    ro->backgroundTexture = SDL_CreateTextureFromSurface(ro->renderer, surface);
    SDL_FreeSurface(surface);
    if (NULL == ro->backgroundTexture) {
        printf("Background texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    surface = SDL_LoadBMP("./it.bmp");
    if (NULL == surface) {
        printf("White surface could not be loaded! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    ro->whiteTexture = SDL_CreateTextureFromSurface(ro->renderer, surface);
    SDL_FreeSurface(surface);
    if (NULL == ro->whiteTexture) {
        printf("White texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    ro->font = TTF_OpenFont("./16984.ttf", 12);
    if (NULL == ro->font) {
        printf("Font could not be opened! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    ro->color = (SDL_Color) {.r=225, .g=225, .b=225};
    ro->t = (SDL_Rect) {.x=475, .y=40, .w=50, .h=40};

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Освободить ресурсы, связанные с графикой.
 */
static void freeRenderObjects(RenderObjects *const ro) {
    if (ro->font) {
        TTF_CloseFont(ro->font);
    }
    if (ro->backgroundTexture) {
        SDL_DestroyTexture(ro->backgroundTexture);
    }
    if (ro->whiteTexture) {
        SDL_DestroyTexture(ro->whiteTexture);
    }
    if (ro->renderer) {
        SDL_DestroyRenderer(ro->renderer);
    }
    if (ro->window) {
        SDL_DestroyWindow(ro->window);
    }

    TTF_Quit();
    SDL_Quit();
}

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

static void initGameState(GameState *const gs) {
    *gs = (GameState) {
        .p1 = (SDL_Rect) {.x=0, .y=10, .w=20, .h=200},
        .p2 = (SDL_Rect) {.x=980, .y=10, .w=20, .h=200},
        .b = (SDL_Rect) {.x=490, .y=290, .w=20, .h=20},
        .c1 = 0, .c2 = 0,
        .bx = 4, .by = 4,
        .l = true
    };
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Переместить шарик.
 *
 * @param dt время с момента последнего обновления (ms).
 */
static void moveBall(const Uint32 dt, GameState *const gs) {
    // TODO: выполнять перемещение пропорционально dt.

    gs->b.x += gs->bx;
    gs->b.y += gs->by;
    if (gs->b.y > 580) {
        gs->b.y = 580;
        gs->by = -gs->by;
    } else if (gs->b.y < 0) {
        gs->b.y = 0;
        gs->by = -gs->by;
    }
    if (gs->b.x <= 20 && gs->b.y > gs->p1.y && gs->b.y < gs->p1.y + gs->p1.h) {
        if (gs->l) {
            gs->l = false;
            gs->bx = -gs->bx;
        }
    } else if (gs->b.x >= 960 && gs->b.y > gs->p2.y && gs->b.y < gs->p2.y + gs->p2.h) {
        if (gs->l) {
            gs->l = false;
            gs->bx = -gs->bx;
        }
    } else {
        gs->l = true;
        if (gs->b.x < -20) {
            gs->b.x = 490;
            gs->b.y = 290;
            gs->bx = -gs->bx;
            gs->c1++;
        } else if (gs->b.x > WINDOW_WIDTH) {
            gs->b.x = 490;
            gs->b.y = 290;
            gs->bx = -gs->bx;
            gs->c2++;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Переместить игроков.
 *
 * @param dt время с момента последнего обновления (ms).
 */
static void movePlayers(const Uint32 dt, GameState *const gs) {
    const Uint8 *const keysDown = SDL_GetKeyboardState(NULL);

    if (keysDown[SDL_SCANCODE_W]) {
        gs->p1.y -= dt * 2 / 7;
        if (gs->p1.y < 0) {
            gs->p1.y = 0;
        }
    } else if (keysDown[SDL_SCANCODE_S]) {
        gs->p1.y += dt * 2 / 7;
        if (gs->p1.y + gs->p1.h > WINDOW_HEIGHT) {
            gs->p1.y = WINDOW_HEIGHT - gs->p1.h;
        }
    }

    if (keysDown[SDL_SCANCODE_UP]) {
        gs->p2.y -= dt * 2 / 7;
        if (gs->p2.y < 0) {
            gs->p2.y = 0;
        }
    } else if (keysDown[SDL_SCANCODE_DOWN]) {
        gs->p2.y += dt * 2 / 7;
        if (gs->p2.y + gs->p2.h > WINDOW_HEIGHT) {
            gs->p2.y = WINDOW_HEIGHT - gs->p2.h;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Обновить состояние игры.
 *
 * @param dt время с момента последнего обновления (ms).
 */
static void updateGameState(const Uint32 dt, GameState *const gs) {
    movePlayers(dt, gs);
    moveBall(dt, gs);
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
