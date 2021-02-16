#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <fstream>
#include <sstream>

#include "linux_parser.h"

class Processor {
 public:
  float Utilization(); 

 private:
    float user, prev_user;
    float nice, prev_nice;
    float system, prev_system;
    float idle, prev_idle;
    float iowait, prev_iowait;
    float irq, prev_irq;
    float softirq, prev_softirq;
    float steal, prev_steal;
    float guest, prev_guest;
    float guest_nice, prev_guest_nice;
};

#endif