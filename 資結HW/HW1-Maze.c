#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<limits.h>

typedef struct position{  //struct position info
    int row;
    int col;
}position;

typedef struct route{     //struct maze info
    int type;             // road or wall
    int num;              // walk by step count
    int last_direction;   // the direction from moving to this node(and is with the smallest step) 
}route;

typedef struct stack_path{ //struct path
    int direction;         //store dir
    struct stack_path *next; //point to next step
}stack_path;

typedef struct go_dir_dis{ //store the the distance of point and dst if go on the dir 
    double distance; //distance of pt to dst
    int direction;   // dir
}go_dir_dis; 

bool P1_not_finish=true,P2_not_finish=true,on_off=true;  // bool finish or not
position tmp_P2; //assign a tmp p2 for pt1 going for dst1
position des1,des2; //assign two tmp dst 
double go_direction_dis[4];  //assign for four direction's distance 

double P2P_distance(int col,int row,position* D){ //calculating for two points' distance
    return fabs(col-D->col)+fabs(row-D->row);
}

void compare(go_dir_dis* list){
    go_dir_dis tmp_store;
    for(int i=0;i<8;i++){
        for(int j=i+1;j<8;j++){
            if(list[i].distance>list[j].distance){
                tmp_store.direction=list[i].direction;
                tmp_store.distance=list[i].distance;
                list[i].direction=list[j].direction;
                list[i].distance=list[j].distance;
                list[j].direction=tmp_store.direction;
                list[j].distance=tmp_store.distance;
            }
        }
    }
}

void find_way_1(int num,position* P1,position* P2,position * D1,position * D2, route*** maze1,route*** maze2){
    go_dir_dis direction_distance[8]; //8 for four direction *2(two dst)
    int four_dir_col[4]={0,1,0,-1},four_dir_row[4]={1,0,-1,0}; //represent the direction to go
    int all_block; //if all road is block
    position next_P1,next_P2;  //tmp store next pos
    for(int i=0;i<4;i++){  // loop for foour direction
        if(((*maze1)[P1->col+four_dir_col[i]][P1->row+four_dir_row[i]].type==0)){ //if the node is road
            //store the distance to dst1 and dir
            direction_distance[i].distance=P2P_distance(P1->col+four_dir_col[i],P1->row+four_dir_row[i],D1);
            direction_distance[i].direction=i;
            //store the distance to dst2 and dir
            direction_distance[i+4].distance=P2P_distance(P1->col+four_dir_col[i],P1->row+four_dir_row[i],D2);
            direction_distance[i+4].direction=i;
        }else{ //if is wall
            direction_distance[i].distance=INT_MAX;
            direction_distance[i].direction=-1;
            direction_distance[i+4].distance=INT_MAX;
            direction_distance[i+4].direction=-1;
        }
    }
    compare(direction_distance); //compare to see which distance is the smallest of the 8
    if((!((P1->col==D1->col&&P1->row==D1->row)||(P1->col==D2->col&&P1->row==D2->row)))&&on_off){ //go if not yet any dst is reached
        int strt=0;
        //do dfs recursion
        while(strt<8&&on_off){
            switch(direction_distance[strt].direction){
                case 1:  //go right
                    if((((*maze1)[P1->col][P1->row].num+1<(*maze1)[P1->col+1][P1->row].num))||((*maze1)[P1->col+1][P1->row].num==-1)){ //if not yet been or this is shorter
                        //use next_P to store now place
                        next_P1.col=P1->col;
                        next_P1.row=P1->row;
                        next_P2.col=P2->col;
                        next_P2.row=P2->row;
                        next_P1.col++;
                        if((*maze2)[P2->col+1][P2->row].type==0){ //do the movement of p2
                                next_P2.col++;
                        }
                        (*maze1)[P1->col+1][P1->row].num=(*maze1)[P1->col][P1->row].num+1; //step+1
                        (*maze1)[P1->col+1][P1->row].last_direction=1; //store dir
                        find_way_1(num,&next_P1,&next_P2,D1,D2,maze1,maze2);//do recursion
                        strt++;//this way can't reach dst, so do other dir
                        break;
                    }else{//this way has shorter path/ has gone before but not reach dst , so don't go ->block it
                        all_block=1;
                        for(int i=0;i<4;i++){
                            if(((*maze1)[P1->col+four_dir_col[i]][P1->row+four_dir_row[i]].num==-1)){//check if hve way to go
                                all_block=0;
                            }
                        }
                        if(all_block){// if all block change the road to wall so not to go again
                            (*maze1)[P1->col][P1->row].type=1;
                           (*maze2)[P1->col][P1->row].type=1;
                            return;
                        }else{
                            strt++;
                            break;
                        }
                    }
                case 3: //go left
                    if(((((*maze1)[P1->col][P1->row].num+1)<((*maze1)[P1->col-1][P1->row].num)))||((*maze1)[P1->col-1][P1->row].num==-1)){//if not yet been or this is shorter path
                        //use next_P to store now place
                        next_P1.col=P1->col;
                        next_P1.row=P1->row;
                        next_P2.col=P2->col;
                        next_P2.row=P2->row;
                        next_P1.col--;
                        if((*maze2)[P2->col-1][P2->row].type==0){//do the movement of p2
                                next_P2.col--;
                        }
                        (*maze1)[P1->col-1][P1->row].num=(*maze1)[P1->col][P1->row].num+1;//step+1
                        (*maze1)[P1->col-1][P1->row].last_direction=3; //store dir
                        find_way_1(num,&next_P1,&next_P2,D1,D2,maze1,maze2); //do recursion
                        strt++; //this way can't reach dst, so do other dir
                        break;
                    }else{//this way has shorter path/ has gone before but not reach dst , so don't go ->block it
                        all_block=1;
                        for(int i=0;i<4;i++){
                            if(((*maze1)[P1->col+four_dir_col[i]][P1->row+four_dir_row[i]].num==-1)){//check if hve way to go
                                all_block=0;
                            }
                        }
                        if(all_block){// if all block change the road to wall so not to go again
                            (*maze1)[P1->col][P1->row].type=1;
                           (*maze2)[P1->col][P1->row].type=1;
                            return;
                        }else{
                            strt++;
                            break;
                        }
                    }
                case 0: //go up
                    if((((*maze1)[P1->col][P1->row].num+1<(*maze1)[P1->col][P1->row+1].num))||((*maze1)[P1->col][P1->row+1].num==-1)){//if not yet been or this is shorter path
                        //use next_P to store now place
                        next_P1.col=P1->col;
                        next_P1.row=P1->row;
                        next_P2.col=P2->col;
                        next_P2.row=P2->row;
                        next_P1.row++;
                        if((*maze2)[P2->col][P2->row+1].type==0){//do the movement of p2
                                next_P2.row++;
                        }
                        (*maze1)[P1->col][P1->row+1].num=(*maze1)[P1->col][P1->row].num+1;//step+1
                        (*maze1)[P1->col][P1->row+1].last_direction=0; //store dir
                        find_way_1(num,&next_P1,&next_P2,D1,D2,maze1,maze2);//do recursion
                        strt++;//this way can't reach dst, so do other dir
                        break;
                    }else{ //this way has shorter path/ has gone before but not reach dst , so don't go ->block it
                        all_block=1;
                        for(int i=0;i<4;i++){
                            if(((*maze1)[P1->col+four_dir_col[i]][P1->row+four_dir_row[i]].num==-1)){ //check if hve way to go
                                all_block=0;
                            }
                        }
                        if(all_block){ // if all block change the road to wall so not to go again
                            (*maze1)[P1->col][P1->row].type=1;
                           (*maze2)[P1->col][P1->row].type=1;
                            return;
                        }else{
                            strt++;
                            break;
                        }
                    }
                case 2: //go down
                    if((((*maze1)[P1->col][P1->row].num+1<(*maze1)[P1->col][P1->row-1].num))||((*maze1)[P1->col][P1->row-1].num==-1)){//if not yet been or this is shorter path
                        //use next_P to store now place
                        next_P1.col=P1->col;
                        next_P1.row=P1->row;
                        next_P2.col=P2->col;
                        next_P2.row=P2->row;
                        next_P1.row--;
                        if((*maze2)[P2->col][P2->row-1].type==0){//do the movement of p2
                                next_P2.row--;
                        }
                        (*maze1)[P1->col][P1->row-1].num=(*maze1)[P1->col][P1->row].num+1;//step+1
                        (*maze1)[P1->col][P1->row-1].last_direction=2; //store dir
                        find_way_1(num,&next_P1,&next_P2,D1,D2,maze1,maze2);//do recursion
                        strt++;//this way can't reach dst, so do other dir
                        break;
                    }else{//this way has shorter path/ has gone before but not reach dst , so don't go ->block it
                        all_block=1;
                        for(int i=0;i<4;i++){
                            if(((*maze1)[P1->col+four_dir_col[i]][P1->row+four_dir_row[i]].num==-1)){//check if hve way to go
                                all_block=0;
                            }
                        }
                        if(all_block){// if all block change the road to wall so not to go again
                            (*maze1)[P1->col][P1->row].type=1;
                           (*maze2)[P1->col][P1->row].type=1;
                            return;
                        }else{
                            strt++;
                            break;
                        }
                    }
                default:
                    if(!on_off){
                        return;
                    }
            }
        }
    }else{ // if some point already reach dst
        on_off=false;
        tmp_P2.col=P2->col;
        tmp_P2.row=P2->row;
        if(P1->col==D1->col&&P1->row==D1->row){  //if is pt1 and dst1
            des1.col=D1->col;
            des1.row=D1->row;
            des2.col=D2->col;
            des2.row=D2->row;
        }else{ // if is not, switch two dst for next find way
            des1.col=D2->col;
            des1.row=D2->row;
            des2.col=D1->col;
            des2.row=D1->row;
        }
        return;
    }
}

//P2 go to the other dst (similar to find_way_1)
void find_way_2(int num,position* P2,route*** maze2){
    position next_P2;
    next_P2.col=tmp_P2.col;
    next_P2.row=tmp_P2.row;
    if(P2->col==des2.col&&P2->row==des2.row){
        return;
    }else{
        if(((*maze2)[P2->col][P2->row].type==0)&&(*maze2)[P2->col+1][P2->row].type==0){
            if(((*maze2)[P2->col][P2->row].num+1<(*maze2)[P2->col+1][P2->row].num)||((*maze2)[P2->col+1][P2->row].num==-1)){
                next_P2.col=P2->col;
                next_P2.row=P2->row;
                next_P2.col++;
                (*maze2)[P2->col+1][P2->row].num=(*maze2)[P2->col][P2->row].num+1;
                (*maze2)[P2->col+1][P2->row].last_direction=1;
                find_way_2(num,&next_P2,maze2);
            }
        }
        if(((*maze2)[P2->col][P2->row].type==0)&&(*maze2)[P2->col-1][P2->row].type==0){
            if(((*maze2)[P2->col][P2->row].num+1<(*maze2)[P2->col-1][P2->row].num)||((*maze2)[P2->col-1][P2->row].num==-1)){
                next_P2.col=P2->col;
                next_P2.row=P2->row;
                next_P2.col--;
                (*maze2)[P2->col-1][P2->row].num=(*maze2)[P2->col][P2->row].num+1;
                (*maze2)[P2->col-1][P2->row].last_direction=3;
                find_way_2(num,&next_P2,maze2);
            }
        }
        if(((*maze2)[P2->col][P2->row].type==0)&&(*maze2)[P2->col][P2->row+1].type==0){
            if(((*maze2)[P2->col][P2->row].num+1<(*maze2)[P2->col][P2->row+1].num)||((*maze2)[P2->col][P2->row+1].num==-1)){
                next_P2.col=P2->col;
                next_P2.row=P2->row;
                next_P2.row++;
                (*maze2)[P2->col][P2->row+1].num=(*maze2)[P2->col][P2->row].num+1;
                (*maze2)[P2->col][P2->row+1].last_direction=0;
                find_way_2(num,&next_P2,maze2);
            }
        }
        if(((*maze2)[P2->col][P2->row].type==0)&&(*maze2)[P2->col][P2->row-1].type==0){
            if(((*maze2)[P2->col][P2->row].num+1<(*maze2)[P2->col][P2->row-1].num)||((*maze2)[P2->col][P2->row-1].num==-1)){
                next_P2.col=P2->col;
                next_P2.row=P2->row;
                next_P2.row--;
                (*maze2)[P2->col][P2->row-1].num=(*maze2)[P2->col][P2->row].num+1;
                (*maze2)[P2->col][P2->row-1].last_direction=2;
                find_way_2(num,&next_P2,maze2);
            }
        }
    }
}

stack_path* add_to_list(stack_path*list,int num){  //add to list in stack (FILO)
    stack_path* tmp;
    tmp=malloc(sizeof(stack_path));
    tmp->direction=num;
    tmp->next=list;
    return tmp;
}

//use the the dir store in maze to list the path to go from strt to dst (two dst's path connected)
stack_path* find_path(position* P1,position* P2,position *D1,position *D2, route***maze1,route***maze2,stack_path*path){
    position now;
    now.col=D2->col;
    now.row=D2->row;
    while(!(now.col==P2->col&&now.row==P2->row)){
        switch((*maze2)[now.col][now.row].last_direction){
            case 0: 
                path=add_to_list(path,0);
                now.row--;
                break;
            case 1:
                path=add_to_list(path,1);
                now.col--;
                break;
            case 2:
                path=add_to_list(path,2);
                now.row++;
                break;
            case 3:
                path=add_to_list(path,3);
                now.col++;
                break;
        }
    }
    now.col=D1->col;
    now.row=D1->row;
    while(!(now.col==P1->col&&now.row==P1->row)){
        switch((*maze1)[now.col][now.row].last_direction){
            case 0: 
                path=add_to_list(path,0);
                now.row--;
                break;
            case 1:
                path=add_to_list(path,1);
                now.col--;
                break;
            case 2:
                path=add_to_list(path,2);
                now.row++;
                break;
            case 3:
                path=add_to_list(path,3);
                now.col++;
                break;
        }
    }
return path;
}

int main(){
    int num;
    scanf("%d",&num);    //scan maze size
    route **maze1,**maze2;  //assign two maze for two dif points
    //malloc spaces
    maze1=malloc(num*sizeof(route*));  
    maze2=malloc(num*sizeof(route*));
    for(int i=0;i<num;i++) {
        maze1[i]=malloc(num*sizeof(route));
        maze2[i]=malloc(num*sizeof(route));
    }
    //initiallize all nodes
    for(int row=num-1;row>=0;row--){
        for(int col=0;col<num;col++){
            scanf("%d",&maze1[col][row].type);
            maze2[col][row].type=maze1[col][row].type; //type (wall or road)
            if(maze1[col][row].type==0){ //if is road
                maze1[col][row].num=-1; //set step num to -1
                maze2[col][row].num=-1;
                maze1[col][row].last_direction=-2; //set last direction to -2
                maze2[col][row].last_direction=-2;
            }else{  //if is wall
                maze1[col][row].num=0;  //set step num to 0
                maze2[col][row].num=0;
                maze1[col][row].last_direction=-1; //set last direction to -1
                maze2[col][row].last_direction=-1;
            }
            
        }
    }
    position S1,S2,D1,D2;
    //scan scr and dst
    scanf("%d %d %d %d",&S1.col,&S1.row,&S2.col,&S2.row);
    scanf("%d %d %d %d",&D1.col,&D1.row,&D2.col,&D2.row);
    //set two scr points' positions' step num & last directions to 0 
    maze1[S1.col][S1.row].num=0;
    maze1[S1.col][S1.row].last_direction=0;
    find_way_1(num,&S1,&S2,&D1,&D2,&maze1,&maze2); //let a point get to one dst first
    //set the point not yet gone to a dst point the position it is in after the other point have reached the goal
    S2.col=tmp_P2.col; 
    S2.row=tmp_P2.row;
    //initial the strt point's info (maze2)
     maze2[S2.col][S2.row].num=0; 
     maze2[S2.col][S2.row].last_direction=0;
    find_way_2(num,&S2,&maze2); //find the way of S2 to dst
    stack_path *path=NULL; //initialize stack_path
    //rest the dsts
    D1.col=des1.col;
    D1.row=des1.row;
    D2.col=des2.col;
    D2.row=des2.row;
    path=find_path(&S1,&S2,&D1,&D2,&maze1,&maze2,path); //find the roate
    //print the roate
    while(path!=NULL){
        printf("%d",path->direction);
        path=path->next;
    }
    printf("\n");
    //free mazes
    for (int i=0;i<num;i++) {
        free(maze1[i]);
        free(maze2[i]);
    }
    free(maze1);
    free(maze2);
    return 0;
}
