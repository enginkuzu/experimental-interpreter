#include <stdlib.h>

enum cmd {
  CMD_PRINT,
  CMD_VALUE,
  CMD_ADD,
  CMD_SUB,
  CMD_COPY,
  CMD_END,
  CMD_CMP_LESS,
  CMD_IF,
  CMD_CALL,
  CMD_RETURN
};

struct function {
  char *name;
  int locals;
  int64_t cmds[100];
};
