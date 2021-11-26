#include <gb.h>







SDL_Event event;
SDL_Window *mainWindow;
SDL_Renderer * mainRenderer;
SDL_Texture * mainTexture;
void *pixels;
int pitch;


void window_init()
{
    // SDL Main Window Init

    SDL_Init(SDL_INIT_VIDEO);   
    mainWindow = SDL_CreateWindow("SDL2 TEST",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 5, SCREEN_HEIGHT * 5, 0);    
    mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    SDL_RenderSetLogicalSize(mainRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(mainRenderer);
    mainTexture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_RGB555 , SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}


void window_drawFrame(uint16_t framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH])
{
    SDL_RenderClear(mainRenderer);
    SDL_LockTexture(mainTexture, NULL, &pixels, &pitch);
    uint16_t * ptr = ((uint16_t *)pixels);

    // DRAWING OF THE DATA

    for (uint16_t j = 0; j < 144; j++){
        for (uint16_t i = 0; i < 160; i++){   
                ptr[i+(j*160)] = framebuffer[j][i];
            }
    }

    // END OF DRAWIN
    SDL_UnlockTexture(mainTexture); 
    SDL_RenderCopy(mainRenderer, mainTexture, NULL, NULL);
    SDL_RenderPresent(mainRenderer);    
}

void window_close()
{
    SDL_DestroyTexture(mainTexture);
    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();    
}