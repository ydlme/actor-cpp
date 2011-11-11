/* 
 * File:   Thread.h
 * Author: maxds
 *
 * Created on 4 novembre 2011, 18:12
 */

#ifndef THREAD_H
#define	THREAD_H

#include <pthread.h>
#include <cstdlib>
#include <queue>


namespace Threading{

    typedef pthread_t           Thread;
    typedef pthread_cond_t      Vcondition;
    typedef pthread_attr_t      ThreadAttributs;
    typedef pthread_condattr_t  VconditionAttributs;
    
    
    typedef pthread_mutex_t     Mutex_t;
    typedef pthread_mutexattr_t Mutex_t_Attributs;
    

    namespace Condition
    {

        /**
         *
         * @param lock
         * @param vc
         * @return
         */
        int wait(Mutex_t* lock,Vcondition* vc){
            return pthread_cond_wait(vc,lock);
        }

        /**
         * 
         * @param vc
         * @return
         */
        int signal(Vcondition* vc){
            return pthread_cond_signal(vc);
        }

        /**
         *
         * @param c
         * @return
         */
        int init(Vcondition* c){
            return pthread_cond_init(c,NULL);
        }

        /**
         * 
         * @param c
         * @return
         */
        int destroy(Vcondition *c){
            return pthread_cond_destroy(c);
        }
    }

    namespace Mutex{

        /**
         *
         * @param m
         * @return
         */
        int init(Mutex_t * m){
            return pthread_mutex_init(m,NULL);
        }



        /**
         *
         * @param m
         * @return
         */
        int destroy(Mutex_t * m){
            return pthread_mutex_destroy(m);
        }

        /**
         *
         * @param m
         * @return
         */
        int lock(Mutex_t *m){
            return pthread_mutex_lock(m);
        }


        /**
         * 
         * @param m
         * @return
         */
        int unlock(Mutex_t* m){
            return pthread_mutex_unlock(m);
        }
    }

    /**
     * Retourne l'identifant du thread appelant
     */
    unsigned int get_thread_id(){
        return (unsigned int) pthread_self();
    }
    /**
     * Crée un thread simple sans propriétés d'ordonnancement
     * @param routine
     * @param args
     * @return
     */
    Thread* create_thread(void* (*routine) (void*), void* args){
        Thread* t = new Thread;
        int ret=pthread_create(t,NULL,routine,args);
        return (ret==0)?t:NULL;
    }


    /**
     * 
     * @param t
     * @param routine
     * @param args
     * @return
     */
    int create_thread(Thread* t,void* (*routine) (void*), void* args){
        int ret=pthread_create(t,NULL,routine,args);
        return ret;
    }

    /**
     *
     * @param t
     * @return
     */
    int join_thread(Thread& t){
        return pthread_join(t,NULL);
    }


    /**
     * 
     * @param s
     */
    void sleep_thread(unsigned int s){
        sleep(s);
    }
}

#endif	/* THREAD_H */

