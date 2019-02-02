    opcode = F8;
    goto *opcodes0f[opcode];

        _0f_0x10:                      /* MOVUPS Gx,Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            memcpy(opx1, opx2, sizeof(sse_regs_t)); // unaligned, so carreful
            NEXT;
        _0f_0x11:                      /* MOVUPS Ex,Gx */
            nextop = F8;
            opx1=GetEx(emu, nextop);
            opx2=GetGx(emu, nextop);
            memcpy(opx1, opx2, sizeof(sse_regs_t)); // unaligned, so carreful
            NEXT;
        _0f_0x12:                      
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            if((nextop&0xf8)==0xc0)     /* MOVHLPS Gx,Ex */
                opx1->q[0] = opx2->q[1];
            else
                opx1->q[0] = opx2->q[0];/* MOVLPS Gx,Ex */
            NEXT;
        _0f_0x13:                      /* MOVLPS Ex,Gx */
            nextop = F8;
            opx1=GetEx(emu, nextop);
            opx2=GetGx(emu, nextop);
            opx1->q[0] = opx2->q[0];
            NEXT;
        _0f_0x14:                      /* UNPCKLPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            opx1->ud[3] = opx2->ud[1];
            opx1->ud[2] = opx1->ud[1];
            opx1->ud[1] = opx2->ud[0];
            NEXT;
        _0f_0x15:                      /* UNPCKHPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            opx1->ud[0] = opx1->ud[2];
            opx1->ud[1] = opx2->ud[2];
            opx1->ud[2] = opx1->ud[3];
            opx1->ud[3] = opx2->ud[3];
            NEXT;
        _0f_0x16:                      /* MOVHPS Ex,Gx */
            nextop = F8;       /* MOVLHPS Ex,Gx */
            opx1=GetEx(emu, nextop);
            opx2=GetGx(emu, nextop);
            opx2->q[1] = opx1->q[0];
            NEXT;
        _0f_0x17:                      /* MOVHPS Gx,Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            opx2->q[0] = opx1->q[1];
            NEXT;

        _0f_0x1F:                      /* NOP (multi-byte) */
            nextop = F8;
            op1=GetEd(emu, nextop);
            NEXT;

        _0f_0x28:                      /* MOVAPS Gx,Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            opx1->q[0] = opx2->q[0];
            opx1->q[1] = opx2->q[1];
            NEXT;
        _0f_0x29:                      /* MOVAPS Ex,Gx */
            nextop = F8;
            opx1=GetEx(emu, nextop);
            opx2=GetGx(emu, nextop);
            opx1->q[0] = opx2->q[0];
            opx1->q[1] = opx2->q[1];
            NEXT;
        _0f_0x2A:  /* CVTPI2PS Gx, Em */
            nextop = F8;
            opm2=GetEm(emu, nextop);
            opx1=GetGx(emu, nextop);
            opx1->f[0] = opm2->sd[0];
            opx1->f[1] = opm2->sd[1];
            NEXT;

        _0f_0x2C:                      /* CVTTPS2PI Gm, Ex */
        _0f_0x2D:                      /* CVTPS2PI Gm, Ex */
            // rounding should be done
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opm1=GetGm(emu, nextop);
            opm1->sd[1] = opx2->f[1];
            opm1->sd[0] = opx2->f[0];
            NEXT;
        _0f_0x2E:                      /* UCOMISS Gx, Ex */
            // same for now
        _0f_0x2F:                      /* COMISS Gx, Ex */
            RESET_FLAGS(emu);
            nextop = F8;
            opx1=GetEx(emu, nextop);
            opx2=GetGx(emu, nextop);
            if(isnan(opx1->f[0]) || isnan(opx2->f[0])) {
                SET_FLAG(F_ZF); SET_FLAG(F_PF); SET_FLAG(F_CF);
            } else if(isgreater(opx2->f[0], opx1->f[0])) {
                CLEAR_FLAG(F_ZF); CLEAR_FLAG(F_PF); CLEAR_FLAG(F_CF);
            } else if(isless(opx2->f[0], opx1->f[0])) {
                CLEAR_FLAG(F_ZF); CLEAR_FLAG(F_PF); SET_FLAG(F_CF);
            } else {
                SET_FLAG(F_ZF); CLEAR_FLAG(F_PF); CLEAR_FLAG(F_CF);
            }
            NEXT;
        
        #define GOCOND(BASE, PREFIX, CONDITIONAL) \
        _0f_##BASE##_0:                          \
            PREFIX                              \
            if(ACCESS_FLAG(F_OF))               \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_1:                          \
            PREFIX                              \
            if(!ACCESS_FLAG(F_OF))              \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_2:                          \
            PREFIX                              \
            if(ACCESS_FLAG(F_CF))               \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_3:                          \
            PREFIX                              \
            if(!ACCESS_FLAG(F_CF))              \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_4:                          \
            PREFIX                              \
            if(ACCESS_FLAG(F_ZF))               \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_5:                          \
            PREFIX                              \
            if(!ACCESS_FLAG(F_ZF))              \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_6:                          \
            PREFIX                              \
            if((ACCESS_FLAG(F_ZF) || ACCESS_FLAG(F_CF)))  \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_7:                          \
            PREFIX                              \
            if(!(ACCESS_FLAG(F_ZF) || ACCESS_FLAG(F_CF))) \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_8:                          \
            PREFIX                              \
            if(ACCESS_FLAG(F_SF))               \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_9:                          \
            PREFIX                              \
            if(!ACCESS_FLAG(F_SF))              \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_A:                          \
            PREFIX                              \
            if(ACCESS_FLAG(F_PF))               \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_B:                          \
            PREFIX                              \
            if(!ACCESS_FLAG(F_PF))              \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_C:                          \
            PREFIX                              \
            if(ACCESS_FLAG(F_SF) != ACCESS_FLAG(F_OF))  \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_D:                          \
            PREFIX                              \
            if(ACCESS_FLAG(F_SF) == ACCESS_FLAG(F_OF)) \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_E:                          \
            PREFIX                              \
            if(ACCESS_FLAG(F_ZF) || (ACCESS_FLAG(F_SF) != ACCESS_FLAG(F_OF))) \
                CONDITIONAL                     \
            NEXT;                              \
        _0f_##BASE##_F:                          \
            PREFIX                              \
            if(!ACCESS_FLAG(F_ZF) && (ACCESS_FLAG(F_SF) == ACCESS_FLAG(F_OF))) \
                CONDITIONAL                     \
            NEXT;

        GOCOND(0x40
            , nextop = F8;
            op2=GetEd(emu, nextop);
            op1=GetG(emu, nextop);
            CHECK_FLAGS(emu);
            , op1->dword[0] = op2->dword[0];
        )                               /* 0x40 -> 0x4F CMOVxx Gd,Ed */ // conditional move, no sign
        GOCOND(0x80
            , tmp32s = F32S; CHECK_FLAGS(emu);
            , R_EIP += tmp32s;
        )                               /* 0x80 -> 0x8F Jxx */
        GOCOND(0x90
            , nextop = F8; CHECK_FLAGS(emu);
            op1=GetEb(emu, nextop);
            , op1->byte[0]=1; else op1->byte[0]=0;
        )                               /* 0x90 -> 0x9F SETxx Eb */

        #undef GOCOND

        _0f_0x50:                      /* MOVMSKPS Gd, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            op1=GetG(emu, nextop);
            op1->dword[0] = 0;
            for(int i=0; i<4; ++i)
                op1->dword[0] |= ((opx2->ud[i]>>31)&1)<<i;
            NEXT;
        _0f_0x51:                      /* SQRTPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i)
                opx1->f[i] = sqrtf(opx2->f[i]);
            NEXT;

        _0f_0x54:                      /* ANDPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i)
                opx1->ud[i] &= opx2->ud[i];
            NEXT;
        _0f_0x55:                      /* ANDNPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i)
                opx1->ud[i] = (~opx1->ud[i]) & opx2->ud[i];
            NEXT;
        _0f_0x56:                      /* ORPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i)
                opx1->ud[i] |= opx2->ud[i];
            NEXT;
        _0f_0x57:                      /* XORPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i)
                opx1->ud[i] ^= opx2->ud[i];
            NEXT;
        _0f_0x58:                      /* ADDPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i)
                opx1->f[i] += opx2->f[i];
            NEXT;
        _0f_0x59:                      /* MULPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i)
                opx1->f[i] *= opx2->f[i];
            NEXT;
        _0f_0x5A:                      /* CVTPS2PD Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            opx1->d[1] = opx2->f[1];
            opx1->d[0] = opx2->f[0];
            NEXT;
        _0f_0x5B:                      /* CVTDQ2PS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            opx1->f[0] = opx2->sd[0];
            opx1->f[1] = opx2->sd[1];
            opx1->f[2] = opx2->sd[2];
            opx1->f[3] = opx2->sd[3];
            NEXT;
        _0f_0x5C:                      /* SUBPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i)
                opx1->f[i] -= opx2->f[i];
            NEXT;
        _0f_0x5D:                      /* MINPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i) {
                if (isnan(opx1->f[i]) || isnan(opx2->f[i]) || isless(opx2->f[i], opx1->f[i]))
                    opx1->f[i] = opx2->f[i];
            }
            NEXT;
        _0f_0x5E:                      /* DIVPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i)
                opx1->f[i] /= opx2->f[i];
            NEXT;
        _0f_0x5F:                      /* MAXPS Gx, Ex */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            for(int i=0; i<4; ++i) {
                if (isnan(opx1->f[i]) || isnan(opx2->f[i]) || isgreater(opx2->f[i], opx1->f[i]))
                    opx1->f[i] = opx2->f[i];
            }
            NEXT;
        _0f_0x60:                      /* PUNPCKLBW Gm, Em */
            nextop = F8;
            opm2=GetEm(emu, nextop);
            opm1=GetGm(emu, nextop);
            opm1->ub[7] = opm2->ub[3];
            opm1->ub[6] = opm1->ub[3];
            opm1->ub[5] = opm2->ub[2];
            opm1->ub[4] = opm1->ub[2];
            opm1->ub[3] = opm2->ub[1];
            opm1->ub[2] = opm1->ub[1];
            opm1->ub[1] = opm2->ub[0];
            NEXT;
        _0f_0x61:                      /* PUNPCKLWD Gm, Em */
            nextop = F8;
            opm2=GetEm(emu, nextop);
            opm1=GetGm(emu, nextop);
            opm1->uw[3] = opm2->uw[1];
            opm1->uw[2] = opm1->uw[1];
            opm1->uw[1] = opm2->uw[0];
            NEXT;
        _0f_0x62:                      /* PUNPCKLDQ Gm, Em */
            nextop = F8;
            opm2=GetEm(emu, nextop);
            opm1=GetGm(emu, nextop);
            opm1->ud[1] = opm2->ud[0];
            NEXT;

        _0f_0x6F:                      /* MOVQ Gm, Em */
            nextop = F8;
            opm2=GetEm(emu, nextop);
            opm1=GetGm(emu, nextop);
            opm1->q = opm2->q;
            NEXT;

        _0f_0x77:                      /* EMMS */
            // empty MMX, FPU now usable
            NEXT;

        _0f_0x7F:                      /* MOVQ Em, Gm */
            nextop = F8;
            opm1=GetEm(emu, nextop);
            opm2=GetGm(emu, nextop);
            opm1->q = opm2->q;
            NEXT;

        _0f_0xA2:                      /* CPUID */
            tmp32u = R_EAX;
            switch(tmp32u) {
                case 0x0:
                    // emulate a P4
                    R_EAX = 0x80000004;
                    // return GuenuineIntel
                    R_EBX = 0x756E6547;
                    R_EDX = 0x49656E69;
                    R_ECX = 0x6C65746E;
                    break;
                case 0x1:
                    R_EAX = 0x00000101; // familly and all
                    R_EBX = 0;          // Brand indexe, CLFlush, Max APIC ID, Local APIC ID
                    R_EDX =   1         // fpu 
                            | 1<<8      // cmpxchg8
                            | 1<<11     // sep (sysenter & sysexit)
                            | 1<<15     // cmov
                            | 1<<19     // clflush (seems to be with SSE2)
                            | 1<<23     // mmx
                            | 1<<24     // fxsr (fxsave, fxrestore)
                            | 1<<25     // SSE
                            | 1<<26     // SSE2
                            ;
                    R_ECX =   1<<12     // fma
                            | 1<<13     // cx16 (cmpxchg16)
                            ;           // also 1<<0 is SSE3 and 1<<9 is SSSE3
                    break;
                default:
                    printf_log(LOG_INFO, "Warning, CPUID command %X unsupported\n", tmp32u);
                    R_EAX = 0;
            }
            NEXT;
        _0f_0xA3:                      /* BT Ed,Gd */
            CHECK_FLAGS(emu);
            nextop = F8;
            op1=GetEd(emu, nextop);
            op2=GetG(emu, nextop);
            tmp8u = op2->byte[0];
            if((nextop&0xc7)<0xc0 || (nextop&0xc7)>0xc7)
            {
                op1=(reg32_t*)(((uint32_t*)(op1))+(tmp8u>>5));
            }
            tmp8u&=31;
            if(tmp8u<32 && op1->dword[0] & (1<<tmp8u))
                SET_FLAG(F_CF);
            else
                CLEAR_FLAG(F_CF);
            NEXT;
        _0f_0xA4:                      /* SHLD Ed,Gd,Ib */
        _0f_0xA5:                      /* SHLD Ed,Gd,CL */
            nextop = F8;
            op1=GetEd(emu, nextop);
            op2=GetG(emu, nextop);
            tmp8u = (opcode==0xA4)?F8:R_CL;
            op1->dword[0] = shld32(emu, op1->dword[0], op2->dword[0], tmp8u);
            NEXT;

        _0f_0xAB:                      /* BTS Ed,Gd */
            CHECK_FLAGS(emu);
            nextop = F8;
            op1=GetEd(emu, nextop);
            op2=GetG(emu, nextop);
            tmp8u = op2->byte[0];
            if((nextop&0xc7)<0xc0 || (nextop&0xc7)>0xc7)
            {
                op1=(reg32_t*)(((uint32_t*)(op1))+(tmp8u>>5));
            }
            tmp8u&=31;
            if(op1->dword[0] & (1<<tmp8u))
                SET_FLAG(F_CF);
            else {
                op1->dword[0] |= (1<<tmp8u);
                CLEAR_FLAG(F_CF);
            }
            NEXT;
        _0f_0xAC:                      /* SHRD Ed,Gd,Ib */
        _0f_0xAD:                      /* SHRD Ed,Gd,CL */
            nextop = F8;
            op1=GetEd(emu, nextop);
            op2=GetG(emu, nextop);
            tmp8u = (opcode==0xAC)?F8:R_CL;
            op1->dword[0] = shrd32(emu, op1->dword[0], op2->dword[0], tmp8u);
            NEXT;

        _0f_0xAE:                      /* Grp Ed (SSE) */
            nextop = F8;
            op1=GetEd(emu, nextop);
            switch((nextop>>3)&7) {
                case 2:                 /* LDMXCSR Md */
                    emu->mxcsr = op1->dword[0];
                    break;
                case 3:                 /* SDMXCSR Md */
                    op1->dword[0] = emu->mxcsr;
                    break;
                default:
                    UnimpOpcode(emu);
            }
            NEXT;
        _0f_0xAF:                      /* IMUL Gd,Ed */
            nextop = F8;
            op2=GetEd(emu, nextop);
            op1=GetG(emu, nextop);
            op1->dword[0] = imul32(emu, op1->dword[0], op2->dword[0]);
            NEXT;

        _0f_0xB0:                      /* CMPXCHG Eb,Gb */
            CHECK_FLAGS(emu);
            nextop = F8;
            op1=GetEb(emu, nextop);
            op2=GetGb(emu, nextop);
            cmp8(emu, R_AL, op1->byte[0]);
            if(R_AL == op1->byte[0]) {
                SET_FLAG(F_ZF);
                op1->byte[0] = op2->byte[0];
            } else {
                CLEAR_FLAG(F_ZF);
                R_AL = op1->byte[0];
            }
            NEXT;
        _0f_0xB1:                      /* CMPXCHG Ed,Gd */
            nextop = F8;
            op1=GetEd(emu, nextop);
            op2=GetG(emu, nextop);
            cmp32(emu, R_EAX, op1->dword[0]);
            if(R_EAX == op1->dword[0]) {
                SET_FLAG(F_ZF);
                op1->dword[0] = op2->dword[0];
            } else {
                CLEAR_FLAG(F_ZF);
                R_EAX = op1->dword[0];
            }
            NEXT;

        _0f_0xB3:                      /* BTR Ed,Gd */
            CHECK_FLAGS(emu);
            nextop = F8;
            op1=GetEd(emu, nextop);
            op2=GetG(emu, nextop);
            tmp8u = op2->byte[0];
            if((nextop&0xc7)<0xc0 || (nextop&0xc7)>0xc7)
            {
                op1=(reg32_t*)(((uint32_t*)(op1))+(tmp8u>>5));
            }
            tmp8u&=31;
            if(op1->dword[0] & (1<<tmp8u)) {
                SET_FLAG(F_CF);
                op1->dword[0] ^= (1<<tmp8u);
            } else
                CLEAR_FLAG(F_CF);
            NEXT;

        _0f_0xB6:                      /* MOVZX Gd,Eb */ // Move with zero extend
            nextop = F8;
            op2=GetEb(emu, nextop);
            op1=GetG(emu, nextop);
            op1->dword[0] = op2->byte[0];
            NEXT;
        _0f_0xB7:                      /* MOVZX Gd,Ew */ // Move with zero extend
            nextop = F8;
            op2=GetEw(emu, nextop);
            op1=GetG(emu, nextop);
            op1->dword[0] = op2->word[0];
            NEXT;

        _0f_0xBB:                      /* BTC Ed,Gd */
            CHECK_FLAGS(emu);
            nextop = F8;
            op1=GetEd(emu, nextop);
            op2=GetG(emu, nextop);
            tmp8u = op2->byte[0];
            if((nextop&0xc7)<0xc0 || (nextop&0xc7)>0xc7)
            {
                op1=(reg32_t*)(((uint32_t*)(op1))+(tmp8u>>5));
            }
            tmp8u&=31;
            if(op1->dword[0] & (1<<tmp8u))
                SET_FLAG(F_CF);
            else
                CLEAR_FLAG(F_CF);
            op1->dword[0] ^= (1<<tmp8u);
            NEXT;

        _0f_0xBA:                      
            nextop = F8;
            switch((nextop>>3)&7) {
                case 4:                 /* BT Ed,Ib */
                    CHECK_FLAGS(emu);
                    op1=GetEd(emu, nextop);
                    tmp8u = F8;
                    if((nextop&0xc7)<0xc0 || (nextop&0xc7)>0xc7)
                    {
                        op1=(reg32_t*)(((uint32_t*)(op1))+(tmp8u>>5));
                    }
                    tmp8u&=31;
                    if(op1->dword[0] & (1<<tmp8u))
                        SET_FLAG(F_CF);
                    else
                        CLEAR_FLAG(F_CF);
                    break;;
                default:
                    UnimpOpcode(emu);
            }
            NEXT;
        _0f_0xBC:                      /* BSF Ed,Gd */
            CHECK_FLAGS(emu);
            nextop = F8;
            op1=GetEd(emu, nextop);
            op2=GetG(emu, nextop);
            tmp32u = op1->dword[0];
            if(tmp32u) {
                CLEAR_FLAG(F_ZF);
                tmp8u = 0;
                while(!(tmp32u&(1<<tmp8u))) ++tmp8u;
                op2->dword[0] = tmp8u;
            } else {
                SET_FLAG(F_ZF);
            }
            NEXT;
        _0f_0xBD:                      /* BSR Ed,Gd */
            CHECK_FLAGS(emu);
            nextop = F8;
            op1=GetEd(emu, nextop);
            op2=GetG(emu, nextop);
            tmp32u = op1->dword[0];
            if(tmp32u) {
                CLEAR_FLAG(F_ZF);
                tmp8u = 31;
                while(!(tmp32u&(1<<tmp8u))) --tmp8u;
                op2->dword[0] = tmp8u;
            } else {
                SET_FLAG(F_ZF);
            }
            NEXT;
        _0f_0xBE:                      /* MOVSX Gd,Eb */ // Move with sign extend
            nextop = F8;
            op2=GetEb(emu, nextop);
            op1=GetG(emu, nextop);
            op1->sdword[0] = op2->sbyte[0];
            NEXT;
        _0f_0xBF:                      /* MOVSX Gd,Ew */ // Move with sign extend
            nextop = F8;
            op2=GetEw(emu, nextop);
            op1=GetG(emu, nextop);
            op1->sdword[0] = op2->sword[0];
            NEXT;

        _0f_0xC0:                      /* XADD Gb,Eb */ // Xchange and Add
            nextop = F8;
            op1=GetEb(emu, nextop);
            op2=GetGb(emu, nextop);
            tmp8u = add8(emu, op1->byte[0], op2->byte[0]);
            op2->byte[0] = op1->byte[0];
            op1->byte[0] = tmp8u;
            NEXT;
        _0f_0xC1:                      /* XADD Gd,Ed */ // Xchange and Add
            nextop = F8;
            op1=GetEd(emu, nextop);
            op2=GetG(emu, nextop);
            tmp32u = add32(emu, op1->dword[0], op2->dword[0]);
            op2->dword[0] = op1->dword[0];
            op1->dword[0] = tmp32u;
            NEXT;
        _0f_0xC2:                      /* CMPPS Gx, Ex, Ib */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            tmp8u = F8;
            for(int i=0; i<4; ++i) {
                tmp8s = 0;
                switch(tmp8u&7) {
                    case 0: tmp8s=(opx1->f[i] == opx2->f[i]); break;
                    case 1: tmp8s=isless(opx1->f[i], opx2->f[i]); break;
                    case 2: tmp8s=islessequal(opx1->f[i], opx2->f[i]); break;
                    case 3: tmp8s=isnan(opx1->f[i]) || isnan(opx2->f[i]); break;
                    case 4: tmp8s=(opx1->f[i] != opx2->f[i]); break;
                    case 5: tmp8s=isgreaterequal(opx1->f[i], opx2->f[i]); break;
                    case 6: tmp8s=isgreater(opx1->f[i], opx2->f[i]); break;
                    case 7: tmp8s=!isnan(opx1->f[i]) && !isnan(opx2->f[i]); break;
                }
                if(tmp8s)
                    opx1->ud[i] = 0xffffffff;
                else
                    opx1->ud[i] = 0;
            }
            NEXT;

        _0f_0xC6:                      /* SHUFPS Gx, Ex, Ib */
            nextop = F8;
            opx2=GetEx(emu, nextop);
            opx1=GetGx(emu, nextop);
            tmp8u = F8;
            for(int i=0; i<2; ++i) {
                eax1.ud[i] = opx1->ud[(tmp8u>>(i*2))&3];
            }
            for(int i=2; i<4; ++i) {
                eax1.ud[i] = opx2->ud[(tmp8u>>(i*2))&3];
            }
            opx1->q[0] = eax1.q[0];
            opx1->q[1] = eax1.q[1];
            NEXT;
        _0f_0xC7:                      /* CMPXCHG8B Gq */
            CHECK_FLAGS(emu);
            nextop = F8;
            op1=GetEd(emu, nextop);
            tmp32u = op1->dword[0];
            tmp32u2= op1->dword[1];
            if(R_EAX == tmp32u && R_EDX == tmp32u2) {
                SET_FLAG(F_ZF);
                op1->dword[0] = R_EBX;
                op1->dword[1] = R_ECX;
            } else {
                CLEAR_FLAG(F_ZF);
                R_EAX = op1->dword[0];
                R_EDX = op1->dword[1];
            }
            NEXT;
        _0f_0xC8:
        _0f_0xC9:
        _0f_0xCA:
        _0f_0xCB:
        _0f_0xCC:
        _0f_0xCD:
        _0f_0xCE:
        _0f_0xCF:                  /* BSWAP reg */
            tmp8s = opcode&7;
            emu->regs[tmp8s].dword[0] = __builtin_bswap32(emu->regs[tmp8s].dword[0]);
            NEXT;
