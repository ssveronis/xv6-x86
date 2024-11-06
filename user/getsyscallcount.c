#include "types.h"
#include "stat.h"
#include "user.h"


int main(int argc, char *argv[]){
    if (argc != 2){
        printf(1, "getsyscallcount: Unexpected input");
        exit();
    }
    getcount(
        atoi( //ASCII to integer
        /*
        atoi("25")->25
        atoi("abc")->¯\_(ツ)_/¯
        Δεν έχει error handling
        */
            argv[1] //SYSCALL number
            )
            );
    exit();
}