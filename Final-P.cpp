#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <mutex>
#include <thread>

using namespace std;


//queue

int Ready[100];
int Waiting[100];

int R1,R2,R3;

int TempTime = 0;
int RealTime = 0;

int cpu1 = -1;
int cpu2 = -1;
int cpu3 = -1;
int cpu4 = -1;

//tasks

struct Task
{
    char Name;
    string State = "Waiting";
    int prorioty;
    int burst_time;
    int waiting_time = 0;
    int RRN = 1;
    int Done = 0;

};


// Create object for mutex

mutex mtx;
mutex mtx2;


bool check_resource(char name) // aya manba darim.
{

    if (name == 'X')
    {
        if (R1 > 0 && R2 > 0)
        {
            return true;
        }else{
            return false;
        }
        
    }
    if (name == 'Y')
    {
        if (R3 > 0 && R2 > 0)
        {
            return true;
        }else{
            return false;
        }
    }
    if (name == 'Z')
    {
        if (R1 > 0 && R3 > 0)
        {
            return true;

        }else{

            return false;
        }

    }
}


bool Wating_To_Ready(Task Task1[], int size) //waiting to ready va kam kardan manba
{
    bool z = false;
    int Turn = -1; // idel
    int r = 0;
    for (int i = 0; i < size; i++)
    {
        if(Task1[i].State == "Waiting" && check_resource(Task1[i].Name))
        {
            z = true;
            if(Task1[i].RRN > r)
            {
                Turn = i;
                r = Task1[i].RRN;
            }else if(Task1[i].RRN == r)
            {
                if (Task1[i].prorioty <= Task1[Turn].prorioty)
                {
                    Turn = i;
                }
                    
            }
        }
    }
    if (z)
    {
        Task1[Turn].State == "Ready";
        if (Task1[Turn].Name == 'X')
        {
            R1--;
            R2--;
        }else if(Task1[Turn].Name == 'Y')
        {
            R2--;
            R3--;
        }else{
            R1--;
            R3--;
        }
    }
    
    return z;
}


int Redy_To_Runnig(Task Task1[], int size, int number) // -> return what task to do processor
{

        int Turn = -1; // idel
        int r = 0;

        for (int i = 0; i < size; i++)
        {
            if(Task1[i].State == "Ready")
            {
                if(Task1[i].RRN > r) // Highest Response Ratio Next
                {
                    Turn = i;
                    r = Task1[i].RRN;
                }else if(Task1[i].RRN == r)
                {
                    if (Task1[i].prorioty <= Task1[Turn].prorioty)
                    {
                        Turn = i;
                    }
                    
                }
            }
        
        }
        return Turn;
}


void calculate_RRN(Task Task1[], int size)
{
    for (int i = 0; i < size; i++)
    {
        Task1[i].RRN = (Task1[i].waiting_time + Task1[i].burst_time) / Task1[i].burst_time;       
    }
    
}

void run(Task Task1[], int size, int number)
{

    // agar taski processor dre be kareshh edame bde .
    if (number == 1 && cpu1 != -1)
    {
        Task1[cpu1].Done ++;
        if (Task1[cpu1].Done == Task1[cpu1].burst_time)
        {
            cpu1 = -1;
            Task1[cpu1].State = 'Done';
        }
        
    }else if (number == 2 && cpu2 != -1)
    {
        Task1[cpu2].Done ++;
        if (Task1[cpu2].Done == Task1[cpu2].burst_time)
        {
            cpu2 = -1;
            Task1[cpu2].State = 'Done';
        }
        
    }else if (number == 3 && cpu3 != -1)
    {
        Task1[cpu3].Done ++;
        if (Task1[cpu3].Done == Task1[cpu3].burst_time)
        {
            cpu3 = -1;
            Task1[cpu3].State = 'Done';
        }
        
    }else if (number == 4 && cpu4 != -1)
    {
        Task1[cpu4].Done ++;
        if (Task1[cpu4].Done == Task1[cpu4].burst_time)
        {
            cpu4 = -1;
            Task1[cpu4].State = 'Done';
        }

    mtx.lock();

    // ezafe kardan task jadid be processor

    }else{

        calculate_RRN(Task1, size);
        
        int Tsk = Processor(Task1, size, number); // yeki balatar bar migrdand , agar  == 0 bayad processor kari anjam nadahad

        Task1[Tsk].State = "Running";

        if (number == 1)
        {
            cpu1 = Tsk;

        }else if (number == 2)
        {
            cpu2 = Tsk;

        }else if (number == 3)
        {
            cpu3 = Tsk;

        }else{
            cpu4 = Tsk;

        }

        Task1[Tsk].Done++;

        if (Task1[Tsk].Done == Task1[Tsk].burst_time)
        {
            Task1[Tsk].State = "Done";
            if (number == 1)
            {
                cpu1 = -1;
            }else if (number == 2)
            {
                cpu2 = -1;
            }else if (number == 3)
            {
                cpu3 = -1;
            }else{
                cpu4 = -1;
            }
        }
    }   
    mtx.unlock();
}


void func(Task Task1[], int size, int number) 
{   
    while (true)
    {
        while (Wating_To_Ready(Task1, size)) // ta vaqti momken bashe az waiting be ready miare  // jash avaz she 
        {}
        run(Task1, size, number);

        TempTime++;

        if (TempTime == 4)
        {
            TempTime = 0;
            RealTime++;
        }
        // shart khoroj -> agar hme cpu ha bikar bodn .
        if (cpu1 == -1 && cpu2 == -1 && cpu3 == -1 && cpu4 == -1)
        {
            break;
        }
        
    }
    
} 

void main_project(Task Task1[], int size) 
{ 


    
    // Creating a new thread 
    thread ptid1(func, Task1, size, 1);
    thread ptid2(func, Task1, size, 2);
    thread ptid3(func, Task1, size, 3); 
    thread ptid4(func, Task1, size, 4);


    ptid1.join();
    ptid2.join(); 
    ptid3.join();
    ptid4.join();
    


} 
  
// Driver code 
int main() 
{
    // input

        // Resource
        cin >> R1 >> R2 >> R3;
        // number of tasks
        int n;
        cin >> n;
        Task Tasks[n];

        // Task
        for (int i = 0; i < n; i++)
        {
            cin >> Tasks[i].Name;
            cin >> Tasks[i].burst_time;
        }
        //cout << Tasks[0].burst_time << endl;      
    


    // main thread

    main_project(Tasks, n);
    // khoroji print shavad
    
    return 0; 
}