static const x86_info_operand insn_operands[] = {
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_None, OPA_VEXImm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_None, OPA_VEXImm, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDRM, OPS_256, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_None, OPA_VEXImm, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_SIMDRM, OPS_256, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_None, OPA_VEXImm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDRM, OPS_256, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_Mem, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_Mem, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_SIMDRM, OPS_256, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_VEXImmSrc, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_SIMDRM, OPS_256, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_DREX, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRegMatch0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 0, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 0, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 0, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_16, 1, 0, OPTM_None, OPA_SImm, OPAP_SImm8},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_SImm, OPAP_SImm8},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_SImm, OPAP_SImm8},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_Mem, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_Mem, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Mem, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_256, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Mem, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_XMM0, OPS_128, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Creg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Creg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Creg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_RM, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_VEX, OPAP_None},
    {OPT_RM, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_RM, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_SpareEA, OPAP_None},
    {OPT_Imm, OPS_8, 0, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_SpareEA, OPAP_None},
    {OPT_Imm, OPS_8, 0, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_SpareEA, OPAP_None},
    {OPT_Imm, OPS_8, 0, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_SpareEA, OPAP_None},
    {OPT_Imm, OPS_16, 1, 0, OPTM_None, OPA_SImm, OPAP_SImm8},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_SpareEA, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_SImm, OPAP_SImm8},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_SpareEA, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_SImm, OPAP_SImm8},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_RM, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_8, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_8, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_ST0, OPS_80, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Reg, OPS_80, 0, 0, OPTM_None, OPA_Op1Add, OPAP_None},
    {OPT_Reg, OPS_80, 0, 0, OPTM_None, OPA_Op1Add, OPAP_None},
    {OPT_ST0, OPS_80, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDRM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDRM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Areg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_8, 1, 1, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_16, 1, 1, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_32, 1, 1, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_64, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_64, 1, 1, OPTM_None, OPA_EA, OPAP_None},
    {OPT_MemOffs, OPS_8, 1, 1, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_16, 1, 1, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_32, 1, 1, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_64, 1, 1, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_64, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Imm, OPS_64, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Areg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_MemOffs, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemOffs, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_ShortMov},
    {OPT_Areg, OPS_8, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_ShortMov},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_ShortMov},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_ShortMov},
    {OPT_Areg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Areg, OPS_8, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_ShortMov},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_ShortMov},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_ShortMov},
    {OPT_Areg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_ShortMov},
    {OPT_Mem, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SegReg, OPS_16, 1, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SegReg, OPS_16, 1, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SegReg, OPS_16, 1, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SegReg, OPS_16, 1, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SegReg, OPS_16, 1, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_8, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Imm, OPS_16, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Imm, OPS_64, 0, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Imm, OPS_64, 1, 0, OPTM_None, OPA_Imm, OPAP_SImm32Avail},
    {OPT_RM, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 0, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_16, 0, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_32, 0, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_32, 0, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_8, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_16, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_RM, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_CR4, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_CRReg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_CRReg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_CR4, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_CRReg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_DRReg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_DRReg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_DRReg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_8, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_64, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDRM, OPS_256, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Areg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Dreg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Areg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Dreg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Dreg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_EAVEX, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_MemrAX, OPS_Any, 0, 0, OPTM_None, OPA_AdSizeEA, OPAP_None},
    {OPT_Creg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Areg, OPS_64, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Areg, OPS_64, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Op0Add, OPAP_None},
    {OPT_Areg, OPS_64, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_64, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_8, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_8, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_RM, OPS_8, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Creg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_8, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm1, OPS_8, 1, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Creg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm1, OPS_8, 1, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Creg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm1, OPS_8, 1, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Creg, OPS_8, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_RM, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm1, OPS_8, 1, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SegReg, OPS_16, 1, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_80, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_Any, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_Any, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Imm, OPS_16, 1, 0, OPTM_None, OPA_Imm, OPAP_SImm8},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_Imm, OPAP_SImm8},
    {OPT_Areg, OPS_64, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_Imm, OPAP_SImm8},
    {OPT_RM, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 0, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_RM, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_16, 0, 0, OPTM_None, OPA_Imm, OPAP_SImm8},
    {OPT_RM, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_16, 1, 0, OPTM_None, OPA_Imm, OPAP_SImm8},
    {OPT_RM, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 0, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_RM, OPS_32, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_32, 0, 0, OPTM_None, OPA_Imm, OPAP_SImm8},
    {OPT_RM, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_Imm, OPAP_SImm8},
    {OPT_RM, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_8, 0, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_RM, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_Imm, OPAP_SImm8},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Reg, OPS_16, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_8, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_128, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_256, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_None, OPA_JmpRel, OPAP_None},
    {OPT_Creg, OPS_16, 0, 0, OPTM_None, OPA_AdSizeR, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_None, OPA_JmpRel, OPAP_None},
    {OPT_Creg, OPS_32, 0, 0, OPTM_None, OPA_AdSizeR, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_None, OPA_JmpRel, OPAP_None},
    {OPT_Creg, OPS_64, 0, 0, OPTM_None, OPA_AdSizeR, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_Short, OPA_JmpRel, OPAP_None},
    {OPT_Creg, OPS_16, 0, 0, OPTM_None, OPA_AdSizeR, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_Short, OPA_JmpRel, OPAP_None},
    {OPT_Creg, OPS_32, 0, 0, OPTM_None, OPA_AdSizeR, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_Short, OPA_JmpRel, OPAP_None},
    {OPT_Creg, OPS_64, 0, 0, OPTM_None, OPA_AdSizeR, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_128, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_80, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SegReg, OPS_16, 1, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDRM, OPS_256, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Areg, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Imm, OPS_16, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Areg, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Areg, OPS_64, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_Imm, OPS_32, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_SIMDReg, OPS_256, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_Mem, OPS_256, 1, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_SIMDReg, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_16, 1, 0, OPTM_None, OPA_EA, OPAP_A16},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_Imm, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Spare, OPAP_None},
    {OPT_RM, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_RM, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SIMDReg, OPS_128, 0, 0, OPTM_None, OPA_SpareVEX, OPAP_None},
    {OPT_RM, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_80, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_16, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_32, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_64, 0, 0, OPTM_None, OPA_EA, OPAP_None},
    {OPT_SS, OPS_Any, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SS, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_SS, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_DS, OPS_Any, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_DS, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_DS, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_ES, OPS_Any, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_ES, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_ES, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_FS, OPS_Any, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_FS, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_FS, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_GS, OPS_Any, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_GS, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_GS, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_MemEAX, OPS_Any, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_ImmNotSegOff, OPS_Any, 0, 0, OPTM_None, OPA_JmpRel, OPAP_None},
    {OPT_ImmNotSegOff, OPS_16, 0, 0, OPTM_None, OPA_JmpRel, OPAP_None},
    {OPT_ImmNotSegOff, OPS_32, 0, 0, OPTM_None, OPA_JmpRel, OPAP_None},
    {OPT_Imm, OPS_16, 0, 0, OPTM_Near, OPA_JmpRel, OPAP_None},
    {OPT_Imm, OPS_32, 0, 0, OPTM_Near, OPA_JmpRel, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_Near, OPA_JmpRel, OPAP_None},
    {OPT_RM, OPS_16, 0, 0, OPTM_Near, OPA_EA, OPAP_None},
    {OPT_RM, OPS_32, 0, 0, OPTM_Near, OPA_EA, OPAP_None},
    {OPT_RM, OPS_64, 0, 0, OPTM_Near, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_Any, 0, 0, OPTM_Near, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_16, 0, 0, OPTM_Far, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_32, 0, 0, OPTM_Far, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_64, 0, 0, OPTM_Far, OPA_EA, OPAP_None},
    {OPT_Mem, OPS_Any, 0, 0, OPTM_Far, OPA_EA, OPAP_None},
    {OPT_Imm, OPS_16, 0, 0, OPTM_Far, OPA_JmpFar, OPAP_None},
    {OPT_Imm, OPS_32, 0, 0, OPTM_Far, OPA_JmpFar, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_Far, OPA_JmpFar, OPAP_None},
    {OPT_Imm, OPS_16, 0, 0, OPTM_None, OPA_JmpFar, OPAP_None},
    {OPT_Imm, OPS_32, 0, 0, OPTM_None, OPA_JmpFar, OPAP_None},
    {OPT_Imm, OPS_Any, 0, 0, OPTM_None, OPA_JmpFar, OPAP_None},
    {OPT_Reg, OPS_80, 0, 0, OPTM_To, OPA_Op1Add, OPAP_None},
    {OPT_Reg, OPS_32, 0, 0, OPTM_None, OPA_Op1Add, OPAP_None},
    {OPT_Reg, OPS_64, 0, 0, OPTM_None, OPA_Op1Add, OPAP_None},
    {OPT_Imm, OPS_16, 0, 0, OPTM_None, OPA_JmpRel, OPAP_None},
    {OPT_Imm, OPS_32, 0, 0, OPTM_None, OPA_JmpRel, OPAP_None},
    {OPT_Imm, OPS_8, 1, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_Imm, OPS_BITS, 1, 0, OPTM_None, OPA_Imm, OPAP_SImm8},
    {OPT_Imm, OPS_32, 0, 0, OPTM_None, OPA_SImm, OPAP_None},
    {OPT_CS, OPS_Any, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_CS, OPS_16, 0, 0, OPTM_None, OPA_None, OPAP_None},
    {OPT_CS, OPS_32, 0, 0, OPTM_None, OPA_None, OPAP_None}
};

static const x86_insn_info empty_insn[] = {
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0 }
};

static const x86_insn_info not64_insn[] = {
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0 }
};

static const x86_insn_info onebyte_insn[] = {
    { 0, 0, 0, 0, 0, {MOD_Op0Add, MOD_OpSizeR, MOD_DOpS64R}, 0, 0, 0, 0, 1, {0x00, 0, 0}, 0, 0, 0 }
};

static const x86_insn_info onebyte_prefix_insn[] = {
    { 0, 0, 0, 0, 0, {MOD_PreAdd, MOD_Op0Add, 0}, 0, 0, 0x00, 0, 1, {0x00, 0, 0}, 0, 0, 0 }
};

static const x86_insn_info twobyte_insn[] = {
    { SUF_L|SUF_Q, 0, 0, 0, 0, {MOD_Op0Add, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0x00, 0x00, 0}, 0, 0, 0 }
};

static const x86_insn_info threebyte_insn[] = {
    { 0, 0, 0, 0, 0, {MOD_Op0Add, MOD_Op1Add, MOD_Op2Add}, 0, 0, 0, 0, 3, {0x00, 0x00, 0x00}, 0, 0, 0 }
};

static const x86_insn_info onebytemem_insn[] = {
    { SUF_L|SUF_Q|SUF_S, 0, 0, 0, 0, {MOD_SpAdd, MOD_Op0Add, 0}, 0, 0, 0, 0, 1, {0x00, 0, 0}, 0, 1, 552 }
};

static const x86_insn_info twobytemem_insn[] = {
    { SUF_L|SUF_Q|SUF_S|SUF_W, 0, 0, 0, 0, {MOD_SpAdd, MOD_Op0Add, MOD_Op1Add}, 0, 0, 0, 0, 2, {0x00, 0x00, 0}, 0, 1, 552 }
};

static const x86_insn_info mov_insn[] = {
    { SUF_B, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xA0, 0, 0}, 0, 2, 393 },
    { SUF_W, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xA1, 0, 0}, 0, 2, 395 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xA1, 0, 0}, 0, 2, 397 },
    { SUF_B, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xA2, 0, 0}, 0, 2, 399 },
    { SUF_W, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xA3, 0, 0}, 0, 2, 401 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xA3, 0, 0}, 0, 2, 403 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xA0, 0, 0}, 0, 2, 369 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xA1, 0, 0}, 0, 2, 371 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xA1, 0, 0}, 0, 2, 373 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xA1, 0, 0}, 0, 2, 375 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xA2, 0, 0}, 0, 2, 377 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xA3, 0, 0}, 0, 2, 379 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xA3, 0, 0}, 0, 2, 381 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xA3, 0, 0}, 0, 2, 383 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x88, 0xA2, 0}, 0, 2, 405 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x89, 0xA3, 0}, 0, 2, 407 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x89, 0xA3, 0}, 0, 2, 409 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x89, 0xA3, 0}, 0, 2, 411 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x88, 0, 0}, 0, 2, 351 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x89, 0, 0}, 0, 2, 288 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x89, 0, 0}, 0, 2, 294 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x89, 0, 0}, 0, 2, 300 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x8A, 0xA0, 0}, 0, 2, 413 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x8B, 0xA1, 0}, 0, 2, 415 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x8B, 0xA1, 0}, 0, 2, 417 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x8B, 0xA1, 0}, 0, 2, 419 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x8A, 0, 0}, 0, 2, 353 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x8B, 0, 0}, 0, 2, 198 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x8B, 0, 0}, 0, 2, 201 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x8B, 0, 0}, 0, 2, 204 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x8C, 0, 0}, 0, 2, 421 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x8C, 0, 0}, 0, 2, 423 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x8C, 0, 0}, 0, 2, 425 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x8C, 0, 0}, 0, 2, 427 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x8E, 0, 0}, 0, 2, 429 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x8E, 0, 0}, 0, 2, 424 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x8E, 0, 0}, 0, 2, 426 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xB0, 0, 0}, 0, 2, 431 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xB8, 0, 0}, 0, 2, 433 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xB8, 0, 0}, 0, 2, 435 },
    { GAS_ILLEGAL, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xB8, 0, 0}, 0, 2, 437 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xB8, 0xC7, 0}, 0, 2, 439 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xC6, 0, 0}, 0, 2, 441 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xC7, 0, 0}, 0, 2, 443 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xC7, 0, 0}, 0, 2, 445 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xC7, 0, 0}, 0, 2, 447 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xC6, 0, 0}, 0, 2, 449 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xC7, 0, 0}, 0, 2, 451 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xC7, 0, 0}, 0, 2, 453 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xC7, 0, 0}, 0, 2, 455 },
    { SUF_L, NOT_64, CPU_586, CPU_Priv, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x22, 0}, 0, 2, 457 },
    { SUF_L, NOT_64, CPU_386, CPU_Priv, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x22, 0}, 0, 2, 459 },
    { SUF_Q, ONLY_64, CPU_Priv, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x22, 0}, 0, 2, 461 },
    { SUF_L, NOT_64, CPU_586, CPU_Priv, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x20, 0}, 0, 2, 463 },
    { SUF_L, NOT_64, CPU_386, CPU_Priv, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x20, 0}, 0, 2, 458 },
    { SUF_Q, ONLY_64, CPU_Priv, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x20, 0}, 0, 2, 465 },
    { SUF_L, NOT_64, CPU_386, CPU_Priv, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x23, 0}, 0, 2, 467 },
    { SUF_Q, ONLY_64, CPU_Priv, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x23, 0}, 0, 2, 469 },
    { SUF_L, NOT_64, CPU_386, CPU_Priv, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x21, 0}, 0, 2, 468 },
    { SUF_Q, ONLY_64, CPU_Priv, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x21, 0}, 0, 2, 471 },
    { GAS_ONLY|SUF_Q, 0, CPU_MMX, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x6F, 0}, 0, 2, 282 },
    { GAS_ONLY|SUF_Q, ONLY_64, CPU_MMX, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x6E, 0}, 0, 2, 323 },
    { GAS_ONLY|SUF_Q, 0, CPU_MMX, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x7F, 0}, 0, 2, 359 },
    { GAS_ONLY|SUF_Q, ONLY_64, CPU_MMX, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 325 },
    { GAS_ONLY|SUF_Q, 0, CPU_SSE2, 0, 0, {0, 0, 0}, 0, 0, 0xF3, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 112 },
    { GAS_ONLY|SUF_Q, 0, CPU_SSE2, 0, 0, {0, 0, 0}, 0, 0, 0xF3, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 361 },
    { GAS_ONLY|SUF_Q, ONLY_64, CPU_SSE2, 0, 0, {0, 0, 0}, 64, 0, 0x66, 0, 2, {0x0F, 0x6E, 0}, 0, 2, 329 },
    { GAS_ONLY|SUF_Q, 0, CPU_SSE2, 0, 0, {0, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0xD6, 0}, 0, 2, 363 },
    { GAS_ONLY|SUF_Q, ONLY_64, CPU_SSE2, 0, 0, {0, 0, 0}, 64, 0, 0x66, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 267 }
};

static const x86_insn_info movabs_insn[] = {
    { SUF_B, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xA0, 0, 0}, 0, 2, 369 },
    { SUF_W, ONLY_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xA1, 0, 0}, 0, 2, 371 },
    { SUF_L, ONLY_64, 0, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xA1, 0, 0}, 0, 2, 373 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xA1, 0, 0}, 0, 2, 375 },
    { SUF_B, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xA2, 0, 0}, 0, 2, 377 },
    { SUF_W, ONLY_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xA3, 0, 0}, 0, 2, 379 },
    { SUF_L, ONLY_64, 0, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xA3, 0, 0}, 0, 2, 381 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xA3, 0, 0}, 0, 2, 383 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xB8, 0, 0}, 0, 2, 385 }
};

static const x86_insn_info movszx_insn[] = {
    { SUF_B, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 581 },
    { SUF_B, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 527 },
    { SUF_B, ONLY_64, 0, 0, 0, {MOD_Op1Add, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 531 },
    { SUF_W, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 2, 529 },
    { SUF_W, ONLY_64, 0, 0, 0, {MOD_Op1Add, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 2, 583 }
};

static const x86_insn_info movsxd_insn[] = {
    { SUF_L, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x63, 0, 0}, 0, 2, 623 }
};

static const x86_insn_info push_insn[] = {
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0x50, 0, 0}, 0, 1, 433 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x50, 0, 0}, 0, 1, 435 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0x50, 0, 0}, 0, 1, 385 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0xFF, 0, 0}, 6, 1, 315 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xFF, 0, 0}, 6, 1, 311 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0xFF, 0, 0}, 6, 1, 314 },
    { GAS_ILLEGAL, 0, CPU_186, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0x6A, 0, 0}, 0, 1, 200 },
    { GAS_ONLY, 0, CPU_186, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0x6A, 0, 0}, 0, 1, 674 },
    { GAS_ONLY|SUF_W, 0, CPU_186, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0x6A, 0x68, 0}, 0, 1, 556 },
    { GAS_ONLY|SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x6A, 0x68, 0}, 0, 1, 558 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 1, {0x6A, 0x68, 0}, 0, 1, 212 },
    { GAS_ILLEGAL, NOT_64, CPU_186, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x6A, 0x68, 0}, 0, 1, 675 },
    { GAS_ILLEGAL, 0, CPU_186, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0x68, 0, 0}, 0, 1, 444 },
    { GAS_ILLEGAL, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x68, 0, 0}, 0, 1, 446 },
    { GAS_ILLEGAL, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 1, {0x68, 0, 0}, 0, 1, 676 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x0E, 0, 0}, 0, 1, 677 },
    { SUF_W, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x0E, 0, 0}, 0, 1, 678 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x0E, 0, 0}, 0, 1, 679 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x16, 0, 0}, 0, 1, 633 },
    { SUF_W, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x16, 0, 0}, 0, 1, 634 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x16, 0, 0}, 0, 1, 635 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x1E, 0, 0}, 0, 1, 636 },
    { SUF_W, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x1E, 0, 0}, 0, 1, 637 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x1E, 0, 0}, 0, 1, 638 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x06, 0, 0}, 0, 1, 639 },
    { SUF_W, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x06, 0, 0}, 0, 1, 640 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x06, 0, 0}, 0, 1, 641 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xA0, 0}, 0, 1, 642 },
    { SUF_W, 0, CPU_386, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0xA0, 0}, 0, 1, 643 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0xA0, 0}, 0, 1, 644 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xA8, 0}, 0, 1, 645 },
    { SUF_W, 0, CPU_386, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0xA8, 0}, 0, 1, 646 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0xA8, 0}, 0, 1, 647 }
};

static const x86_insn_info pop_insn[] = {
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0x58, 0, 0}, 0, 1, 433 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x58, 0, 0}, 0, 1, 435 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0x58, 0, 0}, 0, 1, 385 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0x8F, 0, 0}, 0, 1, 315 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x8F, 0, 0}, 0, 1, 311 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0x8F, 0, 0}, 0, 1, 314 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x17, 0, 0}, 0, 1, 633 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x17, 0, 0}, 0, 1, 634 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x17, 0, 0}, 0, 1, 635 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x1F, 0, 0}, 0, 1, 636 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x1F, 0, 0}, 0, 1, 637 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x1F, 0, 0}, 0, 1, 638 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x07, 0, 0}, 0, 1, 639 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x07, 0, 0}, 0, 1, 640 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x07, 0, 0}, 0, 1, 641 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xA1, 0}, 0, 1, 642 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0xA1, 0}, 0, 1, 643 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0xA1, 0}, 0, 1, 644 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xA9, 0}, 0, 1, 645 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0xA9, 0}, 0, 1, 646 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0xA9, 0}, 0, 1, 647 }
};

static const x86_insn_info xchg_insn[] = {
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x86, 0, 0}, 0, 2, 351 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x86, 0, 0}, 0, 2, 353 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x90, 0, 0}, 0, 2, 509 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x90, 0, 0}, 0, 2, 511 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x87, 0, 0}, 0, 2, 288 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x87, 0, 0}, 0, 2, 198 },
    { SUF_L, ONLY_64, 0, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x87, 0, 0}, 0, 2, 513 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x90, 0, 0}, 0, 2, 515 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x90, 0, 0}, 0, 2, 517 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x87, 0, 0}, 0, 2, 294 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x87, 0, 0}, 0, 2, 201 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x90, 0, 0}, 0, 2, 519 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x90, 0, 0}, 0, 2, 384 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x90, 0, 0}, 0, 2, 521 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x87, 0, 0}, 0, 2, 300 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x87, 0, 0}, 0, 2, 204 }
};

static const x86_insn_info in_insn[] = {
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xE4, 0, 0}, 0, 2, 494 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xE5, 0, 0}, 0, 2, 496 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xE5, 0, 0}, 0, 2, 603 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xEC, 0, 0}, 0, 2, 500 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xED, 0, 0}, 0, 2, 502 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xED, 0, 0}, 0, 2, 498 },
    { GAS_ONLY|SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xE4, 0, 0}, 0, 1, 23 },
    { GAS_ONLY|SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xE5, 0, 0}, 0, 1, 23 },
    { GAS_ONLY|SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xE5, 0, 0}, 0, 1, 23 },
    { GAS_ONLY|SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xEC, 0, 0}, 0, 1, 499 },
    { GAS_ONLY|SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xED, 0, 0}, 0, 1, 499 },
    { GAS_ONLY|SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xED, 0, 0}, 0, 1, 499 }
};

static const x86_insn_info out_insn[] = {
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xE6, 0, 0}, 0, 2, 493 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xE7, 0, 0}, 0, 2, 495 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xE7, 0, 0}, 0, 2, 497 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xEE, 0, 0}, 0, 2, 499 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xEF, 0, 0}, 0, 2, 501 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xEF, 0, 0}, 0, 2, 503 },
    { GAS_ONLY|SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xE6, 0, 0}, 0, 1, 23 },
    { GAS_ONLY|SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xE7, 0, 0}, 0, 1, 23 },
    { GAS_ONLY|SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xE7, 0, 0}, 0, 1, 23 },
    { GAS_ONLY|SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xEE, 0, 0}, 0, 1, 499 },
    { GAS_ONLY|SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xEF, 0, 0}, 0, 1, 499 },
    { GAS_ONLY|SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xEF, 0, 0}, 0, 1, 499 }
};

static const x86_insn_info lea_insn[] = {
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x8D, 0, 0}, 0, 2, 523 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x8D, 0, 0}, 0, 2, 387 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x8D, 0, 0}, 0, 2, 525 }
};

static const x86_insn_info ldes_insn[] = {
    { SUF_W, NOT_64, 0, 0, 0, {MOD_Op0Add, 0, 0}, 16, 0, 0, 0, 1, {0x00, 0, 0}, 0, 2, 551 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {MOD_Op0Add, 0, 0}, 32, 0, 0, 0, 1, {0x00, 0, 0}, 0, 2, 553 }
};

static const x86_insn_info lfgss_insn[] = {
    { SUF_W, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 551 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 553 }
};

static const x86_insn_info arith_insn[] = {
    { SUF_B, 0, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 0, 0, 0, 1, {0x04, 0, 0}, 0, 2, 494 },
    { SUF_W, 0, 0, 0, 0, {MOD_Op2Add, MOD_Op1AddSp, 0}, 16, 0, 0, 0, 2, {0x83, 0xC0, 0x05}, 0, 2, 555 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Op2Add, MOD_Op1AddSp, 0}, 32, 0, 0, 0, 2, {0x83, 0xC0, 0x05}, 0, 2, 557 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_Op2Add, MOD_Op1AddSp, 0}, 64, 0, 0, 0, 2, {0x83, 0xC0, 0x05}, 0, 2, 559 },
    { SUF_B, 0, 0, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 0, 0, 0, 0, 1, {0x80, 0, 0}, 0, 2, 449 },
    { SUF_B, 0, 0, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 0, 0, 0, 0, 1, {0x80, 0, 0}, 0, 2, 441 },
    { SUF_W, 0, 0, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 16, 0, 0, 0, 1, {0x83, 0, 0}, 0, 2, 561 },
    { GAS_ILLEGAL, 0, 0, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 16, 0, 0, 0, 1, {0x83, 0x81, 0}, 0, 2, 563 },
    { SUF_W, 0, 0, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 16, 0, 0, 0, 1, {0x83, 0x81, 0}, 0, 2, 565 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 32, 0, 0, 0, 1, {0x83, 0, 0}, 0, 2, 567 },
    { GAS_ILLEGAL, NOT_64, CPU_386, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 32, 0, 0, 0, 1, {0x83, 0x81, 0}, 0, 2, 569 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 32, 0, 0, 0, 1, {0x83, 0x81, 0}, 0, 2, 571 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 64, 0, 0, 0, 1, {0x83, 0, 0}, 0, 2, 573 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 64, 0, 0, 0, 1, {0x83, 0x81, 0}, 0, 2, 575 },
    { SUF_B, 0, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 0, 0, 0, 1, {0x00, 0, 0}, 0, 2, 351 },
    { SUF_W, 0, 0, 0, 0, {MOD_Op0Add, 0, 0}, 16, 0, 0, 0, 1, {0x01, 0, 0}, 0, 2, 288 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Op0Add, 0, 0}, 32, 0, 0, 0, 1, {0x01, 0, 0}, 0, 2, 294 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_Op0Add, 0, 0}, 64, 0, 0, 0, 1, {0x01, 0, 0}, 0, 2, 300 },
    { SUF_B, 0, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 0, 0, 0, 1, {0x02, 0, 0}, 0, 2, 353 },
    { SUF_W, 0, 0, 0, 0, {MOD_Op0Add, 0, 0}, 16, 0, 0, 0, 1, {0x03, 0, 0}, 0, 2, 198 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Op0Add, 0, 0}, 32, 0, 0, 0, 1, {0x03, 0, 0}, 0, 2, 201 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_Op0Add, 0, 0}, 64, 0, 0, 0, 1, {0x03, 0, 0}, 0, 2, 204 }
};

static const x86_insn_info incdec_insn[] = {
    { SUF_B, 0, 0, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 0, 0, 0, 0, 1, {0xFE, 0, 0}, 0, 1, 449 },
    { SUF_W, NOT_64, 0, 0, 0, {MOD_Op0Add, 0, 0}, 16, 0, 0, 0, 1, {0x00, 0, 0}, 0, 1, 433 },
    { SUF_W, 0, 0, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 16, 0, 0, 0, 1, {0xFF, 0, 0}, 0, 1, 315 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {MOD_Op0Add, 0, 0}, 32, 0, 0, 0, 1, {0x00, 0, 0}, 0, 1, 435 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 32, 0, 0, 0, 1, {0xFF, 0, 0}, 0, 1, 311 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 64, 0, 0, 0, 1, {0xFF, 0, 0}, 0, 1, 314 }
};

static const x86_insn_info f6_insn[] = {
    { SUF_B, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xF6, 0, 0}, 0, 1, 449 },
    { SUF_W, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 16, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 1, 315 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_SpAdd, 0, 0}, 32, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 1, 311 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_SpAdd, 0, 0}, 64, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 1, 314 }
};

static const x86_insn_info div_insn[] = {
    { SUF_B, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xF6, 0, 0}, 0, 1, 449 },
    { SUF_W, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 16, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 1, 315 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_SpAdd, 0, 0}, 32, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 1, 311 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_SpAdd, 0, 0}, 64, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 1, 314 },
    { SUF_B, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xF6, 0, 0}, 0, 2, 479 },
    { SUF_W, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 16, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 2, 481 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_SpAdd, 0, 0}, 32, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 2, 483 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_SpAdd, 0, 0}, 64, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 2, 485 }
};

static const x86_insn_info test_insn[] = {
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xA8, 0, 0}, 0, 2, 494 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xA9, 0, 0}, 0, 2, 611 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xA9, 0, 0}, 0, 2, 613 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xA9, 0, 0}, 0, 2, 615 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xF6, 0, 0}, 0, 2, 449 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xF6, 0, 0}, 0, 2, 441 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 2, 451 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 2, 443 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 2, 453 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 2, 445 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 2, 455 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xF7, 0, 0}, 0, 2, 447 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x84, 0, 0}, 0, 2, 351 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x85, 0, 0}, 0, 2, 288 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x85, 0, 0}, 0, 2, 294 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x85, 0, 0}, 0, 2, 300 },
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x84, 0, 0}, 0, 2, 353 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x85, 0, 0}, 0, 2, 198 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x85, 0, 0}, 0, 2, 201 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x85, 0, 0}, 0, 2, 204 }
};

static const x86_insn_info aadm_insn[] = {
    { 0, 0, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 0, 0, 0, 2, {0xD4, 0x0A, 0}, 0, 0, 0 },
    { 0, 0, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 0, 0, 0, 1, {0xD4, 0, 0}, 0, 1, 23 }
};

static const x86_insn_info imul_insn[] = {
    { SUF_B, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xF6, 0, 0}, 5, 1, 449 },
    { SUF_W, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xF7, 0, 0}, 5, 1, 315 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xF7, 0, 0}, 5, 1, 311 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xF7, 0, 0}, 5, 1, 314 },
    { SUF_W, 0, CPU_386, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0xAF, 0}, 0, 2, 198 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0xAF, 0}, 0, 2, 201 },
    { SUF_Q, ONLY_64, CPU_386, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0xAF, 0}, 0, 2, 204 },
    { SUF_W, 0, CPU_186, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x6B, 0, 0}, 0, 3, 198 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x6B, 0, 0}, 0, 3, 201 },
    { SUF_Q, ONLY_64, CPU_186, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x6B, 0, 0}, 0, 3, 204 },
    { SUF_W, 0, CPU_186, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x6B, 0, 0}, 0, 2, 331 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x6B, 0, 0}, 0, 2, 333 },
    { SUF_Q, ONLY_64, CPU_186, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x6B, 0, 0}, 0, 2, 335 },
    { SUF_W, 0, CPU_186, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x6B, 0x69, 0}, 0, 3, 207 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x6B, 0x69, 0}, 0, 3, 210 },
    { SUF_Q, ONLY_64, CPU_186, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x6B, 0x69, 0}, 0, 3, 213 },
    { SUF_W, 0, CPU_186, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x6B, 0x69, 0}, 0, 2, 337 },
    { SUF_L, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x6B, 0x69, 0}, 0, 2, 339 },
    { SUF_Q, ONLY_64, CPU_186, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x6B, 0x69, 0}, 0, 2, 341 }
};

static const x86_insn_info shift_insn[] = {
    { SUF_B, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xD2, 0, 0}, 0, 2, 533 },
    { SUF_B, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xD0, 0, 0}, 0, 2, 535 },
    { SUF_B, 0, CPU_186, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xC0, 0, 0}, 0, 2, 449 },
    { SUF_W, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 16, 0, 0, 0, 1, {0xD3, 0, 0}, 0, 2, 537 },
    { SUF_W, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 16, 0, 0, 0, 1, {0xD1, 0, 0}, 0, 2, 539 },
    { SUF_W, 0, CPU_186, 0, 0, {MOD_SpAdd, 0, 0}, 16, 0, 0, 0, 1, {0xC1, 0, 0}, 0, 2, 315 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_SpAdd, 0, 0}, 32, 0, 0, 0, 1, {0xD3, 0, 0}, 0, 2, 541 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_SpAdd, 0, 0}, 32, 0, 0, 0, 1, {0xD1, 0, 0}, 0, 2, 543 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_SpAdd, 0, 0}, 32, 0, 0, 0, 1, {0xC1, 0, 0}, 0, 2, 317 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_SpAdd, 0, 0}, 64, 0, 0, 0, 1, {0xD3, 0, 0}, 0, 2, 545 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_SpAdd, 0, 0}, 64, 0, 0, 0, 1, {0xD1, 0, 0}, 0, 2, 547 },
    { SUF_Q, ONLY_64, CPU_186, 0, 0, {MOD_SpAdd, 0, 0}, 64, 0, 0, 0, 1, {0xC1, 0, 0}, 0, 2, 319 },
    { GAS_ONLY|SUF_B, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xD0, 0, 0}, 0, 1, 449 },
    { GAS_ONLY|SUF_W, 0, 0, 0, 0, {MOD_SpAdd, 0, 0}, 16, 0, 0, 0, 1, {0xD1, 0, 0}, 0, 1, 315 },
    { GAS_ONLY|SUF_L, 0, CPU_386, 0, 0, {MOD_SpAdd, 0, 0}, 32, 0, 0, 0, 1, {0xD1, 0, 0}, 0, 1, 311 },
    { GAS_ONLY|SUF_Q, ONLY_64, 0, 0, 0, {MOD_SpAdd, 0, 0}, 64, 0, 0, 0, 1, {0xD1, 0, 0}, 0, 1, 314 }
};

static const x86_insn_info shlrd_insn[] = {
    { SUF_W, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 288 },
    { SUF_W, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 3, 291 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 294 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 3, 297 },
    { SUF_Q, ONLY_64, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 300 },
    { SUF_Q, ONLY_64, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 3, 303 },
    { GAS_ONLY|SUF_W, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 2, 288 },
    { GAS_ONLY|SUF_L, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 2, 294 },
    { GAS_ONLY|SUF_Q, ONLY_64, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 2, 300 }
};

static const x86_insn_info call_insn[] = {
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 649 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 650 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 651 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 651 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0xE8, 0, 0}, 0, 1, 652 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xE8, 0, 0}, 0, 1, 653 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 1, {0xE8, 0, 0}, 0, 1, 653 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0xE8, 0, 0}, 0, 1, 654 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xFF, 0, 0}, 2, 1, 315 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xFF, 0, 0}, 2, 1, 311 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 1, {0xFF, 0, 0}, 2, 1, 314 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0xFF, 0, 0}, 2, 1, 552 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0xFF, 0, 0}, 2, 1, 655 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xFF, 0, 0}, 2, 1, 656 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 1, {0xFF, 0, 0}, 2, 1, 657 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0xFF, 0, 0}, 2, 1, 658 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xFF, 0, 0}, 3, 1, 659 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xFF, 0, 0}, 3, 1, 660 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xFF, 0, 0}, 3, 1, 661 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xFF, 0, 0}, 3, 1, 662 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x9A, 0, 0}, 3, 1, 663 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x9A, 0, 0}, 3, 1, 664 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x9A, 0, 0}, 3, 1, 665 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x9A, 0, 0}, 3, 1, 666 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x9A, 0, 0}, 3, 1, 667 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x9A, 0, 0}, 3, 1, 668 }
};

static const x86_insn_info jmp_insn[] = {
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 649 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 650 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x00, 0, 0}, 0, 1, 651 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0x00, 0, 0}, 0, 1, 651 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0xEB, 0, 0}, 0, 1, 595 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0xE9, 0, 0}, 0, 1, 652 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xE9, 0, 0}, 0, 1, 653 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 1, {0xE9, 0, 0}, 0, 1, 653 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0xE9, 0, 0}, 0, 1, 654 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0xFF, 0, 0}, 4, 1, 315 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xFF, 0, 0}, 4, 1, 311 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 1, {0xFF, 0, 0}, 4, 1, 314 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0xFF, 0, 0}, 4, 1, 552 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 64, 0, 0, 1, {0xFF, 0, 0}, 4, 1, 655 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xFF, 0, 0}, 4, 1, 656 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 1, {0xFF, 0, 0}, 4, 1, 657 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 1, {0xFF, 0, 0}, 4, 1, 658 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xFF, 0, 0}, 5, 1, 659 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xFF, 0, 0}, 5, 1, 660 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 1, {0xFF, 0, 0}, 5, 1, 661 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xFF, 0, 0}, 5, 1, 662 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xEA, 0, 0}, 3, 1, 663 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xEA, 0, 0}, 3, 1, 664 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xEA, 0, 0}, 3, 1, 665 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xEA, 0, 0}, 3, 1, 666 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xEA, 0, 0}, 3, 1, 667 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xEA, 0, 0}, 3, 1, 668 }
};

static const x86_insn_info retnf_insn[] = {
    { 0, NOT_64, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 0, 0, 0, 1, {0x01, 0, 0}, 0, 0, 0 },
    { 0, NOT_64, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 0, 0, 0, 1, {0x00, 0, 0}, 0, 1, 434 },
    { 0, ONLY_64, 0, 0, 0, {MOD_Op0Add, MOD_OpSizeR, 0}, 0, 0, 0, 0, 1, {0x01, 0, 0}, 0, 0, 0 },
    { 0, ONLY_64, 0, 0, 0, {MOD_Op0Add, MOD_OpSizeR, 0}, 0, 0, 0, 0, 1, {0x00, 0, 0}, 0, 1, 434 },
    { SUF_L|SUF_Q|SUF_W, 0, 0, 0, 0, {MOD_Op0Add, MOD_OpSizeR, 0}, 0, 0, 0, 0, 1, {0x01, 0, 0}, 0, 0, 0 },
    { SUF_L|SUF_Q|SUF_W, 0, 0, 0, 0, {MOD_Op0Add, MOD_OpSizeR, 0}, 0, 0, 0, 0, 1, {0x00, 0, 0}, 0, 1, 434 }
};

static const x86_insn_info enter_insn[] = {
    { GAS_NO_REV|SUF_L, NOT_64, CPU_186, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xC8, 0, 0}, 0, 2, 621 },
    { GAS_NO_REV|SUF_Q, ONLY_64, CPU_186, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 1, {0xC8, 0, 0}, 0, 2, 621 },
    { GAS_ONLY|GAS_NO_REV|SUF_W, 0, CPU_186, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0xC8, 0, 0}, 0, 2, 621 }
};

static const x86_insn_info jcc_insn[] = {
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 589 },
    { 0, 0, 0, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 672 },
    { 0, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 673 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 673 },
    { 0, 0, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 64, 0, 0, 1, {0x70, 0, 0}, 0, 1, 595 },
    { 0, 0, CPU_186, 0, 0, {MOD_Op1Add, 0, 0}, 16, 64, 0, 0, 2, {0x0F, 0x80, 0}, 0, 1, 652 },
    { 0, NOT_64, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x80, 0}, 0, 1, 653 },
    { 0, ONLY_64, 0, 0, 0, {MOD_Op1Add, 0, 0}, 64, 64, 0, 0, 2, {0x0F, 0x80, 0}, 0, 1, 653 },
    { 0, 0, CPU_186, 0, 0, {MOD_Op1Add, 0, 0}, 0, 64, 0, 0, 2, {0x0F, 0x80, 0}, 0, 1, 654 }
};

static const x86_insn_info jcxz_insn[] = {
    { 0, 0, 0, 0, 0, {MOD_AdSizeR, 0, 0}, 0, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 589 },
    { 0, 0, 0, 0, 0, {MOD_AdSizeR, 0, 0}, 0, 64, 0, 0, 1, {0xE3, 0, 0}, 0, 1, 595 }
};

static const x86_insn_info loop_insn[] = {
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, {0, 0, 0}, 0, 1, 589 },
    { 0, NOT_64, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, {0, 0, 0}, 0, 2, 589 },
    { 0, 0, CPU_386, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 0, {0, 0, 0}, 0, 2, 591 },
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 0, 64, 0, 0, 0, {0, 0, 0}, 0, 2, 593 },
    { 0, NOT_64, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 0, 0, 0, 1, {0xE0, 0, 0}, 0, 1, 595 },
    { 0, 0, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 64, 0, 0, 1, {0xE0, 0, 0}, 0, 2, 595 },
    { 0, 0, CPU_386, 0, 0, {MOD_Op0Add, 0, 0}, 0, 64, 0, 0, 1, {0xE0, 0, 0}, 0, 2, 597 },
    { 0, ONLY_64, 0, 0, 0, {MOD_Op0Add, 0, 0}, 0, 64, 0, 0, 1, {0xE0, 0, 0}, 0, 2, 599 }
};

static const x86_insn_info setcc_insn[] = {
    { SUF_B, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x90, 0}, 2, 1, 351 }
};

static const x86_insn_info cmpsd_insn[] = {
    { GAS_ILLEGAL, NOT_AVX, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xA7, 0, 0}, 0, 0, 0 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0xF2, 0, 2, {0x0F, 0xC2, 0}, 0, 3, 192 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0xF2, 0, 2, {0x0F, 0xC2, 0}, 0, 3, 195 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC3, 0, 2, {0x0F, 0xC2, 0}, 0, 4, 20 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC3, 0, 2, {0x0F, 0xC2, 0}, 0, 4, 24 }
};

static const x86_insn_info movsd_insn[] = {
    { 0, NOT_AVX, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0xA5, 0, 0}, 0, 0, 0 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0xF2, 0, 2, {0x0F, 0x10, 0}, 0, 2, 192 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0xF2, 0, 2, {0x0F, 0x10, 0}, 0, 2, 121 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0xF2, 0, 2, {0x0F, 0x11, 0}, 0, 2, 59 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC3, 0, 2, {0x0F, 0x10, 0}, 0, 3, 20 }
};

static const x86_insn_info bittest_insn[] = {
    { SUF_W, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 288 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 294 },
    { SUF_Q, ONLY_64, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 300 },
    { SUF_W, 0, CPU_386, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 16, 0, 0, 0, 2, {0x0F, 0xBA, 0}, 0, 2, 315 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 32, 0, 0, 0, 2, {0x0F, 0xBA, 0}, 0, 2, 317 },
    { SUF_Q, ONLY_64, CPU_386, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 64, 0, 0, 0, 2, {0x0F, 0xBA, 0}, 0, 2, 319 }
};

static const x86_insn_info bsfr_insn[] = {
    { SUF_W, 0, 0, 0, 0, {MOD_Op1Add, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 198 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 201 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_Op1Add, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 204 }
};

static const x86_insn_info int_insn[] = {
    { 0, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xCD, 0, 0}, 0, 1, 23 }
};

static const x86_insn_info bound_insn[] = {
    { SUF_W, NOT_64, CPU_186, 0, 0, {0, 0, 0}, 16, 0, 0, 0, 1, {0x62, 0, 0}, 0, 2, 523 },
    { SUF_L, NOT_64, CPU_386, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 1, {0x62, 0, 0}, 0, 2, 387 }
};

static const x86_insn_info arpl_insn[] = {
    { SUF_W, NOT_64, CPU_286, CPU_Prot, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0x63, 0, 0}, 0, 2, 288 }
};

static const x86_insn_info str_insn[] = {
    { SUF_W, 0, CPU_286, CPU_Prot, 0, {0, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x00, 0}, 1, 1, 423 },
    { SUF_L, 0, CPU_386, CPU_Prot, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x00, 0}, 1, 1, 34 },
    { SUF_Q, ONLY_64, CPU_286, CPU_Prot, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x00, 0}, 1, 1, 38 },
    { SUF_L|SUF_W, 0, CPU_286, CPU_Prot, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 1, 1, 199 }
};

static const x86_insn_info prot286_insn[] = {
    { SUF_W, 0, CPU_286, 0, 0, {MOD_SpAdd, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 1, 199 }
};

static const x86_insn_info sldtmsw_insn[] = {
    { SUF_W, 0, CPU_286, 0, 0, {MOD_SpAdd, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 1, 42 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_SpAdd, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 1, 70 },
    { SUF_Q, ONLY_64, CPU_286, 0, 0, {MOD_SpAdd, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 1, 26 },
    { SUF_W, 0, CPU_286, 0, 0, {MOD_SpAdd, MOD_Op1Add, 0}, 16, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 1, 423 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_SpAdd, MOD_Op1Add, 0}, 32, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 1, 34 },
    { SUF_Q, ONLY_64, CPU_286, 0, 0, {MOD_SpAdd, MOD_Op1Add, 0}, 64, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 1, 38 }
};

static const x86_insn_info fld_insn[] = {
    { SUF_S, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xD9, 0, 0}, 0, 1, 631 },
    { SUF_L, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xDD, 0, 0}, 0, 1, 632 },
    { 0, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xDB, 0, 0}, 5, 1, 629 },
    { 0, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0xD9, 0xC0, 0}, 0, 1, 356 }
};

static const x86_insn_info fstp_insn[] = {
    { SUF_S, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xD9, 0, 0}, 3, 1, 631 },
    { SUF_L, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xDD, 0, 0}, 3, 1, 632 },
    { 0, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xDB, 0, 0}, 7, 1, 629 },
    { 0, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0xDD, 0xD8, 0}, 0, 1, 356 }
};

static const x86_insn_info fldstpt_insn[] = {
    { 0, 0, CPU_FPU, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xDB, 0, 0}, 0, 1, 629 }
};

static const x86_insn_info fildstp_insn[] = {
    { SUF_S, 0, CPU_FPU, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xDF, 0, 0}, 0, 1, 630 },
    { SUF_L, 0, CPU_FPU, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xDB, 0, 0}, 0, 1, 631 },
    { SUF_Q, 0, CPU_FPU, 0, 0, {MOD_Gap, MOD_Op0Add, MOD_SpAdd}, 0, 0, 0, 0, 1, {0xDD, 0, 0}, 0, 1, 632 }
};

static const x86_insn_info fbldstp_insn[] = {
    { 0, 0, CPU_FPU, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xDF, 0, 0}, 0, 1, 550 }
};

static const x86_insn_info fst_insn[] = {
    { SUF_S, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xD9, 0, 0}, 2, 1, 631 },
    { SUF_L, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xDD, 0, 0}, 2, 1, 632 },
    { 0, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0xDD, 0xD0, 0}, 0, 1, 356 }
};

static const x86_insn_info fxch_insn[] = {
    { 0, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0xD9, 0xC8, 0}, 0, 1, 356 },
    { 0, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0xD9, 0xC8, 0}, 0, 2, 355 },
    { 0, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0xD9, 0xC8, 0}, 0, 2, 357 },
    { 0, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0xD9, 0xC9, 0}, 0, 0, 0 }
};

static const x86_insn_info fcom_insn[] = {
    { SUF_S, 0, CPU_FPU, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 0, 0, 0, 0, 1, {0xD8, 0, 0}, 0, 1, 631 },
    { SUF_L, 0, CPU_FPU, 0, 0, {MOD_Gap, MOD_SpAdd, 0}, 0, 0, 0, 0, 1, {0xDC, 0, 0}, 0, 1, 632 },
    { 0, 0, CPU_FPU, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0xD8, 0x00, 0}, 0, 1, 356 },
    { GAS_ONLY, 0, CPU_FPU, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0xD8, 0x01, 0}, 0, 0, 0 },
    { GAS_ILLEGAL, 0, CPU_FPU, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0xD8, 0x00, 0}, 0, 2, 355 }
};

static const x86_insn_info fcom2_insn[] = {
    { 0, 0, CPU_286, CPU_FPU, 0, {MOD_Op0Add, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0x00, 0x00, 0}, 0, 1, 356 },
    { 0, 0, CPU_286, CPU_FPU, 0, {MOD_Op0Add, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0x00, 0x00, 0}, 0, 2, 355 }
};

static const x86_insn_info farith_insn[] = {
    { SUF_S, 0, CPU_FPU, 0, 0, {MOD_Gap, MOD_Gap, MOD_SpAdd}, 0, 0, 0, 0, 1, {0xD8, 0, 0}, 0, 1, 631 },
    { SUF_L, 0, CPU_FPU, 0, 0, {MOD_Gap, MOD_Gap, MOD_SpAdd}, 0, 0, 0, 0, 1, {0xDC, 0, 0}, 0, 1, 632 },
    { 0, 0, CPU_FPU, 0, 0, {MOD_Gap, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0xD8, 0x00, 0}, 0, 1, 356 },
    { 0, 0, CPU_FPU, 0, 0, {MOD_Gap, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0xD8, 0x00, 0}, 0, 2, 355 },
    { 0, 0, CPU_FPU, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0xDC, 0x00, 0}, 0, 1, 669 },
    { GAS_ILLEGAL, 0, CPU_FPU, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0xDC, 0x00, 0}, 0, 2, 357 },
    { GAS_ONLY, 0, CPU_FPU, 0, 0, {MOD_Gap, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0xDC, 0x00, 0}, 0, 2, 357 }
};

static const x86_insn_info farithp_insn[] = {
    { 0, 0, CPU_FPU, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0xDE, 0x01, 0}, 0, 0, 0 },
    { 0, 0, CPU_FPU, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0xDE, 0x00, 0}, 0, 1, 356 },
    { 0, 0, CPU_FPU, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0xDE, 0x00, 0}, 0, 2, 357 }
};

static const x86_insn_info fiarith_insn[] = {
    { SUF_S, 0, CPU_FPU, 0, 0, {MOD_SpAdd, MOD_Op0Add, 0}, 0, 0, 0, 0, 1, {0x04, 0, 0}, 0, 1, 630 },
    { SUF_L, 0, CPU_FPU, 0, 0, {MOD_SpAdd, MOD_Op0Add, 0}, 0, 0, 0, 0, 1, {0x00, 0, 0}, 0, 1, 631 }
};

static const x86_insn_info fldnstcw_insn[] = {
    { SUF_W, 0, CPU_FPU, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 1, {0xD9, 0, 0}, 0, 1, 42 }
};

static const x86_insn_info fstcw_insn[] = {
    { SUF_W, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x9B, 0xD9, 0}, 7, 1, 42 }
};

static const x86_insn_info fnstsw_insn[] = {
    { SUF_W, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 1, {0xDD, 0, 0}, 7, 1, 42 },
    { SUF_W, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0xDF, 0xE0, 0}, 0, 1, 371 }
};

static const x86_insn_info fstsw_insn[] = {
    { SUF_W, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x9B, 0xDD, 0}, 7, 1, 42 },
    { SUF_W, 0, CPU_FPU, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 3, {0x9B, 0xDF, 0xE0}, 0, 1, 371 }
};

static const x86_insn_info ffree_insn[] = {
    { 0, 0, CPU_FPU, 0, 0, {MOD_Op0Add, 0, 0}, 0, 0, 0, 0, 2, {0x00, 0xC0, 0}, 0, 1, 356 }
};

static const x86_insn_info bswap_insn[] = {
    { SUF_L, 0, CPU_486, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0xC8, 0}, 0, 1, 670 },
    { SUF_Q, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0xC8, 0}, 0, 1, 671 }
};

static const x86_insn_info cmpxchgxadd_insn[] = {
    { SUF_B, 0, CPU_486, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 351 },
    { SUF_W, 0, CPU_486, 0, 0, {MOD_Op1Add, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 2, 288 },
    { SUF_L, 0, CPU_486, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 2, 294 },
    { SUF_Q, ONLY_64, CPU_486, 0, 0, {MOD_Op1Add, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x01, 0}, 0, 2, 300 }
};

static const x86_insn_info cmpxchg8b_insn[] = {
    { SUF_Q, 0, CPU_586, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xC7, 0}, 1, 1, 26 }
};

static const x86_insn_info cmovcc_insn[] = {
    { SUF_W, 0, CPU_686, 0, 0, {MOD_Op1Add, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x40, 0}, 0, 2, 198 },
    { SUF_L, 0, CPU_686, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x40, 0}, 0, 2, 201 },
    { SUF_Q, ONLY_64, CPU_686, 0, 0, {MOD_Op1Add, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x40, 0}, 0, 2, 204 }
};

static const x86_insn_info fcmovcc_insn[] = {
    { 0, 0, CPU_686, CPU_FPU, 0, {MOD_Op0Add, MOD_Op1Add, 0}, 0, 0, 0, 0, 2, {0x00, 0x00, 0}, 0, 2, 355 }
};

static const x86_insn_info movnti_insn[] = {
    { SUF_L, 0, CPU_P4, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xC3, 0}, 0, 2, 365 },
    { SUF_Q, ONLY_64, CPU_P4, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0xC3, 0}, 0, 2, 367 }
};

static const x86_insn_info clflush_insn[] = {
    { 0, 0, CPU_P3, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xAE, 0}, 7, 1, 62 }
};

static const x86_insn_info movd_insn[] = {
    { 0, 0, CPU_386, CPU_MMX, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x6E, 0}, 0, 2, 321 },
    { 0, ONLY_64, CPU_MMX, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x6E, 0}, 0, 2, 323 },
    { 0, 0, CPU_386, CPU_MMX, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 322 },
    { 0, ONLY_64, CPU_MMX, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 325 },
    { 0, 0, CPU_386, CPU_SSE2, 0, {0, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0x6E, 0}, 0, 2, 327 },
    { 0, ONLY_64, CPU_SSE2, 0, 0, {0, 0, 0}, 64, 0, 0x66, 0, 2, {0x0F, 0x6E, 0}, 0, 2, 329 },
    { 0, 0, CPU_386, CPU_SSE2, 0, {0, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 270 },
    { 0, ONLY_64, CPU_SSE2, 0, 0, {0, 0, 0}, 64, 0, 0x66, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 267 }
};

static const x86_insn_info movq_insn[] = {
    { GAS_ILLEGAL, 0, CPU_MMX, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x6F, 0}, 0, 2, 282 },
    { GAS_ILLEGAL, ONLY_64, CPU_MMX, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x6E, 0}, 0, 2, 323 },
    { GAS_ILLEGAL, 0, CPU_MMX, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x7F, 0}, 0, 2, 359 },
    { GAS_ILLEGAL, ONLY_64, CPU_MMX, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 325 },
    { GAS_ILLEGAL, 0, CPU_SSE2, 0, 0, {0, 0, 0}, 0, 0, 0xF3, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 112 },
    { GAS_ILLEGAL, 0, CPU_SSE2, 0, 0, {0, 0, 0}, 0, 0, 0xF3, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 361 },
    { GAS_ILLEGAL, ONLY_64, CPU_SSE2, 0, 0, {0, 0, 0}, 64, 0, 0x66, 0, 2, {0x0F, 0x6E, 0}, 0, 2, 329 },
    { GAS_ILLEGAL, 0, CPU_SSE2, 0, 0, {0, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0xD6, 0}, 0, 2, 363 },
    { GAS_ILLEGAL, ONLY_64, CPU_SSE2, 0, 0, {0, 0, 0}, 64, 0, 0x66, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 267 }
};

static const x86_insn_info mmxsse2_insn[] = {
    { 0, 0, CPU_MMX, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 282 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0x00, 0}, 0, 2, 81 }
};

static const x86_insn_info pshift_insn[] = {
    { 0, 0, CPU_MMX, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 282 },
    { 0, 0, CPU_MMX, 0, 0, {MOD_Gap, MOD_Op1Add, MOD_SpAdd}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 247 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0x00, 0}, 0, 2, 81 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_Gap, MOD_Op1Add, MOD_SpAdd}, 0, 0, 0x66, 0, 2, {0x0F, 0x00, 0}, 0, 2, 22 }
};

static const x86_insn_info vpshift_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0xC1, 0, 2, {0x0F, 0x00, 0}, 0, 2, 243 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Gap, MOD_Op1Add, MOD_SpAdd}, 0, 0, 0xC1, 0, 2, {0x0F, 0x00, 0}, 0, 2, 505 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0xC1, 0, 2, {0x0F, 0x00, 0}, 0, 3, 0 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Gap, MOD_Op1Add, MOD_SpAdd}, 0, 0, 0xC1, 0, 2, {0x0F, 0x00, 0}, 0, 3, 21 }
};

static const x86_insn_info xmm_xmm128_256_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 243 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 0 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC4, 0, 2, {0x0F, 0x00, 0}, 0, 3, 10 }
};

static const x86_insn_info xmm_xmm128_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 243 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 0 }
};

static const x86_insn_info cvt_rx_xmm32_insn[] = {
    { SUF_L, 0, CPU_386, CPU_SSE, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 249 },
    { SUF_L, 0, CPU_386, CPU_SSE, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 387 },
    { SUF_Q, ONLY_64, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 64, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 255 },
    { SUF_Q, ONLY_64, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 64, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 389 }
};

static const x86_insn_info cvt_mm_xmm64_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 343 },
    { 0, 0, CPU_SSE, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 345 }
};

static const x86_insn_info cvt_xmm_mm_ps_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 361 }
};

static const x86_insn_info cvt_xmm_rmx_insn[] = {
    { SUF_L, 0, CPU_386, CPU_SSE, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 625 },
    { SUF_L, NOT_64, CPU_386, CPU_SSE, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 234 },
    { SUF_Q, ONLY_64, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 64, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 627 },
    { SUF_L, ONLY_AVX|NOT_64, CPU_386, CPU_AVX, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 44 },
    { SUF_L, ONLY_AVX, CPU_386, CPU_AVX, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 309 },
    { SUF_Q, ONLY_64|ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 64, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 312 }
};

static const x86_insn_info xmm_xmm32_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 192 },
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 240 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 20 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 68 }
};

static const x86_insn_info ssecmp_128_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_Imm8, MOD_PreAdd, MOD_SetVEX}, 0, 0, 0, 0, 2, {0x0F, 0xC2, 0}, 0, 2, 243 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Imm8, MOD_PreAdd, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0xC2, 0}, 0, 3, 0 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Imm8, MOD_PreAdd, 0}, 0, 0, 0xC4, 0, 2, {0x0F, 0xC2, 0}, 0, 3, 10 }
};

static const x86_insn_info ssecmp_32_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_Imm8, MOD_PreAdd, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0xC2, 0}, 0, 2, 192 },
    { 0, 0, CPU_SSE, 0, 0, {MOD_Imm8, MOD_PreAdd, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0xC2, 0}, 0, 2, 240 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Imm8, MOD_PreAdd, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0xC2, 0}, 0, 3, 20 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Imm8, MOD_PreAdd, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0xC2, 0}, 0, 3, 68 }
};

static const x86_insn_info xmm_xmm128_imm_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 125 }
};

static const x86_insn_info xmm_xmm128_imm_256_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 243 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 4, 72 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC4, 0, 2, {0x0F, 0x00, 0}, 0, 4, 28 }
};

static const x86_insn_info xmm_xmm32_imm_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 3, 192 },
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 3, 240 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 4, 20 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 4, 68 }
};

static const x86_insn_info ldstmxcsr_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_SpAdd, MOD_SetVEX, 0}, 0, 0, 0, 0, 2, {0x0F, 0xAE, 0}, 0, 1, 70 }
};

static const x86_insn_info maskmovq_insn[] = {
    { 0, 0, CPU_MMX, CPU_P3, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xF7, 0}, 0, 2, 619 }
};

static const x86_insn_info movau_insn[] = {
    { 0, NOT_AVX, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 81 },
    { 0, NOT_AVX, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_Op1Add}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 134 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 81 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_Op1Add}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 134 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC4, 0, 2, {0x0F, 0x00, 0}, 0, 2, 273 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_Op1Add}, 0, 0, 0xC4, 0, 2, {0x0F, 0x00, 0}, 0, 2, 489 }
};

static const x86_insn_info movhllhps_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_Op1Add, MOD_SetVEX, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 192 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 20 }
};

static const x86_insn_info movhlp_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 195 },
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x01, 0}, 0, 2, 59 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 24 }
};

static const x86_insn_info movmsk_insn[] = {
    { SUF_L, 0, CPU_386, CPU_SSE, 0, {MOD_PreAdd, MOD_SetVEX, 0}, 0, 0, 0x00, 0, 2, {0x0F, 0x50, 0}, 0, 2, 249 },
    { SUF_Q, ONLY_64, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_SetVEX, 0}, 64, 0, 0x00, 0, 2, {0x0F, 0x50, 0}, 0, 2, 255 },
    { SUF_L, ONLY_AVX, CPU_386, CPU_AVX, 0, {MOD_PreAdd, 0, 0}, 0, 0, 0xC4, 0, 2, {0x0F, 0x50, 0}, 0, 2, 347 },
    { SUF_Q, ONLY_64|ONLY_AVX, CPU_SSE, 0, 0, {MOD_PreAdd, 0, 0}, 64, 0, 0xC4, 0, 2, {0x0F, 0x50, 0}, 0, 2, 349 }
};

static const x86_insn_info movnt_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 579 }
};

static const x86_insn_info movntq_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xE7, 0}, 0, 2, 391 }
};

static const x86_insn_info movss_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0xF3, 0, 2, {0x0F, 0x10, 0}, 0, 2, 192 },
    { 0, 0, CPU_SSE, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0xF3, 0, 2, {0x0F, 0x10, 0}, 0, 2, 173 },
    { 0, 0, CPU_SSE, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0xF3, 0, 2, {0x0F, 0x11, 0}, 0, 2, 182 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC2, 0, 2, {0x0F, 0x10, 0}, 0, 3, 20 }
};

static const x86_insn_info pextrw_insn[] = {
    { SUF_L, NOT_AVX, CPU_MMX, CPU_P3, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xC5, 0}, 0, 3, 246 },
    { SUF_L, 0, CPU_386, CPU_SSE2, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0xC5, 0}, 0, 3, 249 },
    { SUF_Q, ONLY_64|NOT_AVX, CPU_MMX, CPU_P3, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0xC5, 0}, 0, 3, 252 },
    { SUF_Q, ONLY_64, CPU_SSE2, 0, 0, {MOD_SetVEX, 0, 0}, 64, 0, 0x66, 0, 2, {0x0F, 0xC5, 0}, 0, 3, 255 },
    { 0, 0, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x15}, 0, 3, 258 },
    { 0, 0, CPU_386, CPU_SSE41, 0, {MOD_SetVEX, 0, 0}, 32, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x15}, 0, 3, 261 },
    { 0, ONLY_64, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 64, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x15}, 0, 3, 264 }
};

static const x86_insn_info pinsrw_insn[] = {
    { SUF_L, NOT_AVX, CPU_MMX, CPU_P3, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xC4, 0}, 0, 3, 216 },
    { SUF_Q, ONLY_64|NOT_AVX, CPU_MMX, CPU_P3, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0xC4, 0}, 0, 3, 219 },
    { SUF_L, NOT_AVX, CPU_MMX, CPU_P3, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xC4, 0}, 0, 3, 222 },
    { SUF_L, 0, CPU_386, CPU_SSE2, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0xC4, 0}, 0, 3, 225 },
    { SUF_Q, ONLY_64, CPU_SSE2, 0, 0, {MOD_SetVEX, 0, 0}, 64, 0, 0x66, 0, 2, {0x0F, 0xC4, 0}, 0, 3, 228 },
    { SUF_L, 0, CPU_SSE2, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0xC4, 0}, 0, 3, 231 },
    { SUF_L, ONLY_AVX, CPU_386, CPU_AVX, 0, {0, 0, 0}, 0, 0, 0xC1, 0, 2, {0x0F, 0xC4, 0}, 0, 4, 32 },
    { SUF_Q, ONLY_64|ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 64, 0, 0xC1, 0, 2, {0x0F, 0xC4, 0}, 0, 4, 36 },
    { SUF_L, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC1, 0, 2, {0x0F, 0xC4, 0}, 0, 4, 40 }
};

static const x86_insn_info pmovmskb_insn[] = {
    { SUF_L, NOT_AVX, CPU_MMX, CPU_P3, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xD7, 0}, 0, 2, 246 },
    { SUF_L, 0, CPU_386, CPU_SSE2, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0xD7, 0}, 0, 2, 249 },
    { SUF_Q, ONLY_64|NOT_AVX, CPU_MMX, CPU_P3, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0xD7, 0}, 0, 2, 252 },
    { SUF_Q, ONLY_64, CPU_SSE2, 0, 0, {MOD_SetVEX, 0, 0}, 64, 0, 0x66, 0, 2, {0x0F, 0xD7, 0}, 0, 2, 255 }
};

static const x86_insn_info pshufw_insn[] = {
    { 0, 0, CPU_MMX, CPU_P3, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x70, 0}, 0, 3, 282 }
};

static const x86_insn_info xmm_xmm64_insn[] = {
    { 0, 0, CPU_SSE2, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 192 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 195 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 20 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 3, 24 }
};

static const x86_insn_info ssecmp_64_insn[] = {
    { 0, 0, CPU_SSE2, 0, 0, {MOD_Imm8, MOD_PreAdd, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0xC2, 0}, 0, 2, 192 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_Imm8, MOD_PreAdd, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0xC2, 0}, 0, 2, 195 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Imm8, MOD_PreAdd, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0xC2, 0}, 0, 3, 20 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Imm8, MOD_PreAdd, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0xC2, 0}, 0, 3, 24 }
};

static const x86_insn_info cvt_rx_xmm64_insn[] = {
    { SUF_L, 0, CPU_386, CPU_SSE2, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 249 },
    { SUF_L, 0, CPU_386, CPU_SSE2, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 366 },
    { SUF_Q, ONLY_64, CPU_SSE2, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 64, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 255 },
    { SUF_Q, ONLY_64, CPU_SSE2, 0, 0, {MOD_PreAdd, MOD_Op1Add, MOD_SetVEX}, 64, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 525 }
};

static const x86_insn_info cvt_mm_xmm_insn[] = {
    { 0, 0, CPU_SSE2, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 601 }
};

static const x86_insn_info cvt_xmm_mm_ss_insn[] = {
    { 0, 0, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 2, 361 }
};

static const x86_insn_info vmxmemrd_insn[] = {
    { SUF_L, NOT_64, CPU_P4, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x78, 0}, 0, 2, 294 },
    { SUF_Q, ONLY_64, CPU_P4, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 2, {0x0F, 0x78, 0}, 0, 2, 300 }
};

static const x86_insn_info vmxmemwr_insn[] = {
    { SUF_L, NOT_64, CPU_P4, 0, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x79, 0}, 0, 2, 201 },
    { SUF_Q, ONLY_64, CPU_P4, 0, 0, {0, 0, 0}, 64, 64, 0, 0, 2, {0x0F, 0x79, 0}, 0, 2, 204 }
};

static const x86_insn_info vmxtwobytemem_insn[] = {
    { 0, 0, CPU_P4, 0, 0, {MOD_SpAdd, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0xC7, 0}, 0, 1, 26 }
};

static const x86_insn_info vmxthreebytemem_insn[] = {
    { 0, 0, CPU_P4, 0, 0, {MOD_PreAdd, 0, 0}, 0, 0, 0x00, 0, 2, {0x0F, 0xC7, 0}, 6, 1, 26 }
};

static const x86_insn_info maskmovdqu_insn[] = {
    { 0, 0, CPU_SSE2, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0xF7, 0}, 0, 2, 112 }
};

static const x86_insn_info movdq2q_insn[] = {
    { 0, 0, CPU_SSE2, 0, 0, {0, 0, 0}, 0, 0, 0xF2, 0, 2, {0x0F, 0xD6, 0}, 0, 2, 343 }
};

static const x86_insn_info movq2dq_insn[] = {
    { 0, 0, CPU_SSE2, 0, 0, {0, 0, 0}, 0, 0, 0xF3, 0, 2, {0x0F, 0xD6, 0}, 0, 2, 473 }
};

static const x86_insn_info pslrldq_insn[] = {
    { 0, 0, CPU_SSE2, 0, 0, {MOD_SpAdd, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0x73, 0}, 0, 2, 505 },
    { 0, 0, CPU_SSE2, 0, 0, {MOD_SpAdd, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0x73, 0}, 0, 3, 21 }
};

static const x86_insn_info lddqu_insn[] = {
    { 0, 0, CPU_SSE3, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0xF2, 0, 2, {0x0F, 0xF0, 0}, 0, 2, 577 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC7, 0, 2, {0x0F, 0xF0, 0}, 0, 2, 617 }
};

static const x86_insn_info ssse3_insn[] = {
    { 0, NOT_AVX, CPU_SSSE3, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 282 },
    { 0, 0, CPU_SSSE3, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 243 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x38, 0x00}, 0, 3, 0 }
};

static const x86_insn_info ssse3imm_insn[] = {
    { 0, 0, CPU_SSSE3, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 282 },
    { 0, 0, CPU_SSSE3, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 125 }
};

static const x86_insn_info sse4_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 81 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 273 }
};

static const x86_insn_info sse4imm_256_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 243 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 72 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 28 }
};

static const x86_insn_info sse4imm_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 243 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 72 }
};

static const x86_insn_info sse4m32imm_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 192 },
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 240 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 20 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 68 }
};

static const x86_insn_info sse4m64imm_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 192 },
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 195 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 20 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 24 }
};

static const x86_insn_info sse4xmm0_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 81 },
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x00}, 0, 3, 279 }
};

static const x86_insn_info avx_sse4xmm0_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 0 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 10 }
};

static const x86_insn_info crc32_insn[] = {
    { SUF_B, 0, CPU_386, CPU_SSE42, 0, {0, 0, 0}, 0, 0, 0xF2, 0, 3, {0x0F, 0x38, 0xF0}, 0, 2, 527 },
    { SUF_W, 0, CPU_386, CPU_SSE42, 0, {0, 0, 0}, 16, 0, 0xF2, 0, 3, {0x0F, 0x38, 0xF1}, 0, 2, 529 },
    { SUF_L, 0, CPU_386, CPU_SSE42, 0, {0, 0, 0}, 32, 0, 0xF2, 0, 3, {0x0F, 0x38, 0xF1}, 0, 2, 201 },
    { SUF_B, ONLY_64, CPU_SSE42, 0, 0, {0, 0, 0}, 64, 0, 0xF2, 0, 3, {0x0F, 0x38, 0xF0}, 0, 2, 531 },
    { SUF_Q, ONLY_64, CPU_SSE42, 0, 0, {0, 0, 0}, 64, 0, 0xF2, 0, 3, {0x0F, 0x38, 0xF1}, 0, 2, 204 }
};

static const x86_insn_info extractps_insn[] = {
    { 0, 0, CPU_386, CPU_SSE41, 0, {MOD_SetVEX, 0, 0}, 32, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x17}, 0, 3, 270 },
    { 0, ONLY_64, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 64, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x17}, 0, 3, 264 }
};

static const x86_insn_info insertps_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x21}, 0, 3, 240 },
    { 0, 0, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x21}, 0, 3, 192 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x21}, 0, 4, 68 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x21}, 0, 4, 20 }
};

static const x86_insn_info movntdqa_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x2A}, 0, 2, 577 }
};

static const x86_insn_info sse4pcmpstr_insn[] = {
    { 0, 0, CPU_SSE42, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 125 }
};

static const x86_insn_info pextrb_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x14}, 0, 3, 276 },
    { 0, 0, CPU_386, CPU_SSE41, 0, {MOD_SetVEX, 0, 0}, 32, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x14}, 0, 3, 261 },
    { 0, ONLY_64, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 64, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x14}, 0, 3, 264 }
};

static const x86_insn_info pextrd_insn[] = {
    { 0, 0, CPU_386, CPU_SSE41, 0, {MOD_SetVEX, 0, 0}, 32, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x16}, 0, 3, 270 }
};

static const x86_insn_info pextrq_insn[] = {
    { 0, ONLY_64, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 64, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x16}, 0, 3, 267 }
};

static const x86_insn_info pinsrb_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x20}, 0, 3, 237 },
    { 0, 0, CPU_386, CPU_SSE41, 0, {MOD_SetVEX, 0, 0}, 32, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x20}, 0, 3, 225 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x20}, 0, 4, 60 },
    { 0, ONLY_AVX, CPU_386, CPU_AVX, 0, {0, 0, 0}, 32, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x20}, 0, 4, 64 }
};

static const x86_insn_info pinsrd_insn[] = {
    { 0, 0, CPU_386, CPU_SSE41, 0, {MOD_SetVEX, 0, 0}, 32, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x22}, 0, 3, 234 },
    { 0, ONLY_AVX, CPU_386, CPU_AVX, 0, {0, 0, 0}, 32, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x22}, 0, 4, 44 }
};

static const x86_insn_info pinsrq_insn[] = {
    { 0, ONLY_64, CPU_SSE41, 0, 0, {MOD_SetVEX, 0, 0}, 64, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x22}, 0, 3, 285 },
    { 0, ONLY_64|ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 64, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x22}, 0, 4, 92 }
};

static const x86_insn_info sse4m16_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 475 },
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 112 }
};

static const x86_insn_info sse4m32_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 173 },
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 112 }
};

static const x86_insn_info sse4m64_insn[] = {
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 121 },
    { 0, 0, CPU_SSE41, 0, 0, {MOD_Op2Add, MOD_SetVEX, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 112 }
};

static const x86_insn_info cnt_insn[] = {
    { SUF_W, 0, 0, 0, 0, {MOD_Op1Add, 0, 0}, 16, 0, 0xF3, 0, 2, {0x0F, 0x00, 0}, 0, 2, 198 },
    { SUF_L, 0, CPU_386, 0, 0, {MOD_Op1Add, 0, 0}, 32, 0, 0xF3, 0, 2, {0x0F, 0x00, 0}, 0, 2, 201 },
    { SUF_Q, ONLY_64, 0, 0, 0, {MOD_Op1Add, 0, 0}, 64, 0, 0xF3, 0, 2, {0x0F, 0x00, 0}, 0, 2, 204 }
};

static const x86_insn_info vmovd_insn[] = {
    { 0, ONLY_AVX, CPU_386, CPU_AVX, 0, {0, 0, 0}, 0, 0, 0xC1, 0, 2, {0x0F, 0x6E, 0}, 0, 2, 327 },
    { 0, ONLY_AVX, CPU_386, CPU_AVX, 0, {0, 0, 0}, 0, 0, 0xC1, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 270 }
};

static const x86_insn_info vmovq_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC2, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 112 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC2, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 121 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC1, 0, 2, {0x0F, 0xD6, 0}, 0, 2, 59 },
    { 0, ONLY_64|ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 64, 0, 0xC1, 0, 2, {0x0F, 0x6E, 0}, 0, 2, 329 },
    { 0, ONLY_64|ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 64, 0, 0xC1, 0, 2, {0x0F, 0x7E, 0}, 0, 2, 267 }
};

static const x86_insn_info avx_xmm_xmm128_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 81 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC4, 0, 2, {0x0F, 0x00, 0}, 0, 2, 273 }
};

static const x86_insn_info avx_sse4imm_insn[] = {
    { 0, ONLY_AVX, CPU_SSE41, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 125 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 125 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 273 }
};

static const x86_insn_info vmovddup_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 112 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 121 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC4, 0, 2, {0x0F, 0x00, 0}, 0, 2, 273 }
};

static const x86_insn_info avx_xmm_xmm64_insn[] = {
    { 0, ONLY_AVX, CPU_SSE2, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 112 },
    { 0, ONLY_AVX, CPU_SSE2, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 121 }
};

static const x86_insn_info avx_xmm_xmm32_insn[] = {
    { 0, ONLY_AVX, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 112 },
    { 0, ONLY_AVX, CPU_SSE, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 173 }
};

static const x86_insn_info avx_cvt_xmm64_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 112 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 121 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC4, 0, 2, {0x0F, 0x00, 0}, 0, 2, 477 }
};

static const x86_insn_info avx_ssse3_2op_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x38, 0x00}, 0, 2, 81 }
};

static const x86_insn_info avx_cvt_xmm128_x_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 81 }
};

static const x86_insn_info avx_cvt_xmm128_y_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC4, 0, 2, {0x0F, 0x00, 0}, 0, 2, 609 }
};

static const x86_insn_info avx_cvt_xmm128_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 585 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_PreAdd, MOD_Op1Add, 0}, 0, 0, 0xC4, 0, 2, {0x0F, 0x00, 0}, 0, 2, 587 }
};

static const x86_insn_info vbroadcastss_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x38, 0x18}, 0, 2, 173 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x38, 0x18}, 0, 2, 491 }
};

static const x86_insn_info vbroadcastsd_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x38, 0x19}, 0, 2, 487 }
};

static const x86_insn_info vbroadcastf128_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x38, 0x1A}, 0, 2, 605 }
};

static const x86_insn_info vextractf128_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x3A, 0x19}, 0, 3, 306 }
};

static const x86_insn_info vinsertf128_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x3A, 0x18}, 0, 4, 76 }
};

static const x86_insn_info vzero_insn[] = {
    { 0, 0, CPU_AVX, 0, 0, {MOD_SetVEX, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x77, 0}, 0, 0, 0 }
};

static const x86_insn_info vmaskmov_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x38, 0x00}, 0, 3, 0 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x38, 0x00}, 0, 3, 10 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x38, 0x02}, 0, 3, 98 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x38, 0x02}, 0, 3, 106 }
};

static const x86_insn_info vpermil_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x38, 0x08}, 0, 3, 0 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x38, 0x08}, 0, 3, 10 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 125 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x3A, 0x00}, 0, 3, 273 }
};

static const x86_insn_info vpermil2_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 5, 0 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC9, 0, 3, {0x0F, 0x3A, 0x00}, 0, 5, 5 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x3A, 0x00}, 0, 5, 10 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xCD, 0, 3, {0x0F, 0x3A, 0x00}, 0, 5, 15 }
};

static const x86_insn_info vpermil2_fixed_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, MOD_Imm8, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 0 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, MOD_Imm8, 0}, 0, 0, 0xC9, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 5 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, MOD_Imm8, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 10 },
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {MOD_Op2Add, MOD_Imm8, 0}, 0, 0, 0xCD, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 15 }
};

static const x86_insn_info vperm2f128_insn[] = {
    { 0, ONLY_AVX, CPU_AVX, 0, 0, {0, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x3A, 0x06}, 0, 4, 28 }
};

static const x86_insn_info fma_128_256_insn[] = {
    { 0, ONLY_AVX, CPU_FMA, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 96 },
    { 0, ONLY_AVX, CPU_FMA, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC9, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 100 },
    { 0, ONLY_AVX, CPU_FMA, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC5, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 104 },
    { 0, ONLY_AVX, CPU_FMA, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xCD, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 108 }
};

static const x86_insn_info fma_128_m32_insn[] = {
    { 0, ONLY_AVX, CPU_FMA, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 48 },
    { 0, ONLY_AVX, CPU_FMA, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 84 },
    { 0, ONLY_AVX, CPU_FMA, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC9, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 88 }
};

static const x86_insn_info fma_128_m64_insn[] = {
    { 0, ONLY_AVX, CPU_FMA, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 48 },
    { 0, ONLY_AVX, CPU_FMA, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC1, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 52 },
    { 0, ONLY_AVX, CPU_FMA, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0xC9, 0, 3, {0x0F, 0x3A, 0x00}, 0, 4, 56 }
};

static const x86_insn_info aes_insn[] = {
    { 0, 0, CPU_AES, 0, 0, {MOD_Op1Add, MOD_Op2Add, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x00, 0x00}, 0, 2, 81 }
};

static const x86_insn_info aes_imm_insn[] = {
    { 0, 0, CPU_AES, 0, 0, {MOD_Op1Add, MOD_Op2Add, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x00, 0x00}, 0, 3, 125 }
};

static const x86_insn_info pclmulqdq_fixed_insn[] = {
    { 0, 0, CPU_CLMUL, 0, 0, {MOD_Imm8, 0, 0}, 0, 0, 0x66, 0, 3, {0x0F, 0x3A, 0x44}, 0, 2, 81 }
};

static const x86_insn_info extrq_insn[] = {
    { 0, 0, CPU_SSE4a, 0, 0, {0, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0x78, 0}, 0, 3, 113 },
    { 0, 0, CPU_SSE4a, 0, 0, {0, 0, 0}, 0, 0, 0x66, 0, 2, {0x0F, 0x79, 0}, 0, 2, 112 }
};

static const x86_insn_info insertq_insn[] = {
    { 0, 0, CPU_SSE4a, 0, 0, {0, 0, 0}, 0, 0, 0xF2, 0, 2, {0x0F, 0x78, 0}, 0, 4, 112 },
    { 0, 0, CPU_SSE4a, 0, 0, {0, 0, 0}, 0, 0, 0xF2, 0, 2, {0x0F, 0x79, 0}, 0, 2, 112 }
};

static const x86_insn_info movntsd_insn[] = {
    { 0, 0, CPU_SSE4a, 0, 0, {0, 0, 0}, 0, 0, 0xF2, 0, 2, {0x0F, 0x2B, 0}, 0, 2, 59 }
};

static const x86_insn_info movntss_insn[] = {
    { 0, 0, CPU_SSE4a, 0, 0, {0, 0, 0}, 0, 0, 0xF3, 0, 2, {0x0F, 0x2B, 0}, 0, 2, 182 }
};

static const x86_insn_info sse5com_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x25, 0x00}, 0, 4, 124 }
};

static const x86_insn_info sse5com32_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x25, 0x00}, 0, 4, 116 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x25, 0x00}, 0, 4, 172 }
};

static const x86_insn_info sse5com64_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x25, 0x00}, 0, 4, 116 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x25, 0x00}, 0, 4, 120 }
};

static const x86_insn_info sse5comcc_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, MOD_Imm8, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x25, 0x00}, 0, 3, 80 }
};

static const x86_insn_info sse5comcc32_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, MOD_Imm8, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x25, 0x00}, 0, 3, 116 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, MOD_Imm8, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x25, 0x00}, 0, 3, 172 }
};

static const x86_insn_info sse5comcc64_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, MOD_Imm8, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x25, 0x00}, 0, 3, 116 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, MOD_Imm8, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x25, 0x00}, 0, 3, 120 }
};

static const x86_insn_info cvtph2ps_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x7A, 0x30}, 0, 2, 112 },
    { 0, 0, CPU_SSE5, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x7A, 0x30}, 0, 2, 121 }
};

static const x86_insn_info cvtps2ph_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x7A, 0x31}, 0, 2, 150 },
    { 0, 0, CPU_SSE5, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x7A, 0x31}, 0, 2, 59 }
};

static const x86_insn_info sse5arith_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x00}, 0, 4, 128 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x00}, 0, 4, 132 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x04}, 0, 4, 80 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x04}, 0, 4, 136 }
};

static const x86_insn_info sse5arith32_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x00}, 0, 4, 140 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x00}, 0, 4, 176 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x00}, 0, 4, 148 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x00}, 0, 4, 180 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x04}, 0, 4, 156 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x04}, 0, 4, 184 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x04}, 0, 4, 164 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x04}, 0, 4, 188 }
};

static const x86_insn_info sse5arith64_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x00}, 0, 4, 140 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x00}, 0, 4, 144 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x00}, 0, 4, 148 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x00}, 0, 4, 152 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x04}, 0, 4, 156 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x04}, 0, 4, 160 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x04}, 0, 4, 164 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x04}, 0, 4, 168 }
};

static const x86_insn_info sse5two_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x7A, 0x00}, 0, 2, 81 }
};

static const x86_insn_info sse5two32_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x7A, 0x00}, 0, 2, 112 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x7A, 0x00}, 0, 2, 173 }
};

static const x86_insn_info sse5two64_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x7A, 0x00}, 0, 2, 112 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x7A, 0x00}, 0, 2, 121 }
};

static const x86_insn_info sse5pmacs_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x00}, 0, 4, 80 }
};

static const x86_insn_info sse5prot_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x40}, 0, 3, 80 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x40}, 0, 3, 136 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x7B, 0x40}, 0, 3, 125 }
};

static const x86_insn_info sse5psh_insn[] = {
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x80, 3, {0x0F, 0x24, 0x44}, 0, 3, 80 },
    { 0, 0, CPU_SSE5, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0x88, 3, {0x0F, 0x24, 0x44}, 0, 3, 136 }
};

static const x86_insn_info now3d_insn[] = {
    { 0, 0, CPU_3DNow, 0, 0, {MOD_Imm8, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x0F, 0}, 0, 2, 282 }
};

static const x86_insn_info cmpxchg16b_insn[] = {
    { 0, ONLY_64, 0, 0, 0, {0, 0, 0}, 64, 0, 0, 0, 2, {0x0F, 0xC7, 0}, 1, 1, 578 }
};

static const x86_insn_info invlpga_insn[] = {
    { 0, 0, CPU_SVM, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x01, 0xDF}, 0, 0, 0 },
    { 0, 0, CPU_386, CPU_SVM, 0, {0, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x01, 0xDF}, 0, 2, 507 }
};

static const x86_insn_info skinit_insn[] = {
    { 0, 0, CPU_SVM, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x01, 0xDE}, 0, 0, 0 },
    { 0, 0, CPU_SVM, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x01, 0xDE}, 0, 1, 648 }
};

static const x86_insn_info svm_rax_insn[] = {
    { 0, 0, CPU_SVM, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x01, 0x00}, 0, 0, 0 },
    { 0, 0, CPU_SVM, 0, 0, {MOD_Op2Add, 0, 0}, 0, 0, 0, 0, 3, {0x0F, 0x01, 0x00}, 0, 1, 507 }
};

static const x86_insn_info padlock_insn[] = {
    { 0, 0, CPU_PadLock, 0, 0, {MOD_Imm8, MOD_PreAdd, MOD_Op1Add}, 0, 0, 0x00, 0, 2, {0x0F, 0x00, 0}, 0, 0, 0 }
};

static const x86_insn_info cyrixmmx_insn[] = {
    { 0, 0, CPU_Cyrix, CPU_MMX, 0, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 2, 282 }
};

static const x86_insn_info pmachriw_insn[] = {
    { 0, 0, CPU_Cyrix, CPU_MMX, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x5E, 0}, 0, 2, 345 }
};

static const x86_insn_info rdwrshr_insn[] = {
    { 0, 0, CPU_686, CPU_Cyrix, CPU_SMM, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x36, 0}, 0, 1, 46 }
};

static const x86_insn_info rsdc_insn[] = {
    { 0, 0, CPU_486, CPU_Cyrix, CPU_SMM, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x79, 0}, 0, 2, 549 }
};

static const x86_insn_info cyrixsmm_insn[] = {
    { 0, 0, CPU_486, CPU_Cyrix, CPU_SMM, {MOD_Op1Add, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x00, 0}, 0, 1, 550 }
};

static const x86_insn_info svdc_insn[] = {
    { 0, 0, CPU_486, CPU_Cyrix, CPU_SMM, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x78, 0}, 0, 2, 607 }
};

static const x86_insn_info ibts_insn[] = {
    { 0, 0, CPU_386, CPU_Obs, CPU_Undoc, {0, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0xA7, 0}, 0, 2, 288 },
    { 0, 0, CPU_386, CPU_Obs, CPU_Undoc, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0xA7, 0}, 0, 2, 294 }
};

static const x86_insn_info umov_insn[] = {
    { 0, 0, CPU_386, CPU_Undoc, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x10, 0}, 0, 2, 351 },
    { 0, 0, CPU_386, CPU_Undoc, 0, {0, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x11, 0}, 0, 2, 288 },
    { 0, 0, CPU_386, CPU_Undoc, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x11, 0}, 0, 2, 294 },
    { 0, 0, CPU_386, CPU_Undoc, 0, {0, 0, 0}, 0, 0, 0, 0, 2, {0x0F, 0x12, 0}, 0, 2, 353 },
    { 0, 0, CPU_386, CPU_Undoc, 0, {0, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0x13, 0}, 0, 2, 198 },
    { 0, 0, CPU_386, CPU_Undoc, 0, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0x13, 0}, 0, 2, 201 }
};

static const x86_insn_info xbts_insn[] = {
    { 0, 0, CPU_386, CPU_Obs, CPU_Undoc, {0, 0, 0}, 16, 0, 0, 0, 2, {0x0F, 0xA6, 0}, 0, 2, 523 },
    { 0, 0, CPU_386, CPU_Obs, CPU_Undoc, {0, 0, 0}, 32, 0, 0, 0, 2, {0x0F, 0xA6, 0}, 0, 2, 387 }
};

