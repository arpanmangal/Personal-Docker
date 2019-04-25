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

    // FILE SYSTEM TEST
    sleep(60*pid);
    printf(1,"\nProcess %d executing ls\n",pid);
    ls(".");
    



    exit();
}

void container_slave(int container_id){
    int pid = getpid();
    join_container(container_id);
    sleep(30*container_id);

    // FILE SYSTEM TEST
    sleep(60*pid);
    printf(1,"\nProcess %d executing ls\n",pid);
    ls(".");

    char pid_char = pid+'0';
    int fd = open("file_1", O_CREATE | O_RDWR);

    exit();
}

int main() {
    int cont1 =  create_container();
    int cont2 =  create_container();
    int cont3 =  create_container();
    printf(1,"%d %d %d\n",cont1,cont2, cont3);

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