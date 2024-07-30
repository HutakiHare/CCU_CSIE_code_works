#include<iostream>
#include<vector>
#include<cstring>
#include<utility>
#include<deque>
using namespace std;

class PQB{                    //class of physical qubits
    public:
        int ID;
        int on_LQB;           // the LQB on
        int link_count=0;
        vector<int>nPQBID;    //store all PQB connected
};

class LQB{                   // class of logical qubits 
    public:
        int ID;
        int PQBID;
        int connections=0;
        deque<int>nLQBID;
};
        
class gate{                  //class of gate info
    public:
        int gate_ID;
        int precedence=0;
        pair<int,int> LQBID=make_pair(0,0);
};

class precedence{            //class of precedence info
    public:
        int order=0;
        pair<int,int> gate=make_pair(0,0);
};

void BFS(int phyQBit,int PQB_num,vector<PQB> &PQB_connection,int **table){     // find distance of all PQb to the specified phyQbit
    deque<int> seq;                                                  // store in stack
    for(int i=0;i<PQB_connection[phyQBit].link_count;i++){           // push all dir connected PQB in stack
        seq.push_back(PQB_connection[phyQBit].nPQBID.at(i));
    }
    while(!seq.empty()){                                             // do BFS in iteration till stack empty
        int now=seq.front();
        for(int j=0;j<PQB_connection[now].link_count;j++){           // go through all connected PQB of now_PQB
            if(table[phyQBit-1][PQB_connection[now].nPQBID.at(j)-1]==-1||table[phyQBit-1][PQB_connection[now].nPQBID.at(j)-1]>table[phyQBit-1][now-1]+1){ //check if should push in stack
                seq.push_back(PQB_connection[now].nPQBID.at(j));
                table[phyQBit-1][PQB_connection[now].nPQBID.at(j)-1]=table[phyQBit-1][now-1]+1;  //update distance
            }
        }
        seq.pop_front();   //pop out first element
    }
}

void search_way(int LQB1,int LQB2,int PQB1,int PQB2,int LQB_num,LQB LQB_connection[],vector<PQB> &PQB_connection,int **table){     // find way of PQB1 to PQB2 by PQB_table
    int now=PQB1;
    while(true){
        int i;
        for(i=0;i<PQB_connection[now].link_count;i++){        // go through connected PQBs to find the next target swap
            if(table[PQB2-1][PQB_connection[now].nPQBID.at(i)-1]==table[PQB2-1][now-1]-1){   //if the distance match the smallest way
                //LQBID with smaller ID goes first
                if(PQB_connection[PQB_connection[now].nPQBID.at(i)].on_LQB>LQB1){            
                    cout<<"SWAP q"<<LQB1<<" q"<<PQB_connection[PQB_connection[now].nPQBID.at(i)].on_LQB<<"\n";
                }else{
                    cout<<"SWAP q"<<PQB_connection[PQB_connection[now].nPQBID.at(i)].on_LQB<<" q"<<LQB1<<"\n";
                }
                //do swapping on both PQB and LQB info
                LQB_connection[LQB1].PQBID=PQB_connection[now].nPQBID.at(i);
                LQB_connection[PQB_connection.at(PQB_connection[now].nPQBID.at(i)).on_LQB].PQBID=now;
                PQB_connection.at(now).on_LQB=PQB_connection.at(PQB_connection[now].nPQBID.at(i)).on_LQB;
                PQB_connection.at(PQB_connection[now].nPQBID.at(i)).on_LQB=LQB1;
                break;
            }
        }
        now=PQB_connection[now].nPQBID.at(i);
        if(table[PQB2-1][now-1]==1){             //when find now is already connect with PQB2
            cout<<"CNOT q"<<LQB1<<" q"<<LQB2<<"\n";  // do CNOT
            break;
        }
    }    
}

int main(){
    // data inputs
    int logQubits_num,gates_num,precedence_num,phyQubits_num,phyLinks_num;
    cin>>logQubits_num>>gates_num>>precedence_num>>phyQubits_num>>phyLinks_num;
    vector<gate> gate_task(gates_num+5);                                             //strt 0
    vector<precedence> task_order(precedence_num+5);                                 //strt 0
    LQB LQB_connection[logQubits_num+5];                                             //strt 1
    for(int i=0;i<gates_num;i++){         // gate input
        cin>>gate_task.at(i).gate_ID>>gate_task.at(i).LQBID.first>>gate_task.at(i).LQBID.second;
        LQB_connection[gate_task.at(i).LQBID.first].nLQBID.push_back(gate_task.at(i).LQBID.second);
        LQB_connection[gate_task.at(i).LQBID.first].connections++;
        LQB_connection[gate_task.at(i).LQBID.second].nLQBID.push_back(gate_task.at(i).LQBID.first);
        LQB_connection[gate_task.at(i).LQBID.second].connections++;
    }
    vector<int> order;     // store precedence
    int radix[gates_num];  // see if added to order
    memset(radix,0,sizeof(radix[0])*gates_num);  //set 0 as not added into order
    for(int i=0;i<precedence_num;i++){   // go through all precedence info
        cin>>task_order.at(i).order>>task_order.at(i).gate.first>>task_order.at(i).gate.second;
        if(!order.empty()&&task_order.at(i-1).gate.first==task_order.at(i).gate.first){ // if the two do task at same time
            if(radix[task_order.at(i).gate.second-1]==0){
                order.push_back(task_order.at(i).gate.second);  // only add the newly added gate
                radix[task_order.at(i).gate.second-1]++;
            }
            continue;
        }else if(!order.empty()&&task_order.at(i-1).gate.second==task_order.at(i).gate.second){ //find there are still thing before the last element of order
           if(radix[order.at(order.size()-1)-1]==1){
                radix[order.at(order.size()-1)-1]--;
                order.pop_back();                     // pop out (to put in the new element first)
           }
        }else if(!order.empty()&&task_order.at(i).gate.first==order.back()){ //in order without repeat
            if(radix[order.at(order.size()-1)-1]==1){
                radix[order.at(order.size()-1)-1]--;
                order.pop_back();                       //pop out, later add again
           }
        }
        if(radix[task_order.at(i).gate.first-1]==0){    // see if not yet add
            order.push_back(task_order.at(i).gate.first);
            radix[task_order.at(i).gate.first-1]++;
        }
        if(radix[task_order.at(i).gate.second-1]==0){   //see if not yet add
            order.push_back(task_order.at(i).gate.second);
            radix[task_order.at(i).gate.second-1]++;
        }
    }
    int seq,p1,p2;
    vector<PQB> PQB_connection(phyQubits_num+5);           //PQB info (strt 1)
    for(int i=0;i<phyLinks_num;i++){                       //inputs of PQB info
        cin>>seq>>p1>>p2;
        PQB_connection.at(p1).nPQBID.push_back(p2);
        PQB_connection.at(p1).ID=p1;
        PQB_connection.at(p1).link_count++;
        PQB_connection.at(p2).nPQBID.push_back(p1);
        PQB_connection.at(p2).ID=p2;
        PQB_connection.at(p2).link_count++;
    }
    // make PQB distance table
    int **phyQB_distance_table=new int*[phyQubits_num+5];
    for(int i=0;i<phyQubits_num;i++){
        phyQB_distance_table[i] = new int[phyQubits_num+5];
        memset(phyQB_distance_table[i],-1,sizeof(phyQB_distance_table[0][0])*phyQubits_num);
    }
    for(int i=0;i<phyQubits_num;i++){
        phyQB_distance_table[i][i]=0;
        for(int j=0;j<PQB_connection[i+1].link_count;j++){
            int PQBID=PQB_connection[i+1].nPQBID.at(j);
            phyQB_distance_table[i][PQBID-1]=1; 
        }
        BFS(i+1,phyQubits_num,PQB_connection,phyQB_distance_table);
    }
    //assign PQB to LQB
    deque<int> QB_seq;
    int used[logQubits_num];
    memset(used,0,sizeof(used[0])*logQubits_num);
    for(int i=1;i<=logQubits_num;i++){                  //sort QB_seq in order of connections of QB
        if(!used[i-1]){
            int max=i,j;
            for(j=i+1;j<=logQubits_num;j++){
                if(!used[j-1]&&LQB_connection[j].connections>LQB_connection[max].connections){
                    max=j;
                }
            }
            used[max-1]=1;
            QB_seq.push_back(max);
            i=0;
        }
    }
    memset(used,0,sizeof(used[0])*logQubits_num);
    for(int i=1;i<=logQubits_num;i++){                   //sort PQ in order of links
        if(!used[i-1]){
            int max=i,j;
            for(j=i+1;j<=logQubits_num;j++){
                if(!used[j-1]&&PQB_connection[j].link_count>PQB_connection[max].link_count){
                    max=j;
                }
            }
            used[max-1]=1;
            cout<<QB_seq.at(0)<<" "<<max<<"\n";                     //assign PB to QB in the order of link and connections 
            PQB_connection.at(max).on_LQB=QB_seq.at(0);
            LQB_connection[QB_seq.at(0)].PQBID=max;
            QB_seq.pop_front();
            i=0;
        }
    }
    for(int j=0;j<order.size();j++){
        int q1=gate_task.at(order.at(j)-1).LQBID.first,q2=gate_task.at(order.at(j)-1).LQBID.second; // find what gate to do, and the LQBs
        int now_PQB1=LQB_connection[q1].PQBID; //find the PQBs of the LQBs
        int now_PQB2=LQB_connection[q2].PQBID;
        for(int k=0;k<PQB_connection[now_PQB1].link_count;k++){
            if(PQB_connection[now_PQB1].nPQBID.at(k)==now_PQB2){ //if PQBs of LQBS are connected
                cout<<"CNOT q"<<q1<<" q"<<q2<<"\n";              //output directly
                break;
            }
            if(k==(PQB_connection[now_PQB1].link_count-1)){     //not connected
                search_way(q1,q2,now_PQB1,now_PQB2,logQubits_num,LQB_connection,PQB_connection,phyQB_distance_table); //swap till connected
                break;
            }
        }
    }
    return 0;
}
