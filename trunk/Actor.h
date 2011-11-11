/* 
 * File:   Actor.h
 * Author: Ameziane NAIT ABDELAZIZ
 *
 * Created on 4 novembre 2011, 17:14
 */

#ifndef ACTOR_H
#define	ACTOR_H

#include "Thread.h"
#include "Closures.h"

namespace Acting{


    typedef void* Object;
    typedef void* Memory;
    typedef int   Id;
    typedef int   message_tag;

    /**
     */
    enum actor_status{
        STATUS_SUCCESS,
        STATUS_FAILLUR,
    };

    /**
     */
    struct message_t{
        message_tag tag;
        Memory      data;
        Id          sender;
        Id          receiver;
    };


    /**
     * 
     * @param arg
     * @return
     */
    template <typename ARG_T> struct routine_t {
        public:
            virtual actor_status run(ARG_T arg)=0;
    } ;


    /**
     */
    class Actor {
        
    private:
        Id actor_id;

        Memory actor_stack;
        queue<message_t> actor_message_box;
        Threading::Thread actor_thread;

        static Id compteur;

    public:
        Actor();
        Actor(const Actor& orig);

        /**
         * code exécuté par l'acteur
         */
        virtual void act()=0;

        /**
         * 
         * @return
         */
        Id GetId();

        /**
         * Executer une portion de code
         * @param fun : code à exécuter
         */
        template <typename IN,typename OUT> void act(Functional::Function<IN,OUT> &fun){

            //Creer un item
            //L'enfiler
            //Threading::add_job(fun.)
            
        }

        /**
         * Executer une boucle de code
         * @param fun: corps de la boucle
         */
        template <typename IN,typename OUT> void loop(Functional::Function<IN,OUT> &fun){
            
        }

        /**
         * Executer une portion de code tant qu'une condition est vraie
         * @param fun
         */
        template <typename IN,typename OUT> void loopWhile(Functional::Function<IN,OUT> &fun, bool& condition);
        

        virtual ~Actor();
    private:

    };


    Id Actor::compteur;
    /**
     * Constructor
     */
    Actor::Actor(){
        compteur++;
        this->actor_id = compteur;
    }

    /**
     * Destructor
     */
    Actor::~Actor(){

    }


    /**
     * Constructeur par recopie
     */
    Actor::Actor(const Actor& orig){

    }


    Id Actor::GetId(){
        return this->actor_id;
    }

    /**
     * Sending a message of type T from src to dest
     * @param src : Actor who sends the message
     * @param dest: Actor who will receive the message
     * @param message: The message to transmit
     * @return
     */
     template <typename T> int send(Actor& src,Actor& dest,T& message){
        std::cout << "Sending a message from a bottle"<<std::endl;

        //Ici encapsuler le message dans un paquet
        message_t pqt;
        pqt.data = NULL;
        pqt.receiver=-1;
        pqt.sender  =-1;
        pqt.tag     =-1;
    }

    /**
     * Receiving a message of type T from src
     * @param dest
     * @param src
     * @param message
     * @return
     */
    template <typename T> int receive(Actor& dest, Actor& src, T& message){
        std::cout << "Receiving message from a bottle"<<std::endl;
    }
}

#endif	/* ACTOR_H */

