using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace ConsoleApplication2
{
    class Program
    {
        static void Main(string[] args)
        {
            var cores = Environment.ProcessorCount;
            Stopwatch sw = new Stopwatch();
            int count = 1000;
            int[][] firstMatrix = new int[100][];
            int[][] secondMatrix = new int[100][];
            int[][] thirdMatrix = new int[100][];
            int[][] fourthMatrix = new int[100][];

            for (int i = 0; i < 100; i++)
            {
                firstMatrix[i] = new int[10];
                secondMatrix[i] = new int[10];
                thirdMatrix[i] = new int[10];
                fourthMatrix[i] = new int[10];
            }
            int m = 7;
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    firstMatrix[i][j] = m;
                    secondMatrix[i][j] = m + 1;
                    m++;
                }
            }
            sw.Start();
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j <10; j++)
                {
                    thirdMatrix[i][j] = firstMatrix[i][j] * secondMatrix[i][j];

                }
            }
            sw.Stop();
            Console.WriteLine("Miliseconds: " + sw.ElapsedMilliseconds);
            sw.Reset();
            sw.Start();
            Parallel.For(0, firstMatrix.Length,
                i =>
                {
                    Parallel.For(0, 10,
                        j =>
                        {
                            fourthMatrix[i][j] = firstMatrix[i][j] * secondMatrix[i][j];

                        });
                });
            sw.Stop();
            Console.WriteLine("Miliseconds Parallel: " + sw.ElapsedMilliseconds);
            
            
        }
        
    }
}
