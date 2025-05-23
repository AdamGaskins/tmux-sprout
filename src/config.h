#pragma once
#include "tomlc17.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* name;
    char* script;
} ProjectTab;

typedef struct {
    bool found;
    char* folder;
    char* name;
    ProjectTab* tabs;
    size_t tabCount;
} Project;

Project getProject(toml_result_t config, char* projectName);
