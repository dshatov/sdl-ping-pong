/**
 * Модуль отрисовки.
 *
 * @author Андрей Савельев
 * @author Дмитрий Шатов
 */

#ifndef SDL_PING_PONG_RENDER_H
#define SDL_PING_PONG_RENDER_H

#ifdef __unix__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#else

#include <SDL.h>
#include <SDL_ttf.h>

#endif

#include "logic.h"

//----------------------------------------------------------------------------------------------------------------------

/**
 * Объекты для отрисовки.
 */
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

/**
 * Проинициализировать графику.
 *
 * @param объекты для отрисовки.
 *
 * @return ноль в случае успеха; не ноль в случае ошибки.
 */
int initRenderObjects(RenderObjects *ro);

/**
 * Освободить ресурсы, связанные с графикой.
 *
 * @param объекты для отрисовки.
 */
void freeRenderObjects(RenderObjects *ro);

/**
 * Отрисовать состояние игры на экране.
 *
 * @param ro объекты для отрисовки.
 * @param gs состояние игры.
 */
void redraw(const RenderObjects *ro, const GameState *gs);

#endif //SDL_PING_PONG_RENDER_H
