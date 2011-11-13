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


/**
 * Une fonction
 * @param arg
 * @return
 */
class UneAutreFonction: public virtual Functional::Routine<string,int>{
public:
     int invok(string &arg){
        for(int i=0;i<10;i++){
            std::cout << "Dans UneAutreFonction"<< std::endl;
        }
        return arg.length();
    }
};


class MyActor : public Acting::Actor{
public:
    virtual void act(){
        std::printf("Hello from MyActor [ID %d]\n",this->GetId());
    }
};

class AnotherActor : public Acting::Actor{
public:
    virtual void act(){
        std::printf("Hello from AnotherActor [ID %d]\n",this->GetId());
    }
};

class UnaActor : public Acting::Actor{
public:
    virtual void act(){
        std::printf("Hello from UnaActor [ID %d]\n",this->GetId());
        for(int i =0;i<100;i++){
            //std::printf("Hello from UnaActor [ID %d]\n",this->GetId());
            //sleep(1);
        }
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
       for(int i=0;i<100;i++){
           
           message++;
           tag=this->send<int>(this->pong,&message,0);
           sleep(1);
           this->receive<int>(pong,0,&message);
           
           printf("%s --%d--> %s\n",this->GetName().c_str(),message,this->pong->GetName().c_str());
            
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



inline void fun(){
    
    const int thread_count =7;

    stack<Acting::Actor*> to_free;
    //////////////////////////////////////////////////

    Acting::Actor *a = new MyActor();
    Acting::Actor *b = new AnotherActor();
    Acting::Actor *c = new UnaActor();

    //Mettre une factory ActorFactory
    a->SetName("a");
    b->SetName("b");
    c->SetName("c");

    Acting::ThreadPool<thread_count>::AddItem(a);
    Acting::ThreadPool<thread_count>::AddItem(b);
    Acting::ThreadPool<thread_count>::AddItem(c);

    to_free.push(a);
    to_free.push(b);
    to_free.push(c);

    int msg =11;
    float msg_2 =1.09;

    //int tag_msg=a->send<int>(b,&msg);
    //int tag_msg2=a->send<float>(b,&msg_2);



    cout<<"Message box of actor "<<b->GetId()<<" Size:"<<b->GetMessageCount()<<endl;
    ///int*   recv= b->receive<int>(tag_msg);
    //float* recv2=b->receive<float>(tag_msg2);


    data_t d;
    d.x = 102;
    d.y = 100;
    d.z = 103;
    d.w = NULL;

    //int d_tag = c->send<data_t>(a,&d);
    //data_t* res=a->receive<data_t>(d_tag);


    //assert(res->x==102);
    //assert(res->w==0);

    //printf("recv1 :%d recv2: %f\n",*recv,*recv2);
    cout<<"Message box of actor "<<b->GetId()<<" Size:"<<b->GetMessageCount()<<endl;

}
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


    Acting::ThreadPool<thread_count>::InitPool();

    Ping* ping = new Ping();
    Ping* pong = new Ping();

    ping->SetName(string("PING"));
    pong->SetName(string("PONG"));

    ping->SetPong(pong);
    pong->SetPong(ping);

    Acting::ThreadPool<thread_count>::AddItem(ping);
    Acting::ThreadPool<thread_count>::AddItem(pong);


    vector<Node*> nodes;
    nodes.resize(10);

    
    //////////////////////////////////////////////////
    Acting::ThreadPool<thread_count>::FinalizePool();

    
    return 0;
}

