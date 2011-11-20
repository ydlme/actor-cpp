/* 
 * File:   Actor.h
 * Author: Ameziane NAIT ABDELAZIZ
 * Created on 4 novembre 2011, 17:14
 */

#ifndef ACTOR_H
#define	ACTOR_H

#include "Thread.h"
#include "Closures.h"
#include <list>
#include <string.h>
#include <cassert>
#include <string>


using namespace std;

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
        /**
         * données contigues
         */
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
     * Toute cette classe doit être thread safe
     * Pour la simple raison que des acteurs peuvent créer d'autres acteurs
     * Dans ce cas, les multiples threads doivent se synchroniser sur certaines
     * données static de la class Actor
     */
    class Actor {
        
    private:
        /**
         * Identifiant de l'acteur
         */
        Id __actor_id;
        
        /**
         * Etiquette de l'acteur, a du sens pour l'utilisateur
         */
        string __actor_name;

        /**
         * Identifiant donné par l'utilisateur
         */
        Id __user__id;

        /**
         * Zone mémoire stockant la pile de l'acteur
         * @Attention utilisé dans le cas de l'ordonnancement des acteurs
         */
        Memory __actor_stack;

        /**
         * La file des messages de l'acteur courant
         */
        list<message_t*> __actor_message_box;

        /**
         * Un compteur utilisé pour générer l'identifant de l'acteur
         */
        static Id __compteur;

        /**
         * 
         * @param orig
         */
        Threading::Mutex_t __message_box_mutex;



    public:
        /**
         *
         */
        Actor();

        /**
         *
         * @param orig
         */
        Actor(const Actor& orig);
        
        /**
         * code exécuté par l'acteur
         */
        virtual void act()=0;

        
        /**
         * L'identifiant de l'acteur
         * @return
         */
        Id GetId();


        /**
         * 
         * @param id
         */
        void SetUserId(Id id){
            this->__user__id = id;
        }


        /**
         *
         * @return
         */
        Id GetUserId(){
            return this->__user__id;
        }

        /**
         *
         * @param name
         */
        void SetName(const string name){
            this->__actor_name=name;
        }


        /**
         * 
         * @return
         */
        string  GetName(){
            return this->__actor_name;
        }

        
        /**
         * La taille de la file des messages
         * @return
         */
        int GetMessageCount(){
            return this->__actor_message_box.size();
        }


        

        /**
         * La méthode doit être estampillé
         */
        template <typename T> int send(Actor* dest,T* message,int tag){

            //printf("%s ----> %s [%d octets]\n",this->GetName().c_str(),dest->GetName().c_str(),sizeof(*message));
            //Ici encapsuler le message dans un paquet
            message_t* pqt = new message_t;
            pqt->data = (Memory) message;
            pqt->receiver=dest->GetId();
            pqt->sender  =this->GetId();
            pqt->tag     = tag;//Actor::compteur++;

            Threading::Mutex::lock(&dest->__message_box_mutex);
            dest->__actor_message_box.push_front(pqt);
            
			#ifdef DEBUG
			printf("Message box [actor:%d alias:%s size:%d]\n",this->__actor_id,this->__actor_name.c_str(),this->__actor_message_box.size());
            printf("Message box [actor:%d alias:%s size:%d]\n",dest->__actor_id,dest->__actor_name.c_str(),dest->__actor_message_box.size());
            #endif
			
			Threading::Mutex::unlock(&dest->__message_box_mutex);

            
            return pqt->tag;
        }

        /**
         * Attention au thread safe
         * @param tag
         * @return
         */
        template<typename T> int receive(Actor* src,int tag,T* data){
            //std::cout << "Receiving message tag: "<< tag << std::endl;
            //bool message_receved = false;
            while(true){
                    Threading::Mutex::lock(&this->__message_box_mutex);
                    
                    for(list<message_t*>::iterator it=__actor_message_box.begin();it!=__actor_message_box.end();it++){
                        if(((*it)->tag==tag) && ((*it)->sender==src->GetId())){

                            //message_receved = true;
                            void* ret  = memcpy((void*)data,(*it)->data,sizeof((* ((T*)(*it)->data))));
                            assert(ret!=NULL);
                            assert(data!=NULL);


                            //Suppression du message de la boite à lettres
                            delete (*it);
                            
                            __actor_message_box.remove(*it);
                            Threading::Mutex::unlock(&this->__message_box_mutex);
                            return 0;

                    }
                    
                }

                    Threading::Mutex::unlock(&this->__message_box_mutex);
            }
            

            //Si on arrive ici c'est qu'il n'y a pas de message de la file
            //Il faut mettre une attente sur condition
            //Il faudrait que les envoies de messages s'executent dans plusieurs threads

            //Pour faire cela il est nécessaire d'avoir un autre pool de thread qui exécutera
            //le code d'envoie de message
            //Un appel à send ne sera pas bloquant
            //Un appel à recev le sera, parce que il y'aura une attente sur file de message

            //Dans le modèle producteur consommateur send sera une routine de production
            //recv sera une routine de consommation
            assert(0);
            return 0;
        }
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



    Id Actor::__compteur;
    
    /**
     * Constructor
     */
    Actor::Actor(){
        __compteur++;
        this->__actor_id = __compteur;
        this->SetName(string(""));
        Threading::Mutex::init(&this->__message_box_mutex);

    }

    /**
     * Destructor
     */
    Actor::~Actor(){
        Threading::Mutex::destroy(&this->__message_box_mutex);
    }


    /**
     * Constructeur par recopie
     */
    Actor::Actor(const Actor& orig){

    }


    /**
     * 
     * @return
     */
    Id Actor::GetId(){
        return this->__actor_id;
    }
}

#endif	/* ACTOR_H */

