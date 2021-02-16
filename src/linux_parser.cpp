#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  std::string line, key, value;
  std::ifstream file_stream(kProcDirectory + kMeminfoFilename);
  std::vector<std::string> memory_utilization;
  float total_memory, free_memory;
  if(file_stream.is_open()) {
    for(int i = 0; i < 4; i++) {
      std::getline(file_stream, line);
      std::istringstream line_stream(line);
      line_stream >> key >> value;
      memory_utilization.push_back(value);
    }
  }
  total_memory = stof(memory_utilization[0]);
  free_memory = stof(memory_utilization[1]);
  return (total_memory - free_memory) / total_memory; 
}

long LinuxParser::UpTime() { 
  std::string line, system_up_time;
  std::ifstream file_stream(kProcDirectory + kUptimeFilename);
  if(file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> system_up_time;
    }
  }
  return stol(system_up_time); 
}

int LinuxParser::TotalProcesses() { 
  return SearchValue<int>(kProcDirectory + kStatFilename, "processes");
}

int LinuxParser::RunningProcesses() { 
  return SearchValue<int>(kProcDirectory + kStatFilename, "procs_running"); 
}

string LinuxParser::Command(int pid) { 
    std::string line;
    std::ifstream file_stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if(file_stream.is_open()) {
        std::getline(file_stream, line);
    }
    return line;
}

string LinuxParser::Ram(int pid) { 
    int ram = SearchValue<int>(kProcDirectory + to_string(pid) + kStatusFilename, "VmSize:");
    return to_string(ram / 1000); 
}

string LinuxParser::Uid(int pid) { 
    int uid = LinuxParser::SearchValue<int>(kProcDirectory + to_string(pid) + kStatusFilename, "Uid:");
    return to_string(uid);
  }

string LinuxParser::User(int pid) { 
    string uid_string = Uid(pid);
    std::string line, user, x, value;
    std::ifstream file_stream(LinuxParser::kPasswordPath);
    if(file_stream.is_open()) {
        while (std::getline(file_stream, line)) {
            boost::replace_all(line, ":", " ");
            std::istringstream line_stream(line);
            line_stream >> user >> x >> value;
            if (value == uid_string) break;
        }
    }
    return user;
}

long LinuxParser::UpTime(int pid) { 
    std::string line, value;
    std::ifstream file_stream(kProcDirectory + to_string(pid) + kStatFilename);
    if(file_stream.is_open()) {
        std::getline(file_stream, line);
        std::istringstream line_stream(line);
        for (int i = 0; i < 22; i++) {
            line_stream >> value;
        }
    } 
    return stol(value) / sysconf(_SC_CLK_TCK); }  

template <typename T>
T LinuxParser::SearchValue(std::string file_name, std::string searched_key) {
  std::string line, key, value;
  std::ifstream file_stream(file_name);
  T result;
  if (file_stream.is_open()) {
    while(std::getline(file_stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> key >> value;
      if (key == searched_key) break;
    }
  }
  std::stringstream result_stream(value);
  result_stream >> result;
  return result; 
}