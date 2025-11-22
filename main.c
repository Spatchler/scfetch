#include <sys/sysinfo.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <grp.h>
#include <sys/types.h>
#include <libgen.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <dirent.h>

#define _PACKAGE_PATH "/var/lib/pacman/local/"

#define _KERNEL_COLOUR "\u001b[38;5;16m"
#define _UPTIME_COLOUR "\u001b[37m"
#define _SHELL_COLOUR "\u001b[38;5;16m"
#define _RAM_COLOUR "\u001b[38;5;17m"
#define _SWAP_COLOUR "\u001b[38;5;16m"
#define _PROCESSES_COLOUR "\u001b[37m"
#define _PACKAGES_COLOUR "\u001b[31m"
#define _USER_COLOUR "\u001b[37m"
#define _HOSTNAME_COLOUR "\u001b[34m"
#define _DISTRO_COLOUR "\u001b[38;5;16m"

#define _BAR_BG_COLOUR "\u001b[90m"

void boxFormat(char* icon, char* start, char* end, char* color, unsigned int paddingLeft, unsigned int boxWidth, unsigned int endLen) {
    if (endLen == 0) {
        endLen = strlen(end);
    }
    char out[500] = {};
    char* space = " ";
    char* boxStart = "│ ";
    char* boxEnd = " │";
    for (unsigned int i = paddingLeft; i != 0; --i)
        strcat(out, space);
    strcat(out, boxStart);
    strcat(out, color); // Set color
    strcat(out, icon);
    strcat(out, "\u001b[37m  "); // Reset color
    strcat(out, "\u001b[1m"); // Set bold
    strcat(out, start);
    strcat(out, "\u001b[22m"); // Reset bold
    for (unsigned int i = boxWidth - (strlen(start) + 5) - endLen; i != 0; --i)
        strcat(out, space);
    strcat(out, color); // Set color
    strcat(out, end);
    strcat(out, "\u001b[37m"); // Reset color
    strcat(out, boxEnd);
    strcat(out, "\n");
    printf("%s", out);
}

void bar(char* buf, float percent, unsigned int width, char* primaryCharacter, char* halfwayCharacter, char* secondaryCharacter, char* primaryColour, char* halfwayColour, char* secondaryColour) {
    sprintf(buf, "%s", primaryColour);
    for (unsigned int i = 0; i < width * percent - 1; ++i) {
        sprintf(buf + strlen(buf), "%s", primaryCharacter);
    }
    sprintf(buf + strlen(buf), "%s%s%s", halfwayColour, halfwayCharacter, secondaryColour);
    for (unsigned int i = 0; i < width * (1-percent); ++i) {
        sprintf(buf + strlen(buf), "%s", secondaryCharacter);
    }
}

int main(int argc, char** argv) {
    // printf("\x1b[38;5;16m");
    // printf("     ______           __          __  _\n");
    // printf("    / ____/___ ____  / /__  _____/ /_(_)___ _\n");
    // printf("   / /   / __ `/ _ \\/ / _ \\/ ___/ __/ / __ `/\n");
    // printf("  / /___/ /_/ /  __/ /  __(__  ) /_/ / /_/ /\n");
    // printf("  \\____/\\__,_/\\___/_/\\___/____/\\__/_/\\__,_/\n\n");

    unsigned int paddingLeft = 0;
    unsigned int boxWidth = 35;

    if (argc > 1) {
        for (uint i = 0; i < strlen(argv[1]); ++i) {
            paddingLeft *= 10;
            paddingLeft += argv[1][i] - '0';
        }
        if (argc > 2) {
            boxWidth = 0;
            for (uint i = 0; i < strlen(argv[2]); ++i) {
                boxWidth *= 10;
                boxWidth += argv[2][i] - '0';
            }
        }
    }

    for (unsigned int i = paddingLeft; i != 0; --i)
        printf(" ");
    printf("╭");
    for (unsigned int i = boxWidth; i != 0; --i)
        printf("─");
    printf("╮\n");

    struct sysinfo sys_info;
    char* str = (char*)malloc(100*sizeof(char));

    struct utsname unameData;
    uname(&unameData);

    // Kernel
#ifdef _KERNEL
    boxFormat("", "kernel:", unameData.release, _KERNEL_COLOUR, paddingLeft, boxWidth, 0);
#endif
    // -----------------------------

    // Uptime
#ifdef _UPTIME
    int days, hours, mins, x = 1;

    if(sysinfo(&sys_info) != 0)
        perror("sysinfo");

    days = sys_info.uptime / 86400;
    hours = (sys_info.uptime / 3600) - (days * 24);
    mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);

    sprintf(str, "%d hours, %d minutes", hours, mins);
    boxFormat("", "uptime:", str, _UPTIME_COLOUR, paddingLeft, boxWidth, 0);
#endif
    // -----------------------------

    // Shell
#ifdef _SHELL
    char path[BUFSIZ], comm[BUFSIZ];
    pid_t parent_pid = getppid();
    int fd;
    snprintf(path, BUFSIZ, "/proc/%d/comm", parent_pid);
    fd = open(path, O_RDONLY);
    memset(comm, 0, BUFSIZ);
    read(fd, comm, BUFSIZ-1);

    comm[strcspn(comm, "\n")] = 0; // Remove \n from end of comm
    boxFormat("", "shell:", comm, _SHELL_COLOUR, paddingLeft, boxWidth, 0);
#endif
    // -----------------------------

    // Ram
#ifdef _RAM
    double bytesToGBMultiplier = 9.313225746154785e-10;
#ifdef _BARS
    bar(str, (float)(sys_info.totalram - sys_info.freeram) / sys_info.totalram, 20, "━","╸", "━", _RAM_COLOUR, _RAM_COLOUR, _BAR_BG_COLOUR);
    boxFormat("", "mem:", str, _RAM_COLOUR, paddingLeft, boxWidth, 21);
#else
    sprintf(str, "%.2f GiB / %.2f GiB", (sys_info.totalram - sys_info.freeram) * bytesToGBMultiplier, sys_info.totalram * bytesToGBMultiplier);
    boxFormat("", "mem:", str, _RAM_COLOUR, paddingLeft, boxWidth, 0);
#endif
#endif
    // -----------------------------

    // Swap
#ifdef _SWAP
#ifdef _BARS
    bar(str, (float)(sys_info.totalswap - sys_info.freeswap) / sys_info.totalswap, 20, "━","╸", "━", _SWAP_COLOUR, _SWAP_COLOUR, _BAR_BG_COLOUR);
    boxFormat("", "swap:", str, _SWAP_COLOUR, paddingLeft, boxWidth, 21);
#else
    sprintf(str, "%.2f GiB / %.2f GiB", (sys_info.totalswap - sys_info.freeswap) * bytesToGBMultiplier, sys_info.totalswap * bytesToGBMultiplier);
    boxFormat("", "swap:", str, _SWAP_COLOUR, paddingLeft, boxWidth, 0);
#endif
#endif
    // -----------------------------

    // Processes
#ifdef _PROCESSES
    sprintf(str, "%d", sys_info.procs);
    boxFormat("", "procs:", str, _PROCESSES_COLOUR, paddingLeft, boxWidth, 0);
#endif
    // -----------------------------
    
    // Packages
#ifdef _PACKAGES
    DIR* dir;
    struct dirent* entry;
    unsigned int count = 0;
    dir = opendir(_PACKAGE_PATH);
    while ((entry = readdir(dir)) != NULL) {
        char* name = entry->d_name;
        if (entry->d_type == DT_DIR) {
            if (!strcmp(name, ".") || !strcmp(name, ".."))
                continue;
            ++count;
        }
    }
    closedir(dir);

    sprintf(str, "%u", count);
    boxFormat("", "pkgs:", str, _PACKAGES_COLOUR, paddingLeft, boxWidth, 0);
#endif
    // -----------------------------

    // User
#ifdef _USER
    gid_t gid;
    struct group* grp;
    gid = getgid();
    grp = getgrgid(gid);
    boxFormat("", "user:", grp->gr_name, _USER_COLOUR, paddingLeft, boxWidth, 0);
#endif
    // -----------------------------

    // Hostname
#ifdef _HOSTNAME
    boxFormat("", "hname:", unameData.nodename, _HOSTNAME_COLOUR, paddingLeft, boxWidth, 0);
#endif
    // -----------------------------
    
    // Distro
#ifdef _DISTRO
    char buf[PATH_MAX];
    char* res = realpath("/etc/os-release", buf);
    if (res) {
        FILE* fptr;
        fptr = fopen(buf, "r");
        if (fptr != NULL) {
            char osName[50];
            fgets(osName, 50, fptr);
            fgets(osName, 50, fptr);
            
            sprintf(str, "%.*s", strlen(osName) - 15, osName + (strlen(osName) - 12));

            boxFormat("", "distro:", str, _DISTRO_COLOUR, paddingLeft, boxWidth, 0);
        }
        else {
            printf("Unable to open /etc/os-release to find distribution name\n");
        }
        fclose(fptr);
    }
    else {
        char* errStr = strerror(errno);
        printf("error string: %s\n", errStr);
    }
#endif
    // -----------------------------
    
    for (unsigned int i = paddingLeft; i != 0; --i)
        printf(" ");
    printf("╰");
    for (unsigned int i = boxWidth; i != 0; --i)
        printf("─");
    printf("╯\n");

    free(str);

    return 0;
}

