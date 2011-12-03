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
            this->send<int>(this->successeur,&message,0);
            printf("[Actor %d] --%d-----> [Actor %d]\n",this->GetUserId(),message,successeur->GetUserId());

            
            while(ret!=0)
                ret =this->receive<int>(this->predeccesseur,0,&message);

            if(ret !=0)
            {
                printf("Pas de message\n");
            }
            else{
                printf("Bonjour tout le monde : je viens de recevoir: %d\n",message);
            }
        }
        else
        {
            //Reception du message à partir du prédecesseur


            ret =-1;
            sleep(1);
            
            while(ret!=0)
                ret = this->receive<int>(this->predeccesseur,0,&message);

            printf("Bonjour tout le monde : je viens de recevoir: %d\n",message);
            message=10988;

            this->send<int>(this->successeur,&message,0);
            printf("[Actor %d] --%d-----> [Actor %d]\n",this->GetUserId(),message,successeur->GetUserId());
        }
        
        printf("[Actor %d] %d\n",GetUserId(),message);
        printf("Fin de séance pour l'acteur %d\n",GetUserId());
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

