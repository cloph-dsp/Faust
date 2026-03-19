// program: MNSpectralBlurring
// language: x86:LE:64:default
// requested_offset: 0xd30a
// matched_address: 0000d30a
// function_name: FUN_0000d30a


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0000d30a(undefined8 param_1,undefined8 param_2,undefined4 param_3,uint param_4)

{
  double dVar1;
  float local_28;
  float local_24;
  
  _vDSP_hann_window(param_2,param_3,0);
  _vDSP_hann_window(param_1,param_3,0);
  local_24 = (float)(DAT_00012f58 / (double)param_4);
  dVar1 = (double)_log2();
  _local_28 = CONCAT44(local_24,(float)((double)local_24 * ((double)(float)dVar1 / _DAT_00013060)));
  _vDSP_vsmul(param_1,1,&local_28,param_1,1,param_3);
  return;
}

