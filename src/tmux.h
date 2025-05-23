#pragma once
#include <stdbool.h>

bool tmux_has_session(char* sessionName);
bool tmux_kill_session(char* sessionName);
bool tmux_new_session(char* sessionName, char* pwd);
bool tmux_rename_window(char* sessionName, char* windowName);
bool tmux_new_window(char* sessionName, char* windowName, char* pwd);
bool tmux_select_window(char* sessionName, char* windowName);
bool tmux_attach_session(char* sessionName);
bool tmux_run_command(char* sessionName, char* windowName, char* command);
