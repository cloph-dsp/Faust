// program: MNSpectralBlurring
// language: x86:LE:64:default
// requested_offset: 0xd2a7
// matched_address: 0000d2a7
// function_name: FUN_0000d2a7


void FUN_0000d2a7(uint param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  
  *(undefined4 *)(param_2 + 0x50) = 0xffffffff;
  *(undefined2 *)(param_2 + 0x54) = 0x101;
  *(undefined1 *)(param_2 + 0x56) = 1;
  lVar1 = *(long *)(param_2 + 0x28);
  lVar2 = *(long *)(param_2 + 0x20);
  lVar3 = *(long *)(param_2 + 0x18);
  lVar4 = *(long *)(param_2 + 8);
  lVar5 = *(long *)(param_2 + 0x10);
  lVar6 = 0;
  do {
    *(undefined4 *)(lVar1 + lVar6 * 4) = 0;
    *(undefined4 *)(lVar2 + lVar6 * 4) = 0;
    *(undefined4 *)(lVar3 + lVar6 * 4) = 0;
    *(undefined4 *)(lVar5 + lVar6 * 4) = 0;
    *(undefined4 *)(lVar4 + lVar6 * 4) = 0;
    lVar6 = lVar6 + 1;
  } while ((ulong)param_1 + 1 != lVar6);
  return;
}

