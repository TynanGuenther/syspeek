#include <iostream>
#include <unistd.h>
#include <sys/utsname.h>
#include <fstream>

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

    return 0;

}

