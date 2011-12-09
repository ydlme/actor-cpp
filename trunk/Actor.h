/* 
 * File:   Actor.h
 * Author: Ameziane NAIT ABDELAZIZ
 * Created on 4 novembre 2011, 17:14
 */

#ifndef ACTOR_H
#define	ACTOR_H

#include "Thread.h"
#include "ThreadPool.h"


#include "Closures.h"


#include <list>
#include <string.h>
#include <cassert>
#include <string>
#include <cstdio>


using namespace std;

namespace Acting{

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
     * 
     */
    class ThreadPools{
        
    public:
        static void Init(){
            ThreadPool<10,1>::InitPool();
            ThreadPool<10,2>::InitPool();
        }
        
        
        static void Finit(){
            ThreadPool<10,1>::FinalizePool();
            ThreadPool<10,2>::FinalizePool();
        }
        
        static void AddToSendingPool(IActor* a){
            ThreadPool<10,2>::AddItem(a);
        }
        
        static void AddToStandardPool(IActor*a){
            ThreadPool<10,1>::AddItem(a);
        }
    };
    
    /********************************************/
    class SenderActor : public virtual IActor{
        
        
    private:
        message_t* __message;
        IActor* __recver;
        
    public:
        /**
         * @param msg
         * @param recv
         */
        SenderActor(message_t* msg,IActor* recv){
            this->__message = msg;
            this->__recver  = recv;
        }
        
        
        
        
        
        /*
         * 
         */
        std::list<message_t*>* GetMessageBox(){
            return NULL;
        } 
        
        /**
         * 
         * @return 
         */
        Threading::Mutex_t* GetMessageBoxMutex(){
           return NULL; 
        }
        
        /**
         * 
         */
        void act(){
            
            Threading::Mutex::lock(__recver->GetMessageBoxMutex());
            
            
            __recver->GetMessageBox()->push_front(__message);
            
	    #ifdef DEBUG
            cout << "Je suis dans sender actor"<<endl;    
            #endif
			
	    Threading::Mutex::unlock(__recver->GetMessageBoxMutex());
        }
    };
    
    
    
    
    /**
     * Toute cette classe doit être thread safe
     * Pour la simple raison que des acteurs peuvent créer d'autres acteurs
     * Dans ce cas, les multiples threads doivent se synchroniser sur certaines
     * données static de la class Actor
     */
    class Actor : public virtual IActor{
        
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
         * @param orig
         */
        Threading::Mutex_t __message_box_mutex;



    public:
    
        /**
         * 
         * @return 
         */
        std::list<message_t*>* GetMessageBox(){
            return &__actor_message_box;
        }
        
        
        
        /**
         * 
         * @return 
         */
         Threading::Mutex_t* GetMessageBoxMutex(){
             return &__message_box_mutex;
         }
        
         
         /**
          * 
          */
        static void Init(){
            ThreadPools::Init();
        }
        
        /**
         * 
         */
        static void Finit(){
            ThreadPools::Finit();
        }
        
     
        /**
         * 
         */
        static void AddItemToPool(IActor* a){
            ThreadPools::AddToStandardPool(a);
        }
        
        
        Actor();

        /**
         * @param orig
         */
        Actor(const Actor& orig);
        
        /**
         * Code qui sera exécuté par l'acteur
         */
        virtual void act()=0;

        /**
         * L'identifiant de l'acteur
         * @return
         */
        Id GetId();

        /**
         * Assigne un identifiant utilisateur à l'acteur
         * @param id
         */
        void SetUserId(Id id){
            this->__user__id = id;
        }
        
        /**
         * L'identifiant utilisateur de cet acteur
         * @return
         */
        Id GetUserId(){
            return this->__user__id;
        }

        /**
         * Assigne un nom à cet acteur
         * @param name
         */
        void SetName(const string name){
            this->__actor_name=name;
        }

        /**
         * Renvoie le nom de l'acteur
         * @return
         */
        string  GetName(){
            return this->__actor_name;
        }
       
        /**
         * La taille de la file des messages de l'acteur
         * @return
         */
        int GetMessageCount(){
            return this->__actor_message_box.size();
        }  

        /**
         * La méthode doit être estampillé
         */
        template <typename T> 
        int send(Actor* dest,T* message,int tag){
            
            
            //Ici encapsuler le message dans un paquet
            message_t* pqt = new message_t;
            
                            
            
            pqt->data    = message;
            pqt->receiver=dest->GetUserId();
            pqt->sender  =this->GetUserId();
            pqt->tag     = tag;
            
            Threading::Mutex::lock(&dest->__message_box_mutex);
            dest->__actor_message_box.push_front(pqt);
            printf("%s -- %d --> %s [%d octets]\n",this->GetName().c_str(),
                    (*(int*)pqt->data),dest->GetName().c_str(),sizeof(*message));
            Threading::Mutex::unlock(&dest->__message_box_mutex);
            
           
            
            return pqt->tag;
            
        }

        /**
         * Immediat receive
         * @param tag
         * @return 0 si message reçu, sinon -1
         */
        template<typename T>
        int receive(Actor* src,int tag,T* data){
            //bool message_receved = false;
            while(true)
            {
                    Threading::Mutex::lock(&this->__message_box_mutex);
                    for(list<message_t*>::iterator it=__actor_message_box.begin();it!=__actor_message_box.end();it++){
                        
                        
                        printf("Looking for tag %d message from actor %d\n",tag,src->GetUserId());
                        //sleep(1);
                        printf("sender:%d recever:%d tag:%d Box size:%d\n ",(*it)->sender,(*it)->receiver,(*it)->tag,__actor_message_box.size());
                        
                        
                        if(((*it)->tag==tag) && ((*it)->sender==src->GetId())){
       
                            void* ret  = memcpy((void*)data,(*it)->data,sizeof((* ((T*)(*it)->data))));
                            assert(ret!=NULL);
                            assert(data!=NULL);

                            
                            printf("Received from actor %d %d\n",src->GetUserId(),*((int*) data ));
                           
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
            //assert(0);
            return -1;
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

