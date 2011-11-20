using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Tests.samples
{
    class Node : LibActor.Actor
    {

        /// <summary>
        /// 
        /// </summary>
        private int __rank;

        /// <summary>
        /// 
        /// </summary>
        private Node __succ;

        /// <summary>
        /// 
        /// </summary>
        internal Node _succ
        {
            get { return __succ; }
            set { __succ = value; }
        }
        
        /// <summary>
        /// 
        /// </summary>
        private Node __pred;


        /// <summary>
        /// 
        /// </summary>
        internal Node _pred
        {
            get { return __pred; }
            set { __pred = value; }
        }
        
        /// <summary>
        /// 
        /// </summary>
        public Node(int id)
            : base()
        {
            __rank = id;
            Name = "rank " + __rank;
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Act()
        {
            string result = null;

            if (__rank == 0)
            {
                this.send<string>(__succ,"0",0);
                this.receive<string>(ref result, 0);
                Console.WriteLine(result);
            }
            else
            {
                this.receive<string>(__pred, ref result, 0);
                
                result += " "+__rank;
                this.send<string>(__succ,result,0);
                
            }
        }
    }


    class Anneau
    {
        /// <summary>
        /// 
        /// </summary>
        private Node[] __nodes = null;
        /// <summary>
        /// 
        /// </summary>
        private int __taille;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="taille"></param>
        public Anneau(int taille)
        {
            __taille = taille;
            __nodes = new Node[taille];
            
            for (int i = 0; i < taille; i++)
            {
                __nodes[i] = new Node(i);
            }
            
            for (int i = 0; i < taille; i++)
            {
                __nodes[i]._pred = __nodes[((i - 1) + __taille) % __taille];
                __nodes[i]._succ = __nodes[(i + 1) % __taille];
            }

        }

        /// <summary>
        /// 
        /// </summary>
        public void Start()
        {
            __nodes.ToList().ForEach(a => a.Start());
        }

    }
}
