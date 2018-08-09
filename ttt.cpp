#include <SDL2/SDL.h>
#include <iostream>
#include <array>
#include <math.h>

#include "./tttp.hpp"

int filter(void* argumente, SDL_Event* event){
  return !(event->type == SDL_MOUSEMOTION);
}

void drawX(SDL_Renderer* renner, int n){
    int x = (n % 3)*200 + 125;
    int y = (n/3)*200 + 125;
    SDL_RenderDrawLine(renner, x,y,x+150,y+150);
    SDL_RenderDrawLine(renner, x,y+150,x+150,y);
}

void drawCircle(SDL_Renderer* renner, int n){
    int x = (n % 3)*200 + 200;
    int y = (n/3)*200 + 200;
    
    for(double i = -1.0; i < 1.0; i+=0.01){
        SDL_RenderDrawPoint(renner, x+i*75, y+75*(sqrt(1-i*i)));
        SDL_RenderDrawPoint(renner, x+i*75, y-75*(sqrt(1-i*i)));
    }
}

bool playerMove(SDL_Renderer* renner, SDL_Event event, std::array<int, 9>* field, int playerNumber){
    while(true){
        SDL_SetRenderDrawColor(renner, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renner);
        
        //draw field
        SDL_SetRenderDrawColor(renner, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renner, 100,300,700,300);
        SDL_RenderDrawLine(renner, 100,500,700,500);
        SDL_RenderDrawLine(renner, 300,100,300,700);
        SDL_RenderDrawLine(renner, 500,100,500,700);
        for(int i = 0; i < 9; i++){
            switch((*field)[i]){
                case 0: break;
                case 1: drawX(renner, i); break;
                case 2: drawCircle(renner, i); break;
                default: std::cout << "error" << std::endl;
            }
        }
        
        if(SDL_PollEvent(&event)){
          switch (event.type) {
            case SDL_QUIT: return false;
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                if(x > 100 && x < 700){
                    if(y > 100 && y < 700){
                        x = (x-100)/200;
                        y = (y-100)/200;
                        if(!(*field)[x+3*y]) {(*field)[x+3*y] = playerNumber;
                        return true;}
                    }
                }
                break;
            default: break;
          }
        }
        
        SDL_RenderPresent(renner);
        SDL_Delay(1);
        
        if(didSomebodyWin(*field)){
            std::cout << "Player " << didSomebodyWin(*(field)) << "Wins!" << std::endl; return false;
        }
    }
    return true;
}

int main(){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){std::cout << "error: " << SDL_GetError() << std::endl; return 1;}
    SDL_Window *win = SDL_CreateWindow("Titel",100,200,800,800,SDL_WINDOW_SHOWN);
    if(win == nullptr){std::cout << "winerror: " << SDL_GetError() << std::endl; SDL_Quit(); return 1;}
    SDL_Renderer *renner = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renner == nullptr){SDL_DestroyWindow(win); std::cout << "renderererror: " << SDL_GetError() << std::endl; SDL_Quit(); return 1;}
    SDL_Event event;
    
    SDL_SetEventFilter(filter, NULL);
    
    bool pturn = true;
    bool playing = true;
    std::array<int, 9> field = {0,0,0,0,0,0,0,0,0};
    
    while(playing){
        if(didSomebodyWin(field)){
            std::cout << "Player " << didSomebodyWin(field) << "Wins!" << std::endl;
            break;
        }
        if(isItATie(field)){
            field = {0,0,0,0,0,0,0,0,0};
        }
        
        if(pturn){
            if(SDL_PollEvent(&event)){
                switch (event.type) {
                    case SDL_QUIT: playing = false; break;
                    case SDL_MOUSEBUTTONDOWN:
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        if(x > 100 && x < 700){
                            if(y > 100 && y < 700){
                                x = (x-100)/200;
                                y = (y-100)/200;
                                if(field[x+3*y] == 0) {field[x+3*y] = 1;}
                                pturn = !pturn;
                            }
                        }
                    default: break;
                }
            }
        }else{
            field[getAiMove(field, 2, 1)] = 2;
            pturn = !pturn;
        }

        
        SDL_SetRenderDrawColor(renner, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renner);
        
        //draw field
        SDL_SetRenderDrawColor(renner, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renner, 100,300,700,300);
        SDL_RenderDrawLine(renner, 100,500,700,500);
        SDL_RenderDrawLine(renner, 300,100,300,700);
        SDL_RenderDrawLine(renner, 500,100,500,700);
        for(int i = 0; i < 9; i++){
            switch(field[i]){
                case 0: break;
                case 1: drawX(renner, i); break;
                case 2: drawCircle(renner, i); break;
                default: std::cout << "error" << std::endl;
            }
        }
        
        
        SDL_RenderPresent(renner);
        SDL_Delay(1);
    }
    
    SDL_DestroyRenderer(renner);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
