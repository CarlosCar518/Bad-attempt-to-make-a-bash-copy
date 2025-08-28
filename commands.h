#ifndef COMMANDS_H
#define COMMANDS_H

#define STOP 0
#define RUNNING 1
#define SUCCESS 0

typedef struct {
    char* buff;
    int state;
}flow_struct;

typedef void (*cmd_fn)(void);

void command_exit();
void command_print_arg();
void command_remove();
void command_print_serie();
void command_print_message();


extern flow_struct line;

#endif 