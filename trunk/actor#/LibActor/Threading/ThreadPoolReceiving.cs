using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibActor.Threading
{
    class ThreadPoolReceiving : ThreadPool
    {


        /// <summary>
        /// 
        /// </summary>
        /// <param name="size"></param>
        public ThreadPoolReceiving(int size) : base(size)
        {

        }


        /// <summary>
        /// 
        /// </summary>
        protected override void Loop()
        {
            
        }
    }
}
