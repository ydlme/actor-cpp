/* 
 * File:   Closures.h
 * Author: AMEZIANE NAIT ABDELAZIZ
 *
 * Created on 4 novembre 2011, 20:04
 */

#ifndef CLOSURES_H
#define	CLOSURES_H

#include <iostream>
using namespace std;


namespace Functional{

    
    template <typename IN,typename OUT> class Routine {
    public:
        virtual OUT invok(IN& arg)=0;
        
        //void *(*routine_addr) (void* arg)=0;

        virtual ~Routine(){
            cout << "deleting default instance"<<endl;
        }
    };


    /**
     * Classe fonction
     */
    template <typename IN,typename OUT > class Function{
        Routine<IN,OUT>* routine;

    public:
        Function(Routine<IN,OUT>* r){
            this->routine = r;
        }

        OUT operator () (IN arg){
            return this->routine->invok(arg);
        }
        
        ~Function(){
            //delete routine;
        }
    };
}
#endif	/* CLOSURES_H */

