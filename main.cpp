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
            std::printf("Hello from UnaActor [ID %d]\n",this->GetId());
            //sleep(1);
        }
    }
};

int main() {

    

    const int thread_count =6;
    Acting::ThreadPool<thread_count>::InitPool();

    stack<Acting::Actor*> to_free;
    //////////////////////////////////////////////////

    for(int i=0;i<10;i++){
        Acting::Actor *a = new MyActor();
        Acting::Actor *b = new AnotherActor();
        Acting::Actor *c = new UnaActor();

        Acting::ThreadPool<thread_count>::AddItem(a);
        Acting::ThreadPool<thread_count>::AddItem(b);
        Acting::ThreadPool<thread_count>::AddItem(c);

        to_free.push(a);
        to_free.push(b);
        to_free.push(c);
        
    }
    //////////////////////////////////////////////////
    Acting::ThreadPool<thread_count>::FinalizePool();

    while(!to_free.empty()){
        Acting::Actor* c = to_free.top();
        to_free.pop();
        delete c;
    }
    return 0;
}

