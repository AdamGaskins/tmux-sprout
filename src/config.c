#include "config.h"
#include "common.h"

#include <_ctype.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>

char* toString(toml_datum_t string, char* name)
{
    if (string.type != TOML_STRING) {
        error("string expected: ", name);
    }

    return strdup(string.u.s);
}

bool folderMatches(const char* partialPath, const char* fullPath)
{
    size_t partialPathLen = strlen(partialPath);
    size_t fullPathLen = strlen(fullPath);

    if (fullPathLen < partialPathLen)
        return false;

    size_t diff = fullPathLen - partialPathLen;

    for (int i = 0; i < partialPathLen; ++i) {
        if (fullPath[diff + i] != partialPath[i]) {
            return false;
        }
    }

    return true;
}

char* cleanupName(const char* name)
{
    int len = strlen(name);

    char* c = calloc(len, 1);
    int ci = 0;

    for (int i = 0; i < len; i++) {
        if (
            name[i] == 32 || // space
            name[i] == 45 || // dash
            name[i] == 95 || // underscore
            (name[i] >= '0' && name[i] <= '9') || (name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z')) {
            c[ci] = name[i];
            ci++;
        }
    }

    return c;
}

Project getProject(toml_result_t config, char* projectPath)
{
    if (!config.ok) {
        error(config.errmsg, 0);
    }

    toml_datum_t projects = toml_get(config.toptab, "projects");
    if (projects.type != TOML_ARRAY) {
        error("'projects' array expected.", 0);
    }

    for (int i = 0; i < projects.u.arr.size; i++) {
        toml_datum_t project = projects.u.arr.elem[i];

        if (project.type != TOML_TABLE) {
            continue;
        }

        toml_datum_t projectFolder = toml_get(project, "folder");
        if (!folderMatches(projectFolder.u.s, projectPath)) {
            continue;
        }

        toml_datum_t tabConfig = toml_get(project, "tabs");

        size_t tabCount = 0;
        ProjectTab* tabs = calloc(100, sizeof(*tabs));
        for (; tabCount < tabConfig.u.tab.size; tabCount++) {
            tabs[tabCount].name = strdup(tabConfig.u.tab.key[tabCount]);
            tabs[tabCount].script = toString(tabConfig.u.tab.value[tabCount], "project value");
        }

        return (Project) {
            .found = true,
            .name = cleanupName(toString(toml_get(project, "name"), "name")),
            .folder = toString(projectFolder, "folder"),
            .tabs = tabs,
            .tabCount = tabCount
        };
    }

    return (Project) { .found = false, .name = cleanupName(basename(projectPath)) };
}
