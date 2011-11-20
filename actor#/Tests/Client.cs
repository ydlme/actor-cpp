using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Tests
{
    class Client : A,IInterface
    {

        private B __decorated;

        /// <summary>
        /// 
        /// </summary>
        public Client()
        {
            __decorated = new B();
            Console.WriteLine("Client is of type "+this.GetType().FullName);
        }


        /// <summary>
        /// 
        /// </summary>
        public void Fb()
        {
            __decorated.Fb();
        }
    }
}
