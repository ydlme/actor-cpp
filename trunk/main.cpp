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


typedef struct {
    int x;
    int y;
    float z;
    float *w;
} data_t;

int main() {

    

    const int thread_count =6;
    Acting::ThreadPool<thread_count>::InitPool();

    stack<Acting::Actor*> to_free;
    //////////////////////////////////////////////////

        Acting::Actor *a = new MyActor();
        Acting::Actor *b = new AnotherActor();
        Acting::Actor *c = new UnaActor();

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
        
        int tag_msg=a->send<int>(b,&msg);
        int tag_msg2=a->send<float>(b,&msg_2);



        cout<<"Message box of actor "<<b->GetId()<<" Size:"<<b->GetMessageCount()<<endl;
        int*   recv= b->receive<int>(tag_msg);
        float* recv2=b->receive<float>(tag_msg2);


        data_t d;
        d.x = 102;
        d.y = 100;
        d.z = 103;
        d.w = NULL;

        int d_tag = c->send<data_t>(a,&d);
        data_t* res=a->receive<data_t>(d_tag);

        
        assert(res->x==102);
        assert(res->w==0);

        printf("recv1 :%d recv2: %f\n",*recv,*recv2);
        cout<<"Message box of actor "<<b->GetId()<<" Size:"<<b->GetMessageCount()<<endl;
    
    //////////////////////////////////////////////////
    Acting::ThreadPool<thread_count>::FinalizePool();

    while(!to_free.empty()){
        Acting::Actor* c = to_free.top();
        to_free.pop();
        delete c;
    }
    return 0;
}

