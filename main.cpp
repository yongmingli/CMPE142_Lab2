//
//
//
// CMPE 142 Lab2
// Author: Yongming Li  李涌铭
//
//
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <climits>
#include "schedule.h"

using namespace std;
// main function
void show(schedule jobset[], int jobsize); // show done
void fifo(schedule jobset[], int jobsize); // fifo done
void sjf(schedule jobset[], int jobsize);
void bjf(schedule jobset[], int jobsize);
void stcf(schedule jobset[], int jobsize); // stcf done
void rr(schedule jobset[], int jobsize);

// tool function
void cleanup(schedule jobset[], int jobsize); //functional
void swap(schedule *a, schedule *b);
void SortArvlLtoH(schedule arr[], int n);
void sortArvlnDurLtoH(schedule arr[], int n); //check stcf

int main() {

    int choice = 999;
    int jobsize = 0;

    //start reading from file
    ifstream in;

    //in.open("jobs.txt");
    in.open("jobs.dat");

    if (in.fail()) {
        cout << "Unable to open jobs.dat files. CANNOT continue!(1)" << endl;
        exit(1);
    }

    // CANNOT preset size
    string unused;
    while ( std::getline(in, unused)) ++jobsize;
    in.close();
    // in.open("jobs.txt");
    in.open("jobs.dat");
    if (in.fail()) {
        cout << "Unable to open jobs.dat files. CANNOT continue!(2)" << endl;
        exit(1);
    }

    schedule jobset[jobsize]; //job array

    string nextline;
    for (int i=0;i<jobsize && getline(in >> ws,nextline); ++i)
    {
        int int_job_id;
        int int_arrive_time;
        int int_duration;

        stringstream(nextline) >> int_job_id >> int_arrive_time >> int_duration;
        jobset[i].build(int_job_id, int_arrive_time, int_duration); //original
    }

    cout    <<endl
            << "Hello, Welcome to use LYM scheduling program." << endl
            << "Successfully read from jobs.dat file!\n" << endl;

    while (choice != 0) {
        cout << "Please choose from following option:" << endl
             << "Note: When run STCF, it would create a Segmentation fault and exit!!" <<endl
             << "      The STCF will run successfully and calculate the currect values." <<endl
             << "      Unfortunately, the Segmentation fault havn NOT solved yet, be "<<endl
             << "      sure to run STCF at the last step. Sorry."<<endl
             << "-------------ENJOY-----------------------------------------------------" <<endl
             << "-----------------------------------------------------------------------" << endl

             << "1. Show jobs " << endl
             << "2. FIFO " << endl
             << "3. SJF " << endl
             << "4. BJF " << endl
             << "5. STCF " << endl
             << "6. RR (Does NOT work!)" << endl
             << "0. EXIT! " << endl;

        cin >> choice;

        if (choice == 1) show(jobset, jobsize);
        else if (choice == 2) fifo(jobset, jobsize);
        else if (choice == 3) sjf(jobset, jobsize);
        else if (choice == 4) bjf(jobset, jobsize);
        else if (choice == 5) stcf(jobset, jobsize);
        else if (choice == 6) rr(jobset, jobsize);
        else if (choice == 0) cout <<"Program exiting......" <<endl;
        else cout << "Wrong choice! Please choose again!" << endl;
    }

    in.close();
    cout << "Program exit!!!" << endl;
}

void show(schedule jobset[], int jobsize)
{
    cout << "There are "<< jobsize << " jobs in the jobs.dat file." << endl;
    cout << "Job ID" << "\t" <<"Arrival Time" << "\t" <<"Duration" << endl;
    for (int i=0; i<jobsize; i++)
    {
        cout << jobset[i].job_id << "\t" << jobset[i].arrival_time << "\t\t" <<jobset[i].duration << endl;
    }
    cout <<"\n";
}   // show done
void fifo(schedule jobset[], int jobsize)
{
    SortArvlLtoH(jobset, jobsize);

    cout << "This is FIFO: "<< endl
         << "---------------------------------------------" << endl;
    int time = 0;
    for (int i=0; i<jobsize; i++)
    {
        int start, finish, response;

        if (jobset[i].arrival_time > time) start = jobset[i].arrival_time;
        else start = time;

        jobset[i].start = start;
        jobset[i].finish = jobset[i].start + jobset[i].duration;
        time = jobset[i].finish; //update time
        jobset[i].time = jobset[i].finish - jobset[i].arrival_time;
        jobset[i].response = jobset[i].start - jobset[i].arrival_time;

        cout << "Job ID: " <<jobset[i].job_id << endl
             << "The start time is " << jobset[i].start << " seconds."<< endl
             << "The finish time is " << jobset[i].finish << " seconds."<< endl
             << "The total time elapsed is " << jobset[i].time << " seconds."<< endl
             << "The response time is " << jobset[i].response << " seconds.\n"<< endl;
    }
    cleanup(jobset, jobsize); // clean up
    cout << "---------------------------------------------" << endl
         << "FIFO DONE!\n"<< endl;
}   // fifo done
void stcf(schedule jobset[], int jobsize)
{
    sortArvlnDurLtoH(jobset,jobsize);
    cout << "This is STCF: "<< endl
         << "---------------------------------------------" << endl;

    int processDone = 0;
    int time = 0;
    int process;
    int finish;
    int check[jobsize] = {0}; 

    for(time = 0;processDone != jobsize;time++)
    {
        process = -1;
        int temp = INT_MAX;
        for(int i = 0; i < jobsize; i++)
        {
            if(jobset[i].arrival_time <= time && jobset[i].remain < temp && jobset[i].remain>0)
            {
                process = i;
                temp = jobset[i].duration;
            }
        }
        // if(check[process]==0)
        // {
        //     jobset[process].start = time;
        //     check[process]=1;
        // }
        if(jobset[process].working == false)
        {
            jobset[process].start = time;
            jobset[process].working = true;
        }
        jobset[process].remain--;

        if(jobset[process].remain == 0)
        {
            jobset[process].done = true;
            processDone++;
            finish = time + 1;
            jobset[process].finish = finish;
        }
    }
    for(int i=0; i<jobsize; i++){
        jobset[i].response = jobset[i].start - jobset[i].arrival_time;
        jobset[i].time = jobset[i].finish - jobset[i].arrival_time;
    }

    //Display
    for(int i=0; i<jobsize; i++)
    {
        cout << "Job ID: " <<jobset[i].job_id << endl
             << "The start time is " << jobset[i].start << " seconds."<< endl
             << "The finish time is " << jobset[i].finish << " seconds."<< endl
             << "The total time elapsed is " << jobset[i].time << " seconds."<< endl
             << "The response time is " << jobset[i].response << " seconds.\n"<< endl;
    }
    cleanup(jobset, jobsize); // clean up
    cout << "---------------------------------------------" << endl
         << "STCF DONE!\n"<< endl;
}   // stcf done

void sjf(schedule jobset[], int jobsize){
    SortArvlLtoH(jobset, jobsize);
    cout << "This is SJF: "<< endl
         << "---------------------------------------------" << endl;
    int time = jobset[0].arrival_time;

    int min_time = jobset[0].duration;
    int first;
    for (int i=0; i<jobsize; i++){
        if (jobset[i].arrival_time == jobset[0].arrival_time)
        {
            if (jobset[i].duration < min_time) {
                min_time = jobset[i].duration;
                first = i;
            }
        }
    }

    jobset[first].start = time;
    jobset[first].response = jobset[first].start - jobset[first].arrival_time;
    jobset[first].finish = jobset[first].start + jobset[first].duration;
    time = jobset[first].finish; // keep track of time
    jobset[first].time = jobset[first].finish - jobset[first].arrival_time;

    jobset[first].remain = 0;
    jobset[first].done = true;

    int jobdone = 1;
    bool temp = false;

    //rest of the jobs
    while (jobdone < jobsize)
    {
        temp = false;
        int min=99999999;
        //search for sj
        for (int i=0; i<jobsize; i++)
        {
            if (jobset[i].arrival_time <= time && jobset[i].done == false){
                if (jobset[i].duration < min) min = jobset[i].duration;
            }
            else{
                if (jobset[i].done == false)
                {
                    bool go = true;
                    for (int x=0; x < i; x++)
                    {
                        if (jobset[x].done == false) go = false;
                    }
                    if (go) {
                        time = jobset[i].arrival_time;
                        min = jobset[i].duration;
                    }
                }
            }
        }

        for (int i=0; i<jobsize; i++)
            if (jobset[i].duration == min && jobset[i].done == false && temp == false)
            {
                jobset[i].start = time;
                jobset[i].response = jobset[i].start - jobset[i].arrival_time;

                jobset[i].finish = jobset[i].start + jobset[i].duration;
                time = jobset[i].finish; // keep track of time
                jobset[i].time = jobset[i].finish - jobset[i].arrival_time;
                jobset[i].remain = 0;
                jobset[i].done = true;

                jobdone++;
                temp = true;
            }
    } // done with all
    for(int i=0; i<jobsize; i++)
    {
        cout << "Job ID: " <<jobset[i].job_id << endl
             << "The start time is " << jobset[i].start << " seconds."<< endl
             << "The finish time is " << jobset[i].finish << " seconds."<< endl
             << "The total time elapsed is " << jobset[i].time << " seconds."<< endl
             << "The response time is " << jobset[i].response << " seconds.\n"<< endl;
    }
    cleanup(jobset, jobsize); // clean up
    cout << "---------------------------------------------" << endl
         << "SJF DONE!\n"<< endl;
}     // sjf done
void bjf(schedule jobset[], int jobsize){
    SortArvlLtoH(jobset, jobsize);
    cout << "This is BJF: "<< endl
         << "---------------------------------------------" << endl;
    int time = jobset[0].arrival_time;

    int max_time = jobset[0].duration;
    int first;
    for (int i=0; i<jobsize; i++){
        if (jobset[i].arrival_time == jobset[0].arrival_time)
        {
            if (jobset[i].duration > max_time) {
                max_time = jobset[i].duration;
                first = i;
            }
        }
    }

    jobset[first].start = time;
    jobset[first].response = jobset[first].start - jobset[first].arrival_time;
    jobset[first].finish = jobset[first].start + jobset[first].duration;
    time = jobset[first].finish; // keep track of time
    jobset[first].time = jobset[first].finish - jobset[first].arrival_time;

    jobset[first].remain = 0;
    jobset[first].done = true;

    int jobdone = 1;
    bool temp = false;

    //rest of the jobs
    while (jobdone < jobsize)
    {
        temp = false;
        int max=-1;
        //search for bj
        for (int i=0; i<jobsize; i++)
        {
            if (jobset[i].arrival_time <= time && jobset[i].done == false){
                if (jobset[i].duration > max) max = jobset[i].duration;
            }
            else{
                if (jobset[i].done == false)
                {
                    bool go = true;
                    for (int x=0; x < i; x++)
                    {
                        if (jobset[x].done == false) go = false;
                    }
                    if (go) {
                        time = jobset[i].arrival_time;
                        max = jobset[i].duration;
                    }
                }
            }
        }

        for (int i=0; i<jobsize; i++)
            if (jobset[i].duration == max && jobset[i].done == false && temp == false)
            {
                jobset[i].start = time;
                jobset[i].response = jobset[i].start - jobset[i].arrival_time;

                jobset[i].finish = jobset[i].start + jobset[i].duration;
                time = jobset[i].finish; // keep track of time
                jobset[i].time = jobset[i].finish - jobset[i].arrival_time;
                jobset[i].remain = 0;
                jobset[i].done = true;

                jobdone++;
                temp = true;
            }
    } // done with all

    for(int i=0; i<jobsize; i++)
    {
        cout << "Job ID: " <<jobset[i].job_id << endl
             << "The start time is " << jobset[i].start << " seconds."<< endl
             << "The finish time is " << jobset[i].finish << " seconds."<< endl
             << "The total time elapsed is " << jobset[i].time << " seconds."<< endl
             << "The response time is " << jobset[i].response << " seconds.\n"<< endl;
    }
    cleanup(jobset, jobsize); // clean up
    cout << "---------------------------------------------" << endl
         << "BJF DONE!\n"<< endl;

}     // bjf done
void rr(schedule jobset[], int jobsize){
    SortArvlLtoH(jobset, jobsize);
    int time_quantum;
    cout << "This is RR: "<< endl
         << "---------------------------------------------" << endl;
    cout << "Please enter the Time Quantum: \n";
    cin >> time_quantum;

    cout << "Sorry, RR does not work.\n"<<endl; 
    // int i,
    // n, //size
    // time,
    // remain,
    // temps=0;

    // n = jobsize;

    // int wt[jobsize],tat[jobsize];

    // remain=n;

    // // assigning the number of process to remain variable

    // int at[n]; //arrival time
    // int bt[n]; //duration
    // int rt[n]; //remain time

    // //dynamic array declaration using vector method of (STL)
    // //STL standard template library of C++

    // for(i=0;i<n;i++)
    // {
    //     at[i] = jobset[i].arrival_time;
    //     bt[i] = jobset[i].duration;
    //     rt[i]=bt[i];
    // }

    // for(time=0,i=0;remain!=0;)
    // {
    //     if(rt[i]<=time_quantum && rt[i]>0)
    //     {
    //         if (!jobset[i].working)
    //         {
    //             jobset[i].start = time;
    //             jobset[i].working = true;
    //         }
    //         time += rt[i];
    //         //Addition using shorthand operators
    //         rt[i]=0;
    //         temps=1;
    //     }

    //     else if(rt[i]>0)
    //     {
    //         if (!jobset[i].working)
    //         {
    //             jobset[i].start = time;
    //             jobset[i].working = true;
    //         }
    //         rt[i] -= time_quantum;
    //         //Subtraction using shorthand operators
    //         time += time_quantum;
    //         //Addition using shorthand operators
    //     }

    //     if(rt[i]==0 && temps==1)
    //     {
    //         remain--;
    //         wt[i] = time-at[i]-bt[i];
    //         tat[i] = time-at[i];
    //         temps=0;
    //         jobset[i].done = true;
    //     }

    //     if(i == n-1)
    //         i=0;
    //     else if(at[i+1] <= time)
    //         i++;
    //     else
    //         i=0;
    // }

    // for (int i=0; i<jobsize; i++)
    //     {
    //         jobset[i].start = time;
    //         jobset[i].response = jobset[i].start - jobset[i].arrival_time;

    //         jobset[i].time = tat[i];
    //         jobset[i].finish = jobset[i].time + jobset[i].arrival_time;
    //     }

    // //Display
    // for(int i=0; i<jobsize; i++)
    // {
    //     cout << "Job ID: " <<jobset[i].job_id << endl
    //          << "The start time is " << jobset[i].start << " seconds."<< endl
    //          << "The finish time is " << jobset[i].finish << " seconds."<< endl
    //          << "The total time elapsed is " << jobset[i].time << " seconds."<< endl
    //          << "The response time is " << jobset[i].response << " seconds.\n"<< endl;
    // }

    // cleanup(jobset, jobsize); // clean up
    // cout << "---------------------------------------------" << endl
    //      << "RR DONE!\n"<< endl;
}      // rr doesnot work

void cleanup(schedule jobset[], int jobsize){
    for(int i=0; i<jobsize; i++)
    {
        jobset[i].start = 0;
        jobset[i].finish = 0;
        jobset[i].response = 0;
        jobset[i].time = 0;
        jobset[i].remain= jobset[i].duration;

        jobset[i].done = false;
        jobset[i].working = false;
    }
}
void swap(schedule *a, schedule *b){
    schedule temp;

    temp.job_id = a->job_id;
    temp.arrival_time = a->arrival_time;
    temp.duration = a->duration;
    temp.working = a->working;
    temp.done = a->done;
    temp.start = a->start;
    temp.finish = a->finish;
    temp.response = a->response;
    temp.time = a->time;
    temp.remain = a->remain;

    a->job_id = b->job_id;
    a->arrival_time = b->arrival_time;
    a->duration = b->duration;
    a->working = b->working;
    a->done = b->done;
    a->start = b->start;
    a->finish = b->finish;
    a->response = b->response;
    a->time = b->time;
    a->remain = b->remain;

    b->job_id = temp.job_id;
    b->arrival_time = temp.arrival_time;
    b->duration = temp.duration;
    b->working = temp.working;
    b->done = temp.done;
    b->start = temp.start;
    b->finish = temp.finish;
    b->response = temp.response;
    b->time = temp.time;
    b->remain = temp.remain;
}
void SortArvlLtoH(schedule arr[], int n)
{
    int i, j;
    for (i = 0; i < n-1; i++)
    {
        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++)
        {
            if (arr[j].arrival_time > arr[j+1].arrival_time)
            {
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
}
void sortArvlnDurLtoH(schedule arr[], int n)
{
    int i, j;
    for (i = 0; i < n-1; i++)
    {
        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++)
        {
            if (arr[j].arrival_time > arr[j+1].arrival_time)
            {
                swap(&arr[j], &arr[j+1]);
            }
            else if(arr[j].arrival_time == arr[j+1].arrival_time && arr[j].duration > arr[j+1].duration)
            {
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
}