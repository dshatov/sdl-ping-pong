/**
 * Модуль игровой логики.
 *
 * @author Андрей Савельев
 * @author Дмитрий Шатов
 */

#ifndef SDL_PING_PONG_LOGIC_H
#define SDL_PING_PONG_LOGIC_H

#include <stdbool.h>

#ifdef __unix__

#include <SDL2/SDL.h>

#else

#include <SDL.h>

#endif

//----------------------------------------------------------------------------------------------------------------------

#define WORLD_WIDTH 1000
#define WORLD_HEIGHT 600

//----------------------------------------------------------------------------------------------------------------------

/**
 * Состояние игры.
 */
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

//----------------------------------------------------------------------------------------------------------------------

/**
 * Инициализировать состояние игры.
 *
 * @param gs состояние игры.
 */
void initGameState(GameState *gs);

/**
 * Обновить состояние игры.
 *
 * @param dt время с момента последнего обновления (ms).
 * @param gs состояние игры.
 */

void updateGameState(Uint32 dt, GameState *gs);

#endif //SDL_PING_PONG_LOGIC_H
