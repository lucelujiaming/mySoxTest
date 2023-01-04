#if !defined(AFX_SCODE_H)
#define AFX_SCODE_H
// literals
static int g_scode_Nop                = 0;    // no op
static int g_scode_LoadIM1            = 1;    // push -1 on stack
static int g_scode_LoadI0             = 2;    // push 0 on stack
static int g_scode_LoadI1             = 3;    // push 1 on stack
static int g_scode_LoadI2             = 4;    // push 2 on stack
static int g_scode_LoadI3             = 5;    // push 3 on stack
static int g_scode_LoadI4             = 6;    // push 4 on stack
static int g_scode_LoadI5             = 7;    // push 5 on stack
static int g_scode_LoadIntU1          = 8;    // push u1 int g_scode_literal on stack
static int g_scode_LoadIntU2          = 9;    // push u2 int g_scode_literal on stack
static int g_scode_LoadL0             = 10;   // push 0L on stack
static int g_scode_LoadL1             = 11;   // push 1L on stack
static int g_scode_LoadF0             = 12;   // push 0.0f on stack
static int g_scode_LoadF1             = 13;   // push 1.0f on stack
static int g_scode_LoadD0             = 14;   // push 0.0d on stack
static int g_scode_LoadD1             = 15;   // push 1.0d on stack
static int g_scode_LoadNull           = 16;   // push null pointer (0) on stack
static int g_scode_LoadNullBool       = 17;   // push null bool (2) on stack
static int g_scode_LoadNullFloat      = 18;   // push null float (nan) on stack
static int g_scode_LoadNullDouble     = 19;   // push null double (nan) on stack
static int g_scode_LoadInt            = 20;   // push int g_scode_literal on stack from block index
static int g_scode_LoadFloat          = 21;   // push float literal on stack from block index
static int g_scode_LoadLong           = 22;   // push long literal on stack from block index
static int g_scode_LoadDouble         = 23;   // push double literal on stack from block index
static int g_scode_LoadStr            = 24;   // push Str literal pointer on stack from block index
static int g_scode_LoadBuf            = 25;   // push Buf literal pointer on stack from block index
static int g_scode_LoadType           = 26;   // push type pointer on stack from block index
static int g_scode_LoadSlot           = 27;   // push slot pointer on stack from block index
static int g_scode_LoadDefine         = 28;   // load define literal (ir only)

// load params
static int g_scode_LoadParam0         = 29;   // push param 0 on stack
static int g_scode_LoadParam1         = 30;   // push param 1 on stack
static int g_scode_LoadParam2         = 31;   // push param 2 on stack
static int g_scode_LoadParam3         = 32;   // push param 3 on stack
static int g_scode_LoadParam          = 33;   // push param x on stack
static int g_scode_LoadParamWide      = 34;   // push wide param x on stack (long/double)

// store params
static int g_scode_StoreParam         = 35;   // store stack to param x
static int g_scode_StoreParamWide     = 36;   // store stack to wide param x (long/double)

// load locals
static int g_scode_LoadLocal0         = 37;   // push local 0 on stack
static int g_scode_LoadLocal1         = 38;   // push local 1 on stack
static int g_scode_LoadLocal2         = 39;   // push local 2 on stack
static int g_scode_LoadLocal3         = 40;   // push local 3 on stack
static int g_scode_LoadLocal4         = 41;   // push local 4 on stack
static int g_scode_LoadLocal5         = 42;   // push local 5 on stack
static int g_scode_LoadLocal6         = 43;   // push local 6 on stack
static int g_scode_LoadLocal7         = 44;   // push local 7 on stack
static int g_scode_LoadLocal          = 45;   // push local x on stack
static int g_scode_LoadLocalWide      = 46;   // push wide local x on stack (long/double)

// store locals
static int g_scode_StoreLocal0        = 47;   // store stack to local 0
static int g_scode_StoreLocal1        = 48;   // store stack to local 1
static int g_scode_StoreLocal2        = 49;   // store stack to local 2
static int g_scode_StoreLocal3        = 50;   // store stack to local 3
static int g_scode_StoreLocal4        = 51;   // store stack to local 4
static int g_scode_StoreLocal5        = 52;   // store stack to local 5
static int g_scode_StoreLocal6        = 53;   // store stack to local 6
static int g_scode_StoreLocal7        = 54;   // store stack to local 7
static int g_scode_StoreLocal         = 55;   // store stack to local x
static int g_scode_StoreLocalWide     = 56;   // store stack to local x (long/double)

// int g_scode_compare
static int g_scode_IntEq              = 57;   // a == b
static int g_scode_IntNotEq           = 58;   // a != b
static int g_scode_IntGt              = 59;   // a > b
static int g_scode_IntGtEq            = 60;   // a >= b
static int g_scode_IntLt              = 61;   // a < b
static int g_scode_IntLtEq            = 62;   // a <= b

// int g_scode_math
static int g_scode_IntMul             = 63;   // a * b
static int g_scode_IntDiv             = 64;   // a / b
static int g_scode_IntMod             = 65;   // a % b
static int g_scode_IntAdd             = 66;   // a + b
static int g_scode_IntSub             = 67;   // a - b
static int g_scode_IntOr              = 68;   // a | b
static int g_scode_IntXor             = 69;   // a ^ b
static int g_scode_IntAnd             = 70;   // a & b
static int g_scode_IntNot             = 71;   // ~a
static int g_scode_IntNeg             = 72;   // -a
static int g_scode_IntShiftL          = 73;   // a << b
static int g_scode_IntShiftR          = 74;   // a >> b
static int g_scode_IntInc             = 75;   // a + 1
static int g_scode_IntDec             = 76;   // a - 1

// long compare
static int g_scode_LongEq             = 77;   // a == b
static int g_scode_LongNotEq          = 78;   // a != b
static int g_scode_LongGt             = 79;   // a > b
static int g_scode_LongGtEq           = 80;   // a >= b
static int g_scode_LongLt             = 81;   // a < b
static int g_scode_LongLtEq           = 82;   // a <= b

// long math
static int g_scode_LongMul            = 83;   // a * b
static int g_scode_LongDiv            = 84;   // a / b
static int g_scode_LongMod            = 85;   // a % b
static int g_scode_LongAdd            = 86;   // a + b
static int g_scode_LongSub            = 87;   // a - b
static int g_scode_LongOr             = 88;   // a | b
static int g_scode_LongXor            = 89;   // a ^ b
static int g_scode_LongAnd            = 90;   // a & b
static int g_scode_LongNot            = 91;   // ~a
static int g_scode_LongNeg            = 92;   // -a
static int g_scode_LongShiftL         = 93;   // a << b
static int g_scode_LongShiftR         = 94;   // a >> b

// float compare
static int g_scode_FloatEq            = 95;   // a == b
static int g_scode_FloatNotEq         = 96;   // a != b
static int g_scode_FloatGt            = 97;   // a > b
static int g_scode_FloatGtEq          = 98;   // a >= b
static int g_scode_FloatLt            = 99;   // a < b
static int g_scode_FloatLtEq          = 100;  // a <= b

// float math
static int g_scode_FloatMul           = 101;  // a * b
static int g_scode_FloatDiv           = 102;  // a / b
static int g_scode_FloatAdd           = 103;  // a + b
static int g_scode_FloatSub           = 104;  // a - b
static int g_scode_FloatNeg           = 105;  // -a

// double compare
static int g_scode_DoubleEq           = 106;  // a == b
static int g_scode_DoubleNotEq        = 107;  // a != b
static int g_scode_DoubleGt           = 108;  // a > b
static int g_scode_DoubleGtEq         = 109;  // a >= b
static int g_scode_DoubleLt           = 110;  // a < b
static int g_scode_DoubleLtEq         = 111;  // a <= b

// double math
static int g_scode_DoubleMul          = 112;  // a * b
static int g_scode_DoubleDiv          = 113;  // a / b
static int g_scode_DoubleAdd          = 114;  // a + b
static int g_scode_DoubleSub          = 115;  // a - b
static int g_scode_DoubleNeg          = 116;  // -a

// casts
static int g_scode_IntToFloat         = 117;  // int g_scode_-> float
static int g_scode_IntToLong          = 118;  // int g_scode_-> long
static int g_scode_IntToDouble        = 119;  // int g_scode_-> double
static int g_scode_LongToInt          = 120;  // long -> int
static int g_scode_LongToFloat        = 121;  // long -> float
static int g_scode_LongToDouble       = 122;  // long -> double
static int g_scode_FloatToInt         = 123;  // float -> int
static int g_scode_FloatToLong        = 124;  // float -> long
static int g_scode_FloatToDouble      = 125;  // float -> double
static int g_scode_DoubleToInt        = 126;  // double -> int
static int g_scode_DoubleToLong       = 127;  // double -> long
static int g_scode_DoubleToFloat      = 128;  // double -> float

// object compare
static int g_scode_ObjEq              = 129;  // a == b
static int g_scode_ObjNotEq           = 130;  // a != b

// general purpose compare
static int g_scode_EqZero             = 131;  // a == 0 or null
static int g_scode_NotEqZero          = 132;  // a != 0 or null

// stack manipulation
static int g_scode_Pop                = 133;  // pop top of stack
static int g_scode_Pop2               = 134;  // pop top two cells of stack
static int g_scode_Pop3               = 135;  // pop top three cells of stack
static int g_scode_Dup                = 136;  // duplicate top of stack
static int g_scode_Dup2               = 137;  // duplicate top two cells of stack
static int g_scode_DupDown2           = 138;  // duplicate top of stack, push down 2 cells
static int g_scode_DupDown3           = 139;  // duplicate top of stack, push down 3 cells
static int g_scode_Dup2Down2          = 140;  // duplicate top two cells of stack, push down 2 cells
static int g_scode_Dup2Down3          = 141;  // duplicate top two cells of stack, push down 3 cells

// branching
static int g_scode_Jump               = 142;  // unconditional jump (1 sbyte offset)
static int g_scode_JumpNonZero        = 143;  // jump if non-zero (1 sbyte offset)
static int g_scode_JumpZero           = 144;  // jump if zero (1 sbyte offset)
static int g_scode_Foreach            = 145;  // array, length, counter on stack (1 sbyte offset)
static int g_scode_JumpFar            = 146;  // unconditional far jump (2 sbyte offset)
static int g_scode_JumpFarNonZero     = 147;  // far jump if true (2 sbyte offset)
static int g_scode_JumpFarZero        = 148;  // far jump if false (2 sbyte offset)
static int g_scode_ForeachFar         = 149;  // array, length, counter on stack (2 sbyte offset)

// int g_scode_compare branching
static int g_scode_JumpIntEq          = 150;  // jump if a == b
static int g_scode_JumpIntNotEq       = 151;  // jump if a != b
static int g_scode_JumpIntGt          = 152;  // jump if a > b
static int g_scode_JumpIntGtEq        = 153;  // jump if a >= b
static int g_scode_JumpIntLt          = 154;  // jump if a < b
static int g_scode_JumpIntLtEq        = 155;  // jump if a <= b
static int g_scode_JumpFarIntEq       = 156;  // jump if a == b
static int g_scode_JumpFarIntNotEq    = 157;  // jump if a != b
static int g_scode_JumpFarIntGt       = 158;  // jump if a > b
static int g_scode_JumpFarIntGtEq     = 159;  // jump if a >= b
static int g_scode_JumpFarIntLt       = 160;  // jump if a < b
static int g_scode_JumpFarIntLtEq     = 161;  // jump if a <= b

// storage
static int g_scode_LoadDataAddr       = 162;  // load data segment base address of static fields

// 8 bit storage (bytes, bools)
static int g_scode_Load8BitFieldU1    = 163;  // load 8 bit field (1 ubyte offset)
static int g_scode_Load8BitFieldU2    = 164;  // load 8 bit field (2 ubyte offset)
static int g_scode_Load8BitFieldU4    = 165;  // load 8 bit field (4 ubyte offset)
static int g_scode_Load8BitArray      = 166;  // load 8 bit array item (int g_scode_offset on stack)
static int g_scode_Store8BitFieldU1   = 167;  // store 8 bit field (1 ubyte offset)
static int g_scode_Store8BitFieldU2   = 168;  // store 8 bit field (2 ubyte offset)
static int g_scode_Store8BitFieldU4   = 169;  // store 8 bit field (4 ubyte offset)
static int g_scode_Store8BitArray     = 170;  // store 8 bit array item (int g_scode_offset on stack)
static int g_scode_Add8BitArray       = 171;  // add int g_scode_on stack to 8 bit array pointer

// 16 bit storage (shorts)
static int g_scode_Load16BitFieldU1   = 172;  // load 16 bit field (1 ubyte offset)
static int g_scode_Load16BitFieldU2   = 173;  // load 16 bit field (2 ubyte offset)
static int g_scode_Load16BitFieldU4   = 174;  // load 16 bit field (4 ubyte offset)
static int g_scode_Load16BitArray     = 175;  // load 16 bit array item (int g_scode_offset on stack)
static int g_scode_Store16BitFieldU1  = 176;  // store 16 bit field (1 ubyte offset)
static int g_scode_Store16BitFieldU2  = 177;  // store 16 bit field (2 ubyte offset)
static int g_scode_Store16BitFieldU4  = 178;  // store 16 bit field (4 ubyte offset)
static int g_scode_Store16BitArray    = 179;  // store 16 bit array item (int g_scode_offset on stack)
static int g_scode_Add16BitArray      = 180;  // add int g_scode_on stack to 8 bit array pointer

// 32 bit storage (int/float)
static int g_scode_Load32BitFieldU1   = 181;  // load 32 bit field (1 ubyte offset)
static int g_scode_Load32BitFieldU2   = 182;  // load 32 bit field (2 ubyte offset)
static int g_scode_Load32BitFieldU4   = 183;  // load 32 bit field (4 ubyte offset)
static int g_scode_Load32BitArray     = 184;  // load 32 bit array item (int g_scode_offset on stack)
static int g_scode_Store32BitFieldU1  = 185;  // store 32 bit field (1 ubyte offset)
static int g_scode_Store32BitFieldU2  = 186;  // store 32 bit field (2 ubyte offset)
static int g_scode_Store32BitFieldU4  = 187;  // store 32 bit field (4 ubyte offset)
static int g_scode_Store32BitArray    = 188;  // store 32 bit array item (int g_scode_offset on stack)
static int g_scode_Add32BitArray      = 189;  // add int g_scode_on stack to 32 bit array pointer

// 64 bit storage (long/double)
static int g_scode_Load64BitFieldU1   = 190;  // load 64 bit field (1 ubyte offset)
static int g_scode_Load64BitFieldU2   = 191;  // load 64 bit field (2 ubyte offset)
static int g_scode_Load64BitFieldU4   = 192;  // load 64 bit field (4 ubyte offset)
static int g_scode_Load64BitArray     = 193;  // load 64 bit array item (int g_scode_offset on stack)
static int g_scode_Store64BitFieldU1  = 194;  // store 64 bit field (1 ubyte offset)
static int g_scode_Store64BitFieldU2  = 195;  // store 64 bit field (2 ubyte offset)
static int g_scode_Store64BitFieldU4  = 196;  // store 64 bit field (4 ubyte offset)
static int g_scode_Store64BitArray    = 197;  // store 64 bit array item (int g_scode_offset on stack)
static int g_scode_Add64BitArray      = 198;  // add int g_scode_on stack to 64 bit array pointer

// ref storage (pointers - variable width)
static int g_scode_LoadRefFieldU1     = 199;  // load pointer field (1 ubyte offset)
static int g_scode_LoadRefFieldU2     = 200;  // load pointer field (2 ubyte offset)
static int g_scode_LoadRefFieldU4     = 201;  // load pointer field (4 ubyte offset)
static int g_scode_LoadRefArray       = 202;  // load pointer array item (int g_scode_offset on stack)
static int g_scode_StoreRefFieldU1    = 203;  // store pointer field (1 ubyte offset)
static int g_scode_StoreRefFieldU2    = 204;  // store pointer field (2 ubyte offset)
static int g_scode_StoreRefFieldU4    = 205;  // store pointer field (4 ubyte offset)
static int g_scode_StoreRefArray      = 206;  // store pointer array item (int g_scode_offset on stack)
static int g_scode_AddRefArray        = 207;  // add int g_scode_on stack to pointer array pointer

// const storage (block index)
static int g_scode_LoadConstFieldU1   = 208;  // load const instance field (1 ubyte offset)
static int g_scode_LoadConstFieldU2   = 209;  // load const instance field (2 ubyte offset)
static int g_scode_LoadConstStatic    = 210;  // load const static field (2 block index)
static int g_scode_LoadConstArray     = 211;  // load const pointer array item (int g_scode_offset on stack)

// inline storage (pointer addition)
static int g_scode_LoadInlineFieldU1  = 212;  // load inline field (1 ubyte offset)
static int g_scode_LoadInlineFieldU2  = 213;  // load inline field (2 ubyte offset)
static int g_scode_LoadInlineFieldU4  = 214;  // load inline field (4 ubyte offset)

// param0 + inline storage
static int g_scode_LoadParam0InlineFieldU1 = 215;  // LoadParam0 + LoadInlineFieldU1
static int g_scode_LoadParam0InlineFieldU2 = 216;  // LoadParam0 + LoadInlineFieldU2
static int g_scode_LoadParam0InlineFieldU4 = 217;  // LoadParam0 + LoadInlineFieldU4

// static + inline storage
static int g_scode_LoadDataInlineFieldU1 = 218;  // LoadDataAddr + LoadInlineFieldU1
static int g_scode_LoadDataInlineFieldU2 = 219;  // LoadDataAddr + LoadInlineFieldU2
static int g_scode_LoadDataInlineFieldU4 = 220;  // LoadDataAddr + LoadInlineFieldU4

// call control
static int g_scode_Call               = 221;  // u2 block index (unaligned!) to non-virtual method
static int g_scode_CallVirtual        = 222;  // scode: u2 u1 - block index to type (unaligned!), num params
static int g_scode_CallNative         = 223;  // scode: u1 u1 u1 - kit index, method index, num params
static int g_scode_CallNativeWide     = 224;  // scode: u1 u1 u1 - kit index, method index, num params
static int g_scode_CallNativeVoid     = 225;  // scode: u1 u1 u1 - kit index, method index, num params
static int g_scode_ReturnVoid         = 226;  // return void
static int g_scode_ReturnPop          = 227;  // return top of stack
static int g_scode_ReturnPopWide      = 228;  // return top of stack (wide - long/double)
static int g_scode_LoadParam0Call     = 229;  // param0 + call

// misc
static int g_scode_InitArray          = 230;  // addr, length, sizeof on stack; setup inline obj array pointers
static int g_scode_InitVirt           = 231;  // init Virtual.vtable; this on stack
static int g_scode_InitComp           = 232;  // init Component.type; this on stack
static int g_scode_SizeOf             = 233;  // size of type in bytes (IR only)
static int g_scode_Assert             = 234;  // assert top of stack, u2 is linenum
static int g_scode_Switch             = 235;  // switch with index into jump table (u2 is table count)
static int g_scode_MetaSlot           = 236;  // inline meta-data, index to slot qname triple
static int g_scode_Cast               = 237;  // only used for Java bytecode
static int g_scode_LoadArrayLiteral   = 238;  // in SVM we use LoadBuf as pointer to array in code section
static int g_scode_LoadSlotId         = 239;  // arg is slot qname; subst id at image gen

#endif // !defined(AFX_SCODE_H)
