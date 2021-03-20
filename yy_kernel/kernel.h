
/*
 * void syscall(int code, void* pin, int insize, void* pout, int outsize);
 */

typedef
struct {
  int code;
  void* pin;
  int insize;
  void* pout;
  int outsize;
}__attribute__((packed))  SysCall_Param;


