/* 
 * File:   main.cpp
 * Author: maxds
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
 * une topologie de communication sur des 
 * aspects concurrents
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
   * @return 
   */
  Id GetPredecessor(){
    return this->predeccesseur->GetUserId();
  }
  
  /**
   * 
   * @return 
   */
  Id GetSuccessor(){
    return  this->successeur->GetUserId();
    }
  
  /** 
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
protected:
  void act()
  {
    int message = 100;
    int ret =-1;
    
    if(this->GetUserId() == 0)
      {
	message = 222;
        
	printf("[Actor %d] --%d-----> [Actor %d]\n",this->GetUserId(),message,successeur->GetUserId());
	this->send<int>(this->successeur,&message,1);
        
	ret =this->receive<int>(this->predeccesseur,22,&message);
      }
    else
      {
	
	//Reception du message à partir du prédecesseur
	ret =-1;
	int tag =1;
        
	if(this->GetUserId()==9){
	  tag =22;
	}
	
	ret = this->receive<int>(this->predeccesseur,1,&message);
	message++;
	
	sleep(1);
	
	printf("[Actor %d] --%d-----> [Actor %d]\n",this->GetUserId(),message,successeur->GetUserId());
	
	this->send<int>(this->successeur,&message,tag);
	
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


#define TAILLE 10

inline void Test(){
   /*Initialisation de l'env*/
  Acting::Actor::Init();
  
  vector<Node> nodes;
  nodes.resize(TAILLE);
  
  for(int i=0;i<nodes.size();i++){
    nodes[i].SetUserId(i);
    nodes[i].SetSuccesseur(&nodes[(i+1)%nodes.size()]);
    
    if(i==0){
      nodes[i].SetPredecesseur(&nodes[nodes.size()-1]);
    }
    else
      nodes[i].SetPredecesseur(&nodes[(i-1)%nodes.size()]);
    
    nodes[i].SetUserId(i);
  }
  
  
  for(int i=0;i<nodes.size();i++){
    printf("actor:%d succ:%d pred:%d\n",nodes[i].GetUserId(),nodes[i].GetSuccessor(),nodes[i].GetPredecessor());
    nodes[i].start();
  }
  
  
  
  while(true){
    char c;
    scanf("%c",&c);
    
    if(c=='q')
      break;
    
    for(int i=0;i<nodes.size();i++){
      printf("actor:%d state:%d \n",nodes[i].GetUserId(),nodes[i].GetStat());
    }
  }
  
  /*Fermeture de l'env*/
  Acting::Actor::Finit();
}




const int taille = 4;
actor_context ctxs[taille];


/*changement de context*/
void switch_context(){
  /*choisir le context à restaurer*/
  for(int i=0;i<taille;i++){
    if(ctxs[i].__valide && ctxs[i].__priority == 0){
      restor_context(&ctxs[i]);
    }
  }
}

/*interruption du thread appelant*/
void interrupt(int tid){  
  ctxs[tid].__arg = tid+1;
  //current_tid = tid+1;
  save_context(&ctxs[tid]);
  ctxs[tid].__priority++;
  if(tid+1<taille){
    ctxs[tid+1].__arg = tid+1;
    restor_context(&ctxs[tid+1]);
  }
  //switch_context();
}


int tids[taille] = {1,2,3,4};
int current_tid =0;

/**/
void routine(int tid){
  
  bool context_saved=false;
  
  /*code de début*/
  current_tid=tid++;
  
 init_routine:
  if(context_saved)
    goto code_region;
  
  context_saved = true;
  
  save_context(&ctxs[tid-1]);
  
 code_region:
  printf("Hello from tid %d\n",current_tid);
  
 finit_routine:
  /*code de fin*/
 restor_context(&ctxs[tid]);
 
}

void methode_for()
{
  for(int i=0;i<8;i++){  
    printf("hello from ctx %d step %d\n",i,i);
    //swap();
  }
}


#include <ucontext.h>
#include <list>


std::list<ucontext_t> __ctxs;


/**
 * 
 * @return 
 */
void* _loop(void *arg){
    
    for(int i =0;i<10;i++){
        
        ucontext_t uc;
        getcontext(&uc);
       __ctxs.push_front(uc);
        
        printf("hello from task %d\n",i);
        
        
    }
}
/**/
int main() 
{ 
    //Test();
    Threading::Thread* threads[10];
    
    for(int i =0;i<10;i++)
       threads[i]=Threading::create_thread(_loop,NULL);
    
    for(int i =0;i<10;i++){
        //ici endormi le thread i
        //save son context
        //restorer un autre context
    }
    
    for(int i =0;i<10;i++)
        Threading::join_thread(*threads[i]);
    
    return 0;
}

