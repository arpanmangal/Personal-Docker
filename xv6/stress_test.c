// Stress test the container system
#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_CONTAINERS 10

int
main(void)
{
  /* Create some containers */
  uint container_ids[MAX_CONTAINERS];
  for (int i = 0; i < MAX_CONTAINERS; i++) {
    container_ids[i] = create_container ();
    if (container_ids[i] < 0) {
      printf(1, "ERROR CREATING CONTAINER!! %d %d \n", i, container_ids[i]);
      exit();
    }
  }

  /* Destroy some random containers */
  if (destroy_container(container_ids[0]) < 0) {
    printf(1, "ERROR DESTROYING CONTAINER!! %d %d \n", 0, container_ids[0]);
    exit();
  }
  if (destroy_container(container_ids[3]) < 0) {
    printf(1, "ERROR DESTROYING CONTAINER!! %d %d \n", 3, container_ids[3]);
    exit();
  }

  /* Fork some processes and join them to some containers */
  if (fork() == 0) {
    printf(1, "pid1:%d\n", getpid());
    if (join_container(container_ids[0]) >= 0) {
      printf(1, "JOINED TO A NON-EXISTENT CONTAINER!! %d %d \n", 0, container_ids[0]);
      exit();
    }

    if (join_container(container_ids[1]) < 0) {
      printf(1, "ERROR JOINING CONTAINER!! %d %d \n", 1, container_ids[1]);
      exit();
    } 

    sleep (10); // Wait
    exit();
  }

  if (fork() == 0) {
    printf(1, "pid2:%d\n", getpid());
    if (join_container(container_ids[2]) < 0) {
      printf(1, "ERROR JOINING CONTAINER!! %d %d \n", 2, container_ids[2]);
      exit();
    }

    if (leave_container() < 0) {
      printf(1, "ERROR LEAVING CONTAINER!! %d %d \n", 2, container_ids[2]);
      exit();
    }

    if (join_container(container_ids[1]) < 0) {
      printf(1, "ERROR JOINING CONTAINER!! %d %d \n", 1, container_ids[1]);
      exit();
    }

    sleep(500); // Some wait
    ps(); // Print the processes
    exit();
  }

  if (fork() == 0) {
    printf(1, "pid3:%d\n", getpid());
    if (join_container(container_ids[5]) < 0) {
      printf(1, "ERROR JOINING CONTAINER!! %d %d \n", 5, container_ids[5]);
      exit();
    }

    if (fork() == 0) {
      printf(1, "pid4:%d\n", getpid());
      if (join_container(container_ids[1]) < 0) {
        printf(1, "ERROR JOINING CONTAINER!! %d %d \n", 1, container_ids[1]);
        exit();
      }    
      sleep(1000);
      exit();
    }
    wait();
    exit();
  }

  for (int i = 0; i < 4; i++) {
    wait();
  }
  printf (1, "SUCCESSFULLY PASSED ALL CASES!!!\n");

  exit();
}
