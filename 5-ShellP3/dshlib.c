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

        command_list_t cmd_list;
        rc = build_cmd_list(cmd_buff, &cmd_list);
        if (rc != OK) {
            if (rc == ERR_TOO_MANY_COMMANDS) {
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            }
            continue;
        }
        if (cmd_list.num == 0) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }
        if (strcmp(cmd_list.commands[0].argv[0], EXIT_CMD) == 0) {
            break;
        }
        if (strcmp(cmd_list.commands[0].argv[0], "cd") == 0) {
            if (cmd_list.commands[0].argc > 1) {
                chdir(cmd_list.commands[0].argv[1]);
            }
            continue;
        }

        int pipes[CMD_MAX-1][2];
        for (int i = 0; i < cmd_list.num - 1; i++) {
            if (pipe(pipes[i]) < 0) {
                continue;
            }
        }
        pid_t pids[CMD_MAX];
        for (int i = 0; i < cmd_list.num; i++) {
            pids[i] = fork();
            if (pids[i] < 0) {
                continue;
            }
            else if (pids[i] == 0) {
                if (i > 0) {
                    dup2(pipes[i-1][0], 0);
                }
                if (i < cmd_list.num - 1) {
                    dup2(pipes[i][1], 1);
                }
                for (int j = 0; j < cmd_list.num - 1; j++) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                execvp(cmd_list.commands[i].argv[0], cmd_list.commands[i].argv);
                fprintf(stderr, "error: %s: command not found\n", cmd_list.commands[i].argv[0]);
                _exit(1);
            }
        }
        for (int i = 0; i < cmd_list.num - 1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
        int status;
        for (int i = 0; i < cmd_list.num; i++) {
            waitpid(pids[i], &status, 0);
        }

        free_cmd_list(&cmd_list);
    }

    free(cmd_buff);
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

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (strlen(cmd_line) > SH_CMD_MAX) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    size_t len = strlen(cmd_line);
    if (cmd_line[0] == '|' || (len > 0 && cmd_line[len-1] == '|')) {
        fprintf(stderr, "error: incorrect pipe formatting\n");
        return WARN_NO_CMDS;
    }
    
    memset(clist, 0, sizeof(command_list_t));
    char temp_cmd[SH_CMD_MAX];
    bool in_quotes = false;
    int cmd_start = 0;
    
    for (int i = 0; cmd_line[i] != '\0'; i++) {
        if (cmd_line[i] == '"') {
            in_quotes = !in_quotes;
        }
        else if (cmd_line[i] == '|' && !in_quotes) {
            int cmd_len = i - cmd_start;
            strncpy(temp_cmd, cmd_line + cmd_start, cmd_len);
            temp_cmd[cmd_len] = '\0';
            
            if (clist->num >= CMD_MAX) {
                return ERR_TOO_MANY_COMMANDS;
            }
            
            cmd_buff_t cmd;
            int rc = alloc_cmd_buff(&cmd);
            if (rc != OK) {
                return rc;
            }
            
            rc = build_cmd_buff(temp_cmd, &cmd);
            if (rc != OK || cmd.argc == 0) {
                free_cmd_buff(&cmd);
                return WARN_NO_CMDS;
            }
            
            clist->commands[clist->num++] = cmd;
            cmd_start = i + 1;
        }
    }
    
    if (cmd_start < (int)strlen(cmd_line)) {
        strncpy(temp_cmd, cmd_line + cmd_start, SH_CMD_MAX - 1);
        temp_cmd[SH_CMD_MAX - 1] = '\0';
        
        cmd_buff_t cmd;
        int rc = alloc_cmd_buff(&cmd);
        if (rc != OK) {
            return rc;
        }
        
        rc = build_cmd_buff(temp_cmd, &cmd);
        if (rc != OK || cmd.argc == 0) {
            free_cmd_buff(&cmd);
            return WARN_NO_CMDS;
        }
        
        clist->commands[clist->num++] = cmd;
    }
    
    if (clist->num == 0) {
        return WARN_NO_CMDS;
    }
    
    return OK;
}

int free_cmd_list(command_list_t *cmd_lst)
{
    if(cmd_lst == NULL) {
        return ERR_MEMORY;
    }
    for(int i = 0; i < cmd_lst->num; i++) {
        free_cmd_buff(&cmd_lst->commands[i]);
    }
    return OK;
}