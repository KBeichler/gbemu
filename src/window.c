#include <gb.h>

/* 
__     __         _       _     _
\ \   / /_ _ _ __(_) __ _| |__ | | ___  ___
 \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __|
  \ V / (_| | |  | | (_| | |_) | |  __/\__ \
   \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/
 */
// global Variables needed for SDL handling
SDL_Event event;
SDL_Window *mainWindow;
SDL_Renderer * mainRenderer;
SDL_Texture * mainTexture;
void *pixels;
int pitch;
uint16_t frameDuration; 

void window_init()
{
    frameDuration = (1000 / 60);
    // SDL Main Window Init

    SDL_Init(SDL_INIT_VIDEO);   
    //create windows with *5 pixel sclaing
    mainWindow = SDL_CreateWindow("SDL2 TEST",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 5, SCREEN_HEIGHT * 5, 0);    
    // create renderer for current window and set render size
    mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    SDL_RenderSetLogicalSize(mainRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(mainRenderer);
    //create texture to display pixels
    // use RGB555 color format
    mainTexture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_RGB888 , SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}

//draws current framebuffer on the texture
void window_drawFrame(uint32_t framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH])
{
    static uint32_t lastTick;

    while ( (SDL_GetTicks() - lastTick) < frameDuration );
    lastTick = SDL_GetTicks();
    SDL_RenderClear(mainRenderer);
    SDL_LockTexture(mainTexture, NULL, &pixels, &pitch);
    uint32_t * ptr = ((uint32_t *)pixels);

    // DRAWING OF THE DATA
    // just takes the framebuffer as it was written by the ppu
    // for colors this probably needs some ajustmend
    for (uint16_t j = 0; j < 144; j++){
        for (uint16_t i = 0; i < 160; i++){   
                ptr[i+(j*160)] = framebuffer[j][i];
            }
    }

    // END OF DRAWING
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

// polls events from user io
void window_getIO(uint8_t * io)
{
        SDL_PollEvent(&event); 
        switch (event.type)
        {
            case SDL_QUIT:
                *io = 0 ;

                break;                    
        }

}