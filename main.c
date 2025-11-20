#include <sys/sysinfo.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#include <sys/types.h>
#include <libgen.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <dirent.h>

#define BOX_WIDTH 35
#define BOX_PADDING_LEFT 5

void boxFormat(char* icon, char* start, char* end, char* color) {
    char out[100] = {};
    char* space = " ";
    char* boxStart = "│ ";
    char* boxEnd = " │";
    for (unsigned int i = BOX_PADDING_LEFT; i != 0; --i)
        strcat(out, space);
    strcat(out, boxStart);
    strcat(out, color); // Set color
    strcat(out, icon);
    strcat(out, "\u001b[37m  "); // Reset color
    strcat(out, "\u001b[1m"); // Set bold
    strcat(out, start);
    strcat(out, "\u001b[22m"); // Reset bold
    for (unsigned int i = BOX_WIDTH - (strlen(start) + 5) - strlen(end); i != 0; --i)
        strcat(out, space);
    strcat(out, color); // Set color
    strcat(out, end);
    strcat(out, "\u001b[37m"); // Reset color
    strcat(out, boxEnd);
    strcat(out, "\n");
    printf("%s", out);
}

int main() {
    // printf("\x1b[38;5;16m");
    // printf("     ______           __          __  _\n");
    // printf("    / ____/___ ____  / /__  _____/ /_(_)___ _\n");
    // printf("   / /   / __ `/ _ \\/ / _ \\/ ___/ __/ / __ `/\n");
    // printf("  / /___/ /_/ /  __/ /  __(__  ) /_/ / /_/ /\n");
    // printf("  \\____/\\__,_/\\___/_/\\___/____/\\__/_/\\__,_/\n\n");

    printf("     ╭───────────────────────────────────╮\n");

    struct sysinfo sys_info;
    struct group* grp;

    // kernel
    struct utsname unameData;
    uname(&unameData);
    boxFormat("", "kernel:", unameData.release, "\u001b[38;5;16m");
    // -----------------------------

    // uptime
    int days, hours, mins, x = 1;

    if(sysinfo(&sys_info) != 0)
        perror("sysinfo");

    days = sys_info.uptime / 86400;
    hours = (sys_info.uptime / 3600) - (days * 24);
    mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);

    char* str = (char*)malloc(10*sizeof(char));
    sprintf(str, "%d hours, %d minutes", hours, mins);
    boxFormat("", "uptime:", str, "\u001b[37m");
    // -----------------------------

    // shell
    char path[BUFSIZ], comm[BUFSIZ];
    pid_t parent_pid = getppid();
    int fd;
    snprintf(path, BUFSIZ, "/proc/%d/comm", parent_pid);
    fd = open(path, O_RDONLY);
    memset(comm, 0, BUFSIZ);
    read(fd, comm, BUFSIZ-1);

    comm[strcspn(comm, "\n")] = 0; // Remove \n from end of comm
    boxFormat("", "shell:", comm, "\u001b[38;5;16m");
    // -----------------------------

    // Ram
    double bytesToGBMultiplier = 9.313225746154785e-10;
    sprintf(str, "%.2f GiB / %.2f GiB", (sys_info.totalram - sys_info.freeram) * bytesToGBMultiplier, sys_info.totalram * bytesToGBMultiplier);
    boxFormat("", "mem:", str, "\u001b[38;5;17m");
    // -----------------------------

    // Swap
    sprintf(str, "%.2f GiB / %.2f GiB", (sys_info.totalswap - sys_info.freeswap) * bytesToGBMultiplier, sys_info.totalswap * bytesToGBMultiplier);
    boxFormat("", "swap:", str, "\u001b[38;5;16m");
    // -----------------------------

    // Processes
    sprintf(str, "%d", sys_info.procs);
    boxFormat("", "procs:", str, "\u001b[37m");
    // -----------------------------
    
    // Packages
    DIR* dir;
    struct dirent* entry;
    unsigned int count = 0;
    dir = opendir("/var/lib/pacman/local/");
    while ((entry = readdir(dir)) != NULL) {
        char* name = entry->d_name;
        if (entry->d_type == DT_DIR) {
            if (!strcmp(name, ".") || !strcmp(name, ".."))
                continue;
            ++count;
        }
    }
    closedir (dir);

    sprintf(str, "%u", count);
    boxFormat("", "pkgs:", str, "\u001b[31m");
    // -----------------------------

    // User
    gid_t gid;
    gid = getgid();
    grp = getgrgid(gid);
    boxFormat("", "user:", grp->gr_name, "\u001b[37m");
    // -----------------------------

    // Hostname
    boxFormat("", "hname:", unameData.nodename, "\u001b[34m");
    // -----------------------------
    
    // Distro
    boxFormat("", "distro:", "Arch Linux", "\u001b[38;5;16m");
    // -----------------------------
    
    printf("     ╰───────────────────────────────────╯\n");

    free(str);

    return 0;
}

