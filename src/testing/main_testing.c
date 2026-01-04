#include "errorhandling.h"
int main(int argc, char *argv[])
{
  assert( ERROR_SUCCESS == Error_InitGlobalCtx());
  assert( ERROR_SUCCESS == Error_SetLogON());
  assert( ERROR_SUCCESS == Error_SetLogFile("./errorlog.txt"));
  ErrorStruct_t *errstrct = NULL;
  assert(ERROR_SUCCESS == Error_InitErrorStruct(&errstrct,
                                                ERROR_SUCCESS,
                                                __LINE__,
                                                __func__,
                                                __FILE_NAME__,
                                                "testing if this works"));
  assert( ERROR_SUCCESS == Error_LogError(errstrct));
  assert( ERROR_SUCCESS == Error_CleanupGlobalCtx());
  assert(ERROR_SUCCESS == Error_DestroyErrorStruct(errstrct));
  printf("All tests passed!\n");
  return ERROR_SUCCESS;
}
