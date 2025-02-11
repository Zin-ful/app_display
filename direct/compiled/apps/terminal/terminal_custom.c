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

void handle_cmd(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        printf("\nwelcome to the control terminal\nusing this terminal you can import apps (assuming their in the right folder)\nadjust screen parameters (column/row sizing)\nchange the screen location of apps\nalter mouse sizing\nand probably more. eventually.\n");
    } else if (strcmp(cmd, "exit") == 0) {
        printf("exitiing...\n");
        exit(0);
    } else if (strcmp(cmd, "enter bash") == 0) {
        execlp("bash","bash",NULL);
    }
}

int main() {
    //O_NOCTTY prevents our terminal from controlling the caller
    printf("\033[H\033[J");
    printf("this doesnt actually handle commands yet, it just executes functions based off of strcmp.\n to avoid overhead, we will execute from a file instead of writing individual functions.\n this way we can taylor each command with more accuracy\n");
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
        close(master_fd);
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
        close(slave_fd);
        //close master because we fork (make child, no sexy) and dont need two controllers
        //shell start
        execlp("/bin/sh", "sh", NULL);

        perror("execlp failed\n");

        exit(1);
    }

    if (pid > 0) {
        char buffer[512];
        char buffer_out[512];
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
                char c;
                int text = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
                if (text > 0) {
                    buffer[text] = '\0';
                    if (buffer[text - 1] == '\n') {
                        buffer[text - 1] = '\0';
                    }
                    handle_cmd(buffer);
                }
            }
        }
    }
    return 0;
}

