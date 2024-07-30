#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int initialLength = 20;
double initialAngle = 1.5;
int lengthIncrement = 10;
int screenWidth = 800;
int screenHeight = 600;

// 函數來繪製樹形圖案
void drawTree(SDL_Renderer* renderer, int x, int y, int length, double angle, int depth) {
    if (depth == 0) {
        return;
    }

    int x2 = x + length * cos(angle);   // 計算子樹的終點坐標
    int y2 = y - length * sin(angle);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);   //set line color to green line
    SDL_RenderDrawLine(renderer, x, y, x2, y2);    // 繪製當前分支

    int newLength = length * 0.8;    // 計算子樹的長度和角度
    double newAngle = angle - 0.4;

    drawTree(renderer, x2, y2, newLength, newAngle, depth - 1);    // 遞迴繪製左子樹
    drawTree(renderer, x2, y2, newLength, newAngle + 0.8, depth - 1);     // 遞迴繪製右子樹
}

void yesNoTree(SDL_Renderer* renderer, SDL_Rect yesRect, SDL_Rect oopsRect, SDL_Texture* yes_texture, SDL_Texture* oops_texture, int startY, int startX, int depth){
    int padding = 10; // 圖像之間的間距
    int imageWidth, imageHeight;    
    SDL_QueryTexture(yes_texture, NULL, NULL, &imageWidth, &imageHeight);    // 獲取圖像的寬度和高度

    SDL_RenderCopy(renderer, yes_texture, NULL, &yesRect);
    SDL_RenderCopy(renderer, oops_texture, NULL, &oopsRect);
    SDL_RenderPresent(renderer);
}

void aDayLater(SDL_Renderer* renderer){
    SDL_Texture *adayLater_texture = IMG_LoadTexture(renderer, "./AdayLater.jpg"); 
    SDL_Rect adayLaterRect= { 0, 0, screenWidth, screenHeight };
    SDL_RenderCopy(renderer, adayLater_texture, NULL, &adayLaterRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
}

int main(int argc, char *argv[]) {
    /*basic part main function
    *
    *
    *
    *
    */
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Tree", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    int startX = screenWidth / 2;
    int startY = screenHeight - 50;
    int depth = 10;

    SDL_Texture* yes_texture = IMG_LoadTexture(renderer, "./yes.jpg");   // 載入圖像並創建纹理
    SDL_Texture* oops_texture = IMG_LoadTexture(renderer, "./oops.jpg");
    
    int imageWidth, imageHeight;
    int padding = 10; // 圖像之間的間距    
    SDL_QueryTexture(yes_texture, NULL, NULL, &imageWidth, &imageHeight);    // 獲取圖像的寬度和高度

    SDL_Rect yesRect = {30, 50, imageWidth - 300, imageHeight - 300};
    SDL_Rect oopsRect = { 30 + (imageWidth-300) + padding, 50, imageWidth-300, imageHeight-300 };
    drawTree(renderer, startX, startY, initialLength, initialAngle, depth);

    yesNoTree(renderer, yesRect, oopsRect, yes_texture, oops_texture, startX, startY, depth);
    SDL_Event event;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;  // 退出應用程序
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;  // 退出應用程序
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;    // 獲取鼠標點擊位置
                    int mouseY = event.button.y;

                    // 检查是否在"Yes"選項區域內點擊
                    if (mouseX >= yesRect.x && mouseX <= yesRect.x + yesRect.w && mouseY >= yesRect.y && mouseY <= yesRect.y + yesRect.h){
                        initialLength += lengthIncrement;   // 每次點擊YES矩形，增加樹的長度
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderClear(renderer);
                        drawTree(renderer, startX, startY, initialLength, initialAngle, depth);

                        int i = 10;
                        while(i--){    //動畫 if needed
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderClear(renderer);
                            drawTree(renderer, startX, startY, initialLength, initialAngle, depth);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(80);
                            initialLength += lengthIncrement;
                        }
                        SDL_RenderPresent(renderer);
                        SDL_Delay(1500);
                        aDayLater(renderer);
                        quit = 1;
                    }
                    else if(mouseX >= oopsRect.x && mouseX <= oopsRect.x + oopsRect.w && mouseY >= oopsRect.y && mouseY <= oopsRect.y + oopsRect.h){
                        aDayLater(renderer);
                        quit = 1;
                    }
                }
            }
        }
    }
    SDL_DestroyTexture(yes_texture);
    SDL_DestroyTexture(oops_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}