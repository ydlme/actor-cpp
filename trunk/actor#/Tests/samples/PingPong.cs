using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibActor;


namespace Tests.samples
{
    class PingPong
    {

        /// <summary>
        /// 
        /// </summary>
        /// <param name="size"></param>
        public PingPong(int size)
        {
            __count = size;
        }



        public void Start()
        {
            Actor a = new Actor(() =>
            {
                var nums = Enumerable.Range(1, __count).ToList();
                nums.ForEach(
                    n =>
                    {
                        System.Threading.Thread.Sleep(1000);
                        int recv = 0;
                        var sender = Actor.Self().receive<int>(ref recv, 0);
                        Console.WriteLine(string.Format("actor {0} received {1}", Actor.Self().Name, recv));
                        recv++;
                        Actor.Self().send<int>(sender, recv, 0);
                    });
            }) { Name = "a" };



            Actor b = new Actor(() =>
            {

                int recv = 0;
                var nums = Enumerable.Range(1, __count).ToList();
                nums.ForEach(n =>
                {

                    Actor.Self().send<int>(a, recv, 0);
                    System.Threading.Thread.Sleep(1000);
                    var sender = Actor.Self().receive<int>(ref recv, 0);
                    Console.WriteLine(string.Format("actor {0} received {1}", Actor.Self().Name, recv));
                    recv++;

                });

            }) { Name = "b" };



            a.Start();
            b.Start();
        }
        /// <summary>
        /// 
        /// </summary>
        private int __count;
    }
}
