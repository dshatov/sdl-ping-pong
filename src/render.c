#include <stdio.h>
#include <string.h>

#include "render.h"
#include "logic.h"

//----------------------------------------------------------------------------------------------------------------------

int initRenderObjects(RenderObjects *const ro) {
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
        WORLD_WIDTH, WORLD_HEIGHT,
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

void freeRenderObjects(RenderObjects *const ro) {
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

void redraw(const RenderObjects *const ro, const GameState *const gs) {
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
