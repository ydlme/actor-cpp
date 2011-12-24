using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibActor;


namespace Tests
{
    class Program
    {
        static void Main(string[] args)
        {

            Actor.Init();

            //new samples.PingPong(100).Start();
            //new samples.MassivlyConcurrent(1600).Start();
            new samples.Anneau(11).Start();

            
            Console.ReadLine();

            Actor.Finit();
  
        }
    }
}
