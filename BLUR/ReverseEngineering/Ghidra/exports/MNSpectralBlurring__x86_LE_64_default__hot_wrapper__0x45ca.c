// program: MNSpectralBlurring
// language: x86:LE:64:default
// requested_offset: 0x45ca
// matched_address: 000045ca
// function_name: FUN_000045ca


void FUN_000045ca(long param_1,undefined4 param_2)

{
  long *plVar1;
  undefined8 uVar2;
  
  plVar1 = *(long **)(param_1 + 0x50);
  if (plVar1 == (long *)0x0) {
    if (*(undefined8 **)(param_1 + 0x38) < *(undefined8 **)(param_1 + 0x40)) {
      uVar2 = **(undefined8 **)(param_1 + 0x38);
    }
    else {
      uVar2 = 0;
    }
  }
  else {
    uVar2 = (**(code **)(*plVar1 + 0x20))(plVar1,0);
  }
  FUN_0000a0ae(uVar2,param_2);
  return;
}

