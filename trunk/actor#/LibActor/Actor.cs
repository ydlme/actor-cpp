using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibActor
{

    class Pool
    {
        /// <summary>
        /// 
        /// </summary>
        static private ThreadPool __pool_production=null;
        
        
        /// <summary>
        /// 
        /// </summary>
        static private Threading.ThreadPoolReceiving __pool_consumption = null;


        /// <summary>
        /// 
        /// </summary>
        public static Threading.ThreadPoolReceiving ConsumptionPool
        {
            get {

                   
                    return Pool.__pool_consumption; 
            }
            
        }

        /// <summary>
        /// 
        /// </summary>
        static public ThreadPool PrudctionPool
        {
            get { 
                if (__pool_production == null) {
                    Init(10);
                };
                return __pool_production; 
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="size"></param>
        static private void Init(int size)
        {
            __pool_production = new LibActor.ThreadPool(size);
            __pool_consumption = new Threading.ThreadPoolReceiving(size);


            /**/
            __pool_consumption.InitPool();
            __pool_consumption.startPool();
            /******************************/

            /**/
            __pool_production.InitPool();
            __pool_production.startPool();
        }
    }

    

    /// <summary>
    /// 
    /// </summary>
    public delegate void Job();

    public class Actor
    {

        internal class message_t
        {
            public Object __data;
            public int __tag;
            public int __src;
            public int __dest;
            /*Attention pas encors sûr*/
            public Actor __src_;
            public Actor __dest_;
        }


        #region fields
        
        /// <summary>
        /// 
        /// </summary>
        private Job __item = null;
        
        /// <summary>
        /// 
        /// </summary>
        private string __name;
        
        /// <summary>
        /// 
        /// </summary>
        private int __id = 0;
        
        /// <summary>
        /// 
        /// </summary>
        private static int __id_gen = 0;
        /// <summary>
        /// File de message de l'acteur
        /// </summary>
        private List<message_t> __actor_message_box=null;
        
        /// <summary>
        /// 
        /// </summary>
        private System.Threading.Mutex __mutex_actor_message_box = null;

        /// <summary>
        /// 
        /// </summary>
        private System.Threading.ManualResetEventSlim __condition__message_box_not__empty = null;
        
        /// <summary>
        /// 
        /// </summary>
        private int __host_thread;
        #endregion




        #region properties
        
        /// <summary>
        /// Nom identifiant l'acteur
        /// </summary>
        public string Name
        {
            get { return __name; }
            set { __name = value; }
        }

        /// <summary>
        /// Identifiant du thread abritant cet acteur
        /// </summary>
        public int HostThread
        {
            get { return __host_thread; }
        }



        /// <summary>
        /// Retourne l'identifiant de l'acteur en cours d'exécution
        /// </summary>
        /// <returns></returns>
        public static Actor Self()
        {
            return Pool.PrudctionPool.GetCurrentActor();
        }

        /// <summary>
        /// L'identifiant de l'acteur
        /// </summary>
        public int Id
        {
            get { return __id; }
            set { __id = value; }
        }

        #endregion
        
        
        /// <summary>
        /// Initialise l'environnement d'exécution des acteurs
        /// </summary>
        public static void Init()
        {
            Pool.PrudctionPool.InitPool();
            Pool.PrudctionPool.startPool();
        }

        /// <summary>
        /// Finalise l'environnement d'exécution des acteurs
        /// </summary>
        public static void Finit()
        {
            
        }

        /// <summary>
        /// 
        /// </summary>
        public Actor()
        {
            __condition__message_box_not__empty = new System.Threading.ManualResetEventSlim();
            __actor_message_box = new List<message_t>();
            __mutex_actor_message_box = new System.Threading.Mutex();
            __id = __id_gen;
            __id_gen++;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="j"></param>
        public Actor(Job j):this()
        {
            __item = j;

        }


        /// <summary>
        /// Lancer l'acteur
        /// </summary>
        public void Start()
        {
            Pool.PrudctionPool.AddActor(this);   
        }


        /// <summary>
        /// Code exécuté par l'acteur
        /// </summary>
        public virtual void Act()
        {


            __host_thread=System.Threading.Thread.CurrentThread.ManagedThreadId;
            if (__item != null)
            {
                __item.Invoke();
            }
        }


        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="dest"></param>
        /// <param name="message"></param>
        /// <param name="tag"></param>
        /// <returns></returns>
        public int send<T>(int dest, T message, int tag)
        {
            Actor dest_ =Pool.PrudctionPool.GetActor(dest);
            return send<T>(dest_,message,tag);
        }


        /// <summary>
        /// Envoie un message d'un type donnée 
        /// </summary>
        /// <typeparam name="T">Le type du message</typeparam>
        /// <param name="dest">L'acteur qui va recevoir le message</param>
        /// <param name="message">Le contenu du message</param>
        /// <returns>Code d'erreur</returns>
        public int send<T>(Actor dest, T message,int tag)
        {

            //plutôt que de faire le send dans le même pool que le receive
            //Creer un acteur reduit, qui ne sait pas envoyer de message
            //Dans ce nouveau pool les threads pourront faire des receive

            

            message_t paquet = new message_t();
            paquet.__data = message;
            paquet.__dest = dest.__id;
            paquet.__src = this.__id;
            paquet.__tag = tag;
            paquet.__src_ = this;
            paquet.__dest_ = dest;

            dest.__mutex_actor_message_box.WaitOne();
            dest.__actor_message_box.Add(paquet);
            
            //Ici reveiller plutot le thread de scrutation
            dest.__condition__message_box_not__empty.Set();
            
            dest.__mutex_actor_message_box.ReleaseMutex();

#if DEBUG
            Console.WriteLine("Sending message from " + this.Name + " to " + dest.Name);
#endif
            return 0;
        }


        /// <summary>
        /// Immediat receive
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="src"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        public int receive<T>(Actor src, ref T result,int tag)
        {
            bool cnt = true;
            
            while (cnt)
            {
#if DEBUG 
                System.Threading.Thread.Sleep(100);
                Console.WriteLine("Looking letter box from actor "+this.Name);
#endif
                //Il faudra mettre une attente passive sur message recu
                //Mettre une méthode receive qui fait du time out

                this.__mutex_actor_message_box.WaitOne();


                if (__actor_message_box.Count == 0)
                {
                    __condition__message_box_not__empty.Reset();
                    __mutex_actor_message_box.ReleaseMutex();
                    
                    //A ce niveau il y' a risque d'endordissement perpetuel
                    __condition__message_box_not__empty.Wait();
                    
                    continue;
                }


                var val = __actor_message_box.Find(message =>((message.__tag == tag) && (message.__src == src.__id) && (message.__dest == __id) && (message.__data.GetType() == typeof(T))));

                if (val!= null)
                {
                    __actor_message_box.Remove(val);
                    result = (T)val.__data;
                    this.__mutex_actor_message_box.ReleaseMutex();
                    return 0;

                }

                this.__mutex_actor_message_box.ReleaseMutex();
            
            }
            return 0;
        }


        /// <summary>
        /// Recupère le premier message de la boite ayant le tag fournie
        /// </summary>
        /// <typeparam name="T">Type de l'element à recuperer de la boite aux lettres</typeparam>
        /// <param name="result">Reference qui va pointer l'element récupéré</param>
        /// <param name="tag">Tag du message à recuperer</param>
        /// <returns>Identifiant de l'acteur ayant envoyé le message reçu</returns>
        public int receive<T>(ref T result, int tag)
        {
            bool cnt = true;
        
            while (cnt)
            {
                //Il faudra mettre une attente passive sur message recu
                //Mettre une méthode receive qui fait du time out

                __condition__message_box_not__empty.Wait();

#if DEBUG
                System.Threading.Thread.Sleep(200);
                Console.WriteLine("Looking letter box from actor " + this.Name);
#endif

                __mutex_actor_message_box.WaitOne();
                
                //var val = __actor_message_box.Where(message =>).ToList();


                try
                {
                    var val = __actor_message_box.Find(message => ((message.__tag == tag) && (message.__data.GetType() == typeof(T))));
                    if (val != null)
                    {

                        {
                            //__actor_message_box.Remove(resultat);
                            __actor_message_box.Remove(val);

                            __mutex_actor_message_box.ReleaseMutex();
                            result = (T)val.__data;
                            return val.__src;

                        }
                    }
                    else
                    {
                        __condition__message_box_not__empty.Reset();
                    }
                }
                catch (System.ArgumentNullException e)
                {
                    //Ici faire quelques chose
                    Console.WriteLine(e.Message);
                }

                __mutex_actor_message_box.ReleaseMutex();
            }
            return 0;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="message"></param>
        /// <returns>Le message tag du message recupéré à partir de la file</returns>
        public int receive(ref Object message_recv){

            bool cnt = true;
            while (cnt)
            {
                //Il faudra mettre une attente passive sur message recu
                //Mettre une méthode receive qui fait du time out

                this.__mutex_actor_message_box.WaitOne();
                var val = __actor_message_box;
                if (val.Count != 0)
                {
                    var msg = val.First();
                    val.Remove(msg);
                    message_recv = msg.__data;
                    return msg.__tag;
                }
                this.__mutex_actor_message_box.ReleaseMutex();
            }
            
            return 0;
        }
    }
}
