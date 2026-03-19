// program: MNSpectralBlurring
// language: x86:LE:64:default
// requested_offset: 0x427e
// matched_address: 0000427e
// function_name: FUN_00003766


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_00003766(long *param_1,long param_2,void *param_3,uint param_4)

{
  long *plVar1;
  undefined4 *puVar2;
  double dVar3;
  double dVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  void *pvVar8;
  void *pvVar9;
  long lVar10;
  ulong uVar11;
  uint uVar12;
  ulong uVar13;
  uint uVar14;
  char *pcVar15;
  ulong uVar16;
  size_t sVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  undefined8 uVar22;
  float fVar23;
  float fVar24;
  ulong local_68;
  ulong local_50;
  float local_48;
  float local_44;
  float local_40;
  float local_3c;
  float local_38;
  float local_34;
  
  uVar11 = (ulong)param_4;
  local_3c = (float)FUN_000045ca(param_1[1],8);
  fVar18 = (float)FUN_000045ca(param_1[1],7);
  uVar7 = *(uint *)(&DAT_00012f60 + ((long)fVar18 & 0xffffffffU) * 4);
  uVar16 = (ulong)uVar7;
  uVar6 = uVar7 >> 1;
  local_40 = (float)FUN_000045ca(param_1[1],4);
  fVar18 = (float)FUN_000045ca(param_1[1],5);
  local_44 = (float)FUN_000045ca(param_1[1],6);
  local_38 = (float)FUN_000045ca(param_1[1],0);
  fVar19 = (float)FUN_000045ca(param_1[1],1);
  local_34 = (float)FUN_000045ca(param_1[1],2);
  fVar20 = (float)FUN_000045ca(param_1[1]);
  dVar3 = (double)FUN_0000cbce(param_1[1]);
  if ((local_3c != *(float *)(param_1 + 0x1b)) || (NAN(local_3c) || NAN(*(float *)(param_1 + 0x1b)))
     ) {
    dVar4 = (double)_pow((int)DAT_00012f40,(double)(local_3c / _DAT_00012f24));
    *(float *)(param_1 + 0x22) = (float)dVar4;
    *(float *)(param_1 + 0x1b) = local_3c;
  }
  if (*(uint *)(param_1 + 5) != uVar7) {
    *(undefined8 *)((long)param_1 + 0x14) = 0;
    *(undefined8 *)((long)param_1 + 0x1c) = 0;
    (**(code **)(*param_1 + 0x38))(param_1);
    *(undefined4 *)((long)param_1 + 0x9c) = 1;
    *(undefined4 *)((long)param_1 + 0xa4) = 0;
    dVar4 = (double)FUN_0000cbce(param_1[1]);
    *(int *)(param_1 + 0x14) = (int)(long)dVar4;
    uVar14 = uVar7 >> 2;
    *(uint *)(param_1 + 6) = uVar14;
    *(uint *)((long)param_1 + 0x2c) = uVar14;
    uVar13 = (ulong)(uVar7 * 2);
    *(uint *)((long)param_1 + 0x34) = uVar7 * 2;
    *(uint *)(param_1 + 8) = uVar7 * -2;
    *(uint *)((long)param_1 + 0x3c) = uVar7 * -2;
    *(uint *)(param_1 + 7) = uVar6;
    sVar17 = (ulong)uVar6 + 1;
    *(uint *)(param_1 + 0x15) = (uint)sVar17;
    *(undefined4 *)((long)param_1 + 0x94) = 0;
    *(undefined1 *)((long)param_1 + 0x114) = 0;
    *(uint *)((long)param_1 + 0x24) = uVar7 * 3;
    pvVar8 = _calloc(uVar13,4);
    param_1[0x1c] = (long)pvVar8;
    pvVar8 = _calloc(uVar13,4);
    param_1[0x1d] = (long)pvVar8;
    pvVar8 = _calloc(uVar13,4);
    param_1[0x1e] = (long)pvVar8;
    pvVar8 = _calloc(uVar13,4);
    param_1[0x1f] = (long)pvVar8;
    pvVar8 = _calloc(uVar13,4);
    param_1[0x21] = (long)pvVar8;
    pvVar8 = _calloc(uVar16,4);
    param_1[0x20] = (long)pvVar8;
    pvVar8 = _calloc(1,0x58);
    param_1[0x23] = (long)pvVar8;
    pvVar9 = _calloc(sVar17,4);
    *(void **)((long)pvVar8 + 8) = pvVar9;
    pvVar9 = _calloc(sVar17,4);
    *(void **)((long)pvVar8 + 0x10) = pvVar9;
    pvVar9 = _calloc(sVar17,4);
    *(void **)((long)pvVar8 + 0x18) = pvVar9;
    pvVar9 = _calloc(sVar17,4);
    *(void **)((long)pvVar8 + 0x20) = pvVar9;
    pvVar9 = _calloc(sVar17,4);
    *(void **)((long)pvVar8 + 0x28) = pvVar9;
    pvVar9 = _calloc(0x2000,4);
    *(void **)((long)pvVar8 + 0x30) = pvVar9;
    pvVar8 = _calloc((ulong)uVar14,4);
    param_1[0x16] = (long)pvVar8;
    pvVar9 = _calloc(uVar16,4);
    param_1[0x17] = (long)pvVar9;
    pvVar9 = _calloc((ulong)(uVar7 * 3),4);
    param_1[0x18] = (long)pvVar9;
    dVar4 = (double)_log2(SUB84((double)uVar16,0));
    *(int *)(param_1 + 0x12) = (int)(long)dVar4;
    *(undefined4 *)(param_1 + 0x13) = 1;
    pvVar9 = _calloc(sVar17,4);
    param_1[9] = (long)pvVar9;
    pvVar9 = _calloc(sVar17,4);
    param_1[10] = (long)pvVar9;
    pvVar9 = _calloc(sVar17,4);
    param_1[0xb] = (long)pvVar9;
    pvVar9 = _calloc(sVar17,4);
    param_1[0xc] = (long)pvVar9;
    pvVar9 = _calloc(1,4);
    param_1[0xd] = (long)pvVar9;
    pvVar9 = _calloc(1,4);
    param_1[0xe] = (long)pvVar9;
    pvVar9 = _calloc(sVar17,4);
    param_1[0xf] = (long)pvVar9;
    pvVar9 = _calloc(sVar17,4);
    param_1[0x10] = (long)pvVar9;
    pvVar9 = _calloc((ulong)((uint)sVar17 >> 1),4);
    param_1[0x19] = (long)pvVar9;
    if ((int)param_1[0x15] != 0) {
      dVar4 = *(double *)PTR__pi_00014088;
      uVar13 = 0;
      do {
        iVar5 = _rand();
        uVar22 = ___sincosf_stret((float)((double)((float)iVar5 * _DAT_00012f28) * (dVar4 + dVar4)))
        ;
        *(int *)(param_1[0xf] + uVar13 * 4) = (int)((ulong)uVar22 >> 0x20);
        *(int *)(param_1[0x10] + uVar13 * 4) = (int)uVar22;
        uVar13 = uVar13 + 1;
      } while (uVar13 < *(uint *)(param_1 + 0x15));
      pvVar8 = (void *)param_1[0x16];
    }
    if ((((pvVar8 == (void *)0x0) || (param_1[0x17] == 0)) || (param_1[9] == 0)) ||
       (param_1[10] == 0)) {
      pcVar15 = "\nmalloc failed to allocate memory for the real FFT section of the sample.";
LAB_000043e0:
      _puts(pcVar15);
                    /* WARNING: Subroutine does not return */
      _exit(0);
    }
    lVar10 = _vDSP_create_fftsetup((int)param_1[0x12],0);
    param_1[0x11] = lVar10;
    if (lVar10 == 0) {
      pcVar15 = "\nFFT_Setup failed to allocate enough memory for the real FFT.";
      goto LAB_000043e0;
    }
    FUN_0000d30a(param_1[0x1c],param_1[0x1d],*(undefined4 *)((long)param_1 + 0x34),uVar16,
                 *(undefined4 *)((long)param_1 + 0x2c));
    FUN_0000d2a7((int)param_1[7]);
    *(uint *)(param_1 + 5) = uVar7;
    *(undefined4 *)((long)param_1 + 0xd4) = 0xc2c60000;
    *(undefined4 *)((long)param_1 + 0xac) = 0;
  }
  *(undefined4 *)((long)param_1 + 0x14) = 0;
  if (param_4 != 0) {
    local_50 = (ulong)((double)(local_40 * (float)uVar6) / DAT_00012f38);
    local_68 = (ulong)((double)(fVar18 * (float)uVar6) / DAT_00012f38);
    local_44 = (float)(int)local_44;
    fVar23 = local_38 + local_34;
    fVar18 = local_34 + local_34;
    plVar1 = param_1 + 9;
    uVar6 = *(uint *)(param_1 + 6);
    local_40 = 0.0;
    uVar14 = 0;
    do {
      uVar12 = uVar6 - *(uint *)((long)param_1 + 0x1c);
      if (param_4 - uVar14 < uVar12) {
        uVar12 = param_4 - uVar14;
      }
      _memcpy((void *)((ulong)*(uint *)((long)param_1 + 0x1c) * 4 + param_1[0x16]),
              (void *)(param_2 + (ulong)uVar14 * 4),(ulong)uVar12 * 4);
      uVar14 = *(int *)((long)param_1 + 0x14) + uVar12;
      *(uint *)((long)param_1 + 0x14) = uVar14;
      uVar6 = *(uint *)(param_1 + 6);
      uVar12 = (uVar12 + *(int *)((long)param_1 + 0x1c)) % uVar6;
      *(uint *)((long)param_1 + 0x1c) = uVar12;
      if (uVar12 != 0) goto LAB_00004208;
      FUN_0000d3a7(DAT_00012f08,param_1[0x16],param_1[0x1e],*(undefined4 *)((long)param_1 + 0x34));
      if ((local_34 == _DAT_00012f30) && (fVar21 = local_38, !NAN(local_34) && !NAN(_DAT_00012f30)))
      goto switchD_0000408e_default;
      fVar21 = local_40;
      switch((long)fVar19 & 0xffffffff) {
      case 0:
        fVar21 = local_38;
        break;
      case 1:
        iVar5 = _rand();
        fVar21 = (float)iVar5 * _DAT_00012f28 * fVar18 - local_34;
        goto LAB_00004274;
      case 2:
        fVar21 = (float)_sinf((float)((double)((float)*(uint *)((long)param_1 + 0xac) / (float)dVar3
                                              + fVar20) * _DAT_00012f48));
        fVar21 = fVar21 * local_34;
LAB_00004274:
        fVar21 = fVar21 + local_38;
        break;
      case 3:
        _vDSP_maxmgv(param_1[0x16],1,&local_48,(int)param_1[6]);
        fVar24 = (local_48 + local_48) * local_34 + local_38;
        fVar21 = DAT_00012f0c;
        if (fVar24 <= DAT_00012f0c) {
          fVar21 = fVar24;
        }
        goto LAB_000042f4;
      case 4:
        _vDSP_maxmgv(param_1[0x16],1,&local_48,(int)param_1[6]);
        fVar24 = fVar23 - (local_48 + local_48) * local_34;
        fVar21 = DAT_00012f0c;
        if (fVar24 <= DAT_00012f0c) {
          fVar21 = fVar24;
        }
LAB_000042f4:
        if (fVar21 < 0.0) {
          fVar21 = 0.0;
        }
      }
switchD_0000408e_default:
      local_40 = fVar21;
      if ((local_40 != *(float *)((long)param_1 + 0xd4)) ||
         (NAN(local_40) || NAN(*(float *)((long)param_1 + 0xd4)))) {
        *(float *)((long)param_1 + 0xd4) = local_40;
        dVar4 = (double)_pow(SUB84((double)(local_40 / _DAT_00012f2c + DAT_00012f08),0),DAT_00012f50
                            );
        *(float *)(param_1 + 0x1a) = (float)(DAT_00012f58 - dVar4);
      }
      FUN_0000d433(param_1[0x1e],param_1[0x1c],param_1[0x1f],*(undefined4 *)((long)param_1 + 0x34),
                   param_1[0x20],uVar16,*(undefined4 *)((long)param_1 + 0x3c));
      _vDSP_ctoz(param_1[0x20],2,plVar1,1,(int)param_1[7]);
      _vDSP_fft_zrip(param_1[0x11],plVar1,(long)(int)param_1[0x13],(int)param_1[0x12],1);
      puVar2 = (undefined4 *)param_1[10];
      uVar6 = *(uint *)(param_1 + 7);
      *(undefined4 *)(param_1[9] + (ulong)uVar6 * 4) = *puVar2;
      puVar2[uVar6] = 0;
      *puVar2 = 0;
      *(float *)param_1[0xd] = DAT_00012f08 - *(float *)(param_1 + 0x1a);
      _vDSP_zvsma(plVar1,1,param_1 + 0xd,param_1 + 0xb,1,plVar1,1,(int)param_1[0x15]);
      *(int *)param_1[0xd] = (int)param_1[0x1a];
      _vDSP_zvzsml(plVar1,1,param_1 + 0xd,param_1 + 0xb,1,(int)param_1[0x15]);
      if (local_44._0_1_ != '\0') {
        _vDSP_zvmul(plVar1,1,param_1 + 0xf,1,plVar1,1,(int)param_1[0x15],1);
        uVar6 = _rand();
        uVar6 = (uint)((ulong)uVar6 % (ulong)*(uint *)(param_1 + 0x15) >> 1);
        uVar14 = *(uint *)(param_1 + 0x15) >> 1;
        if (uVar6 < uVar14) {
          uVar14 = uVar6 + 1;
        }
        uVar13 = (ulong)(uVar14 * 4);
        _memcpy((void *)param_1[0x19],(void *)param_1[0xf],uVar13);
        _memcpy((void *)param_1[0xf],(void *)(param_1[0xf] + (ulong)uVar14 * 4),
                (ulong)(((int)param_1[0x15] - uVar14) * 4));
        _memcpy((void *)((ulong)((int)param_1[0x15] - uVar14) * 4 + param_1[0xf]),
                (void *)param_1[0x19],uVar13);
        _memcpy((void *)param_1[0x19],(void *)param_1[0x10],uVar13);
        _memcpy((void *)param_1[0x10],(void *)(param_1[0x10] + (ulong)uVar14 * 4),
                (ulong)(((int)param_1[0x15] - uVar14) * 4));
        _memcpy((void *)((ulong)((int)param_1[0x15] - uVar14) * 4 + param_1[0x10]),
                (void *)param_1[0x19],uVar13);
      }
      if ((uint)local_50 == 0) {
        uVar13 = 0;
      }
      else {
        uVar13 = (ulong)*(uint *)(param_1 + 0x15);
        if ((uint)local_50 < *(uint *)(param_1 + 0x15)) {
          uVar13 = local_50 & 0xffffffff;
        }
        iVar5 = (int)uVar13 * 4;
        ___bzero(param_1[9],iVar5);
        ___bzero(param_1[10],iVar5);
      }
      uVar6 = (uint)local_68;
      if (uVar6 < *(uint *)(param_1 + 0x15)) {
        local_68 = local_68 & 0xffffffff;
        if (uVar6 < (uint)uVar13) {
          local_68 = uVar13;
        }
        ___bzero(param_1[9] + local_68 * 4,(*(uint *)(param_1 + 0x15) - (int)local_68) * 4);
        ___bzero(local_68 * 4 + param_1[10],((int)param_1[0x15] - (int)local_68) * 4);
      }
      *(undefined4 *)param_1[10] = *(undefined4 *)(param_1[9] + (ulong)*(uint *)(param_1 + 7) * 4);
      _vDSP_fft_zrip(param_1[0x11],plVar1,(long)(int)param_1[0x13],(int)param_1[0x12],0xffffffff);
      _vDSP_ztoc(plVar1,1,param_1[0x17],2,(int)param_1[7]);
      FUN_0000de6f(param_1[0x17],param_1[0x1f],uVar16,param_1[0x1d],param_1[0x21],
                   *(undefined4 *)((long)param_1 + 0x34),(int)param_1[8]);
      FUN_0000df77((ulong)*(uint *)(param_1 + 4) * 4 + param_1[0x18],param_1[0x21],
                   *(undefined4 *)((long)param_1 + 0x34),*(undefined4 *)((long)param_1 + 0x2c),
                   (int)param_1[8]);
      if ((local_3c != _DAT_00012f30) || (NAN(local_3c) || NAN(_DAT_00012f30))) {
        lVar10 = (ulong)*(uint *)(param_1 + 4) * 4 + param_1[0x18];
        _vDSP_vsmul(lVar10,1,param_1 + 0x22,lVar10,1,*(undefined4 *)((long)param_1 + 0x2c));
      }
      uVar14 = *(uint *)((long)param_1 + 0x14);
      iVar5 = *(int *)((long)param_1 + 0x2c);
      *(uint *)(param_1 + 4) = (uint)((int)param_1[4] + iVar5) % *(uint *)((long)param_1 + 0x24);
      *(int *)((long)param_1 + 0x94) = *(int *)((long)param_1 + 0x94) + iVar5;
      uVar6 = *(uint *)(param_1 + 6);
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + uVar6;
      *(int *)(param_1 + 8) = (int)param_1[8] + iVar5;
      local_50 = uVar13;
LAB_00004208:
    } while (uVar14 < param_4);
  }
  if (*(char *)((long)param_1 + 0x114) == '\0') {
    if (*(uint *)((long)param_1 + 0x94) <= uVar7) {
      uVar11 = ___bzero(param_3,uVar11 << 2);
      goto LAB_000043a5;
    }
    *(undefined1 *)((long)param_1 + 0x114) = 1;
  }
  uVar7 = *(uint *)(param_1 + 3);
  if (*(uint *)((long)param_1 + 0x24) < uVar7 + param_4) {
    uVar6 = *(uint *)((long)param_1 + 0x24) - uVar7;
    _memcpy(param_3,(void *)((ulong)uVar7 * 4 + param_1[0x18]),(ulong)uVar6 * 4);
    param_3 = (void *)((long)param_3 + (ulong)uVar6 * 4);
    pvVar8 = (void *)param_1[0x18];
    uVar11 = (ulong)(param_4 - uVar6);
  }
  else {
    pvVar8 = (void *)((ulong)uVar7 * 4 + param_1[0x18]);
  }
  _memcpy(param_3,pvVar8,uVar11 << 2);
  uVar7 = (int)param_1[3] + param_4;
  uVar11 = (ulong)uVar7 / (ulong)*(uint *)((long)param_1 + 0x24);
  *(uint *)(param_1 + 3) = uVar7 % *(uint *)((long)param_1 + 0x24);
  *(int *)((long)param_1 + 0x94) = *(int *)((long)param_1 + 0x94) - param_4;
LAB_000043a5:
  *(int *)((long)param_1 + 0xac) = *(int *)((long)param_1 + 0xac) + param_4;
  return uVar11;
}

