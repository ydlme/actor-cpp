using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibActor.Communicators
{
    /// <summary>
    /// Toute implémentation de la notion de communicateur doit 
    /// respecter la norme MPI
    /// Il est indispensable de fournir un index constant une fois 
    /// que le communicateur est finalisé
    /// </summary>
    interface ICommunicator
    {
        #region pointTopoint
        /// <summary>
        /// Communication point à point
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="dest"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        int Send<T>(int dest, T message, int rank);


        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="src"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        int Receive<T>(int src, ref T message);

        #endregion


        #region collectiveCommunications
        /// <summary>
        /// Opération de duplication d'une donnée message
        /// on-to-all
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="source"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        int BroadCast<T>(int source, ref T message);

        /// <summary>
        /// Opération de dispathing d'une donnée 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="source"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        int Scatter<T>(int root, ref T[] message);


        /// <summary>
        /// Opération de regroupement de données
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="root"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        int Gather<T>(int root, ref T[] message);

        /// <summary>
        /// Opération de reduction
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="root"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        int Reduce<T>(int root, ref T message);
        #endregion
    }
}
