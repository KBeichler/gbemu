/*
CPU
PPU
MEMORY

*/
#include <SDL2/SDL.h>
#include "gb.h"
#include "opcode.h"
#include "gb_t.h"
#include "ppu.h"





uint8_t color[4] = {0x00, 0x0F, 0xF0, 0xFF};

ppu_t Ppu;

int main (int argc, char* argv[]) 
{

    ppu_t * ppu = &Ppu;
    void *pixels;
    int pitch;
    uint32_t frame = 0;

    

    cpu_t * cpu = init_cpu();
    load_rom ( cpu, argv[0] ); 

    init_ppu(ppu, cpu);


    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Event event;
    SDL_Window *mainWindow = SDL_CreateWindow("SDL2 TEST",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, 0);    
    SDL_Renderer * mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    SDL_RenderSetLogicalSize(mainRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(mainRenderer);
    SDL_Texture * mainTexture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_RGB332 , SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    

    //SDL_Window *debugWindow = SDL_CreateWindow("SDL2 TEST",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 32*8 * 3, 32*8 * 3, 0);    
    //SDL_Renderer * debugRenderer = SDL_CreateRenderer(debugWindow, -1, 0);
    /*SDL_RenderSetLogicalSize(debugRenderer, 32*8, 32*8);
    SDL_SetRenderDrawColor(debugRenderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(debugRenderer);*/
    //SDL_Texture * debugTexture = SDL_CreateTexture(debugRenderer, SDL_PIXELFORMAT_RGB332 , SDL_TEXTUREACCESS_STREAMING, 32*8, 32*8);



    

    //MEM(LY) = 0x90;
    uint8_t fake  =0;
    uint8_t quit = 1;
    uint32_t c = 0;
    uint16_t br = 0x28;



    while (quit){

        /*printf("\nPC 0x%0x  Current OPCODE 0x%x  +1 0x%x  0x%x  0x%x\n", REG(PC),  MEM(REG(PC)), MEM(REG(PC)+1) , MEM(REG(PC)+2) ,  MEM(REG(PC)+3));
        printf("A 0x%0X  F 0x%0x  \nD 0x%0X  E 0x%0X  \nB 0x%0X  C 0x%0X\nHL 0x%x  - 0x%0x\n", cpu->reg.A, FLAG(R), cpu->reg.D, cpu->reg.E, cpu->reg.B, cpu->reg.C, REG(HL), MEM(REG(HL)) ); 
        printf("SP: 0x%x STACK: 0x%0x 0x%x  0x%x  LY 0x%x\n", REG(SP), MEM(REG(SP)), MEM(REG(SP)+1), MEM(REG(SP)+2), MEM(LY));
        printf("FLAGS: Z: %d  N: %d HC: %d CY: %d  F %x\n ", FLAG(Z), FLAG(N), FLAG(HC), FLAG(CY) , FLAG(R)  );
        printf(" IFlag 0x%0X  IEnab 0x%0X  LCDC 0x%0X  Stat 0x%0X \n", MEM(I_FLAG), MEM(I_ENABLE), MEM(LCDC), MEM(STAT));
        printf("cycle %d\n", c++);
        */

        tick_cpu(cpu);

        while(ppu->cycle < cpu->cycle){
            tick_ppu(ppu);
        }

        if (MEM(DMA) != 0x00){
            uint16_t source = MEM(DMA) << 8;
            for (uint8_t i = 0; i < 160; i++){
                MEM(0xFE00 + i) = MEM(source +i);
            }
            MEM(DMA) = 0x00;
        }



        if (ppu->newFrame == 1){
            uint8_t *frame = &ppu->framebuffer[0][0];
            ppu->newFrame = 0;
            ppu->cycle = 0;
            cpu->cycle = 0;

            //draw screen
            SDL_RenderClear(mainRenderer);
            SDL_LockTexture(mainTexture, NULL, &pixels, &pitch);

            
            uint8_t * ptr = ((uint8_t *)pixels);
            uint8_t col = 0;
            uint8_t pal[4] = {0x00, 45, 153, 0xFF};
            for (uint16_t j = 0; j < 144; j++){
                for (uint16_t i = 0; i < 160; i++){   
                        ptr[i+(j*160)] = pal[ppu->framebuffer[j][i]];
                    }
            }
           
    
            SDL_UnlockTexture(mainTexture); 
            SDL_RenderCopy(mainRenderer, mainTexture, NULL, NULL);
            SDL_RenderPresent(mainRenderer);



            SDL_Delay(10);
            }
            

        
        SDL_PollEvent(&event); 
        
        if ( (REG(PC)) ==  br)
        {
                printf("BREAK");
            }

        switch (event.type)
        {
            case SDL_QUIT:
                quit = 0 ;
                break;
                
        }






        }

      
        
    
        SDL_DestroyTexture(mainTexture);
        SDL_DestroyRenderer(mainRenderer);
        SDL_DestroyWindow(mainWindow);
        //SDL_DestroyTexture(debugTexture);
        //SDL_DestroyRenderer(debugRenderer);
        //SDL_DestroyWindow(debugWindow);
        //SDL_Quit();
    

    
    return 0;
}