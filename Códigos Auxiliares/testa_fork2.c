#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main (void) {
    pid_t processId;
    if ((processId = fork()) == 0) {
        char app[] = "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome";
        char * const argv[] = { app, NULL, NULL };
        if (execv(app, argv) < 0) {
            perror("execv error");
        }
        printf("depois do exec\n");
    } else if (processId < 0) {
        perror("fork error");
    } else {
        printf("eu sou o pai com filho pid = %d\n", processId);
        printf("dormindo...\n");
        sleep(5);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}