using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibActor.Communicators
{
    /// <summary>
    /// Classe permettant de regrouper des acteur dans 
    /// un canal de communication commun
    /// Il manquera clairement une couche ipc
    /// </summary>
    class Communicator : ICommunicator
    {
        /// <summary>
        /// 
        /// </summary>
        private Dictionary<int,Actor> __elements;
        private string __name;
        private int __actor_id=0;
        /// <summary>
        /// 
        /// </summary>
        public Communicator()
        {
            __elements = new Dictionary<int,Actor>();
        }





        #region methods
        /// <summary>
        /// 
        /// </summary>
        public void ConnectActor(Actor a)
        {
            __elements.Add(__actor_id,a);
            __actor_id++;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="dest"></param>
        /// <param name="message"></param>
        /// <param name="tag"></param>
        /// <returns></returns>
        public int Send<T>(int dest, T message,int tag)
        {
            Actor src= new Actor();
            var actor_dest = __elements[dest];
            return src.send<T>(actor_dest, message, tag);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="src"></param>
        /// <param name="message"></param>
        /// <param name="tag"></param>
        /// <returns></returns>
        public int Receive<T>(int src, ref T message,int tag)
        {
            Actor dest = new Actor();
            var actor_src = __elements[src];
            return dest.receive<T>(actor_src,ref message,tag);
        }



        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="source"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        public int BroadCast<T>(int source, ref T message)
        {
            //une copie de message
            //envoi du message à tous le monde
            throw new NotImplementedException();

            __elements.ToList().ForEach(e => 
                {
                    if (e.Key != source)
                    {
                        //Il faut créer un autre pool de thread qui va 
                        //se charger des communicateurs
                        //Ceci afin d'aller vite dans les communications collectives
                    }
                });

            return 0;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="root"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        public int Scatter<T>(int root, ref T[] message)
        {
            throw new NotImplementedException();
        }


        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="root"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        public int Gather<T>(int root, ref T[] message)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="root"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        public int Reduce<T>(int root, ref T message)
        {
            throw new NotImplementedException();
        }
        
       
        #endregion

        
    }
}
