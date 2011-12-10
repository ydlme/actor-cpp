/*
 * File:   main.cpp
 * Author: maxds
 *
 * Created on 4 novembre 2011, 17:14
 * Ce fichier illustre un ping pong entre acteurs 
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

/**
 * Une fonction
 * @param arg
 * @return
 */
class UneFonction: public virtual Functional::Routine<int,int>{
public:
     int invok(int &arg){
        std::cout << "Dans UneFonction"<< std::endl;
        return 0;
    }
};

class Ping : public Acting::Actor{
private:
  Acting::Actor* pong;
  int message;
public:
  
  void SetPong(Acting::Actor* pong){
    this->pong = pong;
  }
  
  Ping():Actor(){
    this->message = this->GetId();
    this->pong = pong;
  }
  
  virtual void act(){
    int tag;
    for(int i=0;i<10;i++){
      
      printf("actor[%d] step [%d]: %s --%d--> %s\n",this->GetId(),i,this->GetName().c_str(),message,this->pong->GetName().c_str());
      tag=this->send<int>(this->pong,&message,0);
      sleep(this->GetId()+2);
      
      this->receive<int>(pong,0,&message);
      printf("actor[%d] step [%d]: %s <--%d-- %s\n",this->GetId(),i,this->GetName().c_str(),message,this->pong->GetName().c_str());
      message++;
      
      
      
    }
  }
};


class Node : public Acting::Actor{
private:
  Acting::Actor* successeur;
  Acting::Actor* predeccesseur;
  
public:
  Node(Acting::Actor* successeur,Acting::Actor* predeccesseur):Actor()
  {
    this->successeur=successeur;
    this->predeccesseur=predeccesseur;
  }
  
  
  virtual void act(){
    int message = 100;
    
    this->send<int>(this->successeur,&message,0);
    this->receive<int>(this->predeccesseur,0,&message);
  }
};

typedef struct {
  int x;
  int y;
  float z;
  float *w;
} data_t;






int main() {
  
  list<data_t> liste;
  data_t t ;
  t.x=2;
  
  liste.push_front(t);
  //liste.remove(&t);
  list<data_t>::iterator it = liste.begin();
  liste.erase(it);
  
  assert(liste.size() == 0);
  
  
  
  const int thread_count =6;
  
  
  Acting::Actor::Init();
  
  Ping* ping = new Ping();
  Ping* pong = new Ping();
  
  ping->SetName(string("PING"));
  pong->SetName(string("PONG"));
  
  ping->SetPong(pong);
  pong->SetPong(ping);
  

  /*
    renommer cette methode comme méthode d'instance, ivoquant une méthode 
    static
   */

  /**
     En gros faire en sorte que:
     ping->start();
     pong->start();
   */
  ping->start();
  pong->start();
  
  
  
  //////////////////////////////////////////////////
  Acting::Actor::Finit();
  
  
  return 0;
}

