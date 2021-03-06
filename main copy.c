#include <gb.h>


extern cpu_t cpu;
extern ppu_t ppu;
extern mem_t mem;

char * testfile = "/media/storage/Documents/DEV/projects/gbemu/builds/cpu_instrs1.gb";
int main (int argc, char* argv[]) 
{
    cpu_init();
    mem_init();
    ppu_init();
    printf(" argc %d\n", argc);
    if (argc < 2)
    {
        if (mem_loadRom(testfile)) return 0;
    }
    else 
    {
        if (mem_loadRom(argv[1])) return 0;
    }

    
    printf("Hello GB \n");


    //HELPERS

    uint8_t quit = 1;


    // SDL Main Windows Init
    void *pixels;
    int pitch;
    SDL_Init(SDL_INIT_VIDEO);    
    SDL_Event event;
    SDL_Window *mainWindow = SDL_CreateWindow("SDL2 TEST",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 5, SCREEN_HEIGHT * 5, 0);    
    SDL_Renderer * mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    SDL_RenderSetLogicalSize(mainRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(mainRenderer);
    SDL_Texture * mainTexture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_RGB555 , SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);






    while (quit){

        cpu_tick();
        while (ppu.clock < cpu.clock)
        {
            ppu_tick();
        }

        if (ppu.newFrame)
        {
            ppu.newFrame = 0;
            //draw screen
            // color from pallette for each pixel
            SDL_RenderClear(mainRenderer);
            SDL_LockTexture(mainTexture, NULL, &pixels, &pitch);
            uint16_t * ptr = ((uint16_t *)pixels);

            // DRAWING OF THE DATA

            for (uint16_t j = 0; j < 144; j++){
                for (uint16_t i = 0; i < 160; i++){   
                        ptr[i+(j*160)] = ppu.framebuffer[j][i];
                    }
            }
            
            // END OF DRAWIN
            SDL_UnlockTexture(mainTexture); 
            SDL_RenderCopy(mainRenderer, mainTexture, NULL, NULL);
            SDL_RenderPresent(mainRenderer);    
        
    
        }

        
        SDL_PollEvent(&event); 
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
    SDL_Quit();    

    mem_close();
 
    return 0;
    

} 

/*
uint8_t color[4] = {0x00, 0x0F, 0xF0, 0xFF};
extern cpu_t cpu;
extern ppu_t ppu;
extern mem_t mem;

int main (int argc, char* argv[]) 
{

    void *pixels;
    int pitch;
    uint32_t frame = 0;

    ;

    cpu_init();
    mem_init();
    
    
    mem_loadRom ( "../tests/testfiles/cpu_instrs.gb" ); 

    ppu_init();

    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Event event;
    SDL_Window *mainWindow = SDL_CreateWindow("SDL2 TEST",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160 * 3, 144 * 3, 0);    
    SDL_Renderer * mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    SDL_RenderSetLogicalSize(mainRenderer, 160, 144);
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(mainRenderer);
    SDL_Texture * mainTexture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_RGB332 , SDL_TEXTUREACCESS_STREAMING, 160, 144);
    

    //SDL_Window *debugWindow = SDL_CreateWindow("SDL2 TEST",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 32*8 * 3, 32*8 * 3, 0);    
    //SDL_Renderer * debugRenderer = SDL_CreateRenderer(debugWindow, -1, 0);
    //SDL_RenderSetLogicalSize(debugRenderer, 32*8, 32*8);
    //SDL_SetRenderDrawColor(debugRenderer, 0x00, 0x00, 0x00, 255);
    //SDL_RenderClear(debugRenderer);
    //SDL_Texture * debugTexture = SDL_CreateTexture(debugRenderer, SDL_PIXELFORMAT_RGB332 , SDL_TEXTUREACCESS_STREAMING, 32*8, 32*8);



    uint8_t quit = 1;

    while (quit){

        //printf("\nPC 0x%0x  Current OPCODE 0x%x  +1 0x%x  0x%x  0x%x\n", REG(PC),  MEM(REG(PC)), MEM(REG(PC)+1) , MEM(REG(PC)+2) ,  MEM(REG(PC)+3));
        //printf("A 0x%0X  F 0x%0x  \nD 0x%0X  E 0x%0X  \nB 0x%0X  C 0x%0X\nHL 0x%x  - 0x%0x\n", cpu->reg.A, FLAG(R), cpu->reg.D, cpu->reg.E, cpu->reg.B, cpu->reg.C, REG(HL), MEM(REG(HL)) ); 
        //printf("SP: 0x%x STACK: 0x%0x 0x%x  0x%x  LY 0x%x\n", REG(SP), MEM(REG(SP)), MEM(REG(SP)+1), MEM(REG(SP)+2), MEM(LY));
        //printf("FLAGS: Z: %d  N: %d HC: %d CY: %d  F %x\n ", FLAG(Z), FLAG(N), FLAG(HC), FLAG(CY) , FLAG(R)  );
       // printf(" IFlag 0x%0X  IEnab 0x%0X  LCDC 0x%0X  Stat 0x%0X \n", MEM(I_FLAG), MEM(I_ENABLE), MEM(LCDC), MEM(STAT));
        //printf("cycle %d\n", c++);
        

        cpu_tick();
        if (cpu.clock % 0xFFFFFFFF == 0) printf("Current clock %d\n", cpu.clock);

        while(ppu.clock < cpu.clock){
            ppu_tick();
        }

        if (mem.DMA != 0x00){
            uint16_t source = mem.DMA << 8;
            for (uint8_t i = 0; i < 160; i++){
                mem_write( 0xFE00 + i , mem_read(source + i) );
            }
            mem.DMA = 0x00;
        }



        if (ppu.newFrame == 1){
            uint8_t *frame = ppu.framebuffer[0][0];
            ppu.newFrame = 0;


            //draw screen
            SDL_RenderClear(mainRenderer);
            SDL_LockTexture(mainTexture, NULL, &pixels, &pitch);

            
            uint8_t * ptr = ((uint8_t *)pixels);
            uint8_t col = 0;
            uint8_t pal[4] = {0x00, 45, 153, 0xFF};
            for (uint16_t j = 0; j < 144; j++){
                for (uint16_t i = 0; i < 160; i++){   
                        ptr[i+(j*160)] = pal[ppu.framebuffer[j][i]];
                    }
            }
           
    
            SDL_UnlockTexture(mainTexture); 
            SDL_RenderCopy(mainRenderer, mainTexture, NULL, NULL);
            SDL_RenderPresent(mainRenderer);



            SDL_Delay(1);
            
            }
            

        
        SDL_PollEvent(&event); 


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
*/










void gb_fakepowerup()
{
    cpu.AF = 0x01B0;
    cpu.PC = 0x100;
    cpu.BC = 0x0013;
    cpu.DE = 0x00D8;
    cpu.HL = 0x014D;
    cpu.SP = 0xFFFE;
    mem_write(0xFF05, 0x00)   ; 
    mem_write(0xFF06, 0x00)   ; 
    mem_write(0xFF07, 0x00)   ; 
    mem_write(0xFF10, 0x80)   ; 
    mem_write(0xFF11, 0xBF)   ; 
    mem_write(0xFF12, 0xF3)   ; 
    mem_write(0xFF14, 0xBF)   ; 
    mem_write(0xFF16, 0x3F)   ; 
    mem_write(0xFF17, 0x00)   ; 
    mem_write(0xFF19, 0xBF)   ; 
    mem_write(0xFF1A, 0x7F)   ; 
    mem_write(0xFF1B, 0xFF)   ;  
    mem_write(0xFF1C, 0x9F)   ; 
    mem_write(0xFF1E, 0xBF)   ; 
    mem_write(0xFF20, 0xFF)   ; 
    mem_write(0xFF21, 0x00)   ; 
    mem_write(0xFF22, 0x00)   ; 
    mem_write(0xFF23, 0xBF)   ; 
    mem_write(0xFF24, 0x77)   ; 
    mem_write(0xFF25, 0xF3)   ; 
    mem_write(0xFF26, 0xF1)   ;
    mem_write(0xFF40, 0x91)   ; 
    mem_write(0xFF42, 0x00)   ; 
    mem_write(0xFF43, 0x00)   ; 
    mem_write(0xFF45, 0x00)   ; 
    mem_write(0xFF47, 0xFC)   ; 
    mem_write(0xFF48, 0xFF)   ; 
    mem_write(0xFF49, 0xFF)   ; 
    mem_write(0xFF4A, 0x00)   ; 
    mem_write(0xFF4B, 0x00)   ; 
    mem_write(0xFFFF, 0x00)   ; 
}