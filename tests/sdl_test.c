/*
CPU
PPU
MEMORY

*/
#include <SDL2/SDL.h>



#define SCREEN_WIDTH  160
#define SCREEN_HEIGHT 144







int main (int argc, char* argv[]) 
{
    //HELPERS
    uint8_t pal[4] = {0x00, 45, 153, 0xFF}; // pixel color pallette used
    uint8_t quit = 1;
    uint16_t br = 0;
 
    // SDL Main Windows Init
    void *pixels;
    int pitch;
    SDL_Init(SDL_INIT_VIDEO);    
    SDL_Event event;
    SDL_Window *mainWindow = SDL_CreateWindow("SDL2 TEST",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, 0);    
    SDL_Renderer * mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    SDL_RenderSetLogicalSize(mainRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(mainRenderer);
    SDL_Texture * mainTexture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_RGB332 , SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);






    while (quit){

        //draw screen
        // color from pallette for each pixel
        SDL_RenderClear(mainRenderer);
        SDL_LockTexture(mainTexture, NULL, &pixels, &pitch);
        uint8_t * ptr = ((uint8_t *)pixels);
        for (uint16_t j = 0; j < 144; j++){
            for (uint16_t i = 0; i < 160; i++){   
                    ptr[i+(j*160)] = pal[br] ;
                }
        }
        SDL_UnlockTexture(mainTexture); 
        SDL_RenderCopy(mainRenderer, mainTexture, NULL, NULL);
        SDL_RenderPresent(mainRenderer);    

        br++;
        if ( br > 3) br=0;

        uint16_t wait = 100;
        while (wait--){
            SDL_PollEvent(&event); 
            SDL_Delay(10);
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = 0 ;
                    wait = 0;
                    break;                    
            }
        }       

    }


    SDL_DestroyTexture(mainTexture);
    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();    
    return 0;
}