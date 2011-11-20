using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace LibActor
{
    /// <summary>
    /// Une classe permettant de manipuler un pool de thread
    /// </summary>
    /// Attention. Il faut dedier un seul thread scrutage des boites à lettres des acteurs de chaque thread
    /// De la sorte, un thread qui fait receive se mettra en attente passive jusqu'à ce que le thread de scrutation le 
    /// reveille.
    /// L'algorithme est le suivant:
    /// send dest message tag:
    ///                      dest.__message_box__mutex.lock();
    ///                      dest.__message__box.Add();
    ///                      dest.__message_box__mutex.unlock();
    ///                      return;
    /// receive dest message tag :
    ///                      this.__message__box__mutex.lock();
    ///                      if(this.__message__box.contains(dest,tag))
    ///                      {
    ///                         //c'est gagné
    ///                      }
    ///                      else
    ///                      {
    ///                         this.__message__box__mutex.unlock();
    ///                         this.__new_message_araised.Wait();
    ///                         continue;
    ///                      }
    ///                      this.__message__box__mutex.unlock();
    /// Scruter :
    ///                     while(true)
    ///                     {
    ///                         __new_message_araised.Wait();
    ///                         for(int i=0;i inf thread_count;i++)
    ///                         {
    ///                            //parcourir
    ///                         }
    ///                     }
    /// Créer un thread qui fait du vol de travail
    /// 

       

    public class ThreadPool
    {
        /// <summary>
        /// Le acteurs en cours d'exécution, autant qu'il ya de threads
        /// </summary>
        protected Actor[] __current_actors = null;


        /// <summary>
        /// 
        /// </summary>
        protected Queue<Actor>[] __working_actors = null;
        
        
        
        /// <summary>
        /// Les threads constituant le pool
        /// </summary>
        protected Thread[] __pool=null;


        /// <summary>
        /// Permet de mesurer le taux d'occupation des threads du pool
        /// </summary>
        protected int[] __priorities = null;

        /// <summary>
        /// Protection du tableau de priorités
        /// </summary>
        protected Mutex __mutex_priorities =null;
        
        /// <summary>
        /// Variables de condition file non vide
        /// </summary>
        protected System.Threading.ManualResetEventSlim[] __condition_queue_not_empty=null;

        /// <summary>
        /// Associations des threads et d'identifiant
        /// </summary>
        protected Dictionary<int, int> __threads_id = null;

        /// <summary>
        /// Un verrou de protection du dictionnaire
        /// </summary>
        protected Mutex __mutex_ditionnary;


        /// <summary>
        /// Protection de la l'acces aux file de travaux
        /// </summary>
        static protected Mutex __mutex_jobs = new Mutex();


        /// <summary>
        /// Files de travaux pour chaque thread du pool
        /// </summary>
        Queue<Actor> [] __jobs=null;

        /// <summary>
        /// Verous de protection de file de travaux par thread du pool
        /// </summary>
        Mutex[] __mutexes__queues=null;

        
        /// <summary>
        /// Taille du pool
        /// </summary>
        int __size;

        /// <summary>
        /// Initialisation des structures de données du pool
        /// </summary>
        /// <param name="size"></param>
        public ThreadPool(int size)
        {
            __size = size;
            __working_actors = new Queue<Actor>[__size];
            __current_actors = new Actor[__size];
            __mutex_priorities = new Mutex();
            __priorities = new int[__size];
            __threads_id = new Dictionary<int, int>();
            __mutexes__queues = new Mutex[__size];
            __pool = new Thread[__size];
            __jobs = new Queue<Actor>[__size];
            __mutex_ditionnary = new Mutex();
            __condition_queue_not_empty = new ManualResetEventSlim[__size];
        }


        /// <summary>
        /// Initialise les structures de données utilisées par le pool de thread
        /// </summary>
        public void InitPool()
        {
            for (int i = 0; i < __size; i++)
            {
                __priorities[i] = 0;
                __condition_queue_not_empty[i] = new ManualResetEventSlim();
                __working_actors[i] = new Queue<Actor>();
                __jobs[i] = new Queue<Actor>();
                __mutexes__queues[i] = new Mutex();
                
                __pool[i]=  new Thread(new ThreadStart
                    (() => 
                    { 
                        this.Loop(); 
                    }));

                __threads_id.Add(__pool[i].ManagedThreadId, i);
            }
        }


        /// <summary>
        /// Renvoie l'acteur en cours d'execution au sein du thread courant
        /// </summary>
        /// <returns></returns>
        public Actor GetCurrentActor()
        {
            //Ici recuperer l'id du thread qui fait la demande
            int tid = Thread.CurrentThread.ManagedThreadId;
            int thread_queue_id = __threads_id[tid];
            
            __mutexes__queues[thread_queue_id].WaitOne();
            var res = __current_actors[thread_queue_id];
            __mutexes__queues[thread_queue_id].ReleaseMutex();

            return res;
        }

        /// <summary>
        /// Lance l'exécution de chaque thread du pool
        /// </summary>
        public void startPool()
        {
            __pool.ToList().ForEach(th => th.Start());
        }




        /// <summary>
        /// Boucle de consommation exécuté par chaque thread du pool 
        /// </summary>
        protected virtual void Loop()
        {
            int tid = Thread.CurrentThread.ManagedThreadId;
            int thread_queue_id = __threads_id[tid];
            Actor current_actor = null;

            while (true)
            {
                __condition_queue_not_empty[thread_queue_id].Wait();
                


                __mutexes__queues[thread_queue_id].WaitOne();



                //Attention
                if (__jobs[thread_queue_id].Count != 0)
                {
                    current_actor = __jobs[thread_queue_id].Dequeue();
                    __working_actors[thread_queue_id].Enqueue(current_actor);

#if DEBUG
                    Console.WriteLine(string.Format("Thread {0} task queue: {1} current actor {2}", tid, __jobs[thread_queue_id].Count, current_actor.Name));
#endif    
                    /*Une réference sur l'acteur en cours d'execution*/
                    __current_actors[thread_queue_id] = current_actor;

                }
                else
                {
                    __condition_queue_not_empty[thread_queue_id].Reset();
                    
                }
                
                __mutexes__queues[thread_queue_id].ReleaseMutex();
                

                
                
                
                if(current_actor!=null)
                {
                    
                    Console.WriteLine("Thread [" + Thread.CurrentThread.ManagedThreadId + "]" + " executing actor " + current_actor.Name);
                    
                    
                    /*exécution du code de l'acteur*/
                    current_actor.Act();
                    current_actor = null;

                    /*On vire l'acteur de la file*/
                    __working_actors[thread_queue_id].Dequeue();
                }



                /*Baisser de 1 la taille des travaux*/
                __mutex_priorities.WaitOne();    
                __priorities[thread_queue_id]--;
                __mutex_priorities.ReleaseMutex();
                /****************************************************
                 * Fin de section critique
                 * */
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected static void Stealing()
        {
            var cnt = true;
            while (cnt)
            {
                //dormir un un laps de temps
                System.Threading.Thread.Sleep(200);

                //trouver les threads x1,x2,...,xn dont la file des jobs est vide

                //trouver les threads y1,y2,...,ym dont la file des jobs n'est pas vide

                //transferer les jobs de {y1,y2,y3,....,ym} vers {x1,....,xn}

                //créer des threads si n<m
            }
        }
        /// <summary>
        /// Ajoute un acteur à la file des travaux
        /// </summary>
        /// <param name="a"></param>
        public void AddActor(Actor a)
        {
            int elected = 0;
            //ici il faut choisir le thread elu pour exécuter l'acteur a
            __mutex_priorities.WaitOne();
            elected = GetMinPriorityThread();


            /*Augmenter de 1 la taille des travaux du thread elu*/
            __priorities[elected]++;
            __mutex_priorities.ReleaseMutex();

            //pour l'instant on choisit le thread 0 comme thread élu
         
                 
            __mutexes__queues[elected].WaitOne();
            __jobs[elected].Enqueue(a);
            
            
            //__mutexes__conditions[elected].WaitOne();
            
            __condition_queue_not_empty[elected].Set();
            //__mutexes__conditions[elected].ReleaseMutex();

            __mutexes__queues[elected].ReleaseMutex();
        }


        /// <summary>
        /// Retourne le thread elu
        /// </summary>
        /// <returns></returns>
        protected int GetMinPriorityThread()
        {
            int min = Int32.MaxValue;
            int min_idx = 0;
            for (int i = 0; i < __size; i++)
            {
                if (__priorities[i] < min)
                {
                    min_idx = i;
                    min = __priorities[i];
                }
            }

            return min_idx;
        }

        /// <summary>
        /// Renvoie une réference vers l'acteur idenitifé par son id
        /// </summary>
        /// <param name="dest">Identifiant de l'acteur</param>
        /// <returns>Une reference vers l'acteur si il existe sinon null</returns>
        internal Actor GetActor(int id)
        {
            Actor res = null;
            for (int i = 0; i < __size; i++)
            {
                __mutexes__queues[i].WaitOne();
                var tmp = __working_actors[i].Where(a => a.Id == id).ToList();
                if (tmp.Count != 0)
                {
                    res = tmp.First();
                    __mutexes__queues[i].ReleaseMutex();
                    break;
                }
                __mutexes__queues[i].ReleaseMutex();
            }
            return res;
        }
    }
}
