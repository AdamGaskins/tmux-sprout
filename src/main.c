#include <_string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "config.h"
#include "tmux.h"

void printUsage(void)
{
    printf("Usage: tmux-sprout [project]\n");
}

void createConfig(char* path)
{
    printf("Creating config %s\n", path);
    FILE* fp = fopen(path, "w");
    if (fp == NULL) {
        error("failed to open config file", 0);
    }

    fprintf(fp, "[[projects]]\n");
    fprintf(fp, "name = \"project\"\n");
    fprintf(fp, "folder = \"code/project\"\n");
    fprintf(fp, "tabs.editor = \"nvim\"\n");
    fprintf(fp, "tabs.build = \"npm run dev\"\n");
    fprintf(fp, "tabs.shell = \"\"\n");

    fclose(fp);
}

char* findConfigPath(void)
{
    char* fname = "tmux-sprout.toml";
    if (access(fname, F_OK) == 0) {
        return strdup(fname);
    }

    if (getenv("XDG_CONFIG_HOME") != 0) {
        char path[255];
        snprintf(path, sizeof(path), "%s/%s", getenv("XDG_CONFIG_HOME"), fname);

        if (access(path, F_OK) != 0) {
            createConfig(path);
        }

        return strdup(path);
    } else if (getenv("HOME") != 0) {
        char path[255];
        snprintf(path, sizeof(path), "%s/%s", getenv("HOME"), fname);

        if (access(path, F_OK) != 0) {
            createConfig(path);
        }

        return strdup(path);
    }

    createConfig(fname);
    return strdup(fname);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printUsage();
        exit(1);
    }

    char* configPath = findConfigPath();
    toml_result_t result = toml_parse_file_ex(configPath);
    free(configPath);

    Project project = getProject(result, argv[1]);

    if (!tmux_has_session(project.name)) {
        tmux_new_session(project.name, argv[1]);

        if (project.found) {
            char* firstWindow;
            for (int i = 0; i < project.tabCount; i++) {
                if (i == 0) {
                    firstWindow = project.tabs[i].name;
                    tmux_rename_window(project.name, project.tabs[i].name);
                } else {
                    tmux_new_window(project.name, project.tabs[i].name, argv[1]);
                }

                if (strlen(project.tabs[i].script) > 0) {
                    tmux_run_command(project.name, project.tabs[i].name, project.tabs[i].script);
                }
            }

            tmux_select_window(project.name, firstWindow);
        }
    }

    tmux_attach_session(project.name);
}
