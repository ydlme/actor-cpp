/* 
 * File:   IActor.h
 * Author: NAIT
 *
 * Created on 4 décembre 2011, 20:11
 */

#ifndef IACTOR_H
#define	IACTOR_H


#include <list>
#include "Thread.h"
#include <cstdio>

typedef void* Object;
typedef void* Memory;
typedef int   Id;
typedef int   message_tag;

    
   
    
    
/**
 * Statut de l'acteur
 */
enum actor_status{
    STATUS_SUCCESS,
    STATUS_FAILLUR,
};


/**
 *Etat de l'acteur
 */
enum actor_stat{
    STAT_SLEEP,
    STAT_EXEC,
    STAT_READY,
    STAT_SENDING,
    STAT_RECVING,
    STAT_ENDED
};


/**
 *Paquet pour envoyer 
 *des messages
 */
struct message_t{
    message_tag tag;
    
    /*données contigue*/
    Memory      data;
    Id          sender;
    Id          receiver;
};

#ifndef STRUCT_ACTOR_CONTEXT
#define STRUCT_ACTOR_CONTEXT
#include <setjmp.h>
  /**
   *Context de l'acteur
   */
typedef struct {
  jmp_buf __ctx;
  Id      __actor_id;
  int     __ret_val;
  int     __priority;
  bool    __valide;
  int     __arg;
} actor_context;

#include <string.h>
static int compteur =0;

/**
 *Sauvgarde du context du thread appelant
 */
#define save_context(c)				\
  setjmp((c)->__ctx)			         	

/**
 *Restore le context c, l'appel à cette fonction
 *Ne retourne jammais
 */
#define restor_context(c)			\
  longjmp((c)->__ctx,1)

#endif

/**
 * Interface pour manipuler 
 * tous les types d'acteurs
 */
class IActor {
 public:
    virtual  void                   Act()=0;
    virtual  std::list<message_t*>* GetMessageBox()=0;
    virtual  Threading::Mutex_t*    GetMessageBoxMutex()=0;
    virtual  actor_stat             GetStat()=0;
    virtual  Id                     GetUserId() {return 0;}
};

#endif	/* IACTOR_H */

