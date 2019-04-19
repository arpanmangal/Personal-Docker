#include "types.h"
#include "stat.h"
#include "user.h"

char *argv_c[] = { "REAdkfdljfDME" };

int main() {
    // exec("ls", argv_c);
    exec("cat", argv_c);
    // ls();
    // cat(1);
    exit();
}