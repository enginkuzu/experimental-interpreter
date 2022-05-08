#include <stdio.h>
#include "stage2.h"

struct function *fn = NULL;

static int64_t stack[16*1024];

void stage3(){

  int64_t sp = 0;
  int64_t cmd_index = 0;

  while(1){

    int64_t *cmd = fn->cmds + cmd_index;
    //printf("RUN : fn %s, cmd_index %ld, cmd %ld\n", fn->name, cmd_index, cmd);

    switch(*cmd){

      case CMD_END:

        goto end;

      case CMD_VALUE:

        {
          int64_t value = *++cmd;
          int64_t i1 = *++cmd;
          cmd_index += 3;

          stack[sp+i1] = value;
        }
        break;

      case CMD_CALL:

        {
          struct function *new_fn = (struct function *)*++cmd;
          int64_t in = *++cmd;
          //int64_t out = *++cmd;
          cmd_index += 4;

          stack[sp+fn->locals+0] = (int64_t)fn;   // -5 : old fn
          stack[sp+fn->locals+1] = cmd_index;     // -4 : old cmd_index
          stack[sp+fn->locals+2] = sp;            // -3 : old sp
          stack[sp+fn->locals+3] = stack[sp+in];  // -2 : in
          stack[sp+fn->locals+4] = 0;             // -1 : out

          sp += fn->locals+5;
          cmd_index = 0;
          fn = new_fn;
        }
        break;

      case CMD_RETURN:

        {
          //cmd_index += 1;

          fn        = (struct function *)stack[sp-5];
          cmd_index = stack[sp-4];
          sp        = stack[sp-3];

          int64_t out = fn->cmds[cmd_index-1];
          stack[sp+out] = stack[sp+fn->locals+4];
        }
        break;

      case CMD_CMP_LESS:

        {
          int64_t i1 = *++cmd;
          int64_t i2 = *++cmd;
          int64_t i3 = *++cmd;
          cmd_index += 4;

          stack[sp+i3] = stack[sp+i1] < stack[sp+i2];
        }
        break;

      case CMD_IF:

        {
          int64_t i1 = *++cmd;
          int64_t skip_value = *++cmd;
          cmd_index += 3;

          if( stack[sp+i1] ){
            ;
          } else {
            cmd_index += skip_value;
          }
        }
        break;

      case CMD_ADD:

        {
          int64_t i1 = *++cmd;
          int64_t i2 = *++cmd;
          int64_t i3 = *++cmd;
          cmd_index += 4;

          stack[sp+i3] = stack[sp+i1] + stack[sp+i2];
        }
        break;

      case CMD_SUB:

        {
          int64_t i1 = *++cmd;
          int64_t i2 = *++cmd;
          int64_t i3 = *++cmd;
          cmd_index += 4;

          stack[sp+i3] = stack[sp+i1] - stack[sp+i2];
        }
        break;

      case CMD_COPY:

        {
          int64_t i1 = *++cmd;
          int64_t i2 = *++cmd;
          cmd_index += 3;

          stack[sp+i2] = stack[sp+i1];
        }
        break;

      case CMD_PRINT:

        {
          int64_t i1 = *++cmd;
          cmd_index += 2;

          printf("fib(37) = %ld\n", stack[sp+i1]);
        }
        break;

      default:

        printf("TODO : %s %ld\n", fn->name, *cmd);
        break;

    }
  }
end:;
}
