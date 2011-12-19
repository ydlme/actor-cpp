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

#define NB_THREADS 10


using namespace std;



namespace Acting{
  
    
#ifndef THREADPOOLS_CLASS
#define THREADPOOLS_CLASS
  /**
   * Ensemble de pools de thread utilisés par les acteurs
   */
  class ThreadPools{
    
  public:
    /**
     * Initialise les pools de threads
     */  
    static void Init()
    {
      ThreadPool<NB_THREADS,1>::InitPool();
      ThreadPool<NB_THREADS,2>::InitPool();
    }
    
    /**
     * Finalise les pools de threads
     */
    static void Finit()
    {
      ThreadPool<NB_THREADS,1>::FinalizePool();
      ThreadPool<NB_THREADS,2>::FinalizePool();
    }
    
    /**
     * Ajoute l'acteur a, au pool d'envoie
     * @param a
     */
    static void AddToSendingPool(IActor* a)
    {
      ThreadPool<NB_THREADS,2>::AddItem(a);
    }
    
    /**
     * Ajoute l'acteur a au pool standard
     * @param a
     */
    static void AddToStandardPool(IActor*a)
    {
      ThreadPool<NB_THREADS,1>::AddItem(a);
    }
  };
#endif
  
  
#ifndef CLASS_ACTOR_PROXY
#define CLASS_ACTOR_PROXY
  
class Actor;

class ProxyActor {
    public:
        static int SendMessage(message_t* packet,Actor* dst);
        static void SayHello();
};


#endif


#ifndef CLASS_SENDER_ACTOR
#define CLASS_SENDER_ACTOR
/**
 * Un acteur utilisé avec envoi asynchrone
 */
 class SenderActor : public virtual IActor{
  
 private:
  message_t* __message;
  IActor* __recver;
  actor_stat __stat;
  
 public:
  
  actor_stat GetStat(){
    return  __stat;
  }
  
  /**
   * @param msg
   * @param recv
   */
  SenderActor(message_t* msg,IActor* recv){
    this->__message = msg;
    this->__recver  = recv;
    this->__stat    = STAT_SLEEP;
  }   
  /**
   * Renvoie NULL
   */
  std::list<message_t*>* GetMessageBox(){
    return NULL;
  } 
    
  /**
   * Ne pas utiliser
   * @return NULL
   */
  Threading::Mutex_t* GetMessageBoxMutex(){
    return NULL; 
  }
  
  /**
   * Ajoute un message dans la box de l'envoyeur
   */
 protected:
  void Act(){
    
    Threading::Mutex::lock(__recver->GetMessageBoxMutex());
    __recver->GetMessageBox()->push_front(__message);
    
#ifdef DEBUG
    cout << "Je suis dans sender actor"<<endl;    
#endif
    
    Threading::Mutex::unlock(__recver->GetMessageBoxMutex());
  }
};
#endif
 
 
 
 
#ifndef CLASS_ACTOR
#define CLASS_ACTOR
    
/**
 * Toute cette classe doit être thread safe
 * Pour la simple raison que des acteurs peuvent créer d'autres acteurs
 * Dans ce cas, les multiples threads doivent se synchroniser sur certaines
 * données static de la class Actor
 */
 class Actor : public virtual IActor{ 
  
  friend int ProxyActor::SendMessage(message_t* m,Actor* dest);
  
 private:
#ifndef REGION_MEMBRES
#define REGION_MEMBRES
  /**
   * Identifiant de l'acteur
   */
  Id __actor_id;
  /**
   * Etat de l'acteur
   */
  actor_stat __stat;
  /**
   * Sauvgarde de l'etat de l'acteur
   */
  actor_stat __saved_stat;
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
   * Un compteur partagé utilisé pour générer l'identifant de l'acteur
   */
  static Id __compteur;
  /**
   * Un mutex pour proteger la boite à lettres de l'acteur
   */
  Threading::Mutex_t __message_box_mutex;
#endif
  
  
#ifndef REGION_METHODES
#define REGION_METHODES
 public:
  /**
   * 
   * @return adresse de la boite à lettres de l'acteur 
   */
  std::list<message_t*>* GetMessageBox();
  /**
   *  
   * @return pointeur sur le mutex protégeant la boite à lettres 
   */
  Threading::Mutex_t* GetMessageBoxMutex();
  
  /**
   * 
   * @return l'etat de l'acteur
   */
    actor_stat GetStat();
    
    /**
     * Initialise les pools de threads utilisés par la bibliothèque d'acteurs
     */
    static void Init();
    /**
     * Finalise les pools de threads utilisés par la bibliothèque d'acteurs
     */
    static void Finit();
    /**
     * Ajoute un acteur au pool standard
     */
    static void AddItemToPool(IActor* a);
    /**
     * Construit un acteur par défaut
     */
    Actor(); 
    /**
     * @param orig
     */
    Actor(const Actor& orig);
    /**
     * Code qui sera exécuté par l'acteur
     */
  protected:
    virtual void act()=0;
   
  private:
      void Act();
    
    
  public:
    /**
     * L'identifiant de l'acteur
     * @return identifiant de l'acteur
     */
    Id GetId();
    
    /**
     * Assigne un identifiant utilisateur à l'acteur
     * @param id
     */
    void SetUserId(Id id);
    
    /**
     * L'identifiant utilisateur de cet acteur
     * @return
     */
    Id GetUserId();
    /**
     * Assigne un nom à cet acteur
     * @param name
     */
    void SetName(const string name);
    
    /**
     * Renvoie le nom de l'acteur
     * @return
     */
    string  GetName();
    
    /**
     * La taille de la file des messages de l'acteur
     * @return
     */
    int GetMessageCount();
    
    
    /**
     * Affiche le contenu de la boite à lettres de l'acteur courant
     */
    inline void PrintMessageBox();
    
    /**
     * La méthode doit être estampillé
     */
    template <typename T> 
      int send(Actor* dest,T* message,int tag);
    
    /**
     * Immediat receive
     * @param tag
     * @return 0 si message reçu, sinon -1
     */
    template<typename T>
      int receive(Actor* src,int tag,T* data);
    
    
    /**
     * Lance l'exécution de l'acteur
     */
    void start();
    /**
     * Executer une portion de code
     * @param fun : code à exécuter
     */
    template <typename IN,typename OUT> void act(Functional::Function<IN,OUT> &fun);
    
    /**
     * Executer une boucle de code
     * @param fun: corps de la boucle
     */
    template <typename IN,typename OUT> void loop(Functional::Function<IN,OUT> &fun);
    
    /**
     * Executer une portion de code tant qu'une condition est vraie
     * @param fun
     */
    template <typename IN,typename OUT> void loopWhile(Functional::Function<IN,OUT> &fun, bool& condition);
    
    
    virtual ~Actor();
    
#endif
    
  };
#endif


  
  
#ifndef CLASS_ACTOR_IMPL
#define CLASS_ACTOR_IMPL
  
  
  /**
   * 
   * @return 
   */
  actor_stat Actor::GetStat(){
      return __stat;
  }
  
  /**
   */
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
  
  std::list<message_t*>* Actor::GetMessageBox(){
    return &__actor_message_box;
  }
  
  
  
  /**
   * Retourne l'adresse de la boite à lettres du thread courant 
   * @return 
   */
  Threading::Mutex_t* Actor::GetMessageBoxMutex(){
    return &__message_box_mutex;
  }
  
  
  /**
   * Initialise les pools de threads utilisés par la bibliothèque d'acteurs
   */
  void Actor::Init(){
    ThreadPools::Init();
  }
  
  /**
   * Finalise les pools de threads utilisés par la bibliothèque d'acteurs
   */
  void Actor::Finit(){
    ThreadPools::Finit();
  }
  
  /**
   * Ajoute un acteur au pool standard
   */
  void Actor::AddItemToPool(IActor* a){
    ThreadPools::AddToStandardPool(a);
  }
  
  /**
   * Méthode exécutée par le thread
   */
  void Actor::Act(){
    //Exécution du code utilisateur 
    act();
    
    //Signale que l'acteur a fini
    printf("L'acteur %d a fini \n",this->GetUserId());
    __stat = STAT_ENDED;
  }
  
    /**
     * 
     * @param id
     */
    void Actor::SetUserId(Id id){
      this->__user__id = id;
    }
    
    /**
     * L'identifiant utilisateur de cet acteur
     * @return
     */
    Id Actor::GetUserId(){
      return this->__user__id;
    }
    
    /**
     * Assigne un nom à cet acteur
     * @param name
     */
    void Actor::SetName(const string name){
      this->__actor_name=name;
    }
    
    /**
     * Renvoie le nom de l'acteur
     * @return
     */
    string  Actor::GetName(){
      return this->__actor_name;
    }
    
    /**
     * La taille de la file des messages de l'acteur
     * @return
     */
    int Actor::GetMessageCount(){
      return this->__actor_message_box.size();
    }  
    
    
    /**
     * Affiche le contenu de la boite à lettres de l'acteur courant
     */
    inline void Actor::PrintMessageBox(){
      
      for(list<message_t*>::iterator it = __actor_message_box.begin()
	    ;it!=__actor_message_box.end();it++)
	{
	  printf("[tag:%d,src:%d,dst:%d data:%d]:: ",(*it)->tag,(*it)->sender,(*it)->receiver,*((int*)(*it)->data)); 
	}
      printf("\n"); 
    }
    
    /**
     * La méthode doit être estampillé
     */
    template <typename T> 
      int Actor::send(Actor* dest,T* message,int tag){
      
      /*Mettre des operations atomiques*/  
      this->__saved_stat = this->__stat;
      this->__stat = STAT_SENDING;
      
      //Ici encapsuler le message dans un paquet
      message_t* pqt = new message_t;
      
      
      
      //Copier à tout prix, si non c'est mort
      
      pqt->data    = new char[sizeof(T)];
      pqt->data    = memcpy(pqt->data,message,sizeof(T));
      pqt->receiver= dest->GetUserId();
      pqt->sender  = this->GetUserId();
      pqt->tag     = tag;
      
      //Introduire un proxy actor, afin de d'abstraire cette action
      ProxyActor::SendMessage(pqt,dest);
      
      
      printf("%d -- %d --> %d [%d octets] [tag: %d]\n",this->GetUserId(),
	     (*(int*)pqt->data),dest->GetUserId(),sizeof(*message),tag);
      
      this->__stat = __saved_stat;
      return pqt->tag;
      
    }
    
    /**
     * Immediat receive
     * @param tag
     * @return 0 si message reçu, sinon -1
     */
    template<typename T>
      int Actor::receive(Actor* src,int tag,T* data){
      //bool message_receved = false;
      
      this->__saved_stat = __stat;
      this->__stat = STAT_RECVING;
      
      int cpt =0;
      while(true)
	{
	  cpt++;
	  Threading::Mutex::lock(&this->__message_box_mutex);
	  for(list<message_t*>::iterator it=__actor_message_box.begin();it!=__actor_message_box.end();it++){
	    
	    
	    printf("Step [%d] [Actor %d] is looking for a message from [actor %d] with [tag %d]\n",
		   cpt,
		   this->GetUserId(),
		   tag,
		   src->GetUserId());
	    
#ifdef DEBUG
	    PrintMessageBox();
#endif
	    printf("sender:%d recever:%d tag:%d Box size:%d\n ",(*it)->sender,(*it)->receiver,(*it)->tag,__actor_message_box.size());
	    
	    
	    if(((*it)->tag==tag) && ((*it)->sender==src->GetUserId())){
	      
	      void* ret  = memcpy((void*)data,(*it)->data,sizeof((* ((T*)(*it)->data))));
	      assert(ret!=NULL);
	      assert(data!=NULL);
	      
	      *data = *((T*) ret);
	      
	      printf("Received from actor %d %d\n",src->GetUserId(),(int) (*data) );
	      
	      //Suppression du message de la boite à lettres
	      delete (*it);
	      
	      __actor_message_box.remove(*it);
	      Threading::Mutex::unlock(&this->__message_box_mutex);
	      this->__stat = __saved_stat;
	      
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
      this->__stat = __saved_stat;
      return -1;
    }
    
    
    /**
     * Lance l'exécution de l'acteur
     */
    void Actor::start(){
      AddItemToPool(this);
    }
    
    /**
     * Executer une portion de code
     * @param fun : code à exécuter
     */
    template <typename IN,typename OUT> void Actor::act(Functional::Function<IN,OUT> &fun){
      
      //Creer un item
      //L'enfiler
      //Threading::add_job(fun.)
      
    }
    
    /**
     * Executer une boucle de code
     * @param fun: corps de la boucle
     */
    template <typename IN,typename OUT> void Actor::loop(Functional::Function<IN,OUT> &fun){
      
    }
    
    /**
     * Executer une portion de code tant qu'une condition est vraie
     * @param fun
     */
    template <typename IN,typename OUT> void Actor::loopWhile(Functional::Function<IN,OUT> &fun, bool& condition){
      
    }
#endif
  
#ifndef CLASS_ACTOR_PROXY_IMPL
#define CLASS_ACTOR_PROXY_IMPL 
    /**
     * 
     * @param packet
     * @param dest
     * @return 
     */
    int ProxyActor::SendMessage(message_t* packet, Actor* dest){
      
      //ici, tester si dest est un acteur locale, si oui
      
      Threading::Mutex::lock(&dest->__message_box_mutex);
      dest->__actor_message_box.push_front(packet);
      Threading::Mutex::unlock(&dest->__message_box_mutex);
      
      return 0;
      
    }
#endif
}
#endif	/* ACTOR_H */

