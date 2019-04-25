#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main() {
    int id = create_container();
    printf(1,"Before joining\n");
    ls(".");
    join_container(id);
    printf(1,"After joining\n");
    ls(".");
    // if (fork() == 0) {
    //     // Join the container
    //     if (join_container(id) < 0) {
    //         printf(1, "ERROR JOINING CONTAINER!! %d %d \n", 1, id);
    //         exit();
    //     }

    //     // Print the current host file system
    //     ls(".");
    //     sleep(100);

        int fd = open("file_1", O_CREATE | O_RDWR);
        if(fd < 0) {
            printf(1, "Error creating file!!\n");
            exit();
        }
    printf(1,"After creation\n");
    ls(".");

        char MSG[] = "Modified by: 1-1\n";
        printf (1, "%s\n", MSG);
        int size = sizeof(MSG);
        if (write (fd, MSG, size) != size) {
            printf(1, "Error writing to file");
            exit();
        }
    cat("file_1");
    //     close(fd);

    //     sleep(400); // Wait for other processes
    //     leave_container();
    //     exit();
    // }

    // if (fork() == 0) {
    //     // Join the container
    //     if (join_container(id) < 0) {
    //         printf(1, "ERROR JOINING CONTAINER!! %d %d \n", 1, id);
    //         exit();
    //     }

    //     // Wait for the other process to do things
    //     sleep(400);

    //     // Print the current host file system
    //     ls(".");

    //     // Read the contents of the file
    //     cat("file_1");

    //     // leave_container();
    //     exit();
    // }

    // wait();
    // wait();
    leave_container();
    printf(1,"After leaving\n");
    ls(".");
    // fd = open("file_1", O_CREATE | O_RDWR);
    // if(fd < 0) {
    //     printf(1, "Error creating file!!\n");
    //     exit();
    // }
    // printf(1,"After second creation\n");
    // ls(".");

    //     char MSG2[]= "Modified by: 1-2\n";
    //     printf (1, "%s\n", MSG2);
    //     size = sizeof(MSG2);
    //     if (write (fd, MSG2, size) != size) {
    //         printf(1, "Error writing to file");
    //         exit();
    //     }
    // cat("file_1");
    id = create_container();
    printf(1,"Before joining\n");
    // ls(".");
    join_container(id);
    printf(1,"After joining\n");
    ls(".");
    fd = open("file_1", O_CREATE | O_RDWR);
        if(fd < 0) {
            printf(1, "Error creating file!!\n");
            exit();
        }
    printf(1,"After creation\n");
    ls(".");

        char MSG2[] = "Modified by: #The new one\n";
        printf (1, "%s\n", MSG2);
        size = sizeof(MSG2);
        if (write (fd, MSG2, size) != size) {
            printf(1, "Error writing to file");
            exit();
        }

    cat ("file_1");

    leave_container();
    printf(1,"After leaving\n");
    ls(".");

    destroy_container (id);
    exit();
}