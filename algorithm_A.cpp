#include <iostream>
#include <stdlib.h>
#include <time.h>
#define MAX 1594871214
using namespace std;

//#include "../include/board.h"
#include "../include/rules.h"
 ///////////////////////////////////////////////////////////////
class bode {
    private:
        Board boardState;
    public:
        void setboard(Board p);
        Board generatedeepcopy();
};
Board bode::generatedeepcopy(){
    return this->boardState;
};
void bode::setboard(Board p){
    this->boardState=p;
};
/////////////////////


template <class T>
class stack{
    private:
        struct Node{
            T data;
            Node *next_node;
            Node(T data, Node *next):data(data),next_node(next){}
        };
        Node* topnode;
    public:
        stack(): topnode(NULL){}
        void push(T element);
        T top();
        bool Isempty();
        void pop();
};

template <class T> 
bool stack<T>::Isempty(){
    if (topnode == NULL) return true;
    else return false;
}

template <class T> 
void stack<T>::push(T  node){
    Node* newnode = new Node(node,topnode);
    topnode = newnode;
}

template <class T> 
void stack<T>::pop() {
    if(Isempty()) return;
    Node* tempnode = topnode;
    topnode = topnode->next_node;
    delete tempnode;
}

template <class T> 
T stack<T>::top(){
    return topnode->data;
}

Board boarddeepcopy(Board board) {
    bode b3;
    b3.setboard(board);
    return b3.generatedeepcopy();
}




class LinkedList;                           
class ListNode{
    private:
        int data;
        ListNode *next;
    public:
        ListNode():data(0),next(NULL){};
        ListNode(int a):data(a),next(NULL){};

        friend class LinkedList;
};
class LinkedList{
    private:
        ListNode *first;            
    public:
        LinkedList():first(NULL){};
        int countlist();
        void add(int x);      
        void Clear();              
};
int LinkedList::countlist(){
    int temp=0;
    if (first == NULL) return 0;

    ListNode *current = first;            
    while (current != NULL) {                
        temp+=2*current->data;
        current = current->next;
    }
    return temp;
}
void LinkedList::add(int x){

    ListNode *newNode = new ListNode(x);   
    if (first == NULL) {   first = newNode; return;}
    ListNode *current = first;
    while (current->next != NULL) {current = current->next;}
    current->next = newNode;              
}
void LinkedList::Clear(){

    while (first != NULL) {            
        ListNode *current = first;
        first = first->next;
        delete current;
        current = 0;
    }
}

class position { 
    public: 
        position(); 
        int x;int y; 
        position(int i, int j):x(i),y(j){}
}; 
int row[4] = { -1, 0, 0, 1 };
int col[4] = { 0, -1, 1, 0 };

int danger(Board board ,int a,int b){return 4-(board.get_capacity(a, b)-board.get_orbs_num(a, b));}

int potential(Board board,int color){
    //this is for the number of critical same color cells
    /////////////////////////////
    LinkedList list;     
    int potential_now=0;
    ///////////////////////
    bool visit[ROW][COL];
    for(int i=0;i<ROW;i++)for(int j=0;j<COL;j++)visit[i][j]=false;

    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            if(!visit[i][j] && board.get_cell_color(i,j)==color && danger( board ,i,j)==3 ){
                stack<position> st;
                position newNode(i,j);
                st.push(newNode);
                potential_now=0;
                while(!st.Isempty()){
                    position currentnode=st.top(); st.pop();
                    visit[currentnode.x][currentnode.y]=true;
                    potential_now++;
                    for (int k = 0; k < 4; k++){
                        int x = i + row[k] ;
                        int y = j + col[k] ;
                        if (!index_range_illegal(x,y)){
                            if(!visit[x][y] && board.get_cell_color(x,y)==color && danger( board ,x,y)==3 ){
                                position newNodev(x,y);
                                st.push(newNodev);
                            }
                        }
                    }
                }  
                //1+ critical cells chains
                if(potential_now>0)list.add(potential_now); 
            }
        }
    }
    int temp=0;
    temp=list.countlist();
    list.Clear();
    return temp;

}
int fullpotential(Board board){
    //this is for the number of chain critical cells (more than one)
    /////////////////////////////
    LinkedList list;     
    int potential_now=0;
    ///////////////////////
    bool visit[ROW][COL];
    for(int i=0;i<ROW;i++)for(int j=0;j<COL;j++)visit[i][j]=false;

    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            if(!visit[i][j] && danger( board ,i,j)==3 ){
                stack<position> st;
                position newNode(i,j);
                st.push(newNode);
                potential_now=0;
                while(!st.Isempty()){
                    position currentnode=st.top(); st.pop();
                    visit[currentnode.x][currentnode.y]=true;
                    potential_now++;
                    for (int k = 0; k < 4; k++){
                        int x = i + row[k] ;
                        int y = j + col[k] ;
                        if (!index_range_illegal(x,y)){
                            if(!visit[x][y]  && danger( board ,x,y)==3 ){
                                position newNodev(x,y);
                                st.push(newNodev);
                            }
                        }
                    }
                }  
                //only 2+ critical cells chains
                if(potential_now>1)list.add(potential_now); 
            }
        }
    }
    int temp=0;
    temp=list.countlist();
    list.Clear();
    return temp;
}

int checkdanger(Board board,int colorOpponent,int i,int j){
    int temp=0;
    if(!index_range_illegal(i+1,j))if(board.get_cell_color(i+1,j)==colorOpponent && danger(board,i+1,j)==3)temp+=6-board.get_capacity(i, j);
    if(!index_range_illegal(i-1,j))if(board.get_cell_color(i-1,j)==colorOpponent && danger(board,i-1,j)==3)temp+=6-board.get_capacity(i, j);
    if(!index_range_illegal(i,j+1))if(board.get_cell_color(i,j+1)==colorOpponent && danger(board,i,j+1)==3)temp+=5-board.get_capacity(i, j);
    if(!index_range_illegal(i,j-1))if(board.get_cell_color(i,j-1)==colorOpponent && danger(board,i,j-1)==3)temp+=5-board.get_capacity(i, j);
    return temp;
}
bool nodangerscore(Board board,int i,int j){
    int temp=0;
    if(board.get_capacity(i, j)==2)temp+=3;
    if(board.get_capacity(i, j)==3)temp+=2;
    if(danger(board ,i,j)==3)temp*=2;
    return temp;
}
int BoardEvaluator(Board board, Player player,Player opponent,bool myturn) {
    char colorPlayer = player.get_color();
    char colorOpponent= opponent.get_color();
    int orbPlayer = 0, orbOpponent = 0; int orbScore = 0;
    int temp=0;
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            if(board.get_cell_color(i,j)==colorPlayer){
                bool invulnerable = true;
                orbPlayer+=board.get_orbs_num(i,j);
                temp=checkdanger(board,colorOpponent,i,j);//if checkdanger get critical opponent cells nearby (temp>0), invulnerable=false
                if(temp>0){
                        orbScore-=checkdanger(board,colorOpponent,i,j);
                        invulnerable = false;
                }
                //profits for edges and corners when count critical opponent cells nearby (critical double profits)
                if(invulnerable)orbScore+=nodangerscore(board,i,j);
            }
            else if(board.get_cell_color(i,j)==colorOpponent){
                orbOpponent+=board.get_orbs_num(i,j);
            }
        }
    }
    orbScore+=orbPlayer;
    int boom=fullpotential( board);
    int oboom=potential( board,colorOpponent);
    if(orbPlayer>1 && orbOpponent==0) return 10000.0;
    if(orbPlayer==0 && orbOpponent>1) return -10000.0;

    if(orbOpponent<40){// enemy does not dominate the game
        if(myturn)orbScore-=oboom;//after I place, my Opponent has fewer chances to blow
        //else
        orbScore+=boom; //blow dominator
    }
    else if(orbOpponent>=40 && myturn){// irrationally find chances to win because the same way cannot turn the table
        srand(time(NULL)*time(NULL));
        orbScore+=(rand() % 50);
    }

    return orbScore;
}
int max (int a,int b) {return (a<b)?b:a;}
int min (int a,int b) {return (a>b)?b:a;}
int alpha_beta(Board board, int alpha,int beta,int depth, Player player,  bool myturn) {
    // alpha beta game tree prune by recursive//
    /////get opponent and its color///////////////////
    int colorPlayer = player.get_color();
    char colorOpponent;
    if(colorPlayer == 'r')
        colorOpponent = 'b';
    else 
        colorOpponent = 'r';
    Player opponent(colorOpponent); 

    int score = BoardEvaluator(board, player,opponent,myturn);

    if(score == 10000) return score;
    else if(score == -10000) return score;
    else if(depth == 0)return score;
    
    int best = -MAX;
    int worst= MAX;
    
    if(!myturn) {
        best = -MAX; 
        for(int i = 0; i < ROW; i++) for(int j = 0; j < COL; j++) {
            if (beta <= alpha) break;//equivalent ===> prune by no more run
            ///finding the sweet point...
            //evaluate opponent future moves after I place
            if(board.get_cell_color(i,j) == colorPlayer || board.get_cell_color(i,j) == 'w') {
                if (beta <= alpha) break;     
                Board board_copy = boarddeepcopy(board);
                board_copy.place_orb(i,j,&player);
                ///new branch for myturn
                best = max(best, alpha_beta(board_copy,alpha,beta, depth-1, player, true)); 
                alpha = max(alpha, best);     
            }
        }
        return best;
    }
    else {
        worst= MAX;
        for(int i = 0; i < ROW; i++) for(int j = 0; j < COL; j++) {
            if (beta <= alpha) break;
            ///finding the sweet point...
            //evaluate my future moves after opponent places
            if(board.get_cell_color(i,j) == colorOpponent || board.get_cell_color(i,j) == 'w') {
                 if (beta <= alpha) break; 
                Board board_copy = boarddeepcopy(board);
                board_copy.place_orb(i,j,&opponent);
                ///new branch for !myturn
                worst = min(worst, alpha_beta(board_copy,alpha,beta, depth-1, player,false));
                beta = min(beta, worst);
            }
        }
        return worst;
    }
}
position God_choose(Board board, Player player,int depth) {
    char colorPlayer = player.get_color();
    int best = -MAX;
    position choose(0,0);
    // initialize all possbile legal places to move
    for(int i = 0; i < ROW; i++) for(int j = 0; j < COL; j++) 
        if(board.get_cell_color(i,j) == colorPlayer || board.get_cell_color(i,j) == 'w'){
            Board board_copy = boarddeepcopy(board);
            board_copy.place_orb(i,j,&player);
            /////////////////////alpha_beta starts//////////////////////////
            int alpha = -MAX;
            int beta =  MAX;
            int movevaluation = alpha_beta(board_copy,alpha,beta, depth-1, player , true);
            if(movevaluation > best) {
                best = movevaluation;
                choose.x = i;
                choose.y = j;
            }
            /////////////////////alpha_beta ends//////////////////////////
        }

    return choose;
}

void algorithm_A(Board board, Player player, int index[]){
    position god = God_choose(board, player,4);
    index[0] = god.x;
    index[1] = god.y;
}