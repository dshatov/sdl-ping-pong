#include "logic.h"

// TODO: вынести числовые константы из функций в именованные константы.

//----------------------------------------------------------------------------------------------------------------------

/**
 * Переместить игроков.
 *
 * @param dt время с момента последнего обновления (ms).
 */
static void movePlayers(Uint32 dt, GameState *gs);

/**
 * Переместить шарик.
 *
 * @param dt время с момента последнего обновления (ms).
 */
static void moveBall(Uint32 dt, GameState *gs);

//----------------------------------------------------------------------------------------------------------------------

void initGameState(GameState *const gs) {
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

void updateGameState(const Uint32 dt, GameState *const gs) {
    movePlayers(dt, gs);
    moveBall(dt, gs);
}

//----------------------------------------------------------------------------------------------------------------------

static void movePlayers(const Uint32 dt, GameState *const gs) {
    const Uint8 *const keysDown = SDL_GetKeyboardState(NULL);

    if (keysDown[SDL_SCANCODE_W]) {
        gs->p1.y -= dt * 2 / 7;
        if (gs->p1.y < 0) {
            gs->p1.y = 0;
        }
    } else if (keysDown[SDL_SCANCODE_S]) {
        gs->p1.y += dt * 2 / 7;
        if (gs->p1.y + gs->p1.h > WORLD_HEIGHT) {
            gs->p1.y = WORLD_HEIGHT - gs->p1.h;
        }
    }

    if (keysDown[SDL_SCANCODE_UP]) {
        gs->p2.y -= dt * 2 / 7;
        if (gs->p2.y < 0) {
            gs->p2.y = 0;
        }
    } else if (keysDown[SDL_SCANCODE_DOWN]) {
        gs->p2.y += dt * 2 / 7;
        if (gs->p2.y + gs->p2.h > WORLD_HEIGHT) {
            gs->p2.y = WORLD_HEIGHT - gs->p2.h;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

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
        } else if (gs->b.x > WORLD_WIDTH) {
            gs->b.x = 490;
            gs->b.y = 290;
            gs->bx = -gs->bx;
            gs->c2++;
        }
    }
}
