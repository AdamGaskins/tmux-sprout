#include "tmux.h"
#include "common.h"

#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int run(char* const* args, char** output)
{
    pid_t pid = fork();

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pid == -1) {
        error("failed to fork", 0);
    } else if (pid > 0) {
        close(pipefd[1]); // Close the write end

        output = calloc(1024 + 1, 1);
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], output, 1024)) > 0)
            ;

        close(pipefd[0]); // Close the read end

        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }

    // int fd = open("/dev/null", O_WRONLY);
    // if (dup2(fd, 1) < 0) { }
    // if (dup2(fd, 2) < 0) { }

    close(pipefd[0]); // Close the read end
    dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the write end
    close(pipefd[1]); // Close the original write end

    // we are the child
    execvp(args[0], args);
    exit(EXIT_FAILURE); // exec never returns
}

bool tmux_has_session(char* sessionName)
{
    return 0 == run((char* const[]) { "tmux", "has-session", "-t", sessionName, NULL }, NULL);
}

bool tmux_kill_session(char* sessionName)
{
    return 0 == run((char* const[]) { "tmux", "kill-session", "-t", sessionName, NULL }, NULL);
}

bool tmux_new_session(char* sessionName, char* pwd)
{
    if (pwd == NULL) {
        return 0 == run((char* const[]) { "tmux", "new-session", "-d", "-s", sessionName, NULL }, NULL);
    }

    return 0 == run((char* const[]) { "tmux", "new-session", "-d", "-s", sessionName, "-c", pwd, NULL }, NULL);
}

bool tmux_rename_window(char* sessionName, char* windowName)
{
    return 0 == run((char* const[]) { "tmux", "rename-window", "-t", sessionName, windowName, NULL }, NULL);
}

bool tmux_new_window(char* sessionName, char* windowName, char* pwd)
{
    if (pwd == NULL) {
        return 0 == run((char* const[]) { "tmux", "new-window", "-t", sessionName, "-n", windowName, NULL }, NULL);
    }

    return 0 == run((char* const[]) { "tmux", "new-window", "-t", sessionName, "-n", windowName, "-c", pwd, NULL }, NULL);
}

bool tmux_select_window(char* sessionName, char* windowName)
{
    char out[1024];
    snprintf(out, sizeof(out), "%s:%s", sessionName, windowName);
    return 0 == run((char* const[]) { "tmux", "select-window", "-t", out, NULL }, NULL);
}

bool tmux_attach_session(char* sessionName)
{
    if (getenv("TMUX") != 0) { // inside tmux
        return 0 == run((char* const[]) { "tmux", "switch-client", "-t", sessionName, NULL }, NULL);
    }

    return 0 == run((char* const[]) { "tmux", "attach-session", "-t", sessionName, NULL }, NULL);
}

bool tmux_run_command(char* sessionName, char* windowName, char* command)
{
    char out[1024];
    snprintf(out, sizeof(out), "%s:%s", sessionName, windowName);
    return 0 == run((char* const[]) { "tmux", "send-keys", "-t", out, command, "Enter", NULL }, NULL);
}
