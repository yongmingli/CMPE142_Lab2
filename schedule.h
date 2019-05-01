//
// Created by 李涌铭 on 2019-04-28.
//

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <climits>

using namespace std;

class schedule
{
public:
    int job_id;
    int arrival_time;
    int duration;

    bool working;
    bool done;

    //time
    int start, finish, response, time;
    int remain;

    void build(int int_job_id, int int_arrival_time, int int_duration) {
        job_id = int_job_id;
        arrival_time = int_arrival_time;
        duration = int_duration;

        working = false;
        done = false;

        start = 0;
        finish = 0;
        response = 0;
        time = 0;

        remain = duration;
    }

    schedule ()
    {
        job_id = 0;
        arrival_time = 0;
        duration = 0;

        working = false;
        done = false;

        start = 0;
        finish = 0;
        response = 0;
        time = 0;

        remain = duration;
    }
};

#endif //SCHEDULE_H
