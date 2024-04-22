#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct node{
    int strt_month;
    int strt_day;
    bool whole_day;
    bool alert;
    int strt_time;
    int end_time;
    char *activity;
    char *place;
    char *others;
    struct node *next;
};

struct priority_queue{
    int time;
    struct node *event;
};

int tree_size=0;
struct priority_queue day_tree[100];

void swap(struct priority_queue *a,struct priority_queue *b) {
   struct priority_queue temp=*a;
    *a=*b;
    *b=temp;
}

struct node *pop(struct  priority_queue *day_tree){
    struct node *pop_activity=NULL;
    pop_activity=malloc(sizeof(struct node));
    if (tree_size<=0) {
        struct node *no_activity=malloc(sizeof(struct node));
        no_activity->activity=malloc(30);
        no_activity->activity="No more activity today.";
        pop_activity=no_activity;
        return pop_activity;
    }
    pop_activity=day_tree[0].event;
    day_tree[0]=day_tree[tree_size-1];
    tree_size--;
    int currentIndex=0;
    while (1) {
         int leftChild = 2 * currentIndex + 1;
        int rightChild = 2 * currentIndex + 2;
        int smallest = currentIndex;
        if ((leftChild<tree_size) && (day_tree[leftChild].time<day_tree[smallest].time)) {
            smallest=leftChild;
        }
        if ((rightChild<tree_size)&&(day_tree[rightChild].time<day_tree[smallest].time)) {
            smallest=rightChild;
        }
        if (smallest!=currentIndex){
            swap(&day_tree[currentIndex],&day_tree[smallest]);
            currentIndex=smallest;
        }
        else {
            break;
        }
    }
    return pop_activity;
}

void push(struct priority_queue *day_tree, int strt_time, struct node *list ){
    if (tree_size>=100) {
        printf("The schedule for today is full.\n");
        return;
    }
    day_tree[tree_size].time=strt_time;
    day_tree[tree_size].event=list;
    int currentIndex=tree_size;
    int parentIndex=(currentIndex-1)/2;
    while ((currentIndex > 0)&&(day_tree[currentIndex].time<day_tree[parentIndex].time)) {
        swap(&day_tree[currentIndex],&day_tree[parentIndex]);
        currentIndex=parentIndex;
        parentIndex=(currentIndex-1)/2;
    }
    tree_size++;
}

void clean_day_tree(){
    for(int i=0;i<100;i++){
        day_tree[i].time=-1;
        day_tree[i].event=NULL;
    }
}

void delete(struct priority_queue *day_tree, int strt_time){
    int i;
    for(i=0;i<tree_size;i++) {
        if (day_tree[i].time==strt_time) {
            day_tree[i]=day_tree[tree_size-1];
            tree_size--;
            break;
        }
    }
    if (i==tree_size) {
        printf("No event at the time.\n");
    }else{
        int currentIndex=i;
        int parentIndex=(currentIndex-1)/2;
        if ((currentIndex>0)&&(day_tree[currentIndex].time<day_tree[parentIndex].time)) {
            while (currentIndex>0&&day_tree[currentIndex].time<day_tree[parentIndex].time) {
                swap(&day_tree[currentIndex],&day_tree[parentIndex]);
                currentIndex=parentIndex;
                parentIndex=(currentIndex-1)/2;
            }
        } else {
            while (1) {
                int leftChild = 2 * currentIndex + 1;
        int rightChild = 2 * currentIndex + 2;
        int smallest = currentIndex;
        if ((leftChild < tree_size) && (day_tree[leftChild].time < day_tree[smallest].time)) {
            smallest = leftChild;
        }
        if ((rightChild < tree_size) && (day_tree[rightChild].time < day_tree[smallest].time)) {
            smallest = rightChild;
        }
        if (smallest != currentIndex) {
            swap(&day_tree[currentIndex], &day_tree[smallest]);
            currentIndex = smallest;
        }
        else {
            break;
        }
            }
        }
    }
    
}

struct node *add_to_ptrlist(struct node *list,int st_mon,int st_da,bool who,bool aler,int str_t,int end_t,char* ac,char *pl,char *ot){
    struct node *tmp;
    tmp=malloc(sizeof(struct node));
    tmp->activity=malloc(strlen(ac)+1);  // Allocate memory for activity
    tmp->place=malloc(strlen(pl)+1);     // Allocate memory for place
    tmp->others=malloc(strlen(ot)+1);    // Allocate memory for others
    strcpy(tmp->activity,ac);
    tmp->strt_month=st_mon;
    tmp->strt_day=st_da;
    tmp->whole_day=who;
    tmp->alert=aler;
    tmp->strt_time=str_t;
    tmp->end_time=end_t;
    strcpy(tmp->place,pl);
    strcpy(tmp->others,ot);
    return tmp;
}

int main() {
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
    //delete(day_tree,9);
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
