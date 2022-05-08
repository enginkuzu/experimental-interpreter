#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stage2.h"

extern char input[];
extern struct function *fn;

static int fn_counter = 0;
static struct function *fn_list[10];

static struct function *active_fn;
static int active_fn_cmd_index;

static int skip_value = 0;
static int skip_start_index;
static int skip_param_index;

int str_to_int(int index){
  if( strcmp(input+index, "in") == 0 ){
    return -2;
  } else if( strcmp(input+index, "out") == 0 ){
    return -1;
  }
  return atoi(input+index);
}

void stage2(int counter, int indexes[10]){

  if( skip_value > 0 ){
    skip_value--;
    if( skip_value == 0 ){
      active_fn->cmds[skip_param_index] = active_fn_cmd_index - skip_start_index;
    }
  }

  if( counter == 4 && strcmp(input+indexes[0], "fn") == 0 && strcmp(input+indexes[2], "locals") == 0 ){
    // 4 : [fn] [fib] [locals] [8]
    active_fn = (struct function *)malloc(sizeof(struct function));
    active_fn->name = input+indexes[1];
    active_fn->locals = atoi(input+indexes[3]);
    active_fn_cmd_index = 0;
    fn_list[fn_counter++] = active_fn;
    if( strcmp(input+indexes[1], "main") == 0 ) fn = active_fn;
  } else if( counter == 3 && strcmp(input+indexes[0], "call") == 0 && strcmp(input+indexes[1], "print") == 0 ){
    // 3 : [call] [print] [1]
    active_fn->cmds[active_fn_cmd_index++] = CMD_PRINT;
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[2]);
  } else if( counter == 4 && strcmp(input+indexes[0], "value") == 0 && strcmp(input+indexes[2], "to") == 0 ){
    // 4 : [value] [123] [to] [0]
    active_fn->cmds[active_fn_cmd_index++] = CMD_VALUE;
    active_fn->cmds[active_fn_cmd_index++] = atoi(input+indexes[1]);
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[3]);
  } else if( counter == 6 && strcmp(input+indexes[0], "call") == 0 && strcmp(input+indexes[1], "+") == 0 && strcmp(input+indexes[4], "to") == 0 ){
    // 6 : [call] [+] [6] [7] [to] [out]
    active_fn->cmds[active_fn_cmd_index++] = CMD_ADD;
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[2]);
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[3]);
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[5]);
  } else if( counter == 6 && strcmp(input+indexes[0], "call") == 0 && strcmp(input+indexes[1], "-") == 0 && strcmp(input+indexes[4], "to") == 0 ){
    // 6 : [call] [-] [in] [2] [to] [4]
    active_fn->cmds[active_fn_cmd_index++] = CMD_SUB;
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[2]);
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[3]);
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[5]);
  } else if( counter == 3 && strcmp(input+indexes[0], "copy") == 0 ){
    // 3 : [copy] [in] [out]
    active_fn->cmds[active_fn_cmd_index++] = CMD_COPY;
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[1]);
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[2]);
  } else if( counter == 1 && strcmp(input+indexes[0], "end") == 0 ){
    // 1 : [end]
    active_fn->cmds[active_fn_cmd_index++] = CMD_END;
  } else if( counter == 5 && strcmp(input+indexes[0], "compare_less") == 0 && strcmp(input+indexes[3], "to") == 0 ){
    // 5 : [compare_less] [in] [0] [to] [1]
    active_fn->cmds[active_fn_cmd_index++] = CMD_CMP_LESS;
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[1]);
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[2]);
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[4]);
  } else if( counter == 4 && strcmp(input+indexes[0], "if") == 0 && strcmp(input+indexes[2], "elseskip") == 0 ){
    // 4 : [if] [1] [elseskip] [2]
    active_fn->cmds[active_fn_cmd_index++] = CMD_IF;
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[1]);
    //active_fn->cmds[active_fn_cmd_index++] = SKIP_IN_BYTES; // to be determined in the future iterations
    skip_value = atoi(input+indexes[3]) + 1;
    skip_start_index = active_fn_cmd_index + 1;
    skip_param_index = active_fn_cmd_index;
    active_fn_cmd_index++;
  } else if( counter == 5 && strcmp(input+indexes[0], "call") == 0 && strcmp(input+indexes[3], "to") == 0 ){
    // 5 : [call] [fib] [4] [to] [6]
    void *target = NULL;
    for(int i=0;i<fn_counter;i++){
      if( strcmp(input+indexes[1], fn_list[i]->name) == 0 ){
        target = fn_list[i];
        break;
      }
    }
    active_fn->cmds[active_fn_cmd_index++] = CMD_CALL;
    active_fn->cmds[active_fn_cmd_index++] = (int64_t)target;
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[2]);
    active_fn->cmds[active_fn_cmd_index++] = str_to_int(indexes[4]);
  } else if( counter == 1 && strcmp(input+indexes[0], "return") == 0 ){
    // 1 : [return]
    active_fn->cmds[active_fn_cmd_index++] = CMD_RETURN;
  } else {
    //printf("%d : ", counter);
    //for(int i=0;i<counter;i++) printf("[%s] ", input+indexes[i]);
    //printf("\n");
  }
}
