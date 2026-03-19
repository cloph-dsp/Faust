// program: MNSpectralBlurring
// language: x86:LE:64:default
// requested_offset: 0xd433
// matched_address: 0000d433
// function_name: FUN_0000d433


void FUN_0000d433(long param_1,long param_2,undefined8 param_3,undefined8 param_4,long param_5,
                 uint param_6,int param_7)

{
  long lVar1;
  uint uVar2;
  ulong uVar3;
  ulong uVar4;
  long lVar5;
  int iVar6;
  
  uVar2 = param_7 - param_6;
  do {
    uVar2 = uVar2 + param_6;
  } while ((int)uVar2 < 0);
  uVar3 = (ulong)uVar2 % (ulong)param_6;
  if ((int)uVar3 < 1) {
    uVar3 = (ulong)param_6;
    _vDSP_vmul(param_1,1,param_2,1,param_5,1,uVar3);
    uVar4 = uVar3;
  }
  else {
    iVar6 = param_6 - (int)uVar3;
    lVar1 = param_5 + uVar3 * 4;
    lVar5 = (long)iVar6;
    _vDSP_vmul(param_1,1,param_2,1,lVar1,1,lVar5);
    _vDSP_vmul(param_1 + lVar5 * 4,1,lVar5 * 4 + param_2,1,param_5,1,uVar3);
    _vDSP_vma(param_1 + (ulong)param_6 * 4,1,(ulong)param_6 * 4 + param_2,1,lVar1,1,lVar1,1,lVar5);
    uVar4 = (long)(int)(iVar6 + param_6);
  }
  _vDSP_vma(param_1 + uVar4 * 4,1,uVar4 * 4 + param_2,1,param_5,1,param_5,1,uVar3);
  return;
}

