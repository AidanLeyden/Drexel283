#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(ARG_MAX);
    if (cmd_buff == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    int rc = 0;
    cmd_buff_t cmd;

    rc = alloc_cmd_buff(&cmd);
    if(rc != OK) {
        return rc;
    }
    // TODO IMPLEMENT MAIN LOOP

    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

        rc = build_cmd_buff(cmd_buff, &cmd);
        if(rc == OK) {
            if(cmd.argc == 0) {
                printf(CMD_WARN_NO_CMD);
                continue;
            }
        }
        else if(rc == ERR_CMD_OR_ARGS_TOO_BIG) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        }
        if(strcmp(cmd.argv[0], EXIT_CMD) == 0) {
            break;
        }
        if(strcmp(cmd.argv[0], "cd") == 0) {
            if(cmd.argc == 1) {
                continue;
            } else {
                chdir(cmd.argv[1]);
                continue;
            }
        }
        pid_t pid = fork();
        if(pid < 0) {
            continue;
        }
        else if(pid == 0) {
            execvp(cmd.argv[0], cmd.argv);
            _exit(1);
        }
        else {
            int status;
            wait(&status);
            continue;
        }
    }

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    return OK;
}

int alloc_cmd_buff(cmd_buff_t *cmd_buff)
{
    if(cmd_buff == NULL) {
        return ERR_MEMORY;
    }
    cmd_buff->argc = 0;
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if(cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }
    for(int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    if (strlen(cmd_line) > SH_CMD_MAX) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    cmd_buff->argc = 0;
    strcpy(cmd_buff->_cmd_buffer, cmd_line);
    
    char *current = cmd_buff->_cmd_buffer;
    while (*current && isspace(*current)) current++;
    if (*current == '\0') {
        return WARN_NO_CMDS;
    }

    char *start = current;
    bool in_quotes = false;
    while (*current != '\0' && cmd_buff->argc < CMD_ARGV_MAX - 1) {
        if (*current == '"') {
            in_quotes = !in_quotes;
            if (!in_quotes) {
                *current = '\0';
                if (strlen(start) > ARG_MAX) {
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                }
                cmd_buff->argv[cmd_buff->argc++] = start;
                start = current + 1;
            } else {
                start = current + 1;
            }
        } else if (isspace(*current) && !in_quotes) {
            *current = '\0';
            if (start != current) {
                if (cmd_buff->argc == 0 && strlen(start) > EXE_MAX) {
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                } else if (strlen(start) > ARG_MAX) {
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                }
                cmd_buff->argv[cmd_buff->argc++] = start;
            }
            start = current + 1;
        }
        current++;
    }
    if (start < current && *start != '\0') {
        if (cmd_buff->argc == 0 && strlen(start) > EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        } else if (strlen(start) > ARG_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        cmd_buff->argv[cmd_buff->argc++] = start;
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff)
{
    if(cmd_buff == NULL) {
        return ERR_MEMORY;
    }
    free(cmd_buff->_cmd_buffer);
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff)
{
    if(cmd_buff == NULL) {
        return ERR_MEMORY;
    }
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    for(int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    cmd_buff->argc = 0;
    return OK;
}