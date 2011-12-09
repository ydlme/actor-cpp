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

struct message_t{
    message_tag tag;
    
    /*données contigue*/
    Memory      data;
    Id          sender;
    Id          receiver;
};

class IActor {
public:
    virtual void act()=0;
    virtual  std::list<message_t*>* GetMessageBox()=0;
    virtual  Threading::Mutex_t*    GetMessageBoxMutex()=0;
};

#endif	/* IACTOR_H */

