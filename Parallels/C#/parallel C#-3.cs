using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace ConsoleApplication17
{
    class Program
    {

        static void Main(string[] args)
        {
            double[,] array = new double[1000, 1000];

            Thread thread1 = new Thread((() => Sum(array)));
            thread1.Name = "ThreadOne";

            Thread thread2 = new Thread((() => Sum(array)));
            thread2.Name = "ThreadTwo";
            thread1.Priority = ThreadPriority.Lowest;
            thread2.Priority = ThreadPriority.Highest;

            thread1.Start();
            thread2.Start();

            thread1.Join();
            thread2.Join();

        }

        public static void Sum(double[,] array)
        {
            Random rnd = new Random();
            double add = rnd.Next(1, 10);
            double sum;

            for (int i = 0; i < 1000; i++)
            {
                for (int j = 0; j < 1000; j++)
                {
                    array[i, j] = rnd.Next(1, 10)*0.2;
                    sum = array[i, j] + add;
                    if (i % 100 == 0 && j % 100 == 0)
                    {
                        Console.WriteLine(Thread.CurrentThread.Name + " " + i + " " + j  );
                    }

                }
            }
        }
    }
}
