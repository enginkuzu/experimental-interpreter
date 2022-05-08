#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>

extern void stage2(int counter, int indexes[10]);
extern void stage3();

char input[] = "\n\
fn fib locals 8\n\
value 2 to 0\n\
compare_less in 0 to 1\n\
if 1 elseskip 2\n\
copy in out\n\
return\n\
value 1 to 2\n\
value 2 to 3\n\
call - in 2 to 4\n\
call - in 3 to 5\n\
call fib 4 to 6\n\
call fib 5 to 7\n\
call + 6 7 to out\n\
return\n\
\n\
fn main locals 2\n\
value 37 to 0\n\
call fib 0 to 1\n\
call print 1\n\
end\n\
";

long current_timestamp(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec*1000LL + tv.tv_usec/1000;
}

int64_t fib(int64_t value){
  if( value < 2 ) return value;
  return fib(value-1)+fib(value-2);
}

int main(){

  int si = 0;
  int param_counter = 0;
  int param_indexes[10];
  int length = strlen(input);
  for(int ci=0;ci<length;ci++){
    char c = input[ci];
    if( c == ' ' ){
      input[ci] = 0;
      param_indexes[param_counter++] = si;
      si = ci + 1;
    } else if( c == '\n' ){
      input[ci] = 0;
      param_indexes[param_counter++] = si;
      si = ci + 1;
      //
      stage2(param_counter, param_indexes);
      param_counter = 0;
    }
  }

  long t1 = current_timestamp();
  stage3();
  long t2 = current_timestamp();
  long delta1 = t2 - t1;
  printf("Interpreter runs in %ld ms\n\n", delta1);

  long t3 = current_timestamp();
  int64_t result = fib(37);
  long t4 = current_timestamp();
  long delta2 = t4 - t3;
  printf("fib(37) = %ld\n", result);
  printf("Real C function runs in %ld ms\n\n", delta2);

  printf("Interpreter %ldx slow!\n\n", delta1/delta2);

  return 0;
}
