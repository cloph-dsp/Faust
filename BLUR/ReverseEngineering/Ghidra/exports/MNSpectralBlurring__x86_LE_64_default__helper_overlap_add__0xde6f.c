// program: MNSpectralBlurring
// language: x86:LE:64:default
// requested_offset: 0xde6f
// matched_address: 0000de6f
// function_name: FUN_0000de6f


void FUN_0000de6f(long param_1,undefined8 param_2,uint param_3,long param_4,long param_5,int param_6
                 ,int param_7)

{
  long lVar1;
  ulong uVar2;
  uint uVar3;
  int iVar4;
  ulong uVar5;
  
  uVar3 = param_7 - param_3;
  do {
    uVar3 = uVar3 + param_3;
  } while ((int)uVar3 < 0);
  uVar5 = 0;
  uVar2 = (ulong)uVar3 % (ulong)param_3;
  iVar4 = (int)uVar2;
  if (param_3 - iVar4 != 0) {
    uVar5 = (ulong)(param_3 - iVar4);
    _vDSP_vma(param_1 + uVar2 * 4,1,param_4,1,param_5,1,param_5,1,uVar5);
  }
  lVar1 = param_5 + uVar5 * 4;
  _vDSP_vma(param_1,1,param_4 + uVar5 * 4,1,lVar1,1,lVar1,1,param_3);
  if (0 < iVar4) {
    param_5 = param_5 + (ulong)(uint)(param_6 - iVar4) * 4;
    _vDSP_vma(param_1,1,param_4 + (ulong)(uint)(param_6 - iVar4) * 4,1,param_5,1,param_5,1,uVar2);
  }
  return;
}

