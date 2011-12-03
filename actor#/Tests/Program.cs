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

            //Actor.Init();

            //new samples.PingPong(100).Start();
            //new samples.MassivlyConcurrent(1600).Start();
            //new samples.Anneau(40).Start();

            List<Mere> els = new List<Mere> { new Mere(12), new Child(1)};
            els.ForEach(e => {
                e.SayHello();
                e.Thanks();
                e.Live();
                Console.WriteLine(e.GetFamilyName());
            });


            var child = new Child(122);
            Console.WriteLine((child as Mere).GetFamilyName());

            Console.ReadLine();

            //Actor.Finit();
  
        }
    }
}
