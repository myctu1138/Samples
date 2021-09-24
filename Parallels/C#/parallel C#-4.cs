using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            double[] array = new double[1000];

            Console.WriteLine("Processor count: {0}",Environment.ProcessorCount);
            
            double k = 8.5;
            for (int i = 0; i < 1000; i++)
            {
                array[i] = k + i+0.68;
            }

            Console.WriteLine("Before operation:");
            for (int i = 0; i < 1000; i++)
            {
                Console.WriteLine("Element {0} = {1}", i, array[i]);
            }

            double num = 2.0;

            Task task1 = Task.Factory.StartNew(() =>
                {
                    arrayMultiply(0, 250, array, num);
                });
           
            Task task2 = Task.Factory.StartNew(() =>
            {
                arrayMultiply(250, 500, array, num);
            }); 
           
            Task task3 = Task.Factory.StartNew(() =>
            {
                arrayMultiply(500, 750, array, num);
            });
           
            Task task4 = Task.Factory.StartNew(() =>
            {
                arrayMultiply(750, 1000, array, num);
            });

            task1.Wait();
            task2.Wait();
            task3.Wait();
            task4.Wait();


            Console.WriteLine("After operation:");
            for (int i = 0; i < 1000; i++)
            {
                Console.WriteLine("Element {0} = {1}", i, array[i]);
            }

             
        }
       

        static void arrayMultiply(int n, int m, double[] array,double num) 
        {
            for (int i = n; i < m; i++)
                array[i] *= num;
        }
    }
}
