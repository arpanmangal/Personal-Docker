#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


char *argv_c[] = { "." };

int main() {
    // exec("ls", argv_c);
    // exec("cat", argv_c);
    // ls();
    // cat(1);
    int id = create_container();
    if (fork() == 0) {
        // Join the container
        if (join_container(id) < 0) {
            printf(1, "ERROR JOINING CONTAINER!! %d %d \n", 1, id);
            exit();
        }

        // Print the current host file system
        if (fork() == 0) {
            exec("ls", argv_c);
        }
        wait();
        sleep(100);

        int fd = open("file_3", O_CREATE | O_RDWR);
        if(fd < 0) {
            printf(1, "Error creating file!!\n");
            exit();
        }

        char MSG[] = "Modified by: ";
        printf (1, "%s | %d\n", MSG, sizeof(MSG));
        int size = sizeof(MSG);
        if (write (fd, MSG, size) != size) {
            printf(1, "Error writing to file");
            exit();
        }

        close(fd);

        sleep(400); // Wait for other processes
        leave_container();
        exit();
    }

    if (fork() == 0) {
        // Join the container
        if (join_container(id) < 0) {
            printf(1, "ERROR JOINING CONTAINER!! %d %d \n", 1, id);
            exit();
        }

        // Wait for the other process to do things
        sleep(200);

        // Print the current host file system
        if (fork() == 0) {
            exec("ls", argv_c);
        }
        wait();

        // Read the contents of the file
        int fd;
        fd = open ("file_3", O_RDONLY);
        if (fd < 0) {
            printf(1, "Error opening file!!");
            exit();
        }

        int sizeofmsg = 14;
        char MSG[sizeofmsg];
        // int sizeofpid = sizeof(int);
        // int pid;
        if (read(fd, &MSG[0], sizeofmsg) != sizeofmsg) {
            printf (1, "Error reading file!!\n");
            exit();
        }

        printf(1, "%s\n", MSG);
        close (fd);

        char *argc[] = {"file_3", "file_3", "\0"};
        if (fork() == 0) {
            printf(1, "Before cat\n");
            exec("cat", argc);
        }
        wait();

        leave_container();
        exit();
    }

    wait();
    wait();
    destroy_container (id);
    exit();
}