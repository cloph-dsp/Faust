// program: MNSpectralBlurring
// language: x86:LE:64:default
// requested_offset: 0xdf77
// matched_address: 0000df77
// function_name: FUN_0000df77


void FUN_0000df77(void *param_1,void *param_2,int param_3,uint param_4)

{
  size_t sVar1;
  
  sVar1 = (ulong)param_4 * 4;
  _memcpy(param_1,param_2,sVar1);
  _memmove(param_2,(void *)((long)param_2 + (ulong)param_4 * 4),(ulong)(param_3 - param_4) * 4);
  ___bzero((void *)((long)param_2 + (ulong)(param_3 - param_4) * 4),sVar1);
  return;
}

