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

/*
 * Une classe Node qui permet de mettre en place
 * une topologie de communication sur des aspects concurrents
 */
class Node : public Acting::Actor{
private:
    Acting::Actor* successeur;
    Acting::Actor* predeccesseur;
   
public:
    Node():Actor()
    {
        printf("Hello from Actor %d\n",this->GetUserId());
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
     * Methode exécuté par le processus concurrent
     */
    virtual void act()
    {
        int message = 100;
        int ret =-1;

        if(this->GetUserId() == 0)
        {
            message = 222;
            this->send<int>(this->successeur,&message,this->successeur->GetUserId());
            
            printf("[Actor %d] --%d-----> [Actor %d]\n",this->GetUserId(),message,successeur->GetUserId());
            
            ret =this->receive<int>(this->predeccesseur,this->predeccesseur->GetUserId(),&message);
        }
        else
        {
            //Reception du message à partir du prédecesseur
            ret =-1;
            sleep(1);
            ret = this->receive<int>(this->predeccesseur,this->predeccesseur->GetUserId(),&message);
            message++;
            this->send<int>(this->successeur,&message,this->successeur->GetUserId());
            
            printf("[Actor %d] --%d-----> [Actor %d]\n",this->GetUserId(),message,successeur->GetUserId());
        }
        
        printf("[Actor %d] %d\n",GetUserId(),message);
        printf("Fin de séance pour l'acteur %d\n",GetUserId());
    }
};



/**
 * 
 */
typedef struct {
    int x;
    int y;
    float z;
    float *w;
} data_t;



/**
 * 
 */
int main() {

    const int thread_count =10;

    /*Initialisation de l'env*/
    Acting::Actor::Init();
    
    vector<Node> nodes;
    nodes.resize(10);

    for(int i=0;i<nodes.size();i++){
        nodes[i].SetUserId(i);
        nodes[i].SetSuccesseur(&nodes[(i+1)%nodes.size()]);
        nodes[i].SetPredecesseur(&nodes[(i-1)%nodes.size()]);
        nodes[i].SetUserId(i);
    }

    for(int i=0;i<nodes.size();i++){
        Acting::Actor::AddItemToPool(&nodes[i]);
    }


 
    
    //////////////////////////////////////////////////
    /*Fermeture de l'env*/
    Acting::Actor::Finit();
    
    return 0;
}

