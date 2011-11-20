/* 
 * File:   actor_basics.cpp
 * Author: maxds
 *
 * Created on 14 novembre 2011, 13:05
 */

#include <cstdlib>
#include "Closures.h"
#include "Actor.h"
#include "ThreadPool.h"
#include <stack>

using namespace std;


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
}

/*
 * 
 */
int main(int argc, char** argv) {

    return 0;
}

