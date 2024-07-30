#include<stdio.h>
#include<stdbool.h>
int money=100,time=15,price=30,earn,speed=50,flavor=100,see_area,choose,keep_on=1;
int area[5],lottery_open_close[900],booster_num[3]={0},booster_open_close[3]={0},booster_affect[2]={1,1};
int lottery_choice,free_choice=0,open,k=1,a=3,choice_money=0,e,b,c,d,f,x=3,s,n=3;
int area_time[5]={15,15,15,15,15};
int area_price[5]={30,30,30,30,30};
int switch_d(),lottery_prize_determine(int);
void open_another(),graph(),see_if_larger(),invalid_input_in_lottery();

int main(){
    for(int t=0;t<9;t++){
    lottery_open_close[t]=1;
    }
    // game begins
    printf("Welcome, young boss!\n");
    // each day begins
    while(keep_on==1){
        earn=0;
        printf("Chop chop, It's dawn.\n");
        printf("You have %d dollars.\n",money);
        printf("You need %d minutes to make a hotdog.\n",time);
        printf("The price of a hotdog is $%d.\n",price);
        printf("You have %d speed booster(s), %d price booster(s), %d area booster(s).\n",booster_num[0],booster_num[1],booster_num[2]);
        // choose booster
        choose=0;
        while(choose!=4){
            printf("Open/Close boosters:\n");
            if(booster_open_close[0]==1){              //if booster=1=open,if booster=0=close
                printf("  [1] Speed booster (now open)\n");
            }else{
                printf("  [1] Speed booster (now close)\n");
            }
            if(booster_open_close[1]==1){
                printf("  [2] Price booster (now open)\n");
            }else{
                printf("  [2] Price booster (now close)\n");
            }
            if(booster_open_close[2]==1){
                printf("  [3] Area booster (now open)\n");
            }else{
                printf("  [3] Area booster (now close)\n");
            }
            printf("  [4] Finish\n");
            printf("Enter the number(s): ");
            scanf("%d",&choose);
            if(choose>4||choose<1){
                printf("Invalid input!!!!\n");
                continue;
            }
            if(choose==4){                     //if choose=4,end booster open or not question
                break;
            }
            for(int i=0,j=1;j<=3;i++,j++){      //determine the open close of booster 
                if(choose==j){                     //if j match choose
                    if(booster_open_close[i]==1){    //open into close
                        booster_open_close[i]=0;
                    }else{                           //close into open
                        booster_open_close[i]=1;
                    }
                }
            }
        }
        // choose action
        booster_affect[0]=1;            //speed booster unactivate set effect one
        booster_affect[1]=1;             //price booster unactivate set effect one
        for(int i=0;i<3;i++){
            if(booster_open_close[i]==1){       //if booster is open
                 if(booster_num[i]==0){          //if there are no boosters to use
                    booster_open_close[i]=0;     //set booster closed
                 }
            }
        }
        for(int i=0;i<2;i++){
            if(booster_open_close[i]==1){         //if booster is open
                booster_num[i]--;
                booster_affect[i]=2;              //set  booster effect to two
            }
        }
        if(booster_open_close[2]==1){             //if area booster is open
            booster_num[2]--;
        }
        printf("Actions you can take for each area:\n");
        printf("  [1] Sell the hotdogs\n");
        printf("  [2] Improve your cooking speed\n");
        printf("      (- $%d, - $%d, - $%d, - $%d for next four upgrades)\n",speed,speed*2,speed*4,speed*8);
        printf("  [3] Improve your hotdog flavor\n");
        printf("      (- $%d, - $%d, - $%d, - $%d for next four upgrades)\n",flavor,flavor*2,flavor*4,flavor*8);
        printf("Enter the number(s): "); 
            scanf("%d %d %d %d",&area[0],&area[1],&area[2],&area[3]);
            for(int i=0;i<4;i++){
                while(area[i]<1||area[i]>3){   // re-enter action
                    printf("Invalid input!!!!\n");
                    printf("Actions you can take at Area %d:\n",i+1);
                    printf("  [1] Sell the hotdogs\n");
                    printf("  [2] Improve your cooking speed\n");
                    printf("  [3] Improve your hotdog flavor\n");
                    printf("Enter the number(s): ");
                    scanf("%d",&area[i]);
                }
            }
        for(int i=0;i<4;i++){
            if(area[i]==1){   //sell hotdog
                area_time[i]=time;
                area_price[i]=price;
                money+=(180/time)*booster_affect[0]*(price*booster_affect[1]);
                earn+=(180/time)*booster_affect[0]*(price*booster_affect[1]);
                continue;
            }else if(area[i]==2){
                if(money<speed){   //not enough money-->sell hotdog
                    area[i]=4;  //set four for no money
                    area_time[i]=time;
                    area_price[i]=price;
                    money+=(180/time)*booster_affect[0]*(price*booster_affect[1]);
                    earn+=(180/time)*booster_affect[0]*(price*booster_affect[1]);
                    continue;
                }else if(time==1){   //time=1-->sell hotdog
                    area[i]=5; //set five for too fast
                    area_time[i]=time;
                    area_price[i]=price;
                    money+=(180/time)*booster_affect[0]*(price*booster_affect[1]);
                    earn+=(180/time)*booster_affect[0]*(price*booster_affect[1]);
                    continue;
                }
                time-=1;                //speed up
                area_time[i]=time;      //store the time at the moment
                area_price[i]=price;     //store the price at the moment
                money-=speed;
                speed*=2;
                continue;
            }else if(area[i]==3){
                if(money<flavor){   //set four for no money
                    area[i]=4;
                    area_time[i]=time;      //store the time at the moment
                    area_price[i]=price;     //store the price at the moment
                    money+=(180/time)*booster_affect[0]*(price*booster_affect[1]);
                    earn+=(180/time)*booster_affect[0]*(price*booster_affect[1]);
                    continue;
                }
                price+=10;           //price up
                area_time[i]=time;     //store the time at the moment
                area_price[i]=price;     //store the price at the moment
                money-=flavor;
                flavor*=2;
                continue;
            }
        }
        if(booster_open_close[2]==1){        //if area booster is open
            area_time[4]=time;        //store the time at the moment
            area_price[4]=price;     //store the price at the moment
            area[4]=1;
            money+=(180/time)*booster_affect[0]*(price*booster_affect[1]);
            earn+=(180/time)*booster_affect[0]*(price*booster_affect[1]);
        }
      
        // end of actions a day
        printf("Well done, you earn $%d today.\n",earn);
        while(1){
            printf("Which result of the area you want to check?\n");
            printf("  [1] Area 1\n");
            printf("  [2] Area 2\n");
            printf("  [3] Area 3\n");
            printf("  [4] Area 4\n");
            if(booster_open_close[2]==1){  // if area booster open
                    printf("  [5] Area 5\n");
                    printf("  [6] Done\n");
                }else{
                    printf("  [5] Done\n");
                }
            printf("Enter the number(s): ");
            scanf("%d",&see_area);
            if(booster_open_close[2]==1){  // if area booster open
                if(see_area>6||see_area<1){
                    printf("Invalid input!!!!\n");
                    continue;
                }
                if(see_area==6){
                        break;
                }
            }else{
                if(see_area>5||see_area<1){
                    printf("Invalid input!!!!\n");
                    continue;
                }
                if(see_area==5){
                        break;
                }
            }
            switch(area[see_area-1]){
                case 1:  // sell hotdogs
                        printf("You make %d hotdogs here!\n",(180/(area_time[see_area-1]))*booster_affect[0]);
                        printf("You earn $%d!\n",(180/(area_time[see_area-1]))*booster_affect[0]*((area_price[see_area-1])*booster_affect[1]));
                        break;
                case 2:  // speed upgrade
                        printf("You glimpse the secret of wind.\n");
                        printf("Your hands can move faster now.\n");
                        break;
                case 3:  // taste upgrade
                        printf("You feel the soul of the ingredients.\n");
                        printf("Your hotdogs are more appetizing now.\n");
                        break;
                case 4:  // not enough money
                        printf("Can't you tell how poor you are?\n");
                        printf("Go vending your hotdogs instead of thinking about self-improvement!\n");
                        printf("You make %d hotdogs here!\n",(180/(area_time[see_area-1]))*booster_affect[0]);
                        printf("You earn $%d!\n",(180/(area_time[see_area-1]))*booster_affect[0]*((area_price[see_area-1])*booster_affect[1]));
                        break;
                case 5:  // cannot reduce any time
                        printf("Do you want to travel through time?\n");
                        printf("GO WORK!!\n");
                        printf("You make %d hotdogs here!\n",(180/(area_time[see_area-1]))*booster_affect[0]);
                        printf("You earn $%d!\n",(180/(area_time[see_area-1]))*booster_affect[0]*((area_price[see_area-1])*booster_affect[1]));
                        break;
            }
        }
        // continue or not
        while(1){
            printf("Do you want to continue or exit?\n");
            printf("  [1] Continue\n");
            printf("  [2] Exit\n");
            printf("Enter the number(s): ");
            scanf("%d",&keep_on);
            if(keep_on!=1&&keep_on!=2){
                printf("Invalid input!!!!\n");
            }else{
                break;
            }
        }
       if(keep_on==1){
           printf("You get one free choice.\n"); // continue, so get one free choice
            see_if_larger(n);
            graph();
            printf("\n");
            printf("You can choose\n");  // choose cell on the lottery table   
            printf("  [number on cell] to open (- $%d)\n",0);    // if free choice remains
            printf("  [0] to continue the game\n");
            printf("Enter the number(s): ");
            scanf("%d",&lottery_choice);
            invalid_input_in_lottery();
            if(lottery_choice==0){   //if don't use free choice, store the chance
                free_choice++;
                continue;
            }
            d=lottery_prize_determine(lottery_choice);
                while(switch_d(d)!=0){}   //determinr prize type
                if(lottery_choice==0){
                    continue;
                }
                see_if_larger(n);
                graph();
                printf("\n");
                see_if_larger(n);  
        while(lottery_choice!=0){        
            printf("You can choose\n");  // choose cell on the lottery table   
            if(free_choice>=1){
                printf("  [number on cell] to open (- $%d)\n",0);    // if free choice remains
                printf("  [0] to continue the game\n");
                printf("Enter the number(s): ");
            }else{
                choice_money+=500; 
                printf("  [number on cell] to open (- $%d)\n",choice_money);    // if no free choice remains
                printf("  [0] to continue the game\n");
                printf("Enter the number(s): ");
            }
            scanf("%d",&lottery_choice);
            
            invalid_input_in_lottery();
            if(lottery_choice==0&&free_choice>=1){   //if no pick cell,no change on free choice num
                    break;
            }else if(lottery_choice==0&&free_choice==0){  //if no pick cell subtract the plus 500
                choice_money-=500;
                break;
            }
            if(free_choice>=1){             //if pick cell minus a free choice
                free_choice--;
            }else if(money<choice_money){    //if no money to pick , sustract the plus 500
                choice_money-=500;
                printf("You have no money!\n");  //if no money, break out to the next day
                break;
            }else{
                 money-=choice_money;           //if no free choice, sustract the pay
            }
                d=lottery_prize_determine(lottery_choice);
                while(switch_d(d)!=0){} //determine the prize type
                if(lottery_choice==0){
                    break;
                }
                see_if_larger();
                graph();
                printf("\n");
                see_if_larger(n);
        }
          }else{
            printf("We will miss you. Bye!");  //choose two to end the game
            break;
          }
    }
    return 0;
}
int lottery_prize_determine(int lottery_choice){
    lottery_open_close[lottery_choice-1]=0;
    if(lottery_choice<10){                       //determine prize type
        d=(lottery_choice%9)+1;
    }else if(lottery_choice<100&&lottery_choice>10){
        b=(lottery_choice/10);
        c=(lottery_choice-10*b);
        d=((c*16+b*1)%9)+1;
    }else if(lottery_choice<1000&&lottery_choice>100){
        e=(lottery_choice/100);
        b=((lottery_choice-100*e)/10);
        c=(lottery_choice-100*e-10*b);
        d=((c*16*16+b*16*+e*1)%9)+1;
    }else if(lottery_choice<10000&&lottery_choice>1000){
        f=(lottery_choice/1000);
        e=((lottery_choice-1000*f)/100);
        b=((lottery_choice-1000*f-100*e)/10);
        c=(lottery_choice-1000*f-100*e-10*b);
        d=((c*16*16*16+b*16*16+e*16+f*1)%9)+1;
    }
    return d;
}
void open_another(){
    printf("Another open on %d!\n",open);
    lottery_open_close[open-1]=0;
    if(open<10){
        d=(open%9)+1;
    }else if(open<100&&open>10){
        b=(open/10);
        c=(open-10*b);
        d=((c*16+b*1)%9)+1;
    }else if(open<1000&&open>100){
        e=(open/100);
        b=((open-100*e)/10);
        c=(open-100*e-10*b);
        d=((c*16*16+b*16*+e*1)%9)+1;
    }else if(open<10000&&open>1000){
        f=(open/1000);
        e=((open-1000*f)/100);
        b=((open-1000*f-100*e)/10);
        c=(open-1000*f-100*e-10*b);
        d=((c*16*16*16+b*16*16+e*16+f*1)%9)+1;
    }
    switch_d(d);
}
void graph(){
    for(int i=0,k=1,a=n;i<n;i++,a+=n){
        printf("+");
        for(int j=0;j<n;j++){         //graph first line
            for(int q=0;q<x;q++){
                printf("-");
            }
            printf("+"); 
        }
        printf("\n|");
            for(;k<=a;k++){
                if(lottery_open_close[k-1]==0){
                    if(n*n>1000){
                        printf("    %c |",'x');     //if open print x
                    }else if(n*n>100){
                        printf("   %c |",'x');
                    }else if(n*n>10){
                        printf("  %c |",'x');
                    }else if(n*n<10){
                        printf(" %c |",'x');
                    }
                }else{ 
                    if(n*n>1000){               //if not open print num
                        printf("%5.d |",k);
                    }else if(n*n>100){
                        printf("%4.d |",k);
                    }else if(n*n>10){           
                        printf("%3.d |",k);
                    }else if(n*n<10){
                        printf("%2.d |",k);
                    }
                }
            }
    printf("\n");
    }
    printf("+");                          //graph last line
    for(int j=0;j<n;j++){
        for (int p=0;p<x;p++){
            printf("-");
        }
            printf("+"); 
    }
}

void see_if_larger(){
    for(int z=0;z<n*n;z++){
        if(lottery_open_close[z]==1){            //if there is any cell not open,break out
            break;
        }else if(z==((n*n)-1)){                   //if all cell were opened,expend the size of lottery
            n+=2;
            if(n*n>1000){                     //the num of "-"
                x=6;
            }else if(n*n>100){
                x=5;
            }else if(n*n>10){
                x=4;
            }
            choice_money=0;                  //choice money refresh
            for(int t=0;t<n*n;t++){
                lottery_open_close[t]=1;          //set all cell unopen
            }
            break;
        }
    }
}
int switch_d(){
switch(d){
        case 1:
                printf("Fortune, fortune! You get $%d!\n",100*price);
                money+=100*price;
                d=0;
                break;
        case 2:
                printf("You get an extra choice!\n");
                see_if_larger(n);
                graph();
                printf("\n");
                see_if_larger(n);
                printf("You can choose\n");  // choose cell on the lottery table   
                printf("  [number on cell] to open (- $%d)\n",0);    //  free choice
                printf("  [0] to continue the game\n");
                printf("Enter the number(s): ");
                scanf("%d",&lottery_choice);
                while(lottery_choice<0||lottery_choice>n*n||(lottery_choice!=0&&lottery_open_close[lottery_choice-1]==0)){
                    printf("Invalid input!!!!\n");
                    see_if_larger(n);
                    graph();
                    printf("\n");
                    printf("You can choose\n");   
                    printf("  [number on cell] to open (- $%d)\n",0);    
                    printf("  [0] to continue the game\n");
                    printf("Enter the number(s): ");
                    scanf("%d",&lottery_choice);
                }
                if(lottery_choice==0){
                    free_choice++;
                    d=0;
                    break;
                }
                d=lottery_prize_determine(lottery_choice);
                switch_d(d);
                break;
        case 3:
                if(lottery_choice-n<1){                             //determine prize type
                    open=(lottery_choice+n*(n-1));
                    lottery_choice=open;
                }else{
                    open=lottery_choice-n;
                    lottery_choice=open;
                }
                if(lottery_open_close[open-1]==0){
                    printf("Bad Luck :(\n");
                    d=0;
                }else{
                    open_another();
                }
                break;
        case 4: 
                if(lottery_choice+n>n*n){                             //determine prize type
                    open=(lottery_choice-(n*(n-1)));
                    lottery_choice=open;
                }else{
                    open=lottery_choice+n;
                    lottery_choice=open;
                }
                if(lottery_open_close[open-1]==0){
                    printf("Bad Luck :(\n");
                    d=0;
                }else{
                     open_another();
                }
                break;
        case 5:
                if((lottery_choice-1)%n==0){                             //determine prize type
                    open=(lottery_choice+n-1);
                    lottery_choice=open;
                }else{
                    open=lottery_choice-1;
                    lottery_choice=open;
                }
                if(lottery_open_close[open-1]==0){
                    printf("Bad Luck :(\n");
                    d=0;
                }else{
                    open_another();
                }
                break;
        case 6:
                if((lottery_choice%n==0)){                             //determine prize type
                    open=(lottery_choice-n+1);
                    lottery_choice=open;
                }else{
                    open=lottery_choice+1;
                    lottery_choice=open;
                }
                if(lottery_open_close[open-1]==0){
                    printf("Bad Luck :(\n");
                    d=0;
                }else{
                     open_another();
                }
                break;
        case 7:
                printf("You get a booster!!\n");
                booster_num[0]++;
                d=0;
                break;
        case 8:
                printf("You get a booster!!\n");
                booster_num[1]++;
                d=0;
                break;
        case 9:
                printf("You get a booster!!\n");
                booster_num[2]++;
                d=0;
                break;
    }
    return d;
}          
void invalid_input_in_lottery(){
    while(lottery_choice<0||lottery_choice>n*n||(lottery_choice!=0&&lottery_open_close[lottery_choice-1]==0)){
        printf("Invalid input!!!!\n");
        see_if_larger(n);
        graph();
        printf("\n");
        printf("You can choose\n");  // choose cell on the lottery table   
        printf("  [number on cell] to open (- $%d)\n",choice_money);    // if free choice remains
        printf("  [0] to continue the game\n");
        printf("Enter the number(s): ");
        scanf("%d",&lottery_choice);
    }
}