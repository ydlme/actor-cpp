using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Tests
{
    class B : IInterface
    {

        /// <summary>
        /// 
        /// </summary>
        public void Fb()
        {
            Console.WriteLine("Hello from Class B which is decorated");
        }
    }
}
