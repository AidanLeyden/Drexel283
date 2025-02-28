#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if(strlen(cmd_line) > SH_CMD_MAX) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    char temp_cmd[SH_CMD_MAX];
    int command_count = 0;
    char *cmd_ptr = cmd_line;
    while (sscanf(cmd_ptr, "%[^|]", temp_cmd) == 1) {
        if(command_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
        command_t command;
        memset(&command, 0, sizeof(command_t));
        char *exe = strtok(temp_cmd, " \t");
        if(exe == NULL) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        if(strlen(exe) > EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        strcpy(command.exe, exe);
        char *remaining = strtok(NULL, "");
        if(remaining != NULL) {
            while(*remaining == ' ' || *remaining == '\t') remaining++;
            if(strlen(remaining) > ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            strcpy(command.args, remaining);
        }
        clist->commands[command_count++] = command;
        clist->num = command_count;
        cmd_ptr = strchr(cmd_ptr, '|');
        if(cmd_ptr == NULL) break;
        cmd_ptr++;
        while(*cmd_ptr == ' ' || *cmd_ptr == '\t') cmd_ptr++;
    }

    return OK;
}