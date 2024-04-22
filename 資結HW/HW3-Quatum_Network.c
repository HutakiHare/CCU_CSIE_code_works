#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct connecting_node{         //store connecting nodes
    int node_id;
    int last_node;
    struct connecting_node *next;
}connecting_node;

typedef struct node_info{               // store node info
    int node_id;
    int quantum_mem;
    connecting_node *next_node;
}node_info;

typedef struct link_graph{              //store links
    int link_id;
    int link_end1;
    int link_end2;
}link_graph;

typedef struct request_info{             //store requests
    int request_id;
    int request_src;
    int request_dst;
}request_info;

typedef struct tree{                     //store (v1,v2)
    int time_slot;
    int strt_point;
    int end_point;
}binary_tree;

void push(connecting_node **node,int node_id){        //push new node in "connecting_node" 
    connecting_node *tmp=malloc(sizeof(connecting_node));
    tmp->node_id=node_id;
    tmp->next=*node;
    *node=tmp; 
}

void push_node(node_info* node,int node_id){        //push new node in "node_info"
    connecting_node *tmp=malloc(sizeof(connecting_node));
    tmp->node_id=node_id;
    tmp->next=node->next_node;
    node->next_node=tmp;
}

int pop(connecting_node **stack){         // pop out value from stack
    if (*stack==NULL) {
        return -1;                      // -1 if indicates an empty stack
    }
    int tmp;
    tmp=(*stack)->node_id;
    (*stack)=(*stack)->next;
    return tmp;
}

int BFS(int i,int node_num,node_info* node,int request_src,int request_dst,int path_determine[][node_num]){     // find the distance between src and dst
    int last_node=request_dst;           //strt from dst
    node_info *tmp_node=&node[request_dst];       //connect to the node of dst
    connecting_node *stack=NULL,*rear=stack,*tmp_stack=tmp_node->next_node;   //find the stack next node of dst
    path_determine[i][last_node]=0;     //set the dst distance as 0
    while((stack!=NULL||last_node!=-1)||last_node==request_dst){ 
        while(tmp_stack!=NULL){
            connecting_node *tmp=malloc(sizeof(connecting_node));
            if(path_determine[i][tmp_stack->node_id]>(path_determine[i][last_node]+1)||(path_determine[i][tmp_stack->node_id])==-1){  //goes in only if never have been gone (-1), or there is shorter distance
                path_determine[i][tmp_stack->node_id]=(path_determine[i][last_node]+1); //assign distance
                tmp->node_id=tmp_stack->node_id;     //use  queue to add in new nodes
                tmp->last_node=last_node;
                tmp->next=NULL;
                if(stack!=NULL){
                    rear->next=tmp;
                    rear=tmp;
                }else{
                    stack=tmp;
                    rear=tmp;
                }
            }
            tmp_stack=tmp_stack->next;
        }
        last_node=pop(&stack);  //pop next num to look in the stack
        if(last_node==-1){
            stack=NULL;
        }
        tmp_node=&node[last_node]; // fin,change to next node in stack as strt
        tmp_stack=tmp_node->next_node;
    }
    return path_determine[i][request_src];    // return the distance of scr and dst
}

int cmp(const void *a,const void *b){    //compare
   return (*(int*)b-*(int*)a);
}

connecting_node *sort(connecting_node *tmp,int mem_size){   //sort pointer in small to big order
    connecting_node* sorted_tmp=NULL;
    int arr[mem_size],n=0;
    memset(arr,0,sizeof(arr[0])*mem_size);
    while(tmp!=NULL){           //put all node in tmp to arr
        arr[n]=tmp->node_id;
        tmp=tmp->next;
        n++;
    }
    qsort(arr,n,sizeof(int),cmp);      //sort arr from big to small 
    for(int i=0;i<n;i++){
        push(&sorted_tmp,arr[i]);      //push info back to sorted_tmp from arr ->result in small to big order
    }
    return sorted_tmp;
}

int main(){
    int node_num,link_num,timeslot,request_num,time_count=0;
    scanf("%d %d %d %d",&node_num,&link_num,&timeslot,&request_num);         //info scanning
    node_info node[node_num];
    int numerlogy_1[timeslot+50][node_num+50],numerlogy_2[timeslot+50][node_num+50];  //numerlogy graph to see the mem use
    for(int i=0;i<timeslot+50;i++){
        memset(numerlogy_1[i],0,sizeof(numerlogy_1[0][0])*(node_num+50));
    } 
    for(int i=0;i<timeslot+50;i++){
        memset(numerlogy_2[i],0,sizeof(numerlogy_2[0][0])*(node_num+50));
    }
    int path_determine[request_num][node_num];   //store each node's distance with the request's distance
    for(int i=0;i<node_num;i++){              //scan node
        scanf("%d %d",&node[i].node_id,&node[i].quantum_mem);
        node[i].next_node=NULL;
    }
    for(int i=0;i<request_num;i++){            //reset path_determine to all not been yet (-1)
        for(int j=0;j<node_num;j++){
           path_determine[i][j]=-1;
        } 
    }
    link_graph link[link_num];
    for(int j=0;j<link_num;j++){              //scan link & connect nodes
        scanf("%d %d %d",&link[j].link_id,&link[j].link_end1,&link[j].link_end2);
        push_node(&(node[link[j].link_end1]),link[j].link_end2);
        push_node(&(node[link[j].link_end2]),link[j].link_end1);    // enternal links, not count as mem
    }
    request_info request[request_num];
    int fin_request=0;                           //count fin num
    int tree[request_num][node_num+5];           //store path in order (from src to dst)
    binary_tree post_tree[request_num][node_num+5];    //store two node's entangle
    for(int k=0;k<request_num;k++){                 //scan request
        scanf("%d %d %d",&request[k].request_id,&request[k].request_src,&request[k].request_dst);
    }
    int count_path[request_num],min=__INT_MAX__,store_pos=-1; //count_path store each request's deistance
    for(int j=0;j<request_num;j++){
        count_path[j]=0;
    } 
    for(int i=0;i<request_num;i++){
        if(request[i].request_id==-1){    // if = -1 means already done
            continue;
        }
        count_path[i]=BFS(i,node_num,node,request[i].request_src,request[i].request_dst,path_determine); //get all distance
    }
    int fin_request_order[request_num];
    for(int i=0;i<request_num;i++){      //reset fin_request_order
        fin_request_order[i]=-1;
    }
    for(int now=0;now<request_num;now++){  //see request one by one
        for(int k=2;k<=timeslot;k++){       //make numerlogy_2=numerlogy_1 (if origin 2 fail)
            for(int s=0;s<node_num;s++){
                numerlogy_2[k][s]=numerlogy_1[k][s];
            }
        }
        int tmp_node=request[now].request_src,store_k=2;
        for(int i=1;path_determine[now][tmp_node]!=0;i++){     //find path of src and dst, store in tree
            tree[now][i]=tmp_node;  
            connecting_node *tmp=node[tmp_node].next_node;
            tmp=sort(tmp,node_num);    //make tmp in small to big order
            while(tmp){
                if(path_determine[now][tmp->node_id]==(path_determine[now][tmp_node]-1)){
                    tmp_node=tmp->node_id;
                    if(path_determine[now][tmp->node_id]==0){
                        i++;
                        tree[now][0]=i;    //strt from 1 to tree[fin_request][0] (<=)
                        tree[now][i]=tmp->node_id;
                    }
                    break;
                }
                tmp=tmp->next;   
            }
        }
        int on_off=0;
        post_tree[now][0].time_slot=count_path[now];      //post_tree[fin_request][0].time_slot=count_path[store_pos]  (<=)
        for(int i=1,j=2;j<=count_path[now]+1;i++,j++){   //connect node to node
            post_tree[now][i].strt_point=tree[now][i];
            post_tree[now][i].end_point=tree[now][j];
        }
         for(int k=2;k<=timeslot;k++){               //see where the process should strt
            if(tree[now][0]>3){    //if path got more than 3 nodes
                for(int s=1;s<=tree[now][0];s++){
                    if(s==1||s==4){                 // start time's first & last node (+1)
                       if((((numerlogy_2[k][tree[now][s]]+1))>node[tree[now][s]].quantum_mem)||(((numerlogy_2[k+1][tree[now][s]]+1))>node[tree[now][s]].quantum_mem)){
                        break;
                        } 
                    }
                    if(s==2||s==3){       //start time's second and third (+2)
                       if((((numerlogy_2[k][tree[now][s]]+2))>node[tree[now][s]].quantum_mem)||(((numerlogy_2[k+1][tree[now][s]]+2))>node[tree[now][s]].quantum_mem)){
                        break;
                        } 
                    }
                    if(s==(tree[now][0])){   // if all mem not full -> store time
                        store_k=k;
                        on_off=1;
                        break;
                    }
                }
            }else{    // if less than 3 node
                for(int s=1;s<=tree[now][0];s++){
                    if(tree[now][0]==3&&s==2){
                        if((((numerlogy_2[k][tree[now][s]]+2))>node[tree[now][s]].quantum_mem)||(((numerlogy_2[k+1][tree[now][s]]+2))>node[tree[now][s]].quantum_mem)){ // start time's second node (+2)
                            break;
                        }
                    }else{
                        if((((numerlogy_2[k][tree[now][s]]+1))>node[tree[now][s]].quantum_mem)||(((numerlogy_2[k+1][tree[now][s]]+1))>node[tree[now][s]].quantum_mem)){ // start time's first & last node (+1)
                            break;
                        }
                    }
                    if(s==(tree[now][0])){   // if all mem not full -> store time
                        store_k=k;
                        on_off=1;
                        break;
                    }
                }
            }
            if(on_off){
                break;
            }
        }
        if(store_k>2){
            store_k++;
        }
        if(on_off&&((store_k+post_tree[now][0].time_slot-1)<=(timeslot))){   //if on_off==1 ->have find place to strt
            post_tree[now][0].strt_point=store_k;
            numerlogy_2[store_k-1][post_tree[now][1].strt_point]++;             //add first time mem
            numerlogy_2[store_k-1][post_tree[now][1].end_point]++;
            numerlogy_2[store_k][post_tree[now][1].strt_point]++;             //add second time mem
            numerlogy_2[store_k][post_tree[now][1].end_point]++;
            if(post_tree[now][0].time_slot>1){
                numerlogy_2[store_k-1][post_tree[now][2].strt_point]++;
                numerlogy_2[store_k-1][post_tree[now][2].end_point]++;
                numerlogy_2[store_k][post_tree[now][2].strt_point]++;
                numerlogy_2[store_k][post_tree[now][2].end_point]++;
                store_k++;
            }
            for(int strt=1,next=2;next<=post_tree[now][0].time_slot;next++){  // add mem
                numerlogy_2[store_k][post_tree[now][strt].strt_point]++;
                numerlogy_2[store_k][post_tree[now][next].end_point]++;
                if(next<post_tree[now][0].time_slot){
                    store_k--;
                    numerlogy_2[store_k][post_tree[now][next+1].strt_point]++;
                    numerlogy_2[store_k][post_tree[now][next+1].end_point]++;
                    store_k++;
                    numerlogy_2[store_k][post_tree[now][next+1].strt_point]++;
                    numerlogy_2[store_k][post_tree[now][next+1].end_point]++;
                }
                store_k++;
            }
            int ok_numerlogy=1;
            for(int k=2;k<=timeslot;k++){          //scan through all node see if mem isn't over
                for(int s=0;s<node_num;s++){
                    if(numerlogy_2[k][s]>node[s].quantum_mem){
                        ok_numerlogy=0;
                        break;
                    }
                }
                if(!ok_numerlogy){
                    break;
                }
            }
            if(!ok_numerlogy){     //over ->reject request
                continue;
            }
            for(int k=2;k<=timeslot;k++){         //success copy 2 to 1 numerlogy
                for(int s=0;s<node_num;s++){
                    numerlogy_1[k][s]=numerlogy_2[k][s];
                }
            }
            fin_request_order[fin_request]=request[now].request_id;   //store request id
            request[now].request_id=-1;                // done
            fin_request++;                             //fin task count
        } 
    }                                                    //traversing
    printf("%d\n",fin_request);                 //req num
    for(int i=0;i<fin_request;i++){ 
        printf("%d ",fin_request_order[i]);      //req_id
        for(int j=1;j<=tree[fin_request_order[i]][0];j++){   //path
            printf("%d ",tree[fin_request_order[i]][j]);
        }
        printf("\n");
        int now_slot=post_tree[fin_request_order[i]][0].strt_point;  //post order tree traverse
        printf("%d %d %d\n",post_tree[fin_request_order[i]][1].strt_point,post_tree[fin_request_order[i]][1].end_point,now_slot++);
        if(post_tree[fin_request_order[i]][0].time_slot>1){
            now_slot--;
            printf("%d %d %d\n",post_tree[fin_request_order[i]][2].strt_point,post_tree[fin_request_order[i]][2].end_point,now_slot++);
        }
        for(int strt=1,next=2;next<=post_tree[fin_request_order[i]][0].time_slot;next++){
            printf("%d %d %d %d %d %d %d\n",post_tree[fin_request_order[i]][strt].strt_point,post_tree[fin_request_order[i]][next].end_point,post_tree[fin_request_order[i]][strt].strt_point,post_tree[fin_request_order[i]][next-1].end_point,post_tree[fin_request_order[i]][next].strt_point,post_tree[fin_request_order[i]][next].end_point,now_slot);
            if(next<post_tree[fin_request_order[i]][0].time_slot){
                printf("%d %d %d\n",post_tree[fin_request_order[i]][next+1].strt_point,post_tree[fin_request_order[i]][next+1].end_point,now_slot++);
            }else{
                now_slot++;
            }
        }
    }
    return 0;
}