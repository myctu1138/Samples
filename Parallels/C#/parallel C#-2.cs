using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ConsoleApplication16
{
     public class Progrma
    {

        static void Main(string[] args)
        {
            Account account = new Account();
            Thread thread1 = new Thread(() => account.Withdraw100());
            thread1.Name = "ThreadOne";
            Thread thread2 = new Thread(() => account.Withdraw100());
            thread2.Name = "ThreadTwo";

            thread1.Start();
            thread2.Start();

            thread1.Join();
            thread2.Join();


        }
    }

    public class Account
    {
        static readonly object Obj = new object();
        public Account()
        {
            mBlance = 1000; 
        }

        public float mBlance { get; set; }

        public  void  Withdraw100()
        {  
             lock(Obj)
             {
                 mBlance = mBlance - 100;
                 Console.WriteLine(Thread.CurrentThread.Name + " " + mBlance);
             }
            
        }


    }


  
}
