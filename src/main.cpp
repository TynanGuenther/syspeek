#include <iostream>
#include <unistd.h>
#include <sys/utsname.h>

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

    return 0;

}

