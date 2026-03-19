// program: MNSpectralBlurring
// language: x86:LE:64:default
// requested_offset: 0xd3a7
// matched_address: 0000d3a7
// function_name: FUN_0000d3a7


void FUN_0000d3a7(float param_1,void *param_2,void *param_3,uint param_4,uint param_5)

{
  float *pfVar1;
  uint uVar2;
  ulong uVar3;
  
  uVar2 = param_4 - param_5;
  uVar3 = (ulong)uVar2;
  _memmove(param_3,(void *)((long)param_3 + (ulong)param_5 * 4),uVar3 * 4);
  _memcpy((void *)((long)param_3 + uVar3 * 4),param_2,(ulong)param_5 << 2);
  if (((param_1 != DAT_00012f08) || (NAN(param_1) || NAN(DAT_00012f08))) && (uVar2 < param_4)) {
    pfVar1 = (float *)((long)param_3 + (long)(int)uVar2 * 4);
    do {
      *pfVar1 = *pfVar1 * param_1;
      pfVar1 = pfVar1 + 1;
      uVar2 = (int)uVar3 + 1;
      uVar3 = (ulong)uVar2;
    } while (uVar2 < param_4);
  }
  return;
}

