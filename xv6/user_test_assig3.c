#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void container_master(int container_id){
    int pid = getpid();
    join_container(container_id);

    // PROCESS ISOLATION
    sleep(20*container_id);
    ps();
    printf(1,"---------\n");
    sleep(20*(3-container_id) + container_id);

    if (container_id==1){
        toggle_scheduler_trace();
    }

    if (container_id==3){
        toggle_scheduler_trace();
    }

    // FILE SYSTEM TEST
    sleep(100*(pid-3));
    printf(1,"\nProcess %d executing ls\n",pid);
    ls(".");
    sleep(100*(8-pid));

    // creating new file
    char pid_char = pid+'0';
    char new_file_name[7] = {'f','i','l','e','_',pid_char};
    int fd = open(new_file_name, O_CREATE | O_RDWR);
    close(fd);

    sleep(100*(pid-3));
    printf(1,"\nProcess %d executing ls after file creation\n",pid);
    ls(".");

    // BARRIER
    sleep(100*(9-pid));

    
    fd = open("my_file", O_CREATE | O_RDWR);
    char msg[15] = "Modified by:  \n";
    msg[13] = pid_char;
    int size = sizeof(msg);
    if (write (fd, msg, size) != size) {
        printf(1, "Error writing to file");
        exit();
    }
    close(fd);
    sleep(50*pid);
    printf(1,"\nProcess %d: my_file contains\n",pid);
    cat("my_file");

    leave_container();
    exit();
}

void container_slave(int container_id){
    int pid = getpid();
    join_container(container_id);
    sleep(90);

    // FILE SYSTEM TEST
    sleep(100*(pid-3));
    printf(1,"\nProcess %d executing ls\n",pid);
    ls(".");
    sleep(100*(8-pid));

    char pid_char = pid+'0';
    char new_file_name[7] = {'f','i','l','e','_',pid_char};
    int fd = open(new_file_name, O_CREATE | O_RDWR);
    close(fd);

    sleep(100*(pid-3));
    printf(1,"\nProcess %d executing ls after file creation\n",pid);
    ls(".");
    leave_container();
    exit();
}

int main() {
    int cont1 =  create_container();
    int cont2 =  create_container();
    int cont3 =  create_container();
    // printf(1,"%d %d %d\n",cont1,cont2, cont3);

    if (fork()==0){
        container_master(cont1);
    }
    if (fork()==0){
        container_slave(cont1);
    }
    if (fork()==0){
        container_slave(cont1);
    }


    if (fork()==0){
        container_master(cont2);
    }
    if (fork()==0){
        container_master(cont3);
    }

    sleep(10);
    ps();
    printf(1,"---------\n");

    wait();
    wait();
    wait();
    wait();
    wait();
    exit();

}