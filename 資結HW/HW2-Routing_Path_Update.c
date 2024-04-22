#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct linked_list{
    int node;                          //the node now
    int num;                           //the next num
    struct linked_list *next;
} linked_list;

bool compare(linked_list *list_1,linked_list *list_2){         // compare every nodes in the strt and result linked lists
    bool same=true;
    while(list_2){
        if((list_1->node!=list_2->node)||(!list_1)){          // return false if not same
            same=false;
            break;
        }
        list_1=list_1->next;
        list_2=list_2->next;
    }
    return same;
}

linked_list *add_to_list_normal(int num,int node,linked_list *list){   //add nodes into a stack linked list
    linked_list *tmp=malloc(sizeof(linked_list));
    tmp->node=node;
    tmp->num=num;
    tmp->next=list;
    return tmp;
}

void tranverse(linked_list *strt_arr,int n){            //print out arr
    for(int i=0;i<n;i++){
        if(i!=n-1){
            printf("%d ",(strt_arr)[i].num);
        }else{
            printf("%d\n",(strt_arr)[i].num);
        }
    }
}

int count_fin(linked_list *strt,linked_list *result,linked_list *result_arr,linked_list *tmp_strt_arr,int n){   //count num of steps
    linked_list *tmp_re=result,*tmp_strt=strt,strt_arr[n];
    int count=1,non_used_assigned=0;   // "count" count the num of steps
    for(int i=0;i<n;i++){
        strt_arr[i]=tmp_strt_arr[i];       //do this cuz din't want to mend the data in arr
        if((strt_arr[i].num==-1)&&(result_arr[i].num!=-1)){           //if have non used nodes, and what their next is not -1
            non_used_assigned=1;           //count+=1 (update once assigning all node to -1)
            strt_arr[i].num=result_arr[i].num;     //assign all non used nodes to their right next node 
        }
    }
    count+=non_used_assigned;        //count+=0 if no non used nodes that need
    while(1){
        int non_release_need_count=0;          //count if need to do releasing nodes
        if(strt_arr[tmp_re->node].num!=tmp_re->num){         //due to stack,start with the destiny node, if dif go in loop
            count++;                             // 1.connection
            strt_arr[tmp_re->node].num=result_arr[tmp_re->node].num;     //connect now directly to the next node it should have (count+=1)
            linked_list *tmp_store_tmp_strt=tmp_strt; //store the place tmp_strt now is
            if(tmp_strt->node!=tmp_re->node){       //if have non used nodes after the connection
                while(tmp_strt->node!=tmp_re->node){    //from the strt list start, to the node working on now
                    if((result_arr[tmp_strt->node].num!=-1)&&(strt_arr[tmp_strt->node].num!=result_arr[tmp_strt->node].num)){  //if any of the release nodes shouldn't point to -1
                        non_release_need_count=1;         //if need to do update
                        break;                          //break out
                    }
                    tmp_strt=tmp_strt->next;
                }
                tmp_strt=tmp_store_tmp_strt;  //assign back to tmp_strt was
                if(non_release_need_count){   //if need to do updating
                    while(tmp_strt->node!=tmp_re->node){    //from the strt list start, to the node working on now
                        strt_arr[tmp_strt->node].num=result_arr[tmp_strt->node].num;  //assign the right next node
                        tmp_strt=tmp_strt->next;
                    }
                    count+=(non_release_need_count);// 2.update non used nodes
                }
            }
        }
        if(tmp_re->node==0){              //if now is start node break out
            bool all_same=true;
            for(int i=0;i<n;i++){       //see if not yet all node well assigned
                if(strt_arr[i].num!=result_arr[i].num){
                    all_same=false;
                   break;
                }
            }
            if(!all_same){ //if not update once to all same 
                count++;
            }
            break;
        }
        tmp_re=tmp_re->next;             //go to next (the node before now)
    }
    return count;
}

void do_fin(linked_list *strt,linked_list *result,linked_list *result_arr,linked_list *strt_arr,int n){  //do the updates
    linked_list *tmp_re=result,*tmp_strt=strt->next;
    int non_used_assigned=0;
    for(int i=0;i<n-1;i++){
        if((strt_arr[i].num==-1)&&(result_arr[i].num!=-1)){             //if have non used nodes, and what their next is not -1
            non_used_assigned=1;                                    // have assign non used nodes =1
            strt_arr[i].num=result_arr[i].num;    //assign all non used nodes to their right next node 
        }
    }
    if(non_used_assigned){
        tranverse(strt_arr,n);           //if have update, print out
    }
    while(1){
        bool non_release_need_count=false;
        if(strt_arr[tmp_re->node].num!=tmp_re->num){       //due to stack,start with the destiny node, if dif go in loop
            strt_arr[tmp_re->node].num=result_arr[tmp_re->node].num;  //connect now directly to the next node it should have
            tranverse(strt_arr,n);           //print out the updates
            linked_list *tmp_store_tmp_strt=tmp_strt;
            if(tmp_strt->node!=tmp_re->node){    //if have no non used nodes after the connection
                while(tmp_strt->node!=tmp_re->node){   //from the strt list start, update till the node working on now
                    if((result_arr[tmp_strt->node].num!=-1)&&(strt_arr[tmp_strt->node].num!=result_arr[tmp_strt->node].num)){  // if all node to connect isn't all to -1
                        non_release_need_count=true;     //need to update
                        break;
                    }
                    tmp_strt=tmp_strt->next;      
                }
                tmp_strt=tmp_store_tmp_strt;          //point tmp_strt back
                if(non_release_need_count){         // if need update
                    while(tmp_strt->node!=tmp_re->node){       //update all between strt_arr to now
                        strt_arr[tmp_strt->node].num=result_arr[tmp_strt->node].num;
                        tmp_strt=tmp_strt->next;
                    }
                    tranverse(strt_arr,n);    //print out the new updates
                }
            }
        }
        if(tmp_re->node==0){        //if now is start node break out
            bool all_same=true;
            for(int i=0;i<n;i++){       //check if all same
                if(strt_arr[i].num!=result_arr[i].num){
                    all_same=false;
                    strt_arr[i].num=result_arr[i].num;
                }
            }
            if(!all_same){ //if not all same, fix it and print out
                tranverse(strt_arr,n);
            }
            break;
        }
        tmp_re=tmp_re->next;   //go to next (the node before now)
    }
}

int main(){
    linked_list *strt=NULL,*result=NULL;
    int n,num;
    scanf("%d",&n);                                   //scan node num
    linked_list strt_arr[n],result_arr[n];
    for(int i=0;i<n;i++){
        scanf("%d",&num);
        strt_arr[i].node=i;
        strt_arr[i].num=num;
    }
    for(int j=0;j<n;j++){                                   //store all in arr sequently
        scanf("%d",&num);
            result_arr[j].node=j;
            result_arr[j].num=num;
    }
    for(int t=0;t!=-1;){                                  // put the used nodes in linked list in stack (LIFO)
        strt=add_to_list_normal(strt_arr[t].num,t,strt);
        t=strt_arr[t].num;
    }
    for(int t=0;t!=-1;){                                   // put the used nodes in linked list in stack
        result=add_to_list_normal(result_arr[t].num,t,result);
        t=result_arr[t].num;
    }
    printf("%d\n",(count_fin(strt,result,result_arr,strt_arr,n)));     //print out the step num
    tranverse(strt_arr,n);
    if(!(compare(strt,result))){                        //if two list not same -> update
        do_fin(strt,result,result_arr,strt_arr,n);         // do updates
    }
    return 0;
}