#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#define A_DAY_HOUR 24
#define NAME_SIZE 29
#define TABLE_SIZE 1000
#define DELETED_NODE (struct event_content*)(0xFFFFFFFFFFFFUL)
int today,this_year;
struct event_content *hash_table[TABLE_SIZE];

int initialLength = 20;
double initialAngle = 1.5;
int lengthIncrement = 10;
int screenWidth = 800;
int screenHeight = 600;


#define RED "\033[0;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define WHITE "\033[1;37m"

int return_today(){
    return today;
}

typedef struct event_date{
    int month;
    int date;
    int event_num;
    struct event_date *next; //point to next node
    struct event_content *content; //point to event content(s)
}Event_date;

typedef struct event_content{
    char *name; //name of event
    char *place;
    char *others;
    int start_time; //unit: hour
    int end_time; //unit: hour
    struct event_content *next; //point to next node
}Event_content;

Event_date *date_head = NULL;

Event_content *find_today(Event_date *date_head,int month,int date);
Event_date *event_date_insert(int month, int date, int command);
void event_content_insert(int month, int date, char *name, int start_time, int end_time, char* place, char* others);
Event_date *delete_date(int month, int date);
Event_content *delete_content(Event_date *cur_date, char *name);

void daily_event(int start_month,int end_month,int start_date,int end_date,char* name,int start_time,int end_time,char* place,char* others);

Event_date *find_current_date(int start_month, int start_date);

void print_event_date_list();

void print_event_content_list(Event_content *list);

void remove_enter(char *sentence);

char ask_event_length();

int check_if_already_have_event(Event_date *date_head, int month, int date, int start_time,int end_time);

int isleap(int year);

int get_content_from_file(Event_date **ptr);

int write_content_on_file(Event_date *ptr);

int find_weekday(int year,int month,int date);

int to_new_day(int curr_date);

int to_new_month(int date);

int search_if_the_time_have_activity(struct event_content *list,int start_time, int end_time, int command);

void print_calendar(int year,int month,int date);

void print_week(int year,int month,int date);

void print_date(int year,int month,int date);

void long_term_event(char selection, int month, int date, int start_time, int end_time,  char *name, char *place, char* others);

void game_1a2b(int *points_of_master);

void search_if_have_activity_on_the_date(Event_date *date_head, int month, int date, int time);

void search_all_day_free_time(Event_date *ptr,int month, int date);

void search_scheduled_time_through_activity(Event_date *ptr, char *activity_name,int month,int date);

unsigned int hash(char *name);

void init_hash_table();

void print_table();

bool hash_table_insert(struct event_content *p);

Event_content *hash_table_lookup(char *name);

Event_content *hash_table_delete(char *name);

Event_content *find_day(Event_date *ptr,int month,int date);

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
    char bad_bad;
    int normal_month_day[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    int leap_month_day[12]={31,29,31,30,31,30,31,31,30,31,30,31};
    char user_name[50];
    int points_of_master = 0;
    int action;
    int working_hour;
    int delete_month, delete_date;
    char delete_name[NAME_SIZE];
    init_hash_table();
    //initialize
    
    printf("\n------------------------------------------------------------------\n");
    printf("Hello master, what should I call you.: ");
    scanf("%s", user_name);

    int year,month,date;
    printf("\n");
    printf("Hello %s, please enter when do you want to start your calender(e.g. 2025 05 16): ", user_name);
    scanf("%d %d %d",&year,&month,&date);
    printf("\n");
    while(1){
        if(month > 12 || month <= 0){
            printf("ERROR MONTH DATA\n");
            printf("Please enter month data.\n");
            scanf("%d",&month);
            continue;
        }
        if(month == 2 && isleap(year)==1){
            if(date > 29 || date <= 0){
                printf("ERROR DATE DATA\n");
                printf("Please enter date data.\n");
                scanf("%d",&date);
                continue;
            }
        }else{
            if(date > normal_month_day[month-1] || date <= 0){
                printf("ERROR DATE DATA\n");
                printf("Please enter date data.\n");
                scanf("%d",&date);
                continue;
            }
        }
        break;
    }
    this_year=year+1;
    print_calendar(year,month,date);
    today = 10000 * year + 100 * month + date;

    while(1){
        printf("Hello, %s ,What kind of action do you want to do:\n", user_name);
        printf(" [1] enter a new event\n [2] search for an event\n [3] fast search for an event\n [4] delete an event\n [5] print out the schedule\n [6] terminate this day\n [7] terminate this month\n ");
        scanf("%d", &action);
    

        if(action == 1){//[1]enter a new event
           printf("Yeah, %s, let's start for buliding an event.\n", user_name);
            char name[30] = {0}, place[30] = {0}, others[30] = {0},selection;
            int start_month, end_month, start_date, end_date;
            printf("Please enter your event's name(limited in 28 words): ");
            scanf(" %29s", name);
            if(name[(signed)strlen(name) - 1] == '\n'){
                name[(signed)strlen(name) - 1] == '\0';
            }
            printf("Please enter the place the event takes place (limited in 28 words): ");
            scanf(" %29s", place);
            printf("Please enter the things you want to memo for the event (limited in 28 words): ");
            scanf(" %29s", others);
            int start_time, end_time;
            bool is_whole_day;
            char temp[10];
            while(1){
                printf("Is it a whole day function (Y or N): ");
                scanf(" %8s", temp);
                if(strcmp(temp, "Y") == 0){
                    start_time = 0000;
                    end_time = 2400;
                    break;
                }
                else if(strcmp(temp, "N") == 0){
                    printf("What is the start time of the event (using 24-hour clock, e.g.1500): ");
                    scanf("%d", &start_time);
                    printf("What is the end time of the event (using 24-hour clock, e.g.1500): ");
                    scanf("%d", &end_time);
                    break;
                }
                else printf("Error in setting event's lasting time\n");
            }
            selection=ask_event_length();
            if(selection=='d'){
                while(1){
                printf("Please enter the start month and date of your event (use spaces to seperate the two numbers, e.g.[10 31]): ");
                scanf("%d %d", &start_month, &start_date);
                printf("Please enter the end month and date of your event (use spaces to seperate the two numbers, e.g.[12 25]: ");
                scanf("%d %d", &end_month, &end_date);
                /**************
                if(start_month<1||start_month>12||end_month<1||end_month>12||start_date>end_month){
                    printf("Invalid Input in month.\n");
                    continue;
                }
                ***************/
                if(isleap(today/10000)){   
                    if((start_date>leap_month_day[start_month-1])||(end_date>leap_month_day[end_month-1])||(start_date<1)||(end_date<1)){
                        printf("Invalid Input in date.\n");
                    }else if(start_month<1||start_month>12||end_month<1||end_month>12||start_month>end_month){
                        printf("Invalid Input in month.\n");
                    }else{
                            break;
                    }
                }else{
                    if((start_date>normal_month_day[start_month-1])||(end_date>normal_month_day[end_month-1])||(start_date<1)||(end_date<1)){
                        printf("Invalid Input in date.\n");
                    }else if(start_month<1||start_month>12||end_month<1||end_month>12||start_month>end_month){
                        printf("Invalid Input in month.\n");
                    }else{
                        break;
                    }
                }
            }
            printf("\n");
            daily_event(start_month, end_month, start_date, end_date, name, start_time, end_time, place, others);
            }else{
                while(1){
                    printf("Please enter the start month and date of your event (use spaces to seperate the two numbers, e.g.[10 31]): ");
                    scanf("%d %d", &start_month, &start_date);
                    if(isleap(today/10000)){   
                        if((start_date>leap_month_day[start_month-1])||(start_date<1)){
                            printf("Invalid Input in date.\n");
                        }else{
                            break;
                        }
                    }else{
                        if((start_date>normal_month_day[start_month-1])||(start_date<1)){
                            printf("Invalid Input in date.\n");
                        }else{
                            break;
                        }
                    }
                }
                long_term_event(selection, start_month, start_date, start_time, end_time,  name, place,  others);
            }
        }
        else if(action == 2){ //[2] search for an event
            int event_search,today_or_not;
            int today_month,today_date,time;
            while(1){
                printf("How would you like to search?\n");
                printf("[1] search if the time has activity\n");
                printf("[2] search all the free time you have\n");
                printf("[3] search scheduled time through entering activity\n");
                printf("[4] back to previous page\n");
                scanf("%d",&event_search);
                if(event_search == 4) break;
            if(event_search==1){
                while(1){
                    printf("What day are you searching? (today[1], others[2]) :\n");
                    scanf("%d",&today_or_not);
                    if(today_or_not==1){
                        int time;
                        today_month=((today%10000)/100);
                        today_date=((today%100));
                        while(1){
                            printf("what time would you like too search (ex: 1800 ): ");
                            scanf("%d",&time);
                            if(time<=2400&&time>=0000){
                                break;
                            }else{
                                printf("Invalid Input in time!!!\n");
                            }
                        }
                        search_if_have_activity_on_the_date(date_head,today_month,today_date,time);
                        break;
                    }else if(today_or_not==2){
                        while(1){
                            printf("What month and date would you like to search? (ex: 06 11 ): ");
                            scanf(" %d %d",&today_month,&today_date);
                            if(today_month<1&&today_month>12){
                                printf("Invalid Input in month!!!\n");
                                continue;
                            }
                            if(isleap(today/10000)){
                                if(today_date>=1&&today_date<=leap_month_day[today_month-1]){
                                    break;
                                }else{
                                    printf("Invalid Input in date!!!\n");
                                }
                            }else{
                                if(today_date>=1&&today_date<=normal_month_day[today_month-1]){
                                    break;
                                }else{
                                    printf("Invalid Input in date!!!\n");
                                }
                            }
                        }
                        while(1){
                            printf("what time would you like too search (ex: 1800 ): ");
                            scanf("%d",&time);
                            if(time<=2400&&time>=0000){
                                break;
                            }else{
                                printf("Invalid Input in time!!!\n");
                            }
                        }
                        search_if_have_activity_on_the_date(date_head,today_month,today_date,time);
                        break;
                    }
                }
                break;
            }else if(event_search==2){
                while(1){
                    printf("What day are you searching? (today[1], others[2]) :\n");
                    scanf("%d",&today_or_not);
                    if(today_or_not==1){
                        int time;
                        today_month=((today%10000)/100);
                        today_date=((today%100));
                        search_all_day_free_time(date_head,today_month,today_date);
                        break;
                    }else if(today_or_not==2){
                        while(1){
                            printf("What month and date would you like to search? (ex: 06 11 ): ");
                            scanf(" %d %d",&today_month,&today_date);
                            if(today_month<1&&today_month>12){
                                printf("Invalid Input in month!!!\n");
                                continue;
                            }
                            if(isleap(today/10000)){
                                if(today_date>=1&&today_date<=leap_month_day[today_month-1]){
                                    break;
                                }else{
                                    printf("Invalid Input in date!!!\n");
                                }
                            }else{
                                if(today_date>=1&&today_date<=normal_month_day[today_month-1]){
                                    break;
                                }else{
                                    printf("Invalid Input in date!!!\n");
                                }
                            }
                        }
                        search_all_day_free_time(date_head,today_month,today_date);
                        break;
                    }
                }
                break;
            }else if(event_search==3){
                char activity_name[30];
                while(1){
                    printf("What day are you searching? (today[1], others[2]) :\n");
                    scanf("%d",&today_or_not);
                    if(today_or_not==1){
                        int time;
                        today_month=((today%10000)/100);
                        today_date=((today%100));
                        printf("What is the name of the activity: ");
                        scanf("%29s", activity_name);
                        search_scheduled_time_through_activity(date_head,activity_name,today_month,today_date);
                        break;
                    }else if(today_or_not==2){
                        while(1){
                            printf("What month and date would you like to search? (ex: 06 11 ): ");
                            scanf(" %d %d",&today_month,&today_date);
                            if(today_month<1&&today_month>12){
                                printf("Invalid Input in month!!!\n");
                                continue;
                            }
                            if(isleap(today/10000)){
                                if(today_date>=1&&today_date<=leap_month_day[today_month-1]){
                                    break;
                                }else{
                                    printf("Invalid Input in date!!!\n");
                                }
                            }else{
                                if(today_date>=1&&today_date<=normal_month_day[today_month-1]){
                                    break;
                                }else{
                                    printf("Invalid Input in date!!!\n");
                                }
                            }
                        }
                        printf("What is the name of the activity: ");
                        scanf(" %29s", activity_name);
                        search_scheduled_time_through_activity(date_head,activity_name,today_month,today_date);
                        break;
                    }
                }
                break;
            }
            }
        }
        else if(action == 3){ //[3] search for an event using hash function
            Event_content searched_event;
            searched_event.name = malloc(sizeof(struct event_content));
            printf("what event do you want to search?\n");
            scanf(" %s", searched_event.name);
            if(searched_event.name[(signed)strlen(searched_event.name) - 1] == '\n'){
                searched_event.name[(signed)strlen(searched_event.name) - 1] = '\0';
            }
            //printf("%s", searched_event.name);
            Event_content *tmp = hash_table_lookup(searched_event.name);
            if(tmp == false){
                printf("event not found!\n");
            }else if(tmp == NULL){
                printf("event has been deleted %s\n", tmp -> name);
            }else{
                printf("found event: %s\nPlace: %s\nOthers: %s\nStart time: %d\nEnd time: %d\n", tmp -> name, tmp -> place, tmp -> others, tmp -> start_time, tmp -> end_time);
            }
            printf("\n");
        }
        else if(action == 4){ //[4] delete an event 
            printf("What event do you want to delete?\n");
            printf("Please input the date of the event first. (ex: 06 11 ): ");
            scanf("%d %d", &delete_month, &delete_date);
            printf("Please input the name of the event: ");
            scanf(" %s", delete_name);
            delete_content(find_current_date(delete_month, delete_date), delete_name);

        }

        else if(action == 5){ //[5] print out the schedule 
            int print_select;
            printf("what do you want to print?\n");
            printf("[1] whole month calendar\n");
            printf("[2] whole week calendar\n");
            printf("[3] whole day calendar\n");
            scanf("%d",&print_select);
            switch (print_select){
            case 1:
                print_calendar(today / 10000, (today % 10000) / 100 ,today %100);
                break;
            case 2:
                print_week(today / 10000, (today % 10000) / 100 ,today %100);
                break;
            case 3:
                print_date(today / 10000, (today % 10000) / 100 ,today %100);
                break;
            default:
                break;
            }
        }
       else if(action == 6){ //[6] terminate this day 
            char ans1;
            printf("How was your day? Congratulate on making it through!\n");
            printf("Did you manage to complete all the tasks for today (Y or N): ");
            scanf(" %c", &ans1);
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

                            // 檢查是否在"Yes"選項區域內點擊
                            if (mouseX >= yesRect.x && mouseX <= yesRect.x + yesRect.w && mouseY >= yesRect.y && mouseY <= yesRect.y + yesRect.h){
                                initialLength += lengthIncrement;   // 每次點擊YES矩形，增加樹的長度
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderClear(renderer);
                                drawTree(renderer, startX, startY, initialLength, initialAngle, depth);

                                int i = 10;
                                while(i--){    //動畫
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
            if(ans1 == 'Y'){
                printf("Excellent! Congratulates on scoring 500 points.\n");
                printf("Let's play a little game to relax.\n");
                game_1a2b(&points_of_master);
            }
            else if(ans1 == 'N'){
                printf("Don't worry, rest is essential to embark on a longer journey.\n");
                printf("You still scored 300 points. Keep up the good work and continue to strive tomorrow.\n");
            }
            today = to_new_day(today);
        }
        else if(action == 7){ //[7] terminate this month
            char ans1;
            printf("How was your month? Congratulations on making it through!\n");
            printf("Did you manage to complete all the tasks for this month (Y or N): ");
            scanf(" %c", &ans1);
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

                            // 檢查是否在"Yes"選項區域內點擊
                            if (mouseX >= yesRect.x && mouseX <= yesRect.x + yesRect.w && mouseY >= yesRect.y && mouseY <= yesRect.y + yesRect.h){
                                initialLength += lengthIncrement;   // 每次點擊YES矩形，增加樹的長度
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderClear(renderer);
                                drawTree(renderer, startX, startY, initialLength, initialAngle, depth);

                                int i = 10;
                                while(i--){    //動畫
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
            if(ans1 == 'Y'){
                printf("Excellent! Congratulations on scoring 5000 points.\n");
                printf("Let's play a little game to relax.\n");
                game_1a2b(&points_of_master);
            }
            else if(ans1 == 'N'){
                printf("Don't worry, rest is essential to embark on a longer journey.\n");
                printf("You still scored 3000 points. Keep up the good work and continue to strive tomorrow.\n");
            }
            today = to_new_month(today);
        }

    }
    
}


struct event_date *event_date_insert(int start_month, int start_date, int command){
    /* allocate node */
    Event_date *new_event_date;
    Event_content *new_event_content;
    Event_date *prev = NULL;
    new_event_date = malloc(sizeof(Event_date));
    new_event_content = malloc(sizeof(Event_content));
    if(new_event_date == NULL){
        printf("Error: malloc failed in event_date_insert\n");
        exit(1);
    }
    if(new_event_content == NULL){
        printf("Error: malloc failed in event_date_insert\n");
        exit(1);
    }
 
    Event_date *cur = date_head; //used to locate the needed node in linked list
 
    /* put in the data */
    new_event_date -> month = start_month;
    new_event_date -> date = start_date;
    new_event_date -> event_num = 0;
    new_event_date -> next = NULL;
    new_event_date -> content = NULL;
 
    /* If linked List is empty */
    if(date_head == NULL){
        date_head = new_event_date;
        return date_head;
    }
 
    /* insert the node */
    while(1){
        if((cur -> month == new_event_date -> month && cur -> date > new_event_date ->date) || (cur -> month > new_event_date -> month)){
            //if 
            if(prev == NULL){
                new_event_date -> next = cur;
                date_head = new_event_date;
                break;
            }
            prev -> next = new_event_date;
            new_event_date -> next = cur;
            break;
        }
        if(cur -> next == NULL){
            cur -> next = new_event_date;
            break;
        }
        prev = cur;
        cur = cur -> next;
    }
    /*
    while(cur -> next != NULL)
        cur = cur -> next;
    */
 
    
    /* insert the last node */
    /*
    cur -> next = new_event_date;
    */
    //printf("ttttttttt\n");
    return date_head;
}

Event_date *find_current_date(int start_month, int start_date){
    Event_date *tmp = date_head;
    while(tmp != NULL){
        if((tmp->month == start_month) && (tmp->date == start_date)){
             //printf("m:%d d:%d sm:%d sd:%d\n",tmp->month,tmp->date,start_month,start_date);
             return tmp;
        }
        else{
            tmp = tmp->next;
        } 
    }
    if(tmp == NULL){
        //printf("there\n");
        return NULL;
    } 
    
}

void event_content_insert(int month, int date, char *name, int start_time, int end_time, char* place, char* others){
    /*
    In this function, we have few steps to do
    1. find if the day has any activity before, if not, then add a nw node of event_date
    2. check if the time is free
        (1) if not, then print out the orignal schedule and end the action
        (2) if is, then insert the schedule
    */
    //printf("m:%d d:%d\n",month,date);
    Event_date *cur_date = find_current_date(month, date);
    if(cur_date == NULL){
        cur_date = event_date_insert(month, date, 1);
        cur_date = find_current_date(month, date);
    }
    //printf("%d %d\n",cur_date->month,cur_date->date);
    if(cur_date!=NULL){
        cur_date -> event_num++; 
    }
    else if(check_if_already_have_event(date_head, month,date,start_time, end_time)==0) return;
    //printf("okok");
    Event_content *new_event_content = malloc(sizeof(Event_content));
    new_event_content -> name = malloc(30);
    new_event_content -> place = malloc(30);
    new_event_content -> others = malloc(30);
    Event_content *content_head = cur_date->content;
    Event_content *curr = content_head;
    Event_content *prev = NULL;
    Event_content *tmp = NULL;
    //printf("okok");
    //printf("bbbbbbbbbbb\n");
    if(new_event_content == NULL){
        printf("Error: malloc failed in event_content_insert\n");
        exit(1);
    }
    /* put in the data */
    new_event_content -> name = name;
    new_event_content -> start_time = start_time;
    new_event_content -> end_time = end_time;
    new_event_content -> place = place;
    new_event_content -> others = others;
    new_event_content -> next = NULL;
    //printf("ccccccccc\n");
    hash_table_insert(new_event_content);
    //print_table();
    //printf("ddddddd\n");
    if(content_head == NULL){ //If linked List is empty
        cur_date->content = new_event_content;
        //printf("okok");
    }
    while(curr != NULL){
        if(prev == NULL){
           // printf("fffffff\n");
            new_event_content->next = curr;
            cur_date->content = new_event_content;
        }
        else if(prev->start_time <= new_event_content->start_time){
            //printf("gggggggggg\n");
            new_event_content->next = curr;
            prev->next = new_event_content;
            tmp -> next = prev;
        }
        tmp=malloc(sizeof(Event_content));
        tmp=prev;
        //tmp->next=malloc(sizeof(Event_content));
       // printf("eeeeeeeeee\n");
        prev = curr;
        curr = curr->next;
    }
}   

Event_date *delete_date(int month, int date){ //month and date stand for target dates
    Event_date *tmp;
    tmp->month = month;
    tmp->date = date;

    Event_date *date_curr = date_head;
    Event_date *date_prev = NULL;

    while(date_curr != NULL){
        if(date_prev == NULL && date_curr->month < month){
            date_head = date_curr->next;
        }

        if(date_curr->month < month){
            date_prev->next = date_curr->next;
        }
            
        else if(date_curr->month == month)
            if(date_curr->date < date){
                date_prev->next = date_curr->next;
        }
                
        date_prev = date_curr;
        date_curr = date_curr->next;
    }
    if(date_curr == NULL) return NULL;
    else return tmp;
}

Event_content *delete_content(Event_date *cur_date, char *name){
    Event_content *tmp;
    tmp->name = name;

    Event_content *content_head = cur_date->content;
    Event_content *content_curr = content_head;
    Event_content *content_prev = NULL;

    while(content_curr != NULL){
        if(content_prev == NULL && strcmp(content_curr->name, name) == 0){
            cur_date->content = content_curr->next;
            cur_date->event_num--;
        }
        else if(strcmp(content_curr->name, name) == 0){
            cur_date->event_num--;
            content_prev->next = content_curr->next;
        }
                
        content_prev = content_curr;
        content_curr = content_curr->next;
    }
    if(content_curr == NULL) return NULL;
    else return tmp;
}

void print_event_date_list(){
  /* print start point for testing */
    Event_date *list = date_head;
    while(list != NULL){
        printf(":: %d %d\n",list->month, list->date);
        list = list -> next;
    }
}

void print_event_content_list(Event_content *list){
    Event_content *tmp = list;
    /* print time for testing */
    while(tmp != NULL){
        printf("%s: %d %d\n", tmp -> name, tmp -> start_time, tmp -> end_time);
        tmp = tmp -> next;
    }
}

void remove_enter(char *sentence){
    /* deal with '\0' */
    char *str = sentence;
    for(; *str && *str != '\n'; str++);
    if(*str == '\n') *str = '\0';
}

//This function will return the value 1 if the year is a leap year
//, and return the value 0 if the year is not a leap year.
int isleap(int year){ //return 1 if leapyear; return 0 if normal year
    if(year % 4000 == 0)
        return 0;
    else if(year % 400 == 0)
        return 1;
    else if(year % 100 == 0)
        return 0;
    else if(year % 4 == 0)
        return 1;
    else return 0;
}

//This function finds the weekday of the date, and returns the value.(sunday return 0)
int find_weekday(int year,int month,int date){
    int day[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days_count=0;
    int d;
    for(int i = 2023; i < year; ++i){//count days during 20230101 to YYYY0101
        if(isleap(i)==1){
            days_count+=366;
        }else{
            days_count+=365;
        }
    }
    for(int i = 1; i < month; ++i){//count days during YYYY0101 to YYYYMM01
        if((i == 2) && (isleap(year) == 1)){
            days_count += 29;
        }else{
            days_count += day[i - 1];
        }
    }
    days_count += date;
    days_count -= 1;
    d=(days_count) % 7;
    return d;
}

int to_new_day(int curr_date){
    int year = curr_date / 10000;
    int day = curr_date % 100;
    int month = (curr_date % 10000) /100;

    if(day < 28) return curr_date+1;
    else if(day == 28){
        if(month == 2){
            if(isleap(year)) return curr_date+1;
            else return curr_date - 28 + 101;
        }
        else return curr_date+1;
    }
    else if(day == 29){
        if(isleap(year)) return curr_date - 29 + 101;
        else return curr_date+1;
    }    
    else if(day == 30){
        if(((month == 1 || month == 3) || (month == 5 || month == 7)) || ((month == 8 || month == 10) || month == 12)) return curr_date + 1;
        else return curr_date - 30 + 101;
    }
    else if(month == 12) return curr_date - 1231 + 10101;
    else return curr_date - 31 + 101;
}

int to_new_month(int date){
    date -= date % 100;
    if(date%10000 == 1200) return date - 1200 + 10101;
    return date + 101;
}

//show the calendar and show the number of the event of the day on the calendar.
void print_calendar(int year,int month,int date){
    int day[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int leap[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
    //system("CLS");
    Event_date *cur = find_current_date(month,date);
    printf("%s         %4d         %2d         \n",WHITE,year,month);
    printf("%s  SUN  MON  TUE  WED  TUE  FRI  SAT \n",WHITE);
    printf("%s+----+----+----+----+----+----+----+\n",BLUE);
    int d = find_weekday(year,month,1);
    printf("%s|",BLUE);
    int cur_date = 1;
    int cur_day = d;
    if(month ==2 && isleap(year)){
        while((cur_date - cur_day) <= leap[month-1] && cur_day <= 6){
            for(int i = cur_date - cur_day ; i < cur_date - cur_day + 7  ; ++i){
                if(i <= 0){
                    printf("%s    |",BLUE);
                }else if(i > leap[month-1]){
                    printf("%s    |",BLUE);
                }else{
                    printf("%s %2d ",YELLOW,i);
                    printf("%s|",BLUE);
                }
            }
            printf("%s\n|",BLUE);
            for(int i = cur_date - cur_day ; i < cur_date - cur_day + 7  ; ++i){
                if(i <= 0){
                    printf("%s    |",BLUE);
                    continue;
                }else{
                    cur = find_current_date(month,i);
                    if(cur == NULL){
                        printf("%s    |",BLUE);
                        continue;
                    }
                    if(i > leap[month-1]){
                        printf("%s    |",BLUE);
                        continue;
                    }
                    switch (cur -> event_num){
                    case 0:
                        printf("%s    |",BLUE);
                        break;
                    case 1:
                        printf("%s.   ",YELLOW);
                        printf("%s|",BLUE);
                        break;
                    case 2:
                        printf("%s..  ",YELLOW);
                        printf("%s|",BLUE);
                        break;
                    case 3:
                        printf("%s... ",YELLOW);
                        printf("%s|",BLUE);
                        break;
                    default:
                        printf("%s....",YELLOW);
                        printf("%s|",BLUE);
                        break;
                    }
                }
                cur = cur -> next;
            }
            printf("\n");
            printf("%s+----+----+----+----+----+----+----+\n",BLUE);
            if((cur_date - cur_day + 7) <= leap[month-1]){
                printf("|");
            }
            cur_date = cur_date + 7;
        }
    }else{
        while((cur_date - cur_day) <= day[month-1] && cur_day <= 6){
            for(int i = cur_date - cur_day ; i < cur_date - cur_day + 7  ; ++i){
                if(i <= 0){
                    printf("%s    |",BLUE);
                }else if(i > day[month-1]){
                    printf("%s    |",BLUE);
                }else{
                    printf("%s %2d ",YELLOW,i);
                    printf("%s|",BLUE);
                }
            }
            printf("%s\n|",BLUE);
            for(int i = cur_date - cur_day ; i < cur_date - cur_day + 7  ; ++i){
                if(i <= 0){
                    printf("%s    |",BLUE);
                    continue;
                }else{
                    cur = find_current_date(month,i);
                    if(cur == NULL){
                        printf("%s    |",BLUE);
                        continue;
                    }
                    if(i > day[month-1]){
                        printf("%s    |",BLUE);
                        continue;
                    }
                    switch (cur -> event_num){
                    case 0:
                        printf("%s    |",BLUE);
                        break;
                    case 1:
                        printf("%s.   ",YELLOW);
                        printf("%s|",BLUE);
                        break;
                    case 2:
                        printf("%s..  ",YELLOW);
                        printf("%s|",BLUE);
                        break;
                    case 3:
                        printf("%s... ",YELLOW);
                        printf("%s|",BLUE);
                        break;
                    default:
                        printf("%s....",YELLOW);
                        printf("%s|",BLUE);
                        break;
                    }
                }
                cur = cur -> next;
            }
            printf("\n");
            printf("%s+----+----+----+----+----+----+----+\n",BLUE);
            if((cur_date - cur_day + 7) <= day[month-1]){
                printf("|");
            }
            cur_date = cur_date + 7;
        }
    }
    printf("%s",WHITE);
    return;
}

//show one week of the calendar, and also the number of the event of the day on the calendar.
void print_week(int year,int month,int date){
    int day[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int leap[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
    Event_date *cur = find_current_date(month,date); //find the current day
    printf("%s         %4d         %2d         \n",WHITE,year,month);
    printf("  SUN  MON  TUE  WED  TUE  FRI  SAT \n");
    printf("%s+----+----+----+----+----+----+----+\n",BLUE);
    int d = find_weekday(year,month,date);
    printf("|");
    int A[7]={0};
    if(isleap(year)){
        date = date - d;
        for(int i = 0;i < 7; ++i){
            if(date <= 0){//go to last month
                if(month == 1){//go to december
                    date = date + leap[11];
                    month = 12;
                }else{
                    date = date + leap[month-2];
                    month--;
                }
            }else if(date > leap[month-1]){
                if(month == 12){//go to december
                    date = date - leap[11];
                    month = 1;
                }else{
                    date = date - leap[month-1];
                    month++;
                }
            }
            printf("%s %2d ",YELLOW,date);
            printf("%s|",BLUE);
            cur = find_current_date(month,date);
            if(cur == NULL){
                A[i] = 0;
            }else{
                A[i] = cur -> event_num;
            }
            date++;
        }
    }else{
        date = date - d;
        for(int i = 0;i < 7; ++i){
            if(date <= 0){//go to last month
                if(month == 1){//go to december
                    date = date + day[11];
                    month = 12;
                }else{
                    date = date + day[month-2];
                    month--;
                }
            }else if(date > day[month-1]){
                if(month == 12){//go to december
                    date = date - leap[11];
                    month = 1;
                }else{
                    date = date - day[month-1];
                    month++;
                }
            }
            printf("%s %2d ",YELLOW,date);
            printf("%s|",BLUE);
            cur = find_current_date(month,date);
            if(cur == NULL){
                A[i] = 0;
            }else{
                A[i] = cur -> event_num;
            }
            date++;
        }
    }
    printf("%s\n|",BLUE);
    for(int i = 0;i < 7; ++i){
        switch (A[i]){
            case 0:
                printf("%s    |",BLUE);
                break;
            case 1:
                printf("%s.   ",YELLOW);
                printf("%s|",BLUE);
                break;
            case 2:
                printf("%s..  ",YELLOW);
                printf("%s|",BLUE);
                break;
            case 3:
                printf("%s... ",YELLOW);
                printf("%s|",BLUE);
                break;
            default:
                printf("%s....",YELLOW);
                printf("%s|",BLUE);
                break;
        }
    }
    printf("\n");
    printf("%s+----+----+----+----+----+----+----+\n",BLUE);
    printf("%s",WHITE);
    return;
}

//show the date schedule and show the event name at the hour.
void print_date(int year,int month,int date){
    Event_date *cur = find_current_date(month,date);
    printf("   %4d   %2d   %2d   \n",year,month,date);
    int A[24]={0};
    if(cur == NULL || cur -> event_num == 0){
        for(int i = 0;i <= 23; ++i){
            printf("%s%2d------------------\n",WHITE,i);
            printf("                    \n");
        }
        printf("%s24------------------\n",WHITE);
        return;
    }
    Event_date* tmp = NULL;
    if (get_content_from_file(&tmp) == 0) {
        printf("Error reading input file!\n");
        return ;
    }
    Event_content* cur_event = find_day(tmp,month,date);
   // Event_content *cur_event = cur->content;
    while(cur_event != NULL){
        for(int i = cur_event->start_time ; i <= cur_event->end_time ; ++i){
            A[i]=1;
        }
        cur_event = cur_event->next;
    }
    cur_event = cur->content;
    for(int i = 0;i <= 23; ++i){
        printf("%s%2d------------------\n",WHITE,i);
        if(A[i]==0){
            printf("                    \n");
        }else{
            printf("%s.%-18s*\n",YELLOW,cur_event->name);
            if(i == cur_event->end_time){
                cur_event = cur_event->next;
            }
        }
    }
    printf("%s24------------------\n",WHITE);
    return;
}

void game_1a2b(int *points_of_master){
    int play = 0;
    while(1){
        printf("\n");
        printf("%s1A2B is a puzzle game.\n",YELLOW);
        printf("If the number what you guess is in the right place then you get 1A.\n");
        printf("If the number is in the number but not in the right place would get 1B.\n");
        printf("If you can guess the correct number less than or equal to 9 times, \n");
        printf("you can get random points from 2,000 to 200,000\n");
        printf("Do you want to play 1A2B ? (No: 0/ Yes: 1) : ");
        scanf("%d", &play);
        if(play == 1) break;
        else if(play == 0) {
            printf("%s",WHITE);
            return;
        }
        else{
            printf("Invalid Input!!!\n");
        }
    }
    int answer[4];  // Array to store the answer
    for(int i = 0; i < 4; i++){
        while(1){
            answer[i] = rand() % 10; // Generate a random number between 0 and 9
            bool is_the_same = false;
            for(int j = 0; j < i; j++){
                if(answer[j] == answer[i]){
                    is_the_same = true;
                }
            }
            if(is_the_same == false) break; 
        }
    }

    int guess_count = 0;
    char guess_number[100];
    while(guess_count < 10){
        printf("This is your %dth guess : ", guess_count + 1);
        scanf("%s", guess_number);
        if(strlen(guess_number) != 4){
            printf("Invalid Input!!!\n");
            printf("%sNotice the length must be 4 digits.\n",RED);
            continue;
        }
        bool is_all_digit = true;
        for(int i = 0 ; i < 4; i++){
            if(guess_number[i] >= '0' && guess_number[i] <= '9'){
                guess_number[i] -= 48; // Convert character to integer by subtracting ASCII value of '0'
            }
            else{
                is_all_digit = false;
            }
        }
        if(is_all_digit == false){
            printf("Invalid Input!!!\n");
            printf("%sYou must input a 4 digit number.\n",RED);
            continue;
        }
        bool have_same_number = false;
        for(int i = 0; i < 3; i++){
            for(int j = i + 1; j < 4; j++){
                if(guess_number[i] == guess_number[j]) have_same_number = true;
            }
        }
        if(have_same_number == true){
            printf("Invalid Input!!!\n");
            printf("%sThe four digits must be different.\n",RED);
            continue;
        }
        int A_num = 0, B_num = 0;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                if(guess_number[i] == answer[j]){
                    if(i == j) A_num++;
                    else B_num++;
                }
            }
        }
        if(A_num == 4){
            printf("All correct.\n");
            int prize = ((rand() % 100) + 1) * 1000;
            *points_of_master += prize;
            printf("You win %d points.\n", prize);
            return;
        }
        else{
            printf("%dA%dB\n", A_num, B_num);
            guess_count++;
        }
    }
    printf("You have been no chance already.\n");
}


int search_if_the_day_have_activity(int month, int date){
    Event_date *tmp = date_head;
    while( tmp != NULL){
        if((tmp->month == month) && (tmp->date == date)) return 1;
        else tmp = tmp->next;
    }
    if(tmp == NULL){
        return 0;
    }
}

int search_if_the_time_have_activity(struct event_content *list,int start_time, int end_time, int command){ //search if have things to do at the time
    /* 
    The function will be used in two condition by the different value of the variable "command".
    "0" is only used to check if the period of time is free
    "1" is to check and print out for user to know
    */
    while(list != NULL){
        int tmp_act = 0;
        if((list->end_time >= start_time) && (list->end_time <= end_time)) tmp_act = 1;
        if((list->start_time >= start_time) && (list->start_time <= end_time)) tmp_act = 1;

        if(tmp_act == 1){ //if the time has already assigned schedule
            if(command == 1) printf("You have %s from %d to %d",list->name, list->start_time, list->end_time);            //print the activity name
            return 1;
        }
        else{
            list=list->next;                                    //if not find yet, go on the next
        }
    }
    if(list == NULL){
        if(command == 1) printf("You are free at the time\n");          // if not find an activity for the whole list
        return 0;
    }
}

void search_all_day_free_time(Event_date *head,int month, int date){                //consider ptr sorted
    int free_time_str = 0000,off=1;
    Event_date *ptr = head; 
    struct event_content *tmp = find_today(ptr,month,date);
    while((tmp != NULL)){
        if(free_time_str < (tmp->start_time)){                     //if free time is smaller than the first activity's star time, it is free 
            off=0;
            printf("You have free time from %d o'clock to %d o'clock.\n"
            ,(free_time_str/100),( tmp->start_time/100));                     //have the free time till the closest activity start
        }
        free_time_str = tmp->end_time;                            //set the free time start count point at the end of the activity
        tmp = tmp->next;                                          //go to the next closest activity
    }
    //printf("%d\n",free_time_str);
    if((free_time_str/100)==A_DAY_HOUR&&off==1&&((today%10000)/100)==month&&((today%100)==date)){
        printf("Uhhhh...What a busy day you have.\n");
        printf("You have no free time today.\n");
    }else if((free_time_str/100)==A_DAY_HOUR&&off==1){
        printf("Wow...What a busy day you have.\n");
        printf("You have no free time at that day.\n");
    }
    if(((free_time_str/100) < A_DAY_HOUR)){
        printf("You have free time from %d o'clock to %d o'clock.\n", (free_time_str/100), A_DAY_HOUR);                                  //if no more activity, it is free till 24
    }   
     printf("\n");
}

void search_scheduled_time_through_activity(Event_date *head, char *activity_name,int month,int date){
    /*
    This function is to look for the time when the activity takes place.
    1. look through the list and find the activity
        (1) if it finds, it will print out the time the activity starts and ends.
        (2) if not, it will print no-message.
    */
    Event_date *ptr= head;
    Event_content *list=find_today(ptr,month,date);
    bool have_the_activity_or_not = 0;
    while(list != NULL){
        if(strcmp(list->name, activity_name) == 0){                 //if there's this activity in schedule
            printf("You have the activity from %d to %d.\n", list->start_time,list->end_time); //print the activity name and time
            have_the_activity_or_not = 1;
        }
        list=list->next;                                        //if not find yet, go on the next
    }
    if(!have_the_activity_or_not&&((today%10000)/100)==month&&((today%100)==date)){
        printf("You don't have the activity today.\n");              // if not find an activity for the whole list
    }else if(!have_the_activity_or_not){
        printf("You don't have the activity at that time.\n");
    }
    printf("\n");
}

int get_content_from_file(Event_date **ptr) {
    FILE *input_file;
    input_file = fopen("file_io.txt", "r");
    if (input_file == NULL) {
        // Handle error
        return 0;
    }
    
    char line[350];
    while (fgets(line, sizeof(line), input_file) != NULL) {
        Event_date *new_node = malloc(sizeof(Event_date));
        new_node->content = malloc(sizeof(Event_content));
        new_node->content->name = malloc(30);
        new_node->content->place = malloc(30);
        new_node->content->others = malloc(30);
        
        sscanf(line, "%d. %d/%d %d-%d",
               &(new_node->event_num),
               &(new_node->month),
               &(new_node->date),
               &(new_node->content->start_time),
               &(new_node->content->end_time));

        char *token;
        token = strtok(line, ",");
        if (token != NULL) {
            // Skip numeric values in the name field
            while ((*token >= '0' && *token <= '9')||( *token=='.')) {
                token++;
            }
            // Remove leading whitespace, if any
            while (*token == ' ') {
                token++;
            }
            while ((*token >= '0' && *token <= '9')||(*token=='/')) {
                token++;
            }
            while (*token == ' ') {
                token++;
            }
            while ((*token >= '0' && *token <= '9')||(*token=='-')) {
                token++;
            }
            while (*token == ' ') {
                token++;
            }
            new_node->content->name = malloc(strlen(token) + 1);
            strcpy(new_node->content->name, token);
        }
        
        token = strtok(NULL, ",");
        if (token != NULL) {
            new_node->content->place = malloc(strlen(token) + 1);
            strcpy(new_node->content->place, token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            new_node->content->others = malloc(strlen(token) + 1);
            strcpy(new_node->content->others, token);
        }
        
        new_node->next = NULL;

        // Link the new node to the existing list
        if (*ptr == NULL) {
            *ptr = new_node;
        } else {
            Event_date *current = *ptr;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
        }
    }

    fclose(input_file);
    return 1;
}

int write_content_on_file(Event_date *head){                    //write content on file
    Event_date *ptr = head;
    FILE *output_file;
    output_file=fopen("file_io.txt","w");                        //connect output_file with output.txt (write only)
    if (output_file == NULL) {
        printf("Error opening output file!\n");                 //if fail connecting
        output_file = fopen("file_io.txt", "w+");
        if (output_file == NULL) {
            printf("Error creating output file!\n");
            return 1;
        }
    }
    while(ptr!=NULL){   //put info into file
        Event_content *tmp_content=ptr->content;   
        for(int i=1;i<=ptr->event_num;i++){
            fprintf(output_file,"%d. %d/%d %d-%d %s,%s,%s\n",i/*event num*/,\
            ptr->month,ptr->date,tmp_content->start_time,tmp_content->end_time\
            ,tmp_content->name,tmp_content->place,tmp_content->others);
            tmp_content=tmp_content->next;
        }            
        ptr=ptr->next;                            
    }
    fclose(output_file);                                        //close the file
    return 1;
}

Event_date *add_to_list(Event_date *head,int st_mon,int st_da,int str_t,int end_t,char* ac,char *pl,char *ot){
    Event_date *list = head;
    Event_date *tmp;
    tmp=malloc(sizeof(Event_date));
    strcpy(tmp->content->name,ac);
    tmp->month=st_mon;
    tmp->date=st_da;
    tmp->content->start_time=str_t;
    tmp->content->end_time=end_t;
    strcpy(tmp->content->place,pl);
    strcpy(tmp->content->others,ot);
    tmp->next=list;
    return tmp;
}

Event_date *annual_activity;

void long_term_event(char selection, int month, int date, int start_time, int end_time,  char *name, char *place, char* others){
    int normal_month_day[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    int leap_month_day[12]={31,29,31,30,31,30,31,31,30,31,30,31};
    if(this_year==(today/10000)){  //if this year == year
            while(annual_activity!=NULL){  //put every event in the annual_activity list into the event list
                event_content_insert(annual_activity->month,annual_activity->date,annual_activity->content->name,annual_activity->content->start_time,annual_activity->content->end_time,annual_activity->content->place,annual_activity->content->others);
                annual_activity=annual_activity->next;
            }
            this_year++;  //this_year+1, so the events won't be add again till next year
        }
        switch(selection){
            case 'w':                                           // if is week, insert activity after every seven days
                   if(month==1||month==3||month==5||month==7||month==8||month==10||month==12){ //if big month
                        for(int i=date;i<31;i+=7){
                            if(check_if_already_have_event(date_head,month,i,start_time, end_time)){
                                event_content_insert(month,i/*date*/,name,start_time,end_time,place,others);
                            }
                        }
                   }else if(month==4||month==6||month==9||month==11){ //if small month
                        for(int i=date;i<30;i+=7){
                            if(check_if_already_have_event(date_head,month,i,start_time, end_time)){
                                event_content_insert(month,i/*date*/,name,start_time,end_time,place,others);
                            }
                        }
                   }else if(month==2){    //if feb
                        if(isleap(today/10000)){              //leap year
                            for(int i=date;i<29;i+=7){
                                if(check_if_already_have_event(date_head,month,i,start_time, end_time)){
                                    event_content_insert(month,i/*date*/,name,start_time,end_time,place,others);
                            }
                            }
                        }else{                                  //normal year
                            for(int i=date;i<30;i+=7){
                                if(check_if_already_have_event(date_head,month,i,start_time, end_time)){
                                    event_content_insert(month,i/*date*/,name,start_time,end_time,place,others);
                            }
                            }
                        }
                    }
                    break;
            case 'm':                                           //if is a monthy activity
                    for(int i=month;i<12;i++){  
                        if(isleap(i)){
                            if(date<=leap_month_day[i-1]){
                                if(check_if_already_have_event(date_head,i,date,start_time, end_time)){
                                    event_content_insert(i/*month*/,date,name,start_time,end_time,place,others); //every month same date
                                }
                            }
                        }else{
                            if(date<=normal_month_day[i-1]){
                                if(check_if_already_have_event(date_head,i,date,start_time, end_time)){                           
                                    event_content_insert(i/*month*/,date,name,start_time,end_time,place,others);  //every month same date
                                }
                            }
                        }  
                    }
                    break;
           case 'y':                                          //if is an annual event                  
                    if(check_if_already_have_event(date_head,month,date,start_time, end_time)){
                        annual_activity=add_to_list(annual_activity,month,date,start_time,end_time,name,place,others); //add the event into annual_activity list                      
                        event_content_insert(month,date,name,start_time,end_time,place,others); //every month same date
                    }
                    break;
            case 'n':                                            //if not long term activity, just insert
                    if(check_if_already_have_event(date_head,month,date,start_time, end_time)){
                        event_content_insert(month,date,name,start_time,end_time,place,others);
                    }
                    break;
        }
        write_content_on_file(date_head);
}

char ask_event_length(){
    char selection;
    while(1){
        printf("This activity a daily event (press d), weekly event (press w), a monthly event (press m), a annual event (press y), or none of them (press n)\n");           //ask whether and which long term type
        scanf(" %c",&selection); 
        if(selection!='d'&&selection!='w'&&selection!='m'&&selection!='y'&&selection!='n'){
            printf("Invalid Input in selection!!!\n");
        }else{
            break;
        }
    }
    return selection;                                //select type
}

int check_if_already_have_event(Event_date *head, int month, int date, int start_time,int end_time){    //check if there is already event ont the day
    Event_date *cur_date = head;
    while(cur_date!=NULL){
        if(cur_date->month==month&&cur_date->date==date){
            break;
        }
        cur_date=cur_date->next;
    }
    if(cur_date!=NULL){
        Event_content *tmp;
        tmp=cur_date->content;
        while(tmp!=NULL){
            if((tmp->start_time<=start_time&&tmp->end_time>=start_time) || (tmp->end_time>=end_time&&tmp->start_time<=end_time)){
                printf("You already have the activity %s at the time\n",tmp->name);
                return 0;
            }
            tmp=tmp->next;
        }
    }
    return 1;
}

void search_if_have_activity_on_the_date(Event_date *ptr, int month, int date, int time){
    Event_date *head = ptr;
    while(head!=NULL){
        if(head->month==month&&head->date==date){
            break;
        }
        head=head->next;
    }
    if(head==NULL){
        printf("You don't have activity at the time\n");
    }else{
        Event_content *tmp;
        tmp=head->content;
        while(tmp!=NULL){
            if(tmp->start_time<=time&&tmp->end_time>=time){
                printf("You have the activity %s at the time\n",tmp->name);
                break;
            }
            tmp=tmp->next;
        }
        if(tmp==NULL){
            printf("You don't have activity at the time\n");
        }
    }
     printf("\n");
}

void daily_event(int start_month,int end_month,int start_date,int end_date,char* name,int start_time,int end_time,char* place,char* others){
    int normal_month_day[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    int leap_month_day[12]={31,29,31,30,31,30,31,31,30,31,30,31};
    if(isleap(today/10000)){
        while(start_date>leap_month_day[start_month-1]||start_date<1){
            printf("Invalid Input in start date!!!\n");
            printf("Please enter the start date again.\n");
            scanf("%d",&start_date);
        }
        while(end_date>leap_month_day[end_month-1]||end_date<1){
            printf("Invalid Input in end date!!!\n");
            printf("Please enter the end date again.\n");
            scanf("%d",&end_date);
        }
    }else{
        while(start_date>normal_month_day[start_month-1]||start_date<1){
            printf("Invalid Input in start date!!!\n");
            printf("Please enter the start date again.\n");
            scanf("%d",&start_date);
        }
        while(end_date>normal_month_day[end_month-1]||end_date<1){
            printf("Invalid Input in end date!!!\n");
            printf("Please enter the end date again.\n");
            scanf("%d",&end_date);
        }

    }
    if(start_month!=end_month){
        for(int i=start_month;i<=end_month;i++){
            if(isleap(today/10000)){
                for(int j=(i==start_month? start_date: 1);j<=(i==end_month? end_date : leap_month_day[i-1]);j++){
                    if(check_if_already_have_event(date_head,i/*month*/,j/*date*/,start_time, end_time)){
                        event_content_insert(i/*month*/,j/*date*/,name,start_time,end_time,place,others);
                    }
                }
            }else{
                for(int j=(i==start_month? start_date: 1);j<=(i==end_month? end_date : normal_month_day[i-1]);j++){
                    if(check_if_already_have_event(date_head,i/*month*/,j/*date*/,start_time, end_time)){
                        event_content_insert(i/*month*/,j/*date*/,name,start_time,end_time,place,others);
                    }
                }

            }
        }
    }else{
        for(int j=start_date;j<=end_date;j++){
            if(check_if_already_have_event(date_head,start_month,j,start_time, end_time)){
                //printf("here\n");
                event_content_insert(start_month,j/*date*/,name,start_time,end_time,place,others);
            }
        }

    }
    write_content_on_file(date_head);
}

Event_content *find_day(Event_date *ptr,int month,int date){
    Event_content *cur=NULL;
    while(ptr!=NULL){       
        if(ptr->month==month&&ptr->date==date){
            Event_content *tmp_content=malloc(sizeof(Event_content));
            tmp_content->name=ptr->content->name;
            tmp_content->start_time=ptr->content->start_time;
            tmp_content->end_time=ptr->content->end_time;
            tmp_content->place=ptr->content->place;
            tmp_content->others=ptr->content->others;
            tmp_content->next=cur;
            cur=tmp_content;
        }
        ptr=ptr->next;
    }
    return cur;
}

Event_content *find_today(Event_date *ptr ,int month,int date){
    Event_date *head = ptr;
    while(head!=NULL){
        if(head->month==month&&head->date==date){
            break;
        }
        head=head->next;
    }
    if(head==NULL){
        return NULL;
    }
    return (head->content);
}

/**************************************priority queue************************************/
struct priority_queue{
    int time;
    Event_content *event;              //put ptr,which stores all info including month, date...
};

int tree_size=0;                      //global variable trace size of tree
struct priority_queue day_tree[100];  //only for the schedule of a day

void swap(struct priority_queue *a,struct priority_queue *b) {  //swap info in day_tree
   struct priority_queue temp=*a;
    *a=*b;
    *b=temp;
}

char *pop(struct  priority_queue *day_tree){   //pop out the smallest time's event name
    Event_content *pop_activity=NULL;                   //store the ptr being popping out
    pop_activity=malloc(sizeof(Event_content));
    if (tree_size<=0) {                               //if tree_size<=0, no more event
        Event_content *no_activity=malloc(sizeof(Event_content));
        no_activity->name=malloc(30);
        no_activity->name="No more activity today.";   //make the string printing out the msg
        pop_activity=no_activity;
        return pop_activity->name;                             //return the msg
    }
    pop_activity=day_tree[0].event;                    //if tree_size!=0, set popping out activity pointing to the first(day_tree[0])'s event
    day_tree[0]=day_tree[tree_size-1];                 //let the last activity=the first
    tree_size--;                                       //minus one so the size shrink one, the last activity got rid
    int currentIndex=0;                                //start comparing from [0]
    while (1) {
        int leftChild=2*currentIndex+1;               //L_child
        int rightChild=2*currentIndex+2;              //R_child
        int smallest=currentIndex;
        if((leftChild<tree_size)&&(day_tree[leftChild].time<day_tree[smallest].time)){  //L_child is not over the bound(size)&&the time of [small] is bigger, then the [small] should "sink" into the lower(further away from being print)
            smallest=leftChild;
        }
        if((rightChild<tree_size)&&(day_tree[rightChild].time<day_tree[smallest].time)){//R_child is not over the bound(size)&&the time of [small] is bigger, then the [small] should "sink" into the lower(further away from being print)
            smallest=rightChild;
        }
        if(smallest!=currentIndex){                               //exchange the content if the former last array is not yet in the right place that is smaller than its children
            swap(&day_tree[currentIndex],&day_tree[smallest]); 
            currentIndex=smallest;
        }else{                                                     //when the the former last is no longer bigger than any of its children
            break;                     
        }
    }
    return pop_activity->name;           //return day_tree[0].event
}

void push(struct priority_queue *day_tree, int strt_time, Event_content *list ){
    if (tree_size>=100) {                 //over the max size of day_tree
        printf("The schedule for today is full.\n");
        return;
    }
    day_tree[tree_size].time=strt_time;              //put info into the empty arr
    day_tree[tree_size].event=list;
    int currentIndex=tree_size;
    int parentIndex=(currentIndex-1)/2;
    while ((currentIndex>0)&&(day_tree[currentIndex].time<day_tree[parentIndex].time)) {  //if time < parent, "float" up into [smaller] arr(cuz [smaller] print out first)
        swap(&day_tree[currentIndex], &day_tree[parentIndex]);
        currentIndex=parentIndex;
        parentIndex=(currentIndex-1)/2;
    }
    tree_size++;                  //tree_size+1
}

void clean_day_tree(){                 //reset the whole day_tree
    for(int i=0;i<100;i++){
        day_tree[i].time=-1;
        day_tree[i].event=NULL;
    }
}

void delete(struct priority_queue *day_tree, int strt_time){  //delete the time of event
    int i;
    for(i=0;i<tree_size;i++) {                               //look for the time
        if (day_tree[i].time==strt_time) {                   //if found
            day_tree[i]=day_tree[tree_size-1];               //set the time to last arr 
            tree_size--;                                     //minus one in size (so the last arr become the former [last-1])
            break;
        }
    }
    if (i==tree_size) {                                       //if not find event at the time
        printf("No event at the time.\n");
    }else{                                                    //do the  "float" and "sink" same as pop and push
        int currentIndex=i;
        int parentIndex=(currentIndex-1)/2;
        if ((currentIndex>0)&&(day_tree[currentIndex].time<day_tree[parentIndex].time)) {     //do same as push when the one being delete is not root, and see if the time replacing the user_enter_time is smaller than parent
            while(currentIndex>0&&day_tree[currentIndex].time<day_tree[parentIndex].time) {
                swap(&day_tree[currentIndex],&day_tree[parentIndex]);                           //keep exchane till bigger than parent
                currentIndex=parentIndex;
                parentIndex=(currentIndex-1)/2;
            }
        } else {                                              //do the same as pop when the one being removed is root
            while(1) {
                int leftChild=2*currentIndex+1;
                int rightChild=2*currentIndex+2;
                int smallest=currentIndex;
                if((leftChild<tree_size)&&(day_tree[leftChild].time<day_tree[smallest].time)){
                    smallest=leftChild;
                }
                if((rightChild<tree_size)&&(day_tree[rightChild].time<day_tree[smallest].time)){
                    smallest=rightChild;
                }
                if(smallest!=currentIndex) {
                    swap(&day_tree[currentIndex],&day_tree[smallest]);
                    currentIndex=smallest;
                } else {
                    break;
                }
            }
        }
    }
    
}

Event_content *add_to_ptrlist(Event_content *list,int str_t,int end_t,char* ac,char *pl,char *ot){  //for testing, put thing in ptr altogather
    Event_content *tmp;
    tmp=malloc(sizeof(Event_content));
    tmp->name=malloc(strlen(ac)+1);  //allocate memory for activity
    tmp->place=malloc(strlen(pl)+1);     //allocate memory for place
    tmp->others=malloc(strlen(ot)+1);    //allocate memory for others
    strcpy(tmp->name,ac);
    tmp->start_time=str_t;
    tmp->end_time=end_t;
    strcpy(tmp->place,pl);
    strcpy(tmp->others,ot);
    return tmp;
}

unsigned int hash(char *name){ //forming correspondence(inpur: name, output: hash value)
    int length = strnlen(name, NAME_SIZE);
    unsigned int hash_value = 0;
    for(int i = 0; i < length; i++){
        hash_value += name[i];
        hash_value = (hash_value * name[i]) % TABLE_SIZE;
    }
    return hash_value;
}

void init_hash_table(){
    for(int i = 0; i < TABLE_SIZE; i++){
        hash_table[i] = NULL;
    }
}

void print_table(){
    printf("Start\n");
    for(int i = 0; i < TABLE_SIZE; i++){
        if(hash_table[i] == NULL){
            printf("\t%d\t---\n", i);
        }else if(hash_table[i] == DELETED_NODE){
            printf("\t%d\t---<deleted>\n", i);
        }else{
            printf("\t%d\t%s\n", i, hash_table[i] -> name);
        }
    }
    printf("End\n");
}

bool hash_table_insert(struct event_content *p){ //
    if(p == NULL){
        return false;
    }
    int index = hash(p -> name); // hash using name in p
    for(int i = 0; i < TABLE_SIZE; i++){
        int try = (i + index) % TABLE_SIZE; //if collision ocurr, try to use linear search to find a space to fit in
        if(hash_table[try] == NULL || hash_table[try] == DELETED_NODE){
            hash_table[try] = p; //if finding a space
            return true; //did find a space
        }
    }
    return false; //did not find a space
}

Event_content *hash_table_lookup(char *name){
    int index = hash(name); // hash using name directly
    //printf("index: %d\n", index);
    for(int i = 0; i < TABLE_SIZE; i++){
        int try = (i + index) % TABLE_SIZE; //if collision ocurr, try to use linear search to find if it found a space to fit in
        if(hash_table[try] == NULL){
            return false;
        }
        if(hash_table[try] == DELETED_NODE){
            continue;
        }
        if(strncmp(hash_table[try] -> name, name, TABLE_SIZE) == 0){
            return hash_table[try]; //if found it, return the node
        }
    }
    return NULL;
}

Event_content *hash_table_delete(char *name){
    int index = hash(name); // hash using name directly
    //printf("index: %d\n", index);
    for(int i = 0; i < TABLE_SIZE; i++){
        int try = (i + index) % TABLE_SIZE; //if collision ocurr, try to use linear search to find if it found a space to fit in
        if(hash_table[try] == NULL){
            return NULL;
        }
        if(hash_table[try] == DELETED_NODE){
            continue;
        }
        if(strncmp(hash_table[try] -> name, name, TABLE_SIZE) == 0){
            struct event_content *tmp = hash_table[try];
            hash_table[try] = DELETED_NODE;
            return tmp; //if found it, return the node
        }
    }
    return NULL;
}
/**************************************priority queue************************************/