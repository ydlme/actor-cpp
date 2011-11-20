/* 
 * File:   main.cpp
 * Author: maxds
 *
 * Created on 4 novembre 2011, 17:14
 */

#include <cstdlib>
#include <iostream>

#include "Actor.h"
#include "Closures.h"
#include "ThreadPool.h"
#include <stack>
#include <algorithm>
#include <assert.h>

using namespace std;

class Node : public Acting::Actor{
private:
    Acting::Actor* successeur;
    Acting::Actor* predeccesseur;
   
public:
    Node():Actor()
    {
    }


    /**
     * 
     * @param succ
     */
    void SetSuccesseur(Acting::Actor* succ){
        this->successeur = succ;
    }

    /**
     * 
     * @param pred
     */
    void SetPredecesseur(Acting::Actor* pred){
        this->predeccesseur = pred;
    }


    /**
     * 
     */
    virtual void act(){
        int message = 100;
        
        if(this->GetUserId() ==0){
            message = 222;
            this->send<int>(this->successeur,&message,0);
            printf("[Actor %d] --%d-----> [Actor %d]\n",this->GetUserId(),message,successeur->GetUserId());
        
        }
        else{
            this->receive<int>(this->predeccesseur,0,&message);
            message++;

            this->send<int>(this->successeur,&message,0);
            printf("[Actor %d] --%d-----> [Actor %d]\n",this->GetUserId(),message,successeur->GetUserId());
        
        }


        printf("[Actor %d] %d\n",GetUserId(),message);

        
        
    }
};

typedef struct {
    int x;
    int y;
    float z;
    float *w;
} data_t;




int main() {

    const int thread_count =6;

    /*Initialisation de l'env*/
    Acting::ThreadPool<thread_count>::InitPool();

    vector<Node> nodes;
    nodes.resize(10);

    for(int i=0;i<nodes.size();i++){
        nodes[i].SetSuccesseur(&nodes[(i+1)%nodes.size()]);
        nodes[i].SetPredecesseur(&nodes[(i-1)%nodes.size()]);
        nodes[i].SetUserId(i);
    }

    for(int i=0;i<nodes.size();i++){
        Acting::ThreadPool<thread_count>::AddItem(&nodes[i]);
    }
    
    //////////////////////////////////////////////////
    /*Fermeture de l'env*/
    Acting::ThreadPool<thread_count>::FinalizePool();
    return 0;
}

