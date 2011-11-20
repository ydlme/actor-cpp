
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace Tests.samples
{
    
    
    /// <summary>
    /// 
    /// </summary>
    class Serveur : LibActor.Actor
    {

        /// <summary>
        /// 
        /// </summary>
        public Serveur(int size):base()
        {
            base.Name = "Serveur";
            __size = size;
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Act()
        {
            int result = 0;
            int count = 0;
            List<int> msgs = new List<int>();

            while (count!=__size)
            {
                //System.Threading.Thread.Sleep(2);

                this.receive<int>(ref result, 0);
                count++;
                msgs.Add(result);
                
                Console.WriteLine("Actor " + Name + " received : "+result + " step: "+count);
            }

            msgs.Sort();
            Console.WriteLine(" :: "+msgs.Count);
        }

        /// <summary>
        /// 
        /// </summary>
        private int __size;
    }


    /// <summary>
    /// 
    /// </summary>
    class Client : LibActor.Actor
    {

        private Serveur __serveur;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="id"></param>
        public Client(int id,Serveur srv)
            : base()
        {
            __serveur = srv;
            base.Name = "Client "+id;
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Act()
        {
            //System.Threading.Thread.Sleep(2000);
            //Console.WriteLine("Hello from actor " + Name);
            this.send<int>(__serveur, this.Id, 0);
        }
    }

    /// <summary>
    /// 
    /// </summary>
    class MassivlyConcurrent
    {

        /// <summary>
        /// 
        /// </summary>
        private int __size;

        /// <summary>
        /// 
        /// </summary>
        private Serveur __srv;
        
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="size"></param>
        public MassivlyConcurrent(int size)
        {
            __size = size;
            __srv = new Serveur(size);
        }



        public void Start()
        {
            List<LibActor.Actor> actors = new List<LibActor.Actor>();


            Enumerable.Range(1, __size).ToList().ForEach(i => actors.Add(new Client(i,__srv)));
            __srv.Start();
            actors.ForEach(a => a.Start());

        }
    }
}
