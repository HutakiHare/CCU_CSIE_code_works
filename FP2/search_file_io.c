#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define A_DAY_HOUR 24

/*************************************test*******************************************/
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

/**************************************test*********************************************/

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

void search_if_have_activity(struct node *list,int time){       //search if have things to do at the time
    while(list!=NULL){
        if(list->strt_time<=time&&list->end_time>=time){                              //if there's thing to do at the time
            printf("You have %s at %d",list->activity,time);            //print the activity name
            break;
        }else{
            list=list->next;                                    //if not find yet, go on the next
        }
    }
    if(list==NULL){
        printf("You don't have activity at %d\n",time);          // if not find an activity for the whole list
    }
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

void search_all_day_free_time(struct node *ptr){                //consider ptr sorted
    int free_time_str=0;
    struct node *tmp=ptr;
    while((tmp!=NULL)){
        if(free_time_str<(ptr->strt_time)){                     //if free time is smaller than the first activity's star time, it is free    
            printf("You have free time from %d to %d.\n"
            ,free_time_str,ptr->strt_time);                     //have the free time till the closest activity start
        }
        free_time_str=ptr->end_time;                            //set the free time start count point at the end of the activity
        tmp=tmp->next;                                          //go to the next closest activity
    }
    if(free_time_str<A_DAY){
        printf("You have free time from %d to %d.\n"
        ,free_time_str,A_DAY);                                  //if no more activity, it is free till 24
    }       
}

void search_scheduled_time_through_activity(struct node *list,char *activity){
     bool have_the_activity_or_not=1;
     while(list!=NULL){
        if(strcmp(list->activity,activity)==0){                 //if there's this activity in schedule
            printf("You have the activity from %d to %d"
            ,list->strt_time,list->end_time);                   //print the activity name and time
            have_the_activity_or_not=0;
        }
        list=list->next;                                        //if not find yet, go on the next
    }
    if(have_the_activity_or_not){
        printf("You don't have activity today\n");              // if not find an activity for the whole list
    }
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
/**************************************priority queue************************************/

/****************************************test********************************************/
int main(){                                                                     //test for file i/o
    struct node *ptr=NULL,*rec=malloc(sizeof(struct node));
    rec->next=NULL;
    ptr=add_to_ptrlist(ptr,5,5,1,1,9,20,"walk sheep","ROS","bring gress");
    ptr=add_to_ptrlist(ptr,7,2,1,1,8,16,"take test","hell","bring pen");
    write_content_on_file(ptr);
    get_content_from_file(rec);
    while(rec->next!=NULL){
        printf("%d/%d %d %d %d %d %s %s %s\n",rec->strt_month,\
        rec->strt_day,rec->whole_day,rec->alert,rec->strt_time,rec->end_time\
        ,rec->activity,rec->place,rec->others);
        rec=rec->next;
    }
    return 0;
}

int main() {                                                                 //test for priority queue
    struct node *ptr=NULL,*rec=malloc(sizeof(struct node));
    rec->next=NULL;
    ptr=add_to_ptrlist(ptr,5,5,1,1,9,20,"walk sheep","ROS","bring gress");
    push(day_tree,ptr->strt_time,ptr);
    ptr=add_to_ptrlist(ptr,7,2,1,1,8,16,"take test","hell","bring pen");
    push(day_tree,ptr->strt_time,ptr);
    struct node *popped=pop(day_tree);
    printf("%s\n",popped->activity);
    ptr=add_to_ptrlist(ptr,7,2,1,1,8,16,"take test","hell","bring pen");
    push(day_tree,ptr->strt_time,ptr);
    delete(day_tree,9);
    popped=pop(day_tree);
    printf("%s\n",popped->activity);
    popped=pop(day_tree);
    printf("%s\n",popped->activity);
    popped=pop(day_tree);
    printf("%s\n",popped->activity);
    free(popped->activity);
    free(popped->place);
    free(popped->others);
    free(popped);
    free(rec);
    return 0;
} 
/****************************************test*************************************************/