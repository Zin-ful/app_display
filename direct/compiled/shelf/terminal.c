#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <fcntl.h>
#include <pty.h>

#define BUFF_SIZE 1024

void init_term(int fd) {
    struct termios term;
    tcgetattr(fd, &term);
    term.c_lflag &= -ECHO; //no echo
    tcsetattr(fd, TCSANOW, &term);
}

int main() {
    int master_fd, slave_fd;
    pid_t child_pid;
    char slave_name[BUFF_SIZE];

    //make master/slave pair

    if (openpty(&master_fd, &slave_fd, slave_name, NULL, NULL) == -1) {
        perror("error opening pty\n");
        exit(1);
    }
    if ((child_pid=fork()) == -1) { //pemdas baby
        perror("error making child ;)\n"); //ayo
        exit(1);
    }
    if (child_pid == 0){
        close(master_fd); //we dont need the child-master dup. we are using the original for the user interaction only

        init_term(slave_fd);

        if (setsid() == -1) {
            perror("error in setting child prioritry\n");
            exit(1);
        }
        execlp("bash","bash",NULL);
        perror("error in execlp\n");
        exit(1);
    } else {
        close(slave_fd);
        char buffer[BUFF_SIZE];
        fd_set read_fds;
        int max_fd;
        while (1) {
            FD_ZERO(&read_fds);
            FD_SET(STDIN_FILENO, &read_fds);
            FD_SET(master_fd, &read_fds);
            max_fd = (STDIN_FILENO > master_fd) ? STDIN_FILENO : master_fd;

            if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
                perror("error selecting\n");
                exit(1);
            }
            if (FD_ISSET(STDIN_FILENO, &read_fds)) {
                ssize_t len = read(STDIN_FILENO, buffer, sizeof(buffer));
                if (len > 0) {
                    buffer[len] = '\0';
                    if (buffer[len - 1] == '\n') {
                        buffer[len - 1] = '\0';
                    }
                    write(master_fd, buffer, len);
                }
            }
            if (FD_ISSET(master_fd, &read_fds)) {
                ssize_t len = read(master_fd, buffer, sizeof(buffer));
                if (len > 0) {
                    write(STDOUT_FILENO, buffer, len);
                }
            }
        }
        waitpid(child_pid, NULL, 0);
    }
    return 0;
}
