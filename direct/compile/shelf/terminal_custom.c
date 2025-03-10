#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>
//terms
//PTY = psuedo bash/terminal
//fd = file descriptor - all IO streams in UNIX

/*
the master pty acts as our voice into the actual shell. we interact with the master (the emulator)
and it passes what we input into the slave which is controlled by the actual shell
*/

int main() {
    //O_NOCTTY prevents our terminal from controlling the caller
    int master_fd;
    char slave_name[128];
    master_fd = posix_openpt(O_RDWR | O_NOCTTY);

    if (master_fd < 0) {
        perror("error opening posix\n");
        return 1;
    }
    //grantpt changes the perms of the slave (master-slave link), then we unlock it while getting the name
    grantpt(master_fd);
    unlockpt(master_fd);
    ptsname_r(master_fd, slave_name, sizeof(slave_name));
    printf("PTY created: %s\n", slave_name);
    //new child process (duplication of current), dup2 redirects IO into the pty
    pid_t pid = fork();
    if (pid == 0) {
        int slave_fd = open(slave_name, O_RDWR);
        if (slave_fd < 0) {
            perror("error opening slave PTY\n");
            return 1;
        }

        setsid(); //session ID

        if (ioctl(slave_fd, TIOCSCTTY, 0) < 0) {
            perror("error settiing controlling terminal\n");
            exit(1);
        }
        
        dup2(slave_fd, STDIN_FILENO);
        dup2(slave_fd, STDOUT_FILENO);
        dup2(slave_fd, STDERR_FILENO);
        //close master because we fork (make child, no sexy) and dont need two controllers
        close(master_fd);
        //shell start
        execlp("/bin/sh", "sh", NULL);
        perror("execlp failed\n");

        exit(1);
    }

    if (pid > 0) {
        char buffer[256];
        fd_set fds;

        while (1) {
            FD_ZERO(&fds); //reset
            FD_SET(STDIN_FILENO, &fds);
            FD_SET(master_fd, &fds);
            //monitor STDIN for activity
            int activity = select(master_fd + 1, &fds, NULL, NULL, NULL);//select monitors multiple events

            if (activity < 0) {
                perror("error selecting\n");
                break;
            }
            if (FD_ISSET(STDIN_FILENO, &fds)){ //is fd ready (if user input)
                int text = read(STDIN_FILENO, buffer, sizeof(buffer));
                if (text > 0) {
                    write(STDOUT_FILENO, buffer, text);
                }
            }
        }
        close(master_fd);
    }

    return 0;
}

