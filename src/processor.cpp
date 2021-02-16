#include "processor.h"

float Processor::Utilization() { 
    prev_user = user;
    prev_nice = nice;
    prev_system = system;
    prev_idle = idle;
    prev_iowait = iowait;
    prev_irq = irq;
    prev_softirq = softirq;
    prev_steal = steal;
    prev_guest = guest;
    prev_guest_nice = guest_nice;
    std::string line, cpu;
    std::ifstream file_stream(LinuxParser::kProcDirectory
     + LinuxParser::kStatFilename);
    if(file_stream.is_open()) {
        while (std::getline(file_stream, line)) {
            std::istringstream line_stream(line);
            line_stream >> cpu >> user >> nice >> system >>
                idle >> iowait >> irq >> softirq >> steal >>
                guest >> guest_nice;
            break;
        }
    }
    float temp_prev_idle = prev_idle + prev_iowait;
    float temp_idle = idle + iowait;

    float temp_prev_non_idle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
    float temp_non_idle = user + nice + system + irq + softirq + steal;

    float prev_total = temp_prev_idle + temp_prev_non_idle;
    float total = temp_idle + temp_non_idle;

    float total_delta = total - prev_total;
    float idle_delta = idle - prev_idle;
    
    return (total_delta - idle_delta) / total_delta;
 }