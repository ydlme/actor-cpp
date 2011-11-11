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
    }
};
int main() {

    
    
    Acting::ThreadPool<2>::InitPool();

    //////////////////////////////////////////////////

    for(int i=0;i<10;i++){
        Acting::Actor *a = new MyActor();
        Acting::ThreadPool<2>::AddItem(a);
    }
    Acting::ThreadPool<2>::AddItem(new AnotherActor());
    Acting::ThreadPool<2>::AddItem(new UnaActor());
    //////////////////////////////////////////////////
    Acting::ThreadPool<2>::FinalizePool();
    return 0;
}

