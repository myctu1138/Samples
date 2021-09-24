using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace ParallelFor
{
    public sealed class Singleton
    {
        private static int counter = 0;
        private static readonly object Instancelock = new object();
        private Singleton()
        {
            counter++;
            Console.WriteLine("Counter value" + counter.ToString());
        }

        private static Singleton instance = null;

        public static Singleton GetInstance
        {
            get
            {
                lock (Instancelock)
                {
                    if (instance == null)
                        instance = new Singleton();
                    return instance;
                }
            }
        }
        public void PrintDetails(string message)
        {
            Console.WriteLine(message);
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Parallel.Invoke(
                ()=>PrintTeacherDetails(),
                ()=>PrintStudentDetails()
                );
            Console.ReadLine();
        }
        private static void PrintTeacherDetails()
        {
            Singleton fromTeacher = Singleton.GetInstance;
            fromTeacher.PrintDetails("From Teacher");
        }
        private static void PrintStudentDetails()
        {
            Singleton fromStudent = Singleton.GetInstance;
            fromStudent.PrintDetails("From Student");
        }
    }
}
