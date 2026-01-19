#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <sys/utsname.h>
#include <fstream>
#include <string>

void print_mem_usage() {
    std::ifstream file("/proc/meminfo");
    if (!file) {
	std::cerr << "Failed to open /proc/meminfo\n";
	return;
    }

    std::string key;
    long val;
    std::string unit;
    
    long mem_tot = 0;
    long mem_avail = 0;

    while (file >> key >> val >> unit) {
	if (key == "MemTotal:") {
	    mem_tot = val;
	} else if (key == "MemAvailable:") {
	    mem_avail = val;
	}

	if (mem_tot && mem_avail) {
	    break;
	}
    }
    if (!mem_tot || !mem_avail) {
	std::cerr << "Memory info not found\n";
	return;
    }

    long mem_used = mem_tot - mem_avail;

    double tot_gb = mem_tot / (1024.0 * 1024.0);
    double used_gb = mem_used / (1024.0 * 1024.0);

    std::cout << "Memory: "
	      << used_gb << " GB / "
	      << tot_gb << " GB\n";
}

void print_uptime() {
    std::ifstream file ("/proc/uptime");
    if (!file) {
	std::cerr << "Failed to open /proc/uptime\n";
	return;
    }

    double seconds;
    file >> seconds;

    long total_minutes = static_cast<long>(seconds) / 60;
    long minutes = total_minutes % 60;
    long total_hours = total_minutes / 60;
    long hours = total_hours % 24;
    long days = total_hours / 24;

    std::cout << "Uptime: "
	      << days << " days, "
	      << hours << " hours, "
	      << minutes << " minutes\n";
}

void print_cpu_model() {
    std::ifstream file("/proc/cpuinfo");
    if(!file) {
	std::cerr << "Failed to open /proc/cpuinfo\n";
	return;
    }

    std::string line;
    while (std::getline(file, line)) {
	if (line.find("model name") != std::string::npos) {
	    auto pos = line.find(':');
	    if (pos != std::string::npos) {
		std::string model = line.substr(pos + 2);
		std::cout << "CPU: " << model << "\n";
	    }
	    return;
	}
    }

    std::cerr << "CPU model not found\n";
}

int main() {

    //Hostname
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
	perror("gethostname");
	return 1;
    }

    //Kernel info
    struct utsname sysinfo;
    if (uname(&sysinfo) != 0) {
	perror("uname");
	return 1;
    }

    std::cout << "Hostname: " << hostname << "\n";
    std::cout << "Kernel: " << sysinfo.sysname << " " << sysinfo.release << "\n";
    print_uptime();
    print_cpu_model();
    print_mem_usage();

    return 0;

}

