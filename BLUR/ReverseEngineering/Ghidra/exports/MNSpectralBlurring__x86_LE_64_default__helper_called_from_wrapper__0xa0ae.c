// program: MNSpectralBlurring
// language: x86:LE:64:default
// requested_offset: 0xa0ae
// matched_address: 0000a0ae
// function_name: FUN_0000a0ae


undefined4 FUN_0000a0ae(long param_1,uint param_2)

{
  ulong uVar1;
  long lVar2;
  undefined4 *puVar3;
  long lVar4;
  ulong uVar5;
  bool bVar6;
  
  if (*(char *)(param_1 + 0x28) == '\0') {
    lVar4 = *(long *)(param_1 + 0x18);
    if (lVar4 != 0) {
      lVar2 = param_1 + 0x18;
      do {
        bVar6 = *(uint *)(lVar4 + 0x1c) < param_2;
        if (!bVar6) {
          lVar2 = lVar4;
        }
        lVar4 = *(long *)(lVar4 + (ulong)bVar6 * 8);
      } while (lVar4 != 0);
      if ((lVar2 != param_1 + 0x18) && (*(uint *)(lVar2 + 0x1c) <= param_2)) {
        lVar2 = lVar2 + 0x20;
        goto LAB_0000a119;
      }
    }
  }
  else {
    uVar1 = (ulong)param_2;
    uVar5 = (*(long *)(param_1 + 0x38) - *(long *)(param_1 + 0x30) >> 3) * -0x5555555555555555;
    if (uVar1 <= uVar5 && uVar5 - uVar1 != 0) {
      lVar2 = *(long *)(param_1 + 0x30) + uVar1 * 0x18;
LAB_0000a119:
      return *(undefined4 *)(lVar2 + 0xc);
    }
  }
  puVar3 = (undefined4 *)___cxa_allocate_exception(4);
  *puVar3 = 0xffffd582;
                    /* WARNING: Subroutine does not return */
  ___cxa_throw(puVar3,PTR_typeinfo_00014038,0);
}

