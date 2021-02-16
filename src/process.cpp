#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return id; }

float Process::CpuUtilization() {
    float u_time, s_time, cu_time, cs_time, start_time; 
    std::string line, value;
    std::ifstream file_stream(LinuxParser::kProcDirectory + to_string(id) + LinuxParser::kStatFilename);
    if(file_stream.is_open()) {
        std::getline(file_stream, line);
        std::istringstream line_stream(line);
        for (int i = 1; i < 23; i++) {
            line_stream >> value;
            switch (i) {
            case 14:
                u_time = std::stof(value);
                break;
            case 15:
                s_time = std::stof(value);
                break;
            case 16:
                cu_time = std::stof(value);
                break;
            case 17:
                cs_time = std::stof(value);
                break;
            case 22:
                start_time = std::stof(value);
                break;
            default:
                break;
            }
        }
    }
    float total_time = u_time + s_time + cu_time + cs_time;
    float seconds =  LinuxParser::UpTime() - (start_time / sysconf(_SC_CLK_TCK));
    cpu_utilization = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
    return cpu_utilization; 
}

string Process::Command() { 
    return LinuxParser::Command(id);
}

string Process::Ram() { 
    return LinuxParser::Ram(id);
}

string Process::User() {
    return LinuxParser::User(id);
}

long int Process::UpTime() {

}

bool Process::operator<(Process const& a) const {
    return this->cpu_utilization < a.cpu_utilization ? true : false; 
}