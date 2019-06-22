/**
 * Пинг-понг.
 *
 * @author Андрей Савельев
 * @author Дмитрий Шатов
 */

#include <stdlib.h>
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


// TODO: избавиться от глобальных переменных.

static int c1 = 0;
static int c2 = 0;
static SDL_Window *window;
static SDL_Rect t;
static SDL_Rect b;
static SDL_Rect p1;
static SDL_Rect p2;
static TTF_Font *font;
static SDL_Color color;
static SDL_Surface *score;
static SDL_Renderer *render;
static SDL_Texture *background;
static SDL_Texture *player_1;
static SDL_Texture *player_2;
static SDL_Texture *ball;
static SDL_Texture *score2;
static int bx = 4;
static int by = 4;
static bool l = true;

//----------------------------------------------------------------------------------------------------------------------

// TODO: Документация к методу.

static void redraw() {
    SDL_RenderClear(render);
    SDL_RenderCopy(render, background, NULL, NULL);
    SDL_RenderCopy(render, player_1, NULL, &p1);
    SDL_RenderCopy(render, player_2, NULL, &p2);
    SDL_RenderCopy(render, ball, NULL, &b);

    char s[32];
    snprintf(s, 32, "%d   %d", c1, c2);
    score = TTF_RenderText_Solid(font, s, color);
    score2 = SDL_CreateTextureFromSurface(render, score);
    SDL_RenderCopy(render, score2, NULL, &t);

    SDL_RenderPresent(render);
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Проинициализировать графику.
 * 
 * @return ноль в случае успеха; не ноль в случае ошибки.
 */
static int initGraphic() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "PING PONG",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (NULL == window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // TODO: обрабатывать другие возможные ошибки инициализации.

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    background = SDL_CreateTextureFromSurface(render, SDL_LoadBMP("./background.bmp"));
    player_1 = SDL_CreateTextureFromSurface(render, SDL_LoadBMP("./it.bmp"));
    player_2 = SDL_CreateTextureFromSurface(render, SDL_LoadBMP("./it.bmp"));
    ball = SDL_CreateTextureFromSurface(render, SDL_LoadBMP("./it.bmp"));
    TTF_Init();
    font = TTF_OpenFont("./16984.ttf", 12);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Обработать все накопившиеся события SDL.
 * 
 * @param shouldExit true, если следует завершить работу программы.
 */
static void pollSdlEvents(bool *shouldExit) {
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

static void initGame() {
    p1 = (SDL_Rect) {.x=0, .y=10, .w=20, .h=200};
    p2 = (SDL_Rect) {.x=980, .y=10, .w=20, .h=200};
    b = (SDL_Rect) {.x=490, .y=290, .w=20, .h=20};
    t = (SDL_Rect) {.x=475, .y=40, .w=50, .h=40};
    color = (SDL_Color) {.r=225, .g=225, .b=225};
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Переместить шарик.
 *
 * @param dt время с момента последнего обновления (ms).
 */
static void moveBall(Uint32 dt) {
    // TODO: выполнять перемещение пропорционально dt.

    b.x += bx;
    b.y += by;
    if (b.y > 580) {
        b.y = 580;
        by = -by;
    } else if (b.y < 0) {
        b.y = 0;
        by = -by;
    }
    if (b.x <= 20 && b.y > p1.y && b.y < p1.y + p1.h) {
        if (l) {
            l = false;
            bx = -bx;
        }
    } else if (b.x >= 960 && b.y > p2.y && b.y < p2.y + p2.h) {
        if (l) {
            l = false;
            bx = -bx;
        }
    } else {
        l = true;
        if (b.x < -20) {
            b.x = 490;
            b.y = 290;
            bx = -bx;
            c1++;
        } else if (b.x > WINDOW_WIDTH) {
            b.x = 490;
            b.y = 290;
            bx = -bx;
            c2++;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Переместить игроков.
 *
 * @param dt время с момента последнего обновления (ms).
 */
static void movePlayers(Uint32 dt) {
    const Uint8 *const keysDown = SDL_GetKeyboardState(NULL);

    if (keysDown[SDL_SCANCODE_W]) {
        p1.y -= dt * 2 / 7;
        if (p1.y < 0) {
            p1.y = 0;
        }
    } else if (keysDown[SDL_SCANCODE_S]) {
        p1.y += dt * 2 / 7;
        if (p1.y + p1.h > WINDOW_HEIGHT) {
            p1.y = WINDOW_HEIGHT - p1.h;
        }
    }

    if (keysDown[SDL_SCANCODE_UP]) {
        p2.y -= dt * 2 / 7;
        if (p2.y < 0) {
            p2.y = 0;
        }
    } else if (keysDown[SDL_SCANCODE_DOWN]) {
        p2.y += dt * 2 / 7;
        if (p2.y + p2.h > WINDOW_HEIGHT) {
            p2.y = WINDOW_HEIGHT - p2.h;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Обновить состояние игры.
 *
 * @param dt время с момента последнего обновления (ms).
 */
static void update(Uint32 dt) {
    movePlayers(dt);
    moveBall(dt);
}

//----------------------------------------------------------------------------------------------------------------------

// TODO: Документация к методу.

static void gameLoop() {

    Uint32 time = SDL_GetTicks();
    Uint32 dt = 1;

    while (true) {
        bool shouldExit;
        pollSdlEvents(&shouldExit);
        if (shouldExit) {
            SDL_DestroyWindow(window);
            // TODO: освобождать прочие ресурсы перед завершением.
            return;
        }
        redraw();
        update(dt);

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
int main(int argc, char *argv[]) {
    if (initGraphic()) {
        return 1;
    }
    initGame();

    gameLoop();
    return 0;
}
