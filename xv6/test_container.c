#include "types.h"
#include "user.h"
#include "date.h"

int stdout = 1;
int main(int argc, char *argv[])
{
    int cont_id1 = create_container();
    int cont_id2 = create_container();
    printf(1,"Parent pid %d\n",getpid());

    join_container(cont_id2);
    if (fork()==0){
        printf(1,"%d Before Joining\n",getpid());
        ps();
        join_container(cont_id1);
        printf(1,"%d After Joining\n",getpid());
        ps();
        sleep(200);
        exit();
    }

    if (fork()==0){
        sleep(100);
        join_container(cont_id1);
        printf(1,"%d After Joining\n",getpid());
        ps();
        leave_container();
        printf(1,"%d After Leaving\n",getpid());
        ps();
        sleep(100);
        exit();
    }
    leave_container();
    wait();
    wait();
    destroy_container(cont_id1);
    destroy_container(cont_id2);
    exit();
}