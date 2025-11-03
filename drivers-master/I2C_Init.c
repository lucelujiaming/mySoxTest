void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct)
{
    uint16_t tmpreg = 0, freqrange = 0;
    uint16_t result = 0x04;
    uint32_t pclk1 = 8000000;

    RCC_ClocksTypeDef rcc_clocks;

    tmpreg = I2Cx->CTLR2;                      // CTLR2默认为零。
    tmpreg &= CTLR2_FREQ_Reset;                // 因为默认为零，因此上清除操作没有改变tmpreg的值。
    RCC_GetClocksFreq(&rcc_clocks);            // 获得RCC时钟主频。
    pclk1 = rcc_clocks.PCLK1_Frequency;        // 主频的值为0x2dc6c00，也就是48000000。
    freqrange = (uint16_t)(pclk1 / 1000000);   // 除以1000000得到48。
    tmpreg |= freqrange;                       // 把这个48赋给tmpreg。因为：
                                               //    I2C 模块时钟频率域，必须输入正确的时钟频率
                                               //    以产生正确的时序，允许的范围在 8-48MHz之间。
                                               //    必须设置在 001000b 到 110000b 之间，
    I2Cx->CTLR2 = tmpreg;                      // 设置I2C 模块时钟频率域。

    I2Cx->CTLR1 &= CTLR1_PE_Reset;             // 清除I2C外设使能位。禁用I2C外设。
    tmpreg = 0;                                // 再次初始化tmpreg。

    if(I2C_InitStruct->I2C_ClockSpeed
                    <= 100000)                 // 如果I2C时钟小于100K。说明为标准模式。这里假设为100K。
    {
        result = (uint16_t)(pclk1 /
           (I2C_InitStruct->I2C_ClockSpeed
                    << 1));                    // 首先I2C时钟左移一位，变成200K。之后被主频除，等于240。
        if(result < 0x04)
        {                                      // 因此上tmpreg等于0xF0，也就是240。
            result = 0x04;                     // 这个值被用于设置I2C 时钟寄存器（I2C1_CKCFGR）。
        }                                      // 此时主模式选择位为0：标准模式。快速模式时的占空比也为0。
        tmpreg |= result;                      // 都无须设置。tmpreg的0xF0这个值可以直接用。
    }
    else                                       // 如果I2C时钟大于100K。说明为快速模式。这里假设为120K。
    {
        if(I2C_InitStruct->I2C_DutyCycle
                     == I2C_DutyCycle_2)       // 如果占空比为2:1。
        {
            result = (uint16_t)(pclk1 / 
             (I2C_InitStruct->I2C_ClockSpeed
                     * 3));                    // 首先I2C时钟乘3，变成360K。之后被主频除，等于133。
        }
        else                                   // 如果占空比为16:9。
        {
            result = (uint16_t)(pclk1 / 
             (I2C_InitStruct->I2C_ClockSpeed
                   * 25));                     // 首先I2C时钟乘25，变成360K。之后被主频除，等于16。
            result |= I2C_DutyCycle_16_9;      // 设置快速模式时的占空比的标志位。
        }

        if((result & CKCFGR_CCR_Set) == 0)     // 如果计算结果是4K的倍数，结果加一。
        {
            result |= (uint16_t)0x0001;
        }

        tmpreg |= (uint16_t)(result
                        | CKCFGR_FS_Set);      // 主模式选择位为1：快速模式。
    }

    I2Cx->CKCFGR = tmpreg;                     // 使用tmpreg设置I2C 时钟寄存器（I2C1_CKCFGR）。
    I2Cx->CTLR1 |= CTLR1_PE_Set;               // I2C外设使能位置位。使能I2C外设。
                                         
    tmpreg = I2Cx->CTLR1;                      // 设置ACK标志。
    tmpreg &= CTLR1_CLEAR_Mask;
    tmpreg |= (uint16_t)((uint32_t)I2C_InitStruct->I2C_Mode
                    | I2C_InitStruct->I2C_Ack);
    I2Cx->CTLR1 = tmpreg;

    I2Cx->OADDR1 = 
      (I2C_InitStruct->I2C_AcknowledgedAddress
          | I2C_InitStruct->I2C_OwnAddress1);  // 设置I2C地址和地址格式。
}