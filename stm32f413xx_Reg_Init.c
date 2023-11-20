#include "stm32f413xx.h"

void Init(void)
{

  ADC1->SR = 0x0000;    /*!< ADC status register,                         Address offset: 0x00 */
  ADC1->CR1 = 0x0000;   /*!< ADC control register 1,                      Address offset: 0x04 */
  ADC1->CR2 = 0x0000;   /*!< ADC control register 2,                      Address offset: 0x08 */
  ADC1->SMPR1 = 0x0000; /*!< ADC sample time register 1,                  Address offset: 0x0C */
  ADC1->SMPR2 = 0x0000; /*!< ADC sample time register 2,                  Address offset: 0x10 */
  ADC1->JOFR1 = 0x0000; /*!< ADC injected channel data offset register 1, Address offset: 0x14 */
  ADC1->JOFR2 = 0x0000; /*!< ADC injected channel data offset register 2, Address offset: 0x18 */
  ADC1->JOFR3 = 0x0000; /*!< ADC injected channel data offset register 3, Address offset: 0x1C */
  ADC1->JOFR4 = 0x0000; /*!< ADC injected channel data offset register 4, Address offset: 0x20 */
  ADC1->HTR = 0x0000;   /*!< ADC watchdog higher threshold register,      Address offset: 0x24 */
  ADC1->LTR = 0x0000;   /*!< ADC watchdog lower threshold register,       Address offset: 0x28 */
  ADC1->SQR1 = 0x0000;  /*!< ADC regular sequence register 1,             Address offset: 0x2C */
  ADC1->SQR2 = 0x0000;  /*!< ADC regular sequence register 2,             Address offset: 0x30 */
  ADC1->SQR3 = 0x0000;  /*!< ADC regular sequence register 3,             Address offset: 0x34 */
  ADC1->JSQR = 0x0000;  /*!< ADC injected sequence register,              Address offset: 0x38 */
  ADC1->JDR1 = 0x0000;  /*!< ADC injected data register 1,                Address offset: 0x3C */
  ADC1->JDR2 = 0x0000;  /*!< ADC injected data register 2,                Address offset: 0x40 */
  ADC1->JDR3 = 0x0000;  /*!< ADC injected data register 3,                Address offset: 0x44 */
  ADC1->JDR4 = 0x0000;  /*!< ADC injected data register 4,                Address offset: 0x48 */
  ADC1->DR = 0x0000;    /*!< ADC regular data register,                   Address offset: 0x4C */

  ADC1_COMMON->CSR; /*!< ADC Common status register,                  Address offset: ADC1 base address + 0x300 */
  ADC1_COMMON->CCR; /*!< ADC common control register,                 Address offset: ADC1 base address + 0x304 */
  ADC1_COMMON->CDR; /*!< ADC common regular data register for dual
                          AND triple modes,                            Address offset: ADC1 base address + 0x308 */

  /**
   * @brief Controller Area Network
   */

  CAN1->MCR;                     /*!< CAN master control register,                           Address offset: 0x00  */
  CAN1->MSR;                     /*!< CAN master status register,                            Address offset: 0x04  */
  CAN1->TSR;                     /*!< CAN transmit status register,                          Address offset: 0x08  */
  CAN1->RF0R;                    /*!< CAN receive FIFO 0 register,                           Address offset: 0x0C  */
  CAN1->RF1R;                    /*!< CAN receive FIFO 1 register,                           Address offset: 0x10  */
  CAN1->IER;                     /*!< CAN interrupt enable register,                         Address offset: 0x14  */
  CAN1->ESR;                     /*!< CAN error status register,                             Address offset: 0x18  */
  CAN1->BTR;                     /*!< CAN bit timing register,                               Address offset: 0x1C  */
  CAN1->sTxMailBox[0].TIR;       /*!< CAN TX mailbox 0 identifier register,                  Address offset: 0x180 */
  CAN1->sTxMailBox[0].TDTR;      /*!< CAN TX mailbox 0 data len ctrl & time stamp reg,       Address offset: 0x184 */
  CAN1->sTxMailBox[0].TDLR;      /*!< CAN Tx mailbox 0 data low register,                    Address offset: 0x188 */
  CAN1->sTxMailBox[0].TDHR;      /*!< CAN Tx mailbox 0 data high register,                   Address offset: 0x18C */
  CAN1->sTxMailBox[1].TIR;       /*!< CAN TX mailbox 1 identifier register,                  Address offset: 0x190 */
  CAN1->sTxMailBox[1].TDTR;      /*!< CAN TX mailbox 1 data len ctrl & time stamp reg,       Address offset: 0x194 */
  CAN1->sTxMailBox[1].TDLR;      /*!< CAN Tx mailbox 1 data low register,                    Address offset: 0x198 */
  CAN1->sTxMailBox[1].TDHR;      /*!< CAN Tx mailbox 1 data high register,                   Address offset: 0x19C */
  CAN1->sTxMailBox[2].TIR;       /*!< CAN TX mailbox 2 identifier register,                  Address offset: 0x1A0 */
  CAN1->sTxMailBox[2].TDTR;      /*!< CAN TX mailbox 2 data len ctrl & time stamp reg,       Address offset: 0x1A4 */
  CAN1->sTxMailBox[2].TDLR;      /*!< CAN Tx mailbox 2 data low register,                    Address offset: 0x1A8 */
  CAN1->sTxMailBox[2].TDHR;      /*!< CAN Tx mailbox 2 data high register,                   Address offset: 0x1AC */
  CAN1->sFIFOMailBox[0].RIR;     /*!< CAN RX FIFO mailbox 0 identifier register,             Address offset: 0x1B0 */
  CAN1->sFIFOMailBox[0].RDTR;    /*!< CAN RX FIFO mailbox 0 data len ctrl & time stamp reg,  Address offset: 0x1B4 */
  CAN1->sFIFOMailBox[0].RDLR;    /*!< CAN RX FIFO mailbox 0 data low register,               Address offset: 0x1B8 */
  CAN1->sFIFOMailBox[0].RDHR;    /*!< CAN RX FIFO mailbox 0 data high register,              Address offset: 0x1BC */
  CAN1->sFIFOMailBox[1].RIR;     /*!< CAN RX FIFO mailbox 1 identifier register,             Address offset: 0x1C0 */
  CAN1->sFIFOMailBox[1].RDTR;    /*!< CAN RX FIFO mailbox 1 data len ctrl & time stamp reg,  Address offset: 0x1C4 */
  CAN1->sFIFOMailBox[1].RDLR;    /*!< CAN RX FIFO mailbox 1 data low register,               Address offset: 0x1C8 */
  CAN1->sFIFOMailBox[1].RDHR;    /*!< CAN RX FIFO mailbox 1 data high register,              Address offset: 0x1CC */
  CAN1->FMR;                     /*!< CAN filter master register,                            Address offset: 0x200 */
  CAN1->FM1R;                    /*!< CAN filter mode register,                              Address offset: 0x204 */
  CAN1->FS1R;                    /*!< CAN filter scale register,                             Address offset: 0x20C */
  CAN1->FFA1R;                   /*!< CAN filter FIFO assignment register,                   Address offset: 0x214 */
  CAN1->FA1R;                    /*!< CAN filter activation register,                        Address offset: 0x21C */
  CAN1->sFilterRegister[0].FR1;  /*!< CAN Filter 0 Filter bank register 1,                   Address offset: 0x240 */
  CAN1->sFilterRegister[0].FR2;  /*!< CAN Filter 0 Filter bank register 2,                   Address offset: 0x244 */
  CAN1->sFilterRegister[1].FR1;  /*!< CAN Filter 1 Filter bank register 1,                   Address offset: 0x248 */
  CAN1->sFilterRegister[1].FR2;  /*!< CAN Filter 1 Filter bank register 2,                   Address offset: 0x24C */
  CAN1->sFilterRegister[2].FR1;  /*!< CAN Filter 2 Filter bank register 1,                   Address offset: 0x250 */
  CAN1->sFilterRegister[2].FR2;  /*!< CAN Filter 2 Filter bank register 2,                   Address offset: 0x254 */
  CAN1->sFilterRegister[3].FR1;  /*!< CAN Filter 3 Filter bank register 1,                   Address offset: 0x258 */
  CAN1->sFilterRegister[3].FR2;  /*!< CAN Filter 3 Filter bank register 2,                   Address offset: 0x25C */
  CAN1->sFilterRegister[4].FR1;  /*!< CAN Filter 4 Filter bank register 1,                   Address offset: 0x260 */
  CAN1->sFilterRegister[4].FR2;  /*!< CAN Filter 4 Filter bank register 2,                   Address offset: 0x264 */
  CAN1->sFilterRegister[5].FR1;  /*!< CAN Filter 5 Filter bank register 1,                   Address offset: 0x268 */
  CAN1->sFilterRegister[5].FR2;  /*!< CAN Filter 5 Filter bank register 2,                   Address offset: 0x26C */
  CAN1->sFilterRegister[6].FR1;  /*!< CAN Filter 6 Filter bank register 1,                   Address offset: 0x270 */
  CAN1->sFilterRegister[6].FR2;  /*!< CAN Filter 6 Filter bank register 2,                   Address offset: 0x274 */
  CAN1->sFilterRegister[7].FR1;  /*!< CAN Filter 7 Filter bank register 1,                   Address offset: 0x278 */
  CAN1->sFilterRegister[7].FR2;  /*!< CAN Filter 7 Filter bank register 2,                   Address offset: 0x27C */
  CAN1->sFilterRegister[8].FR1;  /*!< CAN Filter 8 Filter bank register 1,                   Address offset: 0x280 */
  CAN1->sFilterRegister[8].FR2;  /*!< CAN Filter 8 Filter bank register 2,                   Address offset: 0x284 */
  CAN1->sFilterRegister[9].FR1;  /*!< CAN Filter 9 Filter bank register 1,                   Address offset: 0x288 */
  CAN1->sFilterRegister[9].FR2;  /*!< CAN Filter 9 Filter bank register 2,                   Address offset: 0x28C */
  CAN1->sFilterRegister[10].FR1; /*!< CAN Filter 10 Filter bank register 1,                  Address offset: 0x290 */
  CAN1->sFilterRegister[10].FR2; /*!< CAN Filter 10 Filter bank register 2,                  Address offset: 0x294 */
  CAN1->sFilterRegister[11].FR1; /*!< CAN Filter 11 Filter bank register 1,                  Address offset: 0x298 */
  CAN1->sFilterRegister[11].FR2; /*!< CAN Filter 11 Filter bank register 2,                  Address offset: 0x29C */
  CAN1->sFilterRegister[12].FR1; /*!< CAN Filter 12 Filter bank register 1,                  Address offset: 0x2A0 */
  CAN1->sFilterRegister[12].FR2; /*!< CAN Filter 12 Filter bank register 2,                  Address offset: 0x2A4 */
  CAN1->sFilterRegister[13].FR1; /*!< CAN Filter 13 Filter bank register 1,                  Address offset: 0x2A8 */
  CAN1->sFilterRegister[13].FR2; /*!< CAN Filter 13 Filter bank register 2,                  Address offset: 0x2AC */
  CAN1->sFilterRegister[14].FR1; /*!< CAN Filter 14 Filter bank register 1,                  Address offset: 0x2B0 */
  CAN1->sFilterRegister[14].FR2; /*!< CAN Filter 14 Filter bank register 2,                  Address offset: 0x2B4 */
  CAN1->sFilterRegister[15].FR1; /*!< CAN Filter 15 Filter bank register 1,                  Address offset: 0x2B8 */
  CAN1->sFilterRegister[15].FR2; /*!< CAN Filter 15 Filter bank register 2,                  Address offset: 0x2BC */
  CAN1->sFilterRegister[16].FR1; /*!< CAN Filter 16 Filter bank register 1,                  Address offset: 0x2C0 */
  CAN1->sFilterRegister[16].FR2; /*!< CAN Filter 16 Filter bank register 2,                  Address offset: 0x2C4 */
  CAN1->sFilterRegister[17].FR1; /*!< CAN Filter 17 Filter bank register 1,                  Address offset: 0x2C8 */
  CAN1->sFilterRegister[17].FR2; /*!< CAN Filter 17 Filter bank register 2,                  Address offset: 0x2CC */
  CAN1->sFilterRegister[18].FR1; /*!< CAN Filter 18 Filter bank register 1,                  Address offset: 0x2D0 */
  CAN1->sFilterRegister[18].FR2; /*!< CAN Filter 18 Filter bank register 2,                  Address offset: 0x2D4 */
  CAN1->sFilterRegister[19].FR1; /*!< CAN Filter 19 Filter bank register 1,                  Address offset: 0x2D8 */
  CAN1->sFilterRegister[19].FR2; /*!< CAN Filter 19 Filter bank register 2,                  Address offset: 0x2DC */
  CAN1->sFilterRegister[20].FR1; /*!< CAN Filter 20 Filter bank register 1,                  Address offset: 0x2E0 */
  CAN1->sFilterRegister[20].FR2; /*!< CAN Filter 20 Filter bank register 2,                  Address offset: 0x2E4 */
  CAN1->sFilterRegister[21].FR1; /*!< CAN Filter 21 Filter bank register 1,                  Address offset: 0x2E8 */
  CAN1->sFilterRegister[21].FR2; /*!< CAN Filter 21 Filter bank register 2,                  Address offset: 0x2EC */
  CAN1->sFilterRegister[22].FR1; /*!< CAN Filter 22 Filter bank register 1,                  Address offset: 0x2F0 */
  CAN1->sFilterRegister[22].FR2; /*!< CAN Filter 22 Filter bank register 2,                  Address offset: 0x2F4 */
  CAN1->sFilterRegister[23].FR1; /*!< CAN Filter 23 Filter bank register 1,                  Address offset: 0x2F8 */
  CAN1->sFilterRegister[23].FR2; /*!< CAN Filter 23 Filter bank register 2,                  Address offset: 0x2FC */
  CAN1->sFilterRegister[24].FR1; /*!< CAN Filter 24 Filter bank register 1,                  Address offset: 0x300 */
  CAN1->sFilterRegister[24].FR2; /*!< CAN Filter 24 Filter bank register 2,                  Address offset: 0x304 */
  CAN1->sFilterRegister[25].FR1; /*!< CAN Filter 25 Filter bank register 1,                  Address offset: 0x308 */
  CAN1->sFilterRegister[25].FR2; /*!< CAN Filter 25 Filter bank register 2,                  Address offset: 0x30C */
  CAN1->sFilterRegister[26].FR1; /*!< CAN Filter 26 Filter bank register 1,                  Address offset: 0x310 */
  CAN1->sFilterRegister[26].FR2; /*!< CAN Filter 26 Filter bank register 2,                  Address offset: 0x314 */
  CAN1->sFilterRegister[27].FR1; /*!< CAN Filter 27 Filter bank register 1,                  Address offset: 0x318 */
  CAN1->sFilterRegister[27].FR2; /*!< CAN Filter 27 Filter bank register 2,                  Address offset: 0x31C */
  // CAN_TypeDef;

  /**
   * @brief CRC calculation unit
   */

  CRC->DR;  /*!< CRC Data register,             Address offset: 0x00 */
  CRC->IDR; /*!< CRC Independent data register, Address offset: 0x04 */
  CRC->CR;  /*!< CRC Control register,          Address offset: 0x08 */
  // CRC_TypeDef;

  DFSDM1_Filter0->FLTCR1;     /*!< DFSDM control register1,                           Address offset: 0x100 */
  DFSDM1_Filter0->FLTCR2;     /*!< DFSDM control register2,                           Address offset: 0x104 */
  DFSDM1_Filter0->FLTISR;     /*!< DFSDM interrupt and status register,               Address offset: 0x108 */
  DFSDM1_Filter0->FLTICR;     /*!< DFSDM interrupt flag clear register,               Address offset: 0x10C */
  DFSDM1_Filter0->FLTJCHGR;   /*!< DFSDM injected channel group selection register,   Address offset: 0x110 */
  DFSDM1_Filter0->FLTFCR;     /*!< DFSDM filter control register,                     Address offset: 0x114 */
  DFSDM1_Filter0->FLTJDATAR;  /*!< DFSDM data register for injected group,            Address offset: 0x118 */
  DFSDM1_Filter0->FLTRDATAR;  /*!< DFSDM data register for regular group,             Address offset: 0x11C */
  DFSDM1_Filter0->FLTAWHTR;   /*!< DFSDM analog watchdog high threshold register,     Address offset: 0x120 */
  DFSDM1_Filter0->FLTAWLTR;   /*!< DFSDM analog watchdog low threshold register,      Address offset: 0x124 */
  DFSDM1_Filter0->FLTAWSR;    /*!< DFSDM analog watchdog status register              Address offset: 0x128 */
  DFSDM1_Filter0->FLTAWCFR;   /*!< DFSDM analog watchdog clear flag register          Address offset: 0x12C */
  DFSDM1_Filter0->FLTEXMAX;   /*!< DFSDM extreme detector maximum register,           Address offset: 0x130 */
  DFSDM1_Filter0->FLTEXMIN;   /*!< DFSDM extreme detector minimum register            Address offset: 0x134 */
  DFSDM1_Filter0->FLTCNVTIMR; /*!< DFSDM conversion timer,                            Address offset: 0x138 */
  // DFSDM_Filter_TypeDef;

  /**
   * @brief DFSDM channel configuration registers
   */

  DFSDM1_Channel0->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM1_Channel0->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM1_Channel0->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM1_Channel0->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM1_Channel0->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM1_Channel1->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM1_Channel1->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM1_Channel1->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM1_Channel1->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM1_Channel1->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM1_Channel2->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM1_Channel2->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM1_Channel2->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM1_Channel2->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM1_Channel2->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM1_Channel3->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM1_Channel3->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM1_Channel3->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM1_Channel3->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM1_Channel3->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM2_Channel0->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM2_Channel0->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM2_Channel0->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM2_Channel0->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM2_Channel0->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM2_Channel1->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM2_Channel1->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM2_Channel1->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM2_Channel1->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM2_Channel1->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM2_Channel2->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM2_Channel2->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM2_Channel2->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM2_Channel2->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM2_Channel2->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM2_Channel3->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM2_Channel3->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM2_Channel3->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM2_Channel3->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM2_Channel3->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM2_Channel4->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM2_Channel4->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM2_Channel4->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM2_Channel4->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM2_Channel4->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM2_Channel5->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM2_Channel5->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM2_Channel5->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM2_Channel5->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM2_Channel5->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM2_Channel6->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM2_Channel6->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM2_Channel6->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM2_Channel6->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM2_Channel6->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  DFSDM2_Channel7->CHCFGR1;  /*!< DFSDM channel configuration register1,                               Address offset: 0x00 */
  DFSDM2_Channel7->CHCFGR2;  /*!< DFSDM channel configuration register2,                               Address offset: 0x04 */
  DFSDM2_Channel7->CHAWSCDR; /*!< DFSDM channel analog watchdog and short circuit detector register,   Address offset: 0x08 */
  DFSDM2_Channel7->CHWDATAR; /*!< DFSDM channel watchdog filter data register,                         Address offset: 0x0C */
  DFSDM2_Channel7->CHDATINR; /*!< DFSDM channel data input register,                                   Address offset: 0x10 */
  // DFSDM_Channel_TypeDef;

  /**
   * @brief Digital to Analog Converter
   */

  DAC1->CR;      /*!< DAC control register,                                    Address offset: 0x00 */
  DAC1->SWTRIGR; /*!< DAC software trigger register,                           Address offset: 0x04 */
  DAC1->DHR12R1; /*!< DAC channel1 12-bit right-aligned data holding register, Address offset: 0x08 */
  DAC1->DHR12L1; /*!< DAC channel1 12-bit left aligned data holding register,  Address offset: 0x0C */
  DAC1->DHR8R1;  /*!< DAC channel1 8-bit right aligned data holding register,  Address offset: 0x10 */
  DAC1->DHR12R2; /*!< DAC channel2 12-bit right aligned data holding register, Address offset: 0x14 */
  DAC1->DHR12L2; /*!< DAC channel2 12-bit left aligned data holding register,  Address offset: 0x18 */
  DAC1->DHR8R2;  /*!< DAC channel2 8-bit right-aligned data holding register,  Address offset: 0x1C */
  DAC1->DHR12RD; /*!< Dual DAC 12-bit right-aligned data holding register,     Address offset: 0x20 */
  DAC1->DHR12LD; /*!< DUAL DAC 12-bit left aligned data holding register,      Address offset: 0x24 */
  DAC1->DHR8RD;  /*!< DUAL DAC 8-bit right aligned data holding register,      Address offset: 0x28 */
  DAC1->DOR1;    /*!< DAC channel1 data output register,                       Address offset: 0x2C */
  DAC1->DOR2;    /*!< DAC channel2 data output register,                       Address offset: 0x30 */
  DAC1->SR;      /*!< DAC status register,                                     Address offset: 0x34 */
  // DAC_TypeDef;

  /**
   * @brief Debug MCU
   */

  DBGMCU->IDCODE; /*!< MCU device ID code,               Address offset: 0x00 */
  DBGMCU->CR;     /*!< Debug MCU configuration register, Address offset: 0x04 */
  DBGMCU->APB1FZ; /*!< Debug MCU APB1 freeze register,   Address offset: 0x08 */
  DBGMCU->APB2FZ; /*!< Debug MCU APB2 freeze register,   Address offset: 0x0C */
  // DBGMCU_TypeDef;

  /**
   * @brief DMA Controller
   */

  DMA1_Stream0->CR;   /*!< DMA stream x configuration register      */
  DMA1_Stream0->NDTR; /*!< DMA stream x number of data register     */
  DMA1_Stream0->PAR;  /*!< DMA stream x peripheral address register */
  DMA1_Stream0->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA1_Stream0->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA1_Stream0->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA1_Stream1->CR;   /*!< DMA stream x configuration register      */
  DMA1_Stream1->NDTR; /*!< DMA stream x number of data register     */
  DMA1_Stream1->PAR;  /*!< DMA stream x peripheral address register */
  DMA1_Stream1->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA1_Stream1->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA1_Stream1->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA1_Stream2->CR;   /*!< DMA stream x configuration register      */
  DMA1_Stream2->NDTR; /*!< DMA stream x number of data register     */
  DMA1_Stream2->PAR;  /*!< DMA stream x peripheral address register */
  DMA1_Stream2->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA1_Stream2->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA1_Stream2->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA1_Stream3->CR;   /*!< DMA stream x configuration register      */
  DMA1_Stream3->NDTR; /*!< DMA stream x number of data register     */
  DMA1_Stream3->PAR;  /*!< DMA stream x peripheral address register */
  DMA1_Stream3->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA1_Stream3->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA1_Stream3->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA1_Stream4->CR;   /*!< DMA stream x configuration register      */
  DMA1_Stream4->NDTR; /*!< DMA stream x number of data register     */
  DMA1_Stream4->PAR;  /*!< DMA stream x peripheral address register */
  DMA1_Stream4->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA1_Stream4->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA1_Stream4->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA1_Stream5->CR;   /*!< DMA stream x configuration register      */
  DMA1_Stream5->NDTR; /*!< DMA stream x number of data register     */
  DMA1_Stream5->PAR;  /*!< DMA stream x peripheral address register */
  DMA1_Stream5->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA1_Stream5->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA1_Stream5->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA1_Stream6->CR;   /*!< DMA stream x configuration register      */
  DMA1_Stream6->NDTR; /*!< DMA stream x number of data register     */
  DMA1_Stream6->PAR;  /*!< DMA stream x peripheral address register */
  DMA1_Stream6->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA1_Stream6->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA1_Stream6->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA1_Stream7->CR;   /*!< DMA stream x configuration register      */
  DMA1_Stream7->NDTR; /*!< DMA stream x number of data register     */
  DMA1_Stream7->PAR;  /*!< DMA stream x peripheral address register */
  DMA1_Stream7->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA1_Stream7->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA1_Stream7->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA2_Stream0->CR;   /*!< DMA stream x configuration register      */
  DMA2_Stream0->NDTR; /*!< DMA stream x number of data register     */
  DMA2_Stream0->PAR;  /*!< DMA stream x peripheral address register */
  DMA2_Stream0->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA2_Stream0->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA2_Stream0->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA2_Stream1->CR;   /*!< DMA stream x configuration register      */
  DMA2_Stream1->NDTR; /*!< DMA stream x number of data register     */
  DMA2_Stream1->PAR;  /*!< DMA stream x peripheral address register */
  DMA2_Stream1->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA2_Stream1->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA2_Stream1->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA2_Stream2->CR;   /*!< DMA stream x configuration register      */
  DMA2_Stream2->NDTR; /*!< DMA stream x number of data register     */
  DMA2_Stream2->PAR;  /*!< DMA stream x peripheral address register */
  DMA2_Stream2->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA2_Stream2->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA2_Stream2->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA2_Stream3->CR;   /*!< DMA stream x configuration register      */
  DMA2_Stream3->NDTR; /*!< DMA stream x number of data register     */
  DMA2_Stream3->PAR;  /*!< DMA stream x peripheral address register */
  DMA2_Stream3->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA2_Stream3->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA2_Stream3->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA2_Stream4->CR;   /*!< DMA stream x configuration register      */
  DMA2_Stream4->NDTR; /*!< DMA stream x number of data register     */
  DMA2_Stream4->PAR;  /*!< DMA stream x peripheral address register */
  DMA2_Stream4->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA2_Stream4->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA2_Stream4->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA2_Stream5->CR;   /*!< DMA stream x configuration register      */
  DMA2_Stream5->NDTR; /*!< DMA stream x number of data register     */
  DMA2_Stream5->PAR;  /*!< DMA stream x peripheral address register */
  DMA2_Stream5->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA2_Stream5->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA2_Stream5->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA2_Stream6->CR;   /*!< DMA stream x configuration register      */
  DMA2_Stream6->NDTR; /*!< DMA stream x number of data register     */
  DMA2_Stream6->PAR;  /*!< DMA stream x peripheral address register */
  DMA2_Stream6->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA2_Stream6->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA2_Stream6->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA2_Stream7->CR;   /*!< DMA stream x configuration register      */
  DMA2_Stream7->NDTR; /*!< DMA stream x number of data register     */
  DMA2_Stream7->PAR;  /*!< DMA stream x peripheral address register */
  DMA2_Stream7->M0AR; /*!< DMA stream x memory 0 address register   */
  DMA2_Stream7->M1AR; /*!< DMA stream x memory 1 address register   */
  DMA2_Stream7->FCR;  /*!< DMA stream x FIFO control register       */
  // DMA_Stream_TypeDef;

  DMA1->LISR;  /*!< DMA low interrupt status register,      Address offset: 0x00 */
  DMA1->HISR;  /*!< DMA high interrupt status register,     Address offset: 0x04 */
  DMA1->LIFCR; /*!< DMA low interrupt flag clear register,  Address offset: 0x08 */
  DMA1->HIFCR; /*!< DMA high interrupt flag clear register, Address offset: 0x0C */
  // DMA_TypeDef;

  DMA2->LISR;  /*!< DMA low interrupt status register,      Address offset: 0x00 */
  DMA2->HISR;  /*!< DMA high interrupt status register,     Address offset: 0x04 */
  DMA2->LIFCR; /*!< DMA low interrupt flag clear register,  Address offset: 0x08 */
  DMA2->HIFCR; /*!< DMA high interrupt flag clear register, Address offset: 0x0C */
  // DMA_TypeDef;

  /**
   * @brief External Interrupt/Event Controller
   */

  EXTI->IMR;   /*!< EXTI Interrupt mask register,            Address offset: 0x00 */
  EXTI->EMR;   /*!< EXTI Event mask register,                Address offset: 0x04 */
  EXTI->RTSR;  /*!< EXTI Rising trigger selection register,  Address offset: 0x08 */
  EXTI->FTSR;  /*!< EXTI Falling trigger selection register, Address offset: 0x0C */
  EXTI->SWIER; /*!< EXTI Software interrupt event register,  Address offset: 0x10 */
  EXTI->PR;    /*!< EXTI Pending register,                   Address offset: 0x14 */
  // EXTI_TypeDef;

  /**
   * @brief FLASH Registers
   */

  FLASH->ACR;     /*!< FLASH access control register,   Address offset: 0x00 */
  FLASH->KEYR;    /*!< FLASH key register,              Address offset: 0x04 */
  FLASH->OPTKEYR; /*!< FLASH option key register,       Address offset: 0x08 */
  FLASH->SR;      /*!< FLASH status register,           Address offset: 0x0C */
  FLASH->CR;      /*!< FLASH control register,          Address offset: 0x10 */
  FLASH->OPTCR;   /*!< FLASH option control register ,  Address offset: 0x14 */
  FLASH->OPTCR1;  /*!< FLASH option control register 1, Address offset: 0x18 */
  // FLASH_TypeDef;

  /**
   * @brief Flexible Static Memory Controller
   */

  FSMC_Bank1->BTCR[0]; /*!< NOR/PSRAM chip-select ctrl reg(BCR) and chip-select timing reg(BTR), Address offset: 0x00 */
  FSMC_Bank1->BTCR[1]; /*!< NOR/PSRAM chip-select ctrl reg(BCR) and chip-select timing reg(BTR), Address offset: 0x04 */
  FSMC_Bank1->BTCR[2]; /*!< NOR/PSRAM chip-select ctrl reg(BCR) and chip-select timing reg(BTR), Address offset: 0x08 */
  FSMC_Bank1->BTCR[3]; /*!< NOR/PSRAM chip-select ctrl reg(BCR) and chip-select timing reg(BTR), Address offset: 0x0C */
  FSMC_Bank1->BTCR[4]; /*!< NOR/PSRAM chip-select ctrl reg(BCR) and chip-select timing reg(BTR), Address offset: 0x10 */
  FSMC_Bank1->BTCR[5]; /*!< NOR/PSRAM chip-select ctrl reg(BCR) and chip-select timing reg(BTR), Address offset: 0x14 */
  FSMC_Bank1->BTCR[6]; /*!< NOR/PSRAM chip-select ctrl reg(BCR) and chip-select timing reg(BTR), Address offset: 0x18 */
  FSMC_Bank1->BTCR[7]; /*!< NOR/PSRAM chip-select ctrl reg(BCR) and chip-select timing reg(BTR), Address offset: 0x1C */
  // FSMC_Bank1_TypeDef;

  /**
   * @brief Flexible Static Memory Controller Bank1E
   */

  FSMC_Bank1E->BWTR[0]; /*!< NOR/PSRAM write timing registers, Address offset: 0x104 */
  FSMC_Bank1E->BWTR[1]; /*!< NOR/PSRAM write timing registers, Address offset: 0x108 */
  FSMC_Bank1E->BWTR[2]; /*!< NOR/PSRAM write timing registers, Address offset: 0x10C */
  FSMC_Bank1E->BWTR[3]; /*!< NOR/PSRAM write timing registers, Address offset: 0x110 */
  FSMC_Bank1E->BWTR[4]; /*!< NOR/PSRAM write timing registers, Address offset: 0x114 */
  FSMC_Bank1E->BWTR[5]; /*!< NOR/PSRAM write timing registers, Address offset: 0x118 */
  FSMC_Bank1E->BWTR[6]; /*!< NOR/PSRAM write timing registers, Address offset: 0x11C */
  // FSMC_Bank1E_TypeDef;
  /**
   * @brief General Purpose I/O
   */

  GPIOA->MODER = 0x0000;   /*!< GPIO port mode register,               Address offset: 0x00 */
  GPIOA->OTYPER = 0x0000;  /*!< GPIO port output type register,        Address offset: 0x04 */
  GPIOA->OSPEEDR = 0x0000; /*!< GPIO port output speed register,       Address offset: 0x08 */
  GPIOA->PUPDR = 0x0000;   /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
  GPIOA->IDR = 0x0000;     /*!< GPIO port input data register,         Address offset: 0x10 */
  GPIOA->ODR = 0x0000;     /*!< GPIO port output data register,        Address offset: 0x14 */
  GPIOA->BSRR = 0x0000;    /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
  GPIOA->LCKR = 0x0000;    /*!< GPIO port configuration lock register, Address offset: 0x1C */
  GPIOA->AFR[0] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x20 */
  GPIOA->AFR[1] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x24 */
  // GPIO_TypeDef;

  GPIOB->MODER = 0x0000;   /*!< GPIO port mode register,               Address offset: 0x00 */
  GPIOB->OTYPER = 0x0000;  /*!< GPIO port output type register,        Address offset: 0x04 */
  GPIOB->OSPEEDR = 0x0000; /*!< GPIO port output speed register,       Address offset: 0x08 */
  GPIOB->PUPDR = 0x0000;   /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
  GPIOB->IDR = 0x0000;     /*!< GPIO port input data register,         Address offset: 0x10 */
  GPIOB->ODR = 0x0000;     /*!< GPIO port output data register,        Address offset: 0x14 */
  GPIOB->BSRR = 0x0000;    /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
  GPIOB->LCKR = 0x0000;    /*!< GPIO port configuration lock register, Address offset: 0x1C */
  GPIOB->AFR[0] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x20 */
  GPIOB->AFR[1] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x24 */
  // GPIO_TypeDef;

  GPIOC->MODER = 0x0000;   /*!< GPIO port mode register,               Address offset: 0x00 */
  GPIOC->OTYPER = 0x0000;  /*!< GPIO port output type register,        Address offset: 0x04 */
  GPIOC->OSPEEDR = 0x0000; /*!< GPIO port output speed register,       Address offset: 0x08 */
  GPIOC->PUPDR = 0x0000;   /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
  GPIOC->IDR = 0x0000;     /*!< GPIO port input data register,         Address offset: 0x10 */
  GPIOC->ODR = 0x0000;     /*!< GPIO port output data register,        Address offset: 0x14 */
  GPIOC->BSRR = 0x0000;    /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
  GPIOC->LCKR = 0x0000;    /*!< GPIO port configuration lock register, Address offset: 0x1C */
  GPIOC->AFR[0] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x20 */
  GPIOC->AFR[1] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x24 */
  // GPIO_TypeDef;

  GPIOD->MODER = 0x0000;   /*!< GPIO port mode register,               Address offset: 0x00 */
  GPIOD->OTYPER = 0x0000;  /*!< GPIO port output type register,        Address offset: 0x04 */
  GPIOD->OSPEEDR = 0x0000; /*!< GPIO port output speed register,       Address offset: 0x08 */
  GPIOD->PUPDR = 0x0000;   /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
  GPIOD->IDR = 0x0000;     /*!< GPIO port input data register,         Address offset: 0x10 */
  GPIOD->ODR = 0x0000;     /*!< GPIO port output data register,        Address offset: 0x14 */
  GPIOD->BSRR = 0x0000;    /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
  GPIOD->LCKR = 0x0000;    /*!< GPIO port configuration lock register, Address offset: 0x1C */
  GPIOD->AFR[0] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x20 */
  GPIOD->AFR[1] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x24 */
  // GPIO_TypeDef;

  GPIOE->MODER = 0x0000;   /*!< GPIO port mode register,               Address offset: 0x00 */
  GPIOE->OTYPER = 0x0000;  /*!< GPIO port output type register,        Address offset: 0x04 */
  GPIOE->OSPEEDR = 0x0000; /*!< GPIO port output speed register,       Address offset: 0x08 */
  GPIOE->PUPDR = 0x0000;   /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
  GPIOE->IDR = 0x0000;     /*!< GPIO port input data register,         Address offset: 0x10 */
  GPIOE->ODR = 0x0000;     /*!< GPIO port output data register,        Address offset: 0x14 */
  GPIOE->BSRR = 0x0000;    /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
  GPIOE->LCKR = 0x0000;    /*!< GPIO port configuration lock register, Address offset: 0x1C */
  GPIOE->AFR[0] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x20 */
  GPIOE->AFR[1] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x24 */
  // GPIO_TypeDef;

  GPIOF->MODER = 0x0000;   /*!< GPIO port mode register,               Address offset: 0x00 */
  GPIOF->OTYPER = 0x0000;  /*!< GPIO port output type register,        Address offset: 0x04 */
  GPIOF->OSPEEDR = 0x0000; /*!< GPIO port output speed register,       Address offset: 0x08 */
  GPIOF->PUPDR = 0x0000;   /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
  GPIOF->IDR = 0x0000;     /*!< GPIO port input data register,         Address offset: 0x10 */
  GPIOF->ODR = 0x0000;     /*!< GPIO port output data register,        Address offset: 0x14 */
  GPIOF->BSRR = 0x0000;    /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
  GPIOF->LCKR = 0x0000;    /*!< GPIO port configuration lock register, Address offset: 0x1C */
  GPIOF->AFR[0] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x20 */
  GPIOF->AFR[1] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x24 */
  // GPIO_TypeDef;

  GPIOG->MODER = 0x0000;   /*!< GPIO port mode register,               Address offset: 0x00 */
  GPIOG->OTYPER = 0x0000;  /*!< GPIO port output type register,        Address offset: 0x04 */
  GPIOG->OSPEEDR = 0x0000; /*!< GPIO port output speed register,       Address offset: 0x08 */
  GPIOG->PUPDR = 0x0000;   /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
  GPIOG->IDR = 0x0000;     /*!< GPIO port input data register,         Address offset: 0x10 */
  GPIOG->ODR = 0x0000;     /*!< GPIO port output data register,        Address offset: 0x14 */
  GPIOG->BSRR = 0x0000;    /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
  GPIOG->LCKR = 0x0000;    /*!< GPIO port configuration lock register, Address offset: 0x1C */
  GPIOG->AFR[0] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x20 */
  GPIOG->AFR[1] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x24 */
  // GPIO_TypeDef;

  GPIOH->MODER = 0x0000;   /*!< GPIO port mode register,               Address offset: 0x00 */
  GPIOH->OTYPER = 0x0000;  /*!< GPIO port output type register,        Address offset: 0x04 */
  GPIOH->OSPEEDR = 0x0000; /*!< GPIO port output speed register,       Address offset: 0x08 */
  GPIOH->PUPDR = 0x0000;   /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
  GPIOH->IDR = 0x0000;     /*!< GPIO port input data register,         Address offset: 0x10 */
  GPIOH->ODR = 0x0000;     /*!< GPIO port output data register,        Address offset: 0x14 */
  GPIOH->BSRR = 0x0000;    /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
  GPIOH->LCKR = 0x0000;    /*!< GPIO port configuration lock register, Address offset: 0x1C */
  GPIOH->AFR[0] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x20 */
  GPIOH->AFR[1] = 0x0000;  /*!< GPIO alternate function registers,     Address offset: 0x24 */
  // GPIO_TypeDef;

  /**
   * @brief System configuration controller
   */

  SYSCFG->MEMRMP;    /*!< SYSCFG memory remap register,                      Address offset: 0x00 */
  SYSCFG->PMC;       /*!< SYSCFG peripheral mode configuration register,     Address offset: 0x04 */
  SYSCFG->EXTICR[0]; /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08 */
  SYSCFG->EXTICR[1]; /*!< SYSCFG external interrupt configuration registers, Address offset: 0x0C */
  SYSCFG->EXTICR[2]; /*!< SYSCFG external interrupt configuration registers, Address offset: 0x10 */
  SYSCFG->EXTICR[3]; /*!< SYSCFG external interrupt configuration registers, Address offset: 0x14 */
  SYSCFG->CFGR2;     /*!< SYSCFG Configuration register2,                    Address offset: 0x1C */
  SYSCFG->CMPCR;     /*!< SYSCFG Compensation cell control register,         Address offset: 0x20 */
  SYSCFG->CFGR;      /*!< SYSCFG Configuration register,                     Address offset: 0x2C */
  SYSCFG->MCHDLYCR;  /*!< SYSCFG multi-channel delay register,               Address offset: 0x30 */
  // SYSCFG_TypeDef;

  /**
   * @brief Inter-integrated Circuit Interface
   */

  I2C1->CR1;   /*!< I2C Control register 1,     Address offset: 0x00 */
  I2C1->CR2;   /*!< I2C Control register 2,     Address offset: 0x04 */
  I2C1->OAR1;  /*!< I2C Own address register 1, Address offset: 0x08 */
  I2C1->OAR2;  /*!< I2C Own address register 2, Address offset: 0x0C */
  I2C1->DR;    /*!< I2C Data register,          Address offset: 0x10 */
  I2C1->SR1;   /*!< I2C Status register 1,      Address offset: 0x14 */
  I2C1->SR2;   /*!< I2C Status register 2,      Address offset: 0x18 */
  I2C1->CCR;   /*!< I2C Clock control register, Address offset: 0x1C */
  I2C1->TRISE; /*!< I2C TRISE register,         Address offset: 0x20 */
  I2C1->FLTR;  /*!< I2C FLTR register,          Address offset: 0x24 */
  // I2C_TypeDef;

  I2C2->CR1;   /*!< I2C Control register 1,     Address offset: 0x00 */
  I2C2->CR2;   /*!< I2C Control register 2,     Address offset: 0x04 */
  I2C2->OAR1;  /*!< I2C Own address register 1, Address offset: 0x08 */
  I2C2->OAR2;  /*!< I2C Own address register 2, Address offset: 0x0C */
  I2C2->DR;    /*!< I2C Data register,          Address offset: 0x10 */
  I2C2->SR1;   /*!< I2C Status register 1,      Address offset: 0x14 */
  I2C2->SR2;   /*!< I2C Status register 2,      Address offset: 0x18 */
  I2C2->CCR;   /*!< I2C Clock control register, Address offset: 0x1C */
  I2C2->TRISE; /*!< I2C TRISE register,         Address offset: 0x20 */
  I2C2->FLTR;  /*!< I2C FLTR register,          Address offset: 0x24 */
  // I2C_TypeDef;

  I2C3->CR1;   /*!< I2C Control register 1,     Address offset: 0x00 */
  I2C3->CR2;   /*!< I2C Control register 2,     Address offset: 0x04 */
  I2C3->OAR1;  /*!< I2C Own address register 1, Address offset: 0x08 */
  I2C3->OAR2;  /*!< I2C Own address register 2, Address offset: 0x0C */
  I2C3->DR;    /*!< I2C Data register,          Address offset: 0x10 */
  I2C3->SR1;   /*!< I2C Status register 1,      Address offset: 0x14 */
  I2C3->SR2;   /*!< I2C Status register 2,      Address offset: 0x18 */
  I2C3->CCR;   /*!< I2C Clock control register, Address offset: 0x1C */
  I2C3->TRISE; /*!< I2C TRISE register,         Address offset: 0x20 */
  I2C3->FLTR;  /*!< I2C FLTR register,          Address offset: 0x24 */
  // I2C_TypeDef;

  /**
   * @brief Inter-integrated Circuit Interface
   */

  FMPI2C1->CR1;      /*!< FMPI2C Control register 1,            Address offset: 0x00 */
  FMPI2C1->CR2;      /*!< FMPI2C Control register 2,            Address offset: 0x04 */
  FMPI2C1->OAR1;     /*!< FMPI2C Own address 1 register,        Address offset: 0x08 */
  FMPI2C1->OAR2;     /*!< FMPI2C Own address 2 register,        Address offset: 0x0C */
  FMPI2C1->TIMINGR;  /*!< FMPI2C Timing register,               Address offset: 0x10 */
  FMPI2C1->TIMEOUTR; /*!< FMPI2C Timeout register,              Address offset: 0x14 */
  FMPI2C1->ISR;      /*!< FMPI2C Interrupt and status register, Address offset: 0x18 */
  FMPI2C1->ICR;      /*!< FMPI2C Interrupt clear register,      Address offset: 0x1C */
  FMPI2C1->PECR;     /*!< FMPI2C PEC register,                  Address offset: 0x20 */
  FMPI2C1->RXDR;     /*!< FMPI2C Receive data register,         Address offset: 0x24 */
  FMPI2C1->TXDR;     /*!< FMPI2C Transmit data register,        Address offset: 0x28 */
  // FMPI2C_TypeDef;

  /**
   * @brief Independent WATCHDOG
   */

  IWDG->KR;  /*!< IWDG Key register,       Address offset: 0x00 */
  IWDG->PR;  /*!< IWDG Prescaler register, Address offset: 0x04 */
  IWDG->RLR; /*!< IWDG Reload register,    Address offset: 0x08 */
  IWDG->SR;  /*!< IWDG Status register,    Address offset: 0x0C */
  // IWDG_TypeDef;

  /**
   * @brief Power Control
   */

  PWR->CR;  /*!< PWR power control register,        Address offset: 0x00 */
  PWR->CSR; /*!< PWR power control/status register, Address offset: 0x04 */
  // PWR_TypeDef;

  /**
   * @brief Reset and Clock Control
   */

  RCC->CR;         /*!< RCC clock control register,                                  Address offset: 0x00 */
  RCC->PLLCFGR;    /*!< RCC PLL configuration register,                              Address offset: 0x04 */
  RCC->CFGR;       /*!< RCC clock configuration register,                            Address offset: 0x08 */
  RCC->CIR;        /*!< RCC clock interrupt register,                                Address offset: 0x0C */
  RCC->AHB1RSTR;   /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
  RCC->AHB2RSTR;   /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
  RCC->AHB3RSTR;   /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
  RCC->APB1RSTR;   /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
  RCC->APB2RSTR;   /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
  RCC->AHB1ENR;    /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
  RCC->AHB2ENR;    /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
  RCC->AHB3ENR;    /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
  RCC->APB1ENR;    /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
  RCC->APB2ENR;    /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
  RCC->AHB1LPENR;  /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
  RCC->AHB2LPENR;  /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
  RCC->AHB3LPENR;  /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
  RCC->APB1LPENR;  /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
  RCC->APB2LPENR;  /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
  RCC->BDCR;       /*!< RCC Backup domain control register,                          Address offset: 0x70 */
  RCC->CSR;        /*!< RCC clock control & status register,                         Address offset: 0x74 */
  RCC->SSCGR;      /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
  RCC->PLLI2SCFGR; /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
  RCC->DCKCFGR;    /*!< RCC Dedicated Clocks configuration register,                 Address offset: 0x8C */
  RCC->CKGATENR;   /*!< RCC Clocks Gated ENable Register,                            Address offset: 0x90 */
  RCC->DCKCFGR2;   /*!< RCC Dedicated Clocks configuration register 2,               Address offset: 0x94 */
  // RCC_TypeDef;

  /**
   * @brief Real-Time Clock
   */

  RTC->TR;       /*!< RTC time register,                                        Address offset: 0x00 */
  RTC->DR;       /*!< RTC date register,                                        Address offset: 0x04 */
  RTC->CR;       /*!< RTC control register,                                     Address offset: 0x08 */
  RTC->ISR;      /*!< RTC initialization and status register,                   Address offset: 0x0C */
  RTC->PRER;     /*!< RTC prescaler register,                                   Address offset: 0x10 */
  RTC->WUTR;     /*!< RTC wakeup timer register,                                Address offset: 0x14 */
  RTC->CALIBR;   /*!< RTC calibration register,                                 Address offset: 0x18 */
  RTC->ALRMAR;   /*!< RTC alarm A register,                                     Address offset: 0x1C */
  RTC->ALRMBR;   /*!< RTC alarm B register,                                     Address offset: 0x20 */
  RTC->WPR;      /*!< RTC write protection register,                            Address offset: 0x24 */
  RTC->SSR;      /*!< RTC sub second register,                                  Address offset: 0x28 */
  RTC->SHIFTR;   /*!< RTC shift control register,                               Address offset: 0x2C */
  RTC->TSTR;     /*!< RTC time stamp time register,                             Address offset: 0x30 */
  RTC->TSDR;     /*!< RTC time stamp date register,                             Address offset: 0x34 */
  RTC->TSSSR;    /*!< RTC time-stamp sub second register,                       Address offset: 0x38 */
  RTC->CALR;     /*!< RTC calibration register,                                 Address offset: 0x3C */
  RTC->TAFCR;    /*!< RTC tamper and alternate function configuration register, Address offset: 0x40 */
  RTC->ALRMASSR; /*!< RTC alarm A sub second register,                          Address offset: 0x44 */
  RTC->ALRMBSSR; /*!< RTC alarm B sub second register,                          Address offset: 0x48 */
  RTC->BKP0R;    /*!< RTC backup register 1,                                    Address offset: 0x50 */
  RTC->BKP1R;    /*!< RTC backup register 1,                                    Address offset: 0x54 */
  RTC->BKP2R;    /*!< RTC backup register 2,                                    Address offset: 0x58 */
  RTC->BKP3R;    /*!< RTC backup register 3,                                    Address offset: 0x5C */
  RTC->BKP4R;    /*!< RTC backup register 4,                                    Address offset: 0x60 */
  RTC->BKP5R;    /*!< RTC backup register 5,                                    Address offset: 0x64 */
  RTC->BKP6R;    /*!< RTC backup register 6,                                    Address offset: 0x68 */
  RTC->BKP7R;    /*!< RTC backup register 7,                                    Address offset: 0x6C */
  RTC->BKP8R;    /*!< RTC backup register 8,                                    Address offset: 0x70 */
  RTC->BKP9R;    /*!< RTC backup register 9,                                    Address offset: 0x74 */
  RTC->BKP10R;   /*!< RTC backup register 10,                                   Address offset: 0x78 */
  RTC->BKP11R;   /*!< RTC backup register 11,                                   Address offset: 0x7C */
  RTC->BKP12R;   /*!< RTC backup register 12,                                   Address offset: 0x80 */
  RTC->BKP13R;   /*!< RTC backup register 13,                                   Address offset: 0x84 */
  RTC->BKP14R;   /*!< RTC backup register 14,                                   Address offset: 0x88 */
  RTC->BKP15R;   /*!< RTC backup register 15,                                   Address offset: 0x8C */
  RTC->BKP16R;   /*!< RTC backup register 16,                                   Address offset: 0x90 */
  RTC->BKP17R;   /*!< RTC backup register 17,                                   Address offset: 0x94 */
  RTC->BKP18R;   /*!< RTC backup register 18,                                   Address offset: 0x98 */
  RTC->BKP19R;   /*!< RTC backup register 19,                                   Address offset: 0x9C */
  // RTC_TypeDef;

  /**
   * @brief Serial Audio Interface
   */

  SAI1->GCR; /*!< SAI global configuration register,        Address offset: 0x00 */
  // SAI_TypeDef;

  SAI1_Block_A->CR1;   /*!< SAI block x configuration register 1,     Address offset: 0x04 */
  SAI1_Block_A->CR2;   /*!< SAI block x configuration register 2,     Address offset: 0x08 */
  SAI1_Block_A->FRCR;  /*!< SAI block x frame configuration register, Address offset: 0x0C */
  SAI1_Block_A->SLOTR; /*!< SAI block x slot register,                Address offset: 0x10 */
  SAI1_Block_A->IMR;   /*!< SAI block x interrupt mask register,      Address offset: 0x14 */
  SAI1_Block_A->SR;    /*!< SAI block x status register,              Address offset: 0x18 */
  SAI1_Block_A->CLRFR; /*!< SAI block x clear flag register,          Address offset: 0x1C */
  SAI1_Block_A->DR;    /*!< SAI block x data register,                Address offset: 0x20 */
  // SAI_Block_TypeDef;

  SAI1_Block_B->CR1;   /*!< SAI block x configuration register 1,     Address offset: 0x04 */
  SAI1_Block_B->CR2;   /*!< SAI block x configuration register 2,     Address offset: 0x08 */
  SAI1_Block_B->FRCR;  /*!< SAI block x frame configuration register, Address offset: 0x0C */
  SAI1_Block_B->SLOTR; /*!< SAI block x slot register,                Address offset: 0x10 */
  SAI1_Block_B->IMR;   /*!< SAI block x interrupt mask register,      Address offset: 0x14 */
  SAI1_Block_B->SR;    /*!< SAI block x status register,              Address offset: 0x18 */
  SAI1_Block_B->CLRFR; /*!< SAI block x clear flag register,          Address offset: 0x1C */
  SAI1_Block_B->DR;    /*!< SAI block x data register,                Address offset: 0x20 */
  // SAI_Block_TypeDef;

  /**
   * @brief SD host Interface
   */

  SDIO->POWER;   /*!< SDIO power control register,    Address offset: 0x00 */
  SDIO->CLKCR;   /*!< SDI clock control register,     Address offset: 0x04 */
  SDIO->ARG;     /*!< SDIO argument register,         Address offset: 0x08 */
  SDIO->CMD;     /*!< SDIO command register,          Address offset: 0x0C */
  SDIO->RESPCMD; /*!< SDIO command response register, Address offset: 0x10 */
  SDIO->RESP1;   /*!< SDIO response 1 register,       Address offset: 0x14 */
  SDIO->RESP2;   /*!< SDIO response 2 register,       Address offset: 0x18 */
  SDIO->RESP3;   /*!< SDIO response 3 register,       Address offset: 0x1C */
  SDIO->RESP4;   /*!< SDIO response 4 register,       Address offset: 0x20 */
  SDIO->DTIMER;  /*!< SDIO data timer register,       Address offset: 0x24 */
  SDIO->DLEN;    /*!< SDIO data length register,      Address offset: 0x28 */
  SDIO->DCTRL;   /*!< SDIO data control register,     Address offset: 0x2C */
  SDIO->DCOUNT;  /*!< SDIO data counter register,     Address offset: 0x30 */
  SDIO->STA;     /*!< SDIO status register,           Address offset: 0x34 */
  SDIO->ICR;     /*!< SDIO interrupt clear register,  Address offset: 0x38 */
  SDIO->MASK;    /*!< SDIO mask register,             Address offset: 0x3C */
  SDIO->FIFOCNT; /*!< SDIO FIFO counter register,     Address offset: 0x48 */
  SDIO->FIFO;    /*!< SDIO data FIFO register,        Address offset: 0x80 */
  // SDIO_TypeDef;

  /**
   * @brief Serial Peripheral Interface
   */

  SPI1->CR1;     /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
  SPI1->CR2;     /*!< SPI control register 2,                             Address offset: 0x04 */
  SPI1->SR;      /*!< SPI status register,                                Address offset: 0x08 */
  SPI1->DR;      /*!< SPI data register,                                  Address offset: 0x0C */
  SPI1->CRCPR;   /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
  SPI1->RXCRCR;  /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
  SPI1->TXCRCR;  /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
  SPI1->I2SCFGR; /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
  SPI1->I2SPR;   /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
  // SPI_TypeDef;

  SPI2->CR1;     /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
  SPI2->CR2;     /*!< SPI control register 2,                             Address offset: 0x04 */
  SPI2->SR;      /*!< SPI status register,                                Address offset: 0x08 */
  SPI2->DR;      /*!< SPI data register,                                  Address offset: 0x0C */
  SPI2->CRCPR;   /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
  SPI2->RXCRCR;  /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
  SPI2->TXCRCR;  /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
  SPI2->I2SCFGR; /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
  SPI2->I2SPR;   /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
  // SPI_TypeDef;

  SPI3->CR1;     /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
  SPI3->CR2;     /*!< SPI control register 2,                             Address offset: 0x04 */
  SPI3->SR;      /*!< SPI status register,                                Address offset: 0x08 */
  SPI3->DR;      /*!< SPI data register,                                  Address offset: 0x0C */
  SPI3->CRCPR;   /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
  SPI3->RXCRCR;  /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
  SPI3->TXCRCR;  /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
  SPI3->I2SCFGR; /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
  SPI3->I2SPR;   /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
  // SPI_TypeDef;

  SPI4->CR1;     /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
  SPI4->CR2;     /*!< SPI control register 2,                             Address offset: 0x04 */
  SPI4->SR;      /*!< SPI status register,                                Address offset: 0x08 */
  SPI4->DR;      /*!< SPI data register,                                  Address offset: 0x0C */
  SPI4->CRCPR;   /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
  SPI4->RXCRCR;  /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
  SPI4->TXCRCR;  /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
  SPI4->I2SCFGR; /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
  SPI4->I2SPR;   /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
  // SPI_TypeDef;

  SPI5->CR1;     /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
  SPI5->CR2;     /*!< SPI control register 2,                             Address offset: 0x04 */
  SPI5->SR;      /*!< SPI status register,                                Address offset: 0x08 */
  SPI5->DR;      /*!< SPI data register,                                  Address offset: 0x0C */
  SPI5->CRCPR;   /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
  SPI5->RXCRCR;  /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
  SPI5->TXCRCR;  /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
  SPI5->I2SCFGR; /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
  SPI5->I2SPR;   /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
  // SPI_TypeDef;

  I2S2ext->CR1;     /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
  I2S2ext->CR2;     /*!< SPI control register 2,                             Address offset: 0x04 */
  I2S2ext->SR;      /*!< SPI status register,                                Address offset: 0x08 */
  I2S2ext->DR;      /*!< SPI data register,                                  Address offset: 0x0C */
  I2S2ext->CRCPR;   /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
  I2S2ext->RXCRCR;  /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
  I2S2ext->TXCRCR;  /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
  I2S2ext->I2SCFGR; /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
  I2S2ext->I2SPR;   /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
  // SPI_TypeDef;

  I2S3ext->CR1;     /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
  I2S3ext->CR2;     /*!< SPI control register 2,                             Address offset: 0x04 */
  I2S3ext->SR;      /*!< SPI status register,                                Address offset: 0x08 */
  I2S3ext->DR;      /*!< SPI data register,                                  Address offset: 0x0C */
  I2S3ext->CRCPR;   /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
  I2S3ext->RXCRCR;  /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
  I2S3ext->TXCRCR;  /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
  I2S3ext->I2SCFGR; /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
  I2S3ext->I2SPR;   /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
  // SPI_TypeDef;

  /**
   * @brief QUAD Serial Peripheral Interface
   */

  QUADSPI->CR;    /*!< QUADSPI Control register,                           Address offset: 0x00 */
  QUADSPI->DCR;   /*!< QUADSPI Device Configuration register,              Address offset: 0x04 */
  QUADSPI->SR;    /*!< QUADSPI Status register,                            Address offset: 0x08 */
  QUADSPI->FCR;   /*!< QUADSPI Flag Clear register,                        Address offset: 0x0C */
  QUADSPI->DLR;   /*!< QUADSPI Data Length register,                       Address offset: 0x10 */
  QUADSPI->CCR;   /*!< QUADSPI Communication Configuration register,       Address offset: 0x14 */
  QUADSPI->AR;    /*!< QUADSPI Address register,                           Address offset: 0x18 */
  QUADSPI->ABR;   /*!< QUADSPI Alternate Bytes register,                   Address offset: 0x1C */
  QUADSPI->DR;    /*!< QUADSPI Data register,                              Address offset: 0x20 */
  QUADSPI->PSMKR; /*!< QUADSPI Polling Status Mask register,               Address offset: 0x24 */
  QUADSPI->PSMAR; /*!< QUADSPI Polling Status Match register,              Address offset: 0x28 */
  QUADSPI->PIR;   /*!< QUADSPI Polling Interval register,                  Address offset: 0x2C */
  QUADSPI->LPTR;  /*!< QUADSPI Low Power Timeout register,                 Address offset: 0x30 */
  // QUADSPI_TypeDef;

  /**
   * @brief TIM
   */

  TIM1->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM1->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM1->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM1->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM1->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM1->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM1->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM1->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM1->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM1->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM1->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM1->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM1->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM1->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM1->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM1->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM1->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM1->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM1->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM1->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM1->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM2->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM2->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM2->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM2->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM2->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM2->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM2->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM2->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM2->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM2->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM2->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM2->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM2->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM2->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM2->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM2->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM2->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM2->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM2->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM2->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM2->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM3->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM3->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM3->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM3->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM3->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM3->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM3->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM3->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM3->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM3->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM3->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM3->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM3->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM3->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM3->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM3->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM3->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM3->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM3->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM3->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM3->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM4->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM4->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM4->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM4->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM4->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM4->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM4->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM4->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM4->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM4->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM4->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM4->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM4->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM4->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM4->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM4->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM4->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM4->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM4->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM4->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM4->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM5->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM5->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM5->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM5->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM5->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM5->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM5->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM5->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM5->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM5->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM5->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM5->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM5->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM5->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM5->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM5->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM5->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM5->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM5->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM5->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM5->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM6->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM6->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM6->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM6->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM6->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM6->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM6->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM6->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM6->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM6->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM6->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM6->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM6->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM6->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM6->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM6->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM6->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM6->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM6->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM6->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM6->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM7->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM7->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM7->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM7->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM7->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM7->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM7->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM7->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM7->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM7->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM7->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM7->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM7->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM7->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM7->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM7->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM7->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM7->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM7->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM7->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM7->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM8->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM8->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM8->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM8->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM8->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM8->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM8->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM8->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM8->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM8->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM8->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM8->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM8->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM8->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM8->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM8->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM8->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM8->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM8->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM8->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM8->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM9->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM9->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM9->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM9->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM9->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM9->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM9->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM9->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM9->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM9->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM9->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM9->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM9->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM9->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM9->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM9->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM9->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM9->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM9->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM9->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM9->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM10->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM10->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM10->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM10->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM10->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM10->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM10->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM10->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM10->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM10->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM10->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM10->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM10->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM10->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM10->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM10->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM10->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM10->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM10->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM10->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM10->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM11->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM11->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM11->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM11->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM11->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM11->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM11->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM11->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM11->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM11->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM11->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM11->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM11->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM11->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM11->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM11->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM11->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM11->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM11->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM11->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM11->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM12->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM12->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM12->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM12->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM12->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM12->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM12->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM12->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM12->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM12->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM12->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM12->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM12->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM12->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM12->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM12->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM12->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM12->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM12->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM12->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM12->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM13->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM13->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM13->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM13->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM13->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM13->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM13->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM13->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM13->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM13->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM13->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM13->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM13->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM13->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM13->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM13->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM13->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM13->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM13->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM13->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM13->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  TIM14->CR1;   /*!< TIM control register 1,              Address offset: 0x00 */
  TIM14->CR2;   /*!< TIM control register 2,              Address offset: 0x04 */
  TIM14->SMCR;  /*!< TIM slave mode control register,     Address offset: 0x08 */
  TIM14->DIER;  /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  TIM14->SR;    /*!< TIM status register,                 Address offset: 0x10 */
  TIM14->EGR;   /*!< TIM event generation register,       Address offset: 0x14 */
  TIM14->CCMR1; /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  TIM14->CCMR2; /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  TIM14->CCER;  /*!< TIM capture/compare enable register, Address offset: 0x20 */
  TIM14->CNT;   /*!< TIM counter register,                Address offset: 0x24 */
  TIM14->PSC;   /*!< TIM prescaler,                       Address offset: 0x28 */
  TIM14->ARR;   /*!< TIM auto-reload register,            Address offset: 0x2C */
  TIM14->RCR;   /*!< TIM repetition counter register,     Address offset: 0x30 */
  TIM14->CCR1;  /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  TIM14->CCR2;  /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  TIM14->CCR3;  /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  TIM14->CCR4;  /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  TIM14->BDTR;  /*!< TIM break and dead-time register,    Address offset: 0x44 */
  TIM14->DCR;   /*!< TIM DMA control register,            Address offset: 0x48 */
  TIM14->DMAR;  /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  TIM14->OR;    /*!< TIM option register,                 Address offset: 0x50 */
  // TIM_TypeDef;

  /**
   * @brief Universal Synchronous Asynchronous Receiver Transmitter
   */

  USART2->SR;   /*!< USART Status register,                   Address offset: 0x00 */
  USART2->DR;   /*!< USART Data register,                     Address offset: 0x04 */
  USART2->BRR;  /*!< USART Baud rate register,                Address offset: 0x08 */
  USART2->CR1;  /*!< USART Control register 1,                Address offset: 0x0C */
  USART2->CR2;  /*!< USART Control register 2,                Address offset: 0x10 */
  USART2->CR3;  /*!< USART Control register 3,                Address offset: 0x14 */
  USART2->GTPR; /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  // USART_TypeDef;

  USART3->SR;   /*!< USART Status register,                   Address offset: 0x00 */
  USART3->DR;   /*!< USART Data register,                     Address offset: 0x04 */
  USART3->BRR;  /*!< USART Baud rate register,                Address offset: 0x08 */
  USART3->CR1;  /*!< USART Control register 1,                Address offset: 0x0C */
  USART3->CR2;  /*!< USART Control register 2,                Address offset: 0x10 */
  USART3->CR3;  /*!< USART Control register 3,                Address offset: 0x14 */
  USART3->GTPR; /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  // USART_TypeDef;

  UART4->SR;   /*!< USART Status register,                   Address offset: 0x00 */
  UART4->DR;   /*!< USART Data register,                     Address offset: 0x04 */
  UART4->BRR;  /*!< USART Baud rate register,                Address offset: 0x08 */
  UART4->CR1;  /*!< USART Control register 1,                Address offset: 0x0C */
  UART4->CR2;  /*!< USART Control register 2,                Address offset: 0x10 */
  UART4->CR3;  /*!< USART Control register 3,                Address offset: 0x14 */
  UART4->GTPR; /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  // USART_TypeDef;

  UART5->SR;   /*!< USART Status register,                   Address offset: 0x00 */
  UART5->DR;   /*!< USART Data register,                     Address offset: 0x04 */
  UART5->BRR;  /*!< USART Baud rate register,                Address offset: 0x08 */
  UART5->CR1;  /*!< USART Control register 1,                Address offset: 0x0C */
  UART5->CR2;  /*!< USART Control register 2,                Address offset: 0x10 */
  UART5->CR3;  /*!< USART Control register 3,                Address offset: 0x14 */
  UART5->GTPR; /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  // USART_TypeDef;

  UART7->SR;   /*!< USART Status register,                   Address offset: 0x00 */
  UART7->DR;   /*!< USART Data register,                     Address offset: 0x04 */
  UART7->BRR;  /*!< USART Baud rate register,                Address offset: 0x08 */
  UART7->CR1;  /*!< USART Control register 1,                Address offset: 0x0C */
  UART7->CR2;  /*!< USART Control register 2,                Address offset: 0x10 */
  UART7->CR3;  /*!< USART Control register 3,                Address offset: 0x14 */
  UART7->GTPR; /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  // USART_TypeDef;

  UART8->SR;   /*!< USART Status register,                   Address offset: 0x00 */
  UART8->DR;   /*!< USART Data register,                     Address offset: 0x04 */
  UART8->BRR;  /*!< USART Baud rate register,                Address offset: 0x08 */
  UART8->CR1;  /*!< USART Control register 1,                Address offset: 0x0C */
  UART8->CR2;  /*!< USART Control register 2,                Address offset: 0x10 */
  UART8->CR3;  /*!< USART Control register 3,                Address offset: 0x14 */
  UART8->GTPR; /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  // USART_TypeDef;

  USART1->SR;   /*!< USART Status register,                   Address offset: 0x00 */
  USART1->DR;   /*!< USART Data register,                     Address offset: 0x04 */
  USART1->BRR;  /*!< USART Baud rate register,                Address offset: 0x08 */
  USART1->CR1;  /*!< USART Control register 1,                Address offset: 0x0C */
  USART1->CR2;  /*!< USART Control register 2,                Address offset: 0x10 */
  USART1->CR3;  /*!< USART Control register 3,                Address offset: 0x14 */
  USART1->GTPR; /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  // USART_TypeDef;

  USART6->SR;   /*!< USART Status register,                   Address offset: 0x00 */
  USART6->DR;   /*!< USART Data register,                     Address offset: 0x04 */
  USART6->BRR;  /*!< USART Baud rate register,                Address offset: 0x08 */
  USART6->CR1;  /*!< USART Control register 1,                Address offset: 0x0C */
  USART6->CR2;  /*!< USART Control register 2,                Address offset: 0x10 */
  USART6->CR3;  /*!< USART Control register 3,                Address offset: 0x14 */
  USART6->GTPR; /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  // USART_TypeDef;

  UART9->SR;   /*!< USART Status register,                   Address offset: 0x00 */
  UART9->DR;   /*!< USART Data register,                     Address offset: 0x04 */
  UART9->BRR;  /*!< USART Baud rate register,                Address offset: 0x08 */
  UART9->CR1;  /*!< USART Control register 1,                Address offset: 0x0C */
  UART9->CR2;  /*!< USART Control register 2,                Address offset: 0x10 */
  UART9->CR3;  /*!< USART Control register 3,                Address offset: 0x14 */
  UART9->GTPR; /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  // USART_TypeDef;

  UART10->SR;   /*!< USART Status register,                   Address offset: 0x00 */
  UART10->DR;   /*!< USART Data register,                     Address offset: 0x04 */
  UART10->BRR;  /*!< USART Baud rate register,                Address offset: 0x08 */
  UART10->CR1;  /*!< USART Control register 1,                Address offset: 0x0C */
  UART10->CR2;  /*!< USART Control register 2,                Address offset: 0x10 */
  UART10->CR3;  /*!< USART Control register 3,                Address offset: 0x14 */
  UART10->GTPR; /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  // USART_TypeDef;
  /**
   * @brief Window WATCHDOG
   */

  WWDG->CR;  /*!< WWDG Control register,       Address offset: 0x00 */
  WWDG->CFR; /*!< WWDG Configuration register, Address offset: 0x04 */
  WWDG->SR;  /*!< WWDG Status register,        Address offset: 0x08 */
  // WWDG_TypeDef;

  /**
   * @brief RNG
   */

  RNG->CR; /*!< RNG control register, Address offset: 0x00 */
  RNG->SR; /*!< RNG status register,  Address offset: 0x04 */
  RNG->DR; /*!< RNG data register,    Address offset: 0x08 */
  // RNG_TypeDef;

  /**
   * @brief USB_OTG_Core_Registers
   */

  USB_OTG_FS->GOTGCTL;            /*!< USB_OTG Control and Status Register          Address offset: 0x00  */
  USB_OTG_FS->GOTGINT;            /*!< USB_OTG Interrupt Register                   Address offset: 0x04  */
  USB_OTG_FS->GAHBCFG;            /*!< Core AHB Configuration Register              Address offset: 0x08  */
  USB_OTG_FS->GUSBCFG;            /*!< Core USB Configuration Register              Address offset: 0x0C  */
  USB_OTG_FS->GRSTCTL;            /*!< Core Reset Register                          Address offset: 0x10  */
  USB_OTG_FS->GINTSTS;            /*!< Core Interrupt Register                      Address offset: 0x14  */
  USB_OTG_FS->GINTMSK;            /*!< Core Interrupt Mask Register                 Address offset: 0x18  */
  USB_OTG_FS->GRXSTSR;            /*!< Receive Sts Q Read Register                  Address offset: 0x1C  */
  USB_OTG_FS->GRXSTSP;            /*!< Receive Sts Q Read & POP Register            Address offset: 0x20  */
  USB_OTG_FS->GRXFSIZ;            /*!< Receive FIFO Size Register                   Address offset: 0x24  */
  USB_OTG_FS->DIEPTXF0_HNPTXFSIZ; /*!< EP0 / Non Periodic Tx FIFO Size Register     Address offset: 0x28  */
  USB_OTG_FS->HNPTXSTS;           /*!< Non Periodic Tx FIFO/Queue Sts reg           Address offset: 0x2C  */
  USB_OTG_FS->GCCFG;              /*!< General Purpose IO Register                  Address offset: 0x38  */
  USB_OTG_FS->CID;                /*!< User ID Register                             Address offset: 0x3C  */
  USB_OTG_FS->GHWCFG3;            /*!< User HW config3                              Address offset: 0x4C  */
  USB_OTG_FS->GLPMCFG;            /*!< LPM Register                                 Address offset: 0x54  */
  USB_OTG_FS->GDFIFOCFG;          /*!< DFIFO Software Config Register               Address offset: 0x5C  */
  USB_OTG_FS->HPTXFSIZ;           /*!< Host Periodic Tx FIFO Size Reg               Address offset: 0x100 */
  USB_OTG_FS->DIEPTXF[0];         /*!< dev Periodic Transmit FIFO                   Address offset: 0x104 */
  USB_OTG_FS->DIEPTXF[1];         /*!< dev Periodic Transmit FIFO                   Address offset: 0x108 */
  USB_OTG_FS->DIEPTXF[2];         /*!< dev Periodic Transmit FIFO                   Address offset: 0x10C */
  USB_OTG_FS->DIEPTXF[3];         /*!< dev Periodic Transmit FIFO                   Address offset: 0x110 */
  USB_OTG_FS->DIEPTXF[4];         /*!< dev Periodic Transmit FIFO                   Address offset: 0x114 */
  USB_OTG_FS->DIEPTXF[5];         /*!< dev Periodic Transmit FIFO                   Address offset: 0x118 */
  USB_OTG_FS->DIEPTXF[6];         /*!< dev Periodic Transmit FIFO                   Address offset: 0x11C */
  USB_OTG_FS->DIEPTXF[7];         /*!< dev Periodic Transmit FIFO                   Address offset: 0x120 */
  USB_OTG_FS->DIEPTXF[8];         /*!< dev Periodic Transmit FIFO                   Address offset: 0x124 */
  USB_OTG_FS->DIEPTXF[9];         /*!< dev Periodic Transmit FIFO                   Address offset: 0x128 */
  USB_OTG_FS->DIEPTXF[10];        /*!< dev Periodic Transmit FIFO                   Address offset: 0x12C */
  USB_OTG_FS->DIEPTXF[11];        /*!< dev Periodic Transmit FIFO                   Address offset: 0x130 */
  USB_OTG_FS->DIEPTXF[12];        /*!< dev Periodic Transmit FIFO                   Address offset: 0x134 */
  USB_OTG_FS->DIEPTXF[13];        /*!< dev Periodic Transmit FIFO                   Address offset: 0x138 */
  USB_OTG_FS->DIEPTXF[14];        /*!< dev Periodic Transmit FIFO                   Address offset: 0x13C */
                                  // USB_OTG_GlobalTypeDef;

  /**
   * @brief LPTIMER
   */

  LPTIM1->ISR;  /*!< LPTIM Interrupt and Status register,                Address offset: 0x00 */
  LPTIM1->ICR;  /*!< LPTIM Interrupt Clear register,                     Address offset: 0x04 */
  LPTIM1->IER;  /*!< LPTIM Interrupt Enable register,                    Address offset: 0x08 */
  LPTIM1->CFGR; /*!< LPTIM Configuration register,                       Address offset: 0x0C */
  LPTIM1->CR;   /*!< LPTIM Control register,                             Address offset: 0x10 */
  LPTIM1->CMP;  /*!< LPTIM Compare register,                             Address offset: 0x14 */
  LPTIM1->ARR;  /*!< LPTIM Autoreload register,                          Address offset: 0x18 */
  LPTIM1->CNT;  /*!< LPTIM Counter register,                             Address offset: 0x1C */
  LPTIM1->OR;   /*!< LPTIM Option register,                              Address offset: 0x20 */
  // LPTIM_TypeDef;
}