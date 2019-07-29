#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>



// This is the extra credit project for CS214 worth 40 points total.
// The tast of this project was to duplicate the funcionality of "ps aux". This command
// stands for process status and is a tool that will display information about the
// running processes.
// Devvrat Patel
// dap299
// Cs214


// A struct that contains all the required information for the processes




typedef struct pInfo {
    char*   User;
    float   CPU, MEM;
    int     VSZ, RSS, PID;
    char    TTY;
    char*   STAT;
    char*   TIME;
    char*   START;
    char*   COMMAND;
} prInfo;

char* vanchFile(char* path);
prInfo* parse_status(char* status, prInfo *proc);

prInfo* parse_status(char* status, prInfo *proc) {
    char* uid_line = strstr(status, "Uid:");
    if(!uid_line) {
        return NULL;
    }
    int uid;
    sscanf(uid_line, "Uid:\t%d", &uid);
    struct passwd *pwd;
    pwd = getpwuid(uid);
    proc->User = pwd->pw_name;
    char* PID_line = strstr(status, "Pid:");
    if(!PID_line) {
        return NULL;
    }

    int PID;
    sscanf(PID_line, "Pid:\t%d\n", &PID);
    proc->PID = PID;

    char* VSZ_line = strstr(status, "VmSize:");
    if (!VSZ_line) {
        proc->VSZ = 0;
    } else {
        int VmSize;
        sscanf(VSZ_line,"VmSize:\t%d ", &VmSize);
        proc->VSZ = VmSize;
    }

    char* RSS_line = strstr(status, "VmRSS:");
    if (!RSS_line) {
        proc->RSS = 0;
    }
    else {
        int VmRSS;
        sscanf(RSS_line, "VmRSS:\t%d ", &VmRSS);
        proc->RSS = VmRSS;
    }

    char *VmLck_line = strstr(status, "VmLck:");
    if(VmLck_line) {
        char VmLck;
        sscanf(VmLck_line, "VmLck:\t%c ", &VmLck);
        proc->TTY = VmLck;
    } else {
        proc->TTY = '0';
    }

    return proc;
}

double uTimeG() {
    char* path = "/proc/uptime";
    char *buff = vanchFile(path);

    double uptime;
    sscanf(buff, "%lf", &uptime);
    return uptime;
}

char** spread(char *file) {
    int spaces = 0;
    int i = 0;
    while (i < strlen(file)) {
        if(file[i] == ' ') {
            spaces += 1;
        }
        i += 1;
    }

    char **sp = (char**) malloc(sizeof(char*) * (spaces+1));

    char delim[2] = " ";
    char *token = strtok(file, delim);
    sp[0] = token;

    i = 1;
    while(token != NULL) {
        token = strtok(NULL, delim);
        sp[i] = token;
        i += 1;
    }

    return sp;
}

int vadThread(int PID) {
    char path[50];
    sprintf(path, "/proc/%d/task", PID);

    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        return 0;
    }

    int i = 0;
    struct dirent *de;
    while ((de = readdir(dir)) != NULL)
        i += 1;

    return i > 3 ? 1 : 0;
}
/*

        gettimeofday(&begin, NULL);

        malloced = 0;
        occupied = 0;
        done =0;

        while(!done){

            int r = rand()% 20;

            if(malloced != 150 && r > 15){
                int index = rand() % 150;

                if(array[index] == &garbage){

                    int size = (rand() % 64) + 1;
                    array[index] = malloc(size);
                    malloced ++;
                    occupied++;
                }

            }else{
                int index = rand() % (150);

                if(array[index] != &garbage){
                    free(array[index]);
                    array[index] = &garbage;
                    occupied--;
                }
            }
            if(malloced == 150 && occupied == 0){

                done =1 ;

            }
        }

        gettimeofday(&end, NULL);
        elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        d+= elapsed;


        // TEST E (Description in testplan.txt)

        gettimeofday(&begin, NULL);

        int  size = (rand() % 63) +2;

        int capacity = (4997)/ (size +1) -1;

        void * pointers[capacity];

        for(i = 0; i<capacity; i++){

            pointers[i] = malloc(size);
        }

        for(i = 0; i<capacity; i+=2){

            free(pointers[i]);
        }
        int freeChunks = size - 1;

        for(i = 0; i<capacity; i+=2){

            pointers[i] = malloc (freeChunks);
        }
        for(i = 0; i<capacity; i++){

            free(pointers[i]);
        }

        gettimeofday(&end, NULL);
        elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        e+= elapsed;

        // TEST F (Description in test plan.txt)

        size = (rand() % 55) +10;

        capacity = (4997)/ (size +1) -1;

        void * pointers2[capacity];

        for(i = 0; i<capacity; i++){

            pointers2[i] = malloc(size);
        }

        for(i = 0; i<capacity; i+=2){



*/

prInfo* pSfile(char* statfile, prInfo *proc) {
    double uptime = uTimeG();
    char** stat = spread(statfile);

    int utime, stime, cutime, cstime, starttime;
    utime = atof(stat[13]);
    stime = atof(stat[14]);
    cutime = atof(stat[15]);
    cstime = atof(stat[16]);
    starttime = atof(stat[21]);
    long Hertz = sysconf(_SC_CLK_TCK);

    int total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime / Hertz);

    float CPU = 100 * ((total_time / Hertz) / seconds);
    proc->CPU = CPU;
    proc->STAT = (char*) malloc(15);
    memset(proc->STAT, '\0', 15);

    proc->STAT[0] = stat[2][0];
    int end = 1;
    if(stat[18][0] == '-' ){
        proc->STAT[end] = '<';
        end += 1;
    }

    else if (stat[18][0] != '0') {
        proc->STAT[end] = 'N';
        end += 1;
    }

    if(proc->TTY != '0') {
        proc->STAT[end] = 'L';
        end += 1;
    }
    proc->TTY = '?';
    if(vadThread(proc->PID)){
        proc->STAT[end] = 'l';
        end += 1;
    }
    int sid;
    sscanf(stat[5], "%d", &sid);

    if(proc->PID == sid){
        proc->STAT[end] = 's';
        end += 1;
    }

    int tpig;
    sscanf(stat[7], "%d", &tpig);
    if(tpig == proc->PID) {
        proc->STAT[end] = '+';
        end += 1;
    }

    proc->STAT[end] = '\0';
    int minutes = seconds / 60;
    int hours = minutes / 60;

    return proc;
}

char* vanchFile(char* path) {

    int fd = open(path, O_RDONLY);
    int size = 1000;
    char* buff = (char*) malloc(size);
    memset(buff, '\0', size);

    int status = 1;
    int i = 0;
    while(status > 0) {
        status = read(fd, buff + i, 1);
        i += 1;
        if(i >= size) {
            char* buff2 = (char*)realloc(buff, 2*size);
            size = size * 2;
            buff = buff2;
        }
    }

    close(fd);
    return buff;
}

prInfo* get_mem(prInfo* proc) {
    char* buff = vanchFile("/proc/meminfo");
    int memTotal;
    sscanf(buff, "MemTotal:       %d kB", &memTotal);
    proc->MEM = 100 * ((float)proc->RSS / (float)memTotal);
    free(buff);

    return proc;
}

prInfo* get_command(prInfo* proc) {
    char path[50];
    sprintf(path, "/proc/%d/cmdline", proc->PID);
    char *buff = (char*) malloc(1024);
    memset(buff, '\0', 1024);

    int fd = open(path, O_RDONLY);

    int status = 1;
    int i = 0;
    char c;
    while (status > 0) {
        status = read(fd, &c, 1);
        if(c == '\0') {
            buff[i] = ' ';
        } else {
            buff[i] = c;
        }
        i += 1;
    }
    close(fd);
    if(i == 1) {
        memset(buff, '\0', 1024);
        memset(path, '\0', 50);
        sprintf(path, "/proc/%d/comm", proc->PID);
        fd = open(path, O_RDONLY);
        status = 1;
        buff[0] = '[';
        i = 1;
        while (status > 0) {
            status = read(fd, &c, 1);
            if(c == '\0') {
                buff[i] = ' ';
            } else {
                buff[i] = c;
            }
            i += 1;
        }
        buff[i-2] = ']';
        buff[i-1] = '\0';
    }

    proc->COMMAND = buff;

    close(fd);
    return proc;
}

int main() {
    printf("%-16s%8s%8s%8s%10s%8s%8s%8s%8s%8s%10s\n", "USER", "PID", "%CPU", "%MEM", "VSZ", "RSS", "TTY", "STAT", "START", "TIME", "COMMAND");
    DIR *dir = opendir("/proc");
    struct dirent *dp;
    while((dp = readdir(dir) ) != NULL) {

        prInfo *proc = (prInfo*) malloc(sizeof(prInfo));
        char path[100];

        //Get user from status file
        sprintf(path, "/proc/%s/status", dp->d_name);
        char* status_file = vanchFile(path);
        if(!status_file)
            continue;

        proc = parse_status(status_file, proc);
        free(status_file);
        if(!proc)
            continue;

        memset(path, '\0', 100);
        sprintf(path, "/proc/%s/stat", dp->d_name);
        char* statfile = vanchFile(path);
        proc = pSfile(statfile, proc);
        proc = get_mem(proc);
        proc = get_command(proc);
        printf("%-16s%8d%8.1f%8.1f%10d%8d%8s%8s%8s%8s   %s\n",proc->User, proc->PID, proc->CPU, proc->MEM, proc->VSZ, proc->RSS, "?", proc->STAT, "N/A", "N/A", proc->COMMAND);

        free(proc->STAT);
        free(proc->COMMAND);
        free(proc);
        free(statfile);
    }
}
