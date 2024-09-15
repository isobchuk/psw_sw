#include <cstdint>

int main();
extern "C" void Reset_Handler();
[[gnu::weak]] void NMI_Handler();
void HardFault_Handler();
[[gnu::weak]] void MemoryManagement_Handler();
[[gnu::weak]] void BusFault_Handler();
[[gnu::weak]] void UsageFault_Handler();
[[gnu::weak]] void SVC_Handler();
[[gnu::weak]] void DebugMon_Handler();
[[gnu::weak]] void PendSV_Handler();
[[gnu::weak]] void SysTick_Handler();

[[gnu::weak]] void WWDG_IRQHandler();               /* Window WatchDog              */
[[gnu::weak]] void PVD_IRQHandler();                /* PVD through EXTI Line detection */
[[gnu::weak]] void TAMP_STAMP_IRQHandler();         /* Tamper and TimeStamps through the EXTI line */
[[gnu::weak]] void RTC_WKUP_IRQHandler();           /* RTC Wakeup through the EXTI line */
[[gnu::weak]] void FLASH_IRQHandler();              /* FLASH                        */
[[gnu::weak]] void RCC_IRQHandler();                /* RCC                          */
[[gnu::weak]] void EXTI0_IRQHandler();              /* EXTI Line0                   */
[[gnu::weak]] void EXTI1_IRQHandler();              /* EXTI Line1                   */
[[gnu::weak]] void EXTI2_IRQHandler();              /* EXTI Line2                   */
[[gnu::weak]] void EXTI3_IRQHandler();              /* EXTI Line3                   */
[[gnu::weak]] void EXTI4_IRQHandler();              /* EXTI Line4                   */
[[gnu::weak]] void DMA1_Stream0_IRQHandler();       /* DMA1 Stream 0                */
[[gnu::weak]] void DMA1_Stream1_IRQHandler();       /* DMA1 Stream 1                */
[[gnu::weak]] void DMA1_Stream2_IRQHandler();       /* DMA1 Stream 2                */
[[gnu::weak]] void DMA1_Stream3_IRQHandler();       /* DMA1 Stream 3                */
[[gnu::weak]] void DMA1_Stream4_IRQHandler();       /* DMA1 Stream 4                */
[[gnu::weak]] void DMA1_Stream5_IRQHandler();       /* DMA1 Stream 5                */
[[gnu::weak]] void DMA1_Stream6_IRQHandler();       /* DMA1 Stream 6                */
[[gnu::weak]] void ADC_IRQHandler();                /* ADC1, ADC2 and ADC3s         */
[[gnu::weak]] void CAN1_TX_IRQHandler();            /* CAN1 TX                      */
[[gnu::weak]] void CAN1_RX0_IRQHandler();           /* CAN1 RX0                     */
[[gnu::weak]] void CAN1_RX1_IRQHandler();           /* CAN1 RX1                     */
[[gnu::weak]] void CAN1_SCE_IRQHandler();           /* CAN1 SCE                     */
[[gnu::weak]] void EXTI9_5_IRQHandler();            /* External Line[9:5]s          */
[[gnu::weak]] void TIM1_BRK_TIM9_IRQHandler();      /* TIM1 Break and TIM9          */
[[gnu::weak]] void TIM1_UP_TIM10_IRQHandler();      /* TIM1 Update and TIM10        */
[[gnu::weak]] void TIM1_TRG_COM_TIM11_IRQHandler(); /* TIM1 Trigger and Commutation and TIM11 */
[[gnu::weak]] void TIM1_CC_IRQHandler();            /* TIM1 Capture Compare         */
[[gnu::weak]] void TIM2_IRQHandler();               /* TIM2                         */
[[gnu::weak]] void TIM3_IRQHandler();               /* TIM3                         */
[[gnu::weak]] void TIM4_IRQHandler();               /* TIM4                         */
[[gnu::weak]] void I2C1_EV_IRQHandler();            /* I2C1 Event                   */
[[gnu::weak]] void I2C1_ER_IRQHandler();            /* I2C1 Error                   */
[[gnu::weak]] void I2C2_EV_IRQHandler();            /* I2C2 Event                   */
[[gnu::weak]] void I2C2_ER_IRQHandler();            /* I2C2 Error                   */
[[gnu::weak]] void SPI1_IRQHandler();               /* SPI1                         */
[[gnu::weak]] void SPI2_IRQHandler();               /* SPI2                         */
[[gnu::weak]] void USART1_IRQHandler();             /* USART1                       */
[[gnu::weak]] void USART2_IRQHandler();             /* USART2                       */
[[gnu::weak]] void USART3_IRQHandler();             /* USART3                       */
[[gnu::weak]] void EXTI15_10_IRQHandler();          /* External Line[15:10]s        */
[[gnu::weak]] void RTC_Alarm_IRQHandler();          /* RTC Alarm (A and B) through EXTI Line */
[[gnu::weak]] void OTG_FS_WKUP_IRQHandler();        /* USB OTG FS Wakeup through EXTI line */
[[gnu::weak]] void TIM8_BRK_TIM12_IRQHandler();     /* TIM8 Break and TIM12         */
[[gnu::weak]] void TIM8_UP_TIM13_IRQHandler();      /* TIM8 Update and TIM13        */
[[gnu::weak]] void TIM8_TRG_COM_TIM14_IRQHandler(); /* TIM8 Trigger and Commutation and TIM14 */
[[gnu::weak]] void TIM8_CC_IRQHandler();            /* TIM8 Capture Compare         */
[[gnu::weak]] void DMA1_Stream7_IRQHandler();       /* DMA1 Stream7                 */
[[gnu::weak]] void FSMC_IRQHandler();               /* FSMC                         */
[[gnu::weak]] void SDIO_IRQHandler();               /* SDIO                         */
[[gnu::weak]] void TIM5_IRQHandler();               /* TIM5                         */
[[gnu::weak]] void SPI3_IRQHandler();               /* SPI3                         */
[[gnu::weak]] void UART4_IRQHandler();              /* UART4                        */
[[gnu::weak]] void UART5_IRQHandler();              /* UART5                        */
[[gnu::weak]] void TIM6_DAC_IRQHandler();           /* TIM6 and DAC1&2 underrun errors */
[[gnu::weak]] void TIM7_IRQHandler();               /* TIM7                         */
[[gnu::weak]] void DMA2_Stream0_IRQHandler();       /* DMA2 Stream 0                */
[[gnu::weak]] void DMA2_Stream1_IRQHandler();       /* DMA2 Stream 1                */
[[gnu::weak]] void DMA2_Stream2_IRQHandler();       /* DMA2 Stream 2                */
[[gnu::weak]] void DMA2_Stream3_IRQHandler();       /* DMA2 Stream 3                */
[[gnu::weak]] void DMA2_Stream4_IRQHandler();       /* DMA2 Stream 4                */
[[gnu::weak]] void ETH_IRQHandler();                /* Ethernet                     */
[[gnu::weak]] void ETH_WKUP_IRQHandler();           /* Ethernet Wakeup through EXTI line */
[[gnu::weak]] void CAN2_TX_IRQHandler();            /* CAN2 TX                      */
[[gnu::weak]] void CAN2_RX0_IRQHandler();           /* CAN2 RX0                     */
[[gnu::weak]] void CAN2_RX1_IRQHandler();           /* CAN2 RX1                     */
[[gnu::weak]] void CAN2_SCE_IRQHandler();           /* CAN2 SCE                     */
[[gnu::weak]] void OTG_FS_IRQHandler();             /* USB OTG FS                   */
[[gnu::weak]] void DMA2_Stream5_IRQHandler();       /* DMA2 Stream 5                */
[[gnu::weak]] void DMA2_Stream6_IRQHandler();       /* DMA2 Stream 6                */
[[gnu::weak]] void DMA2_Stream7_IRQHandler();       /* DMA2 Stream 7                */
[[gnu::weak]] void USART6_IRQHandler();             /* USART6                       */
[[gnu::weak]] void I2C3_EV_IRQHandler();            /* I2C3 event                   */
[[gnu::weak]] void I2C3_ER_IRQHandler();            /* I2C3 error                   */
[[gnu::weak]] void OTG_HS_EP1_OUT_IRQHandler();     /* USB OTG HS End Point 1 Out   */
[[gnu::weak]] void OTG_HS_EP1_IN_IRQHandler();      /* USB OTG HS End Point 1 In    */
[[gnu::weak]] void OTG_HS_WKUP_IRQHandler();        /* USB OTG HS Wakeup through EXTI */
[[gnu::weak]] void OTG_HS_IRQHandler();             /* USB OTG HS                   */
[[gnu::weak]] void DCMI_IRQHandler();               /* DCMI                         */
[[gnu::weak]] void CRYP_IRQHandler();               /* CRYP crypto                  */
[[gnu::weak]] void HASH_RNG_IRQHandler();           /* Hash and Rng                 */

extern "C" void DefaultHandler();

#pragma weak NMI_Handler = DefaultHandler
#pragma weak MemoryManagement_Handler = DefaultHandler
#pragma weak BusFault_Handler = DefaultHandler
#pragma weak UsageFault_Handler = DefaultHandler
#pragma weak SVC_Handler = DefaultHandler
#pragma weak DebugMon_Handler = DefaultHandler
#pragma weak PendSV_Handler = DefaultHandler
#pragma weak SysTick_Handler = DefaultHandler

#pragma weak WWDG_IRQHandler = DefaultHandler               /* Window WatchDog              */
#pragma weak PVD_IRQHandler = DefaultHandler                /* PVD through EXTI Line detection */
#pragma weak TAMP_STAMP_IRQHandler = DefaultHandler         /* Tamper and TimeStamps through the EXTI line */
#pragma weak RTC_WKUP_IRQHandler = DefaultHandler           /* RTC Wakeup through the EXTI line */
#pragma weak FLASH_IRQHandler = DefaultHandler              /* FLASH                        */
#pragma weak RCC_IRQHandler = DefaultHandler                /* RCC                          */
#pragma weak EXTI0_IRQHandler = DefaultHandler              /* EXTI Line0                   */
#pragma weak EXTI1_IRQHandler = DefaultHandler              /* EXTI Line1                   */
#pragma weak EXTI2_IRQHandler = DefaultHandler              /* EXTI Line2                   */
#pragma weak EXTI3_IRQHandler = DefaultHandler              /* EXTI Line3                   */
#pragma weak EXTI4_IRQHandler = DefaultHandler              /* EXTI Line4                   */
#pragma weak DMA1_Stream0_IRQHandler = DefaultHandler       /* DMA1 Stream 0                */
#pragma weak DMA1_Stream1_IRQHandler = DefaultHandler       /* DMA1 Stream 1                */
#pragma weak DMA1_Stream2_IRQHandler = DefaultHandler       /* DMA1 Stream 2                */
#pragma weak DMA1_Stream3_IRQHandler = DefaultHandler       /* DMA1 Stream 3                */
#pragma weak DMA1_Stream4_IRQHandler = DefaultHandler       /* DMA1 Stream 4                */
#pragma weak DMA1_Stream5_IRQHandler = DefaultHandler       /* DMA1 Stream 5                */
#pragma weak DMA1_Stream6_IRQHandler = DefaultHandler       /* DMA1 Stream 6                */
#pragma weak ADC_IRQHandler = DefaultHandler                /* ADC1, ADC2 and ADC3s         */
#pragma weak CAN1_TX_IRQHandler = DefaultHandler            /* CAN1 TX                      */
#pragma weak CAN1_RX0_IRQHandler = DefaultHandler           /* CAN1 RX0                     */
#pragma weak CAN1_RX1_IRQHandler = DefaultHandler           /* CAN1 RX1                     */
#pragma weak CAN1_SCE_IRQHandler = DefaultHandler           /* CAN1 SCE                     */
#pragma weak EXTI9_5_IRQHandler = DefaultHandler            /* External Line[9:5]s          */
#pragma weak TIM1_BRK_TIM9_IRQHandler = DefaultHandler      /* TIM1 Break and TIM9          */
#pragma weak TIM1_UP_TIM10_IRQHandler = DefaultHandler      /* TIM1 Update and TIM10        */
#pragma weak TIM1_TRG_COM_TIM11_IRQHandler = DefaultHandler /* TIM1 Trigger and Commutation and TIM11 */
#pragma weak TIM1_CC_IRQHandler = DefaultHandler            /* TIM1 Capture Compare         */
#pragma weak TIM2_IRQHandler = DefaultHandler               /* TIM2                         */
#pragma weak TIM3_IRQHandler = DefaultHandler               /* TIM3                         */
#pragma weak TIM4_IRQHandler = DefaultHandler               /* TIM4                         */
#pragma weak I2C1_EV_IRQHandler = DefaultHandler            /* I2C1 Event                   */
#pragma weak I2C1_ER_IRQHandler = DefaultHandler            /* I2C1 Error                   */
#pragma weak I2C2_EV_IRQHandler = DefaultHandler            /* I2C2 Event                   */
#pragma weak I2C2_ER_IRQHandler = DefaultHandler            /* I2C2 Error                   */
#pragma weak SPI1_IRQHandler = DefaultHandler               /* SPI1                         */
#pragma weak SPI2_IRQHandler = DefaultHandler               /* SPI2                         */
#pragma weak USART1_IRQHandler = DefaultHandler             /* USART1                       */
#pragma weak USART2_IRQHandler = DefaultHandler             /* USART2                       */
#pragma weak USART3_IRQHandler = DefaultHandler             /* USART3                       */
#pragma weak EXTI15_10_IRQHandler = DefaultHandler          /* External Line[15:10]s        */
#pragma weak RTC_Alarm_IRQHandler = DefaultHandler          /* RTC Alarm (A and B) through EXTI Line */
#pragma weak OTG_FS_WKUP_IRQHandler = DefaultHandler        /* USB OTG FS Wakeup through EXTI line */
#pragma weak TIM8_BRK_TIM12_IRQHandler = DefaultHandler     /* TIM8 Break and TIM12         */
#pragma weak TIM8_UP_TIM13_IRQHandler = DefaultHandler      /* TIM8 Update and TIM13        */
#pragma weak TIM8_TRG_COM_TIM14_IRQHandler = DefaultHandler /* TIM8 Trigger and Commutation and TIM14 */
#pragma weak TIM8_CC_IRQHandler = DefaultHandler            /* TIM8 Capture Compare         */
#pragma weak DMA1_Stream7_IRQHandler = DefaultHandler       /* DMA1 Stream7                 */
#pragma weak FSMC_IRQHandler = DefaultHandler               /* FSMC                         */
#pragma weak SDIO_IRQHandler = DefaultHandler               /* SDIO                         */
#pragma weak TIM5_IRQHandler = DefaultHandler               /* TIM5                         */
#pragma weak SPI3_IRQHandler = DefaultHandler               /* SPI3                         */
#pragma weak UART4_IRQHandler = DefaultHandler              /* UART4                        */
#pragma weak UART5_IRQHandler = DefaultHandler              /* UART5                        */
#pragma weak TIM6_DAC_IRQHandler = DefaultHandler           /* TIM6 and DAC1&2 underrun errors */
#pragma weak TIM7_IRQHandler = DefaultHandler               /* TIM7                         */
#pragma weak DMA2_Stream0_IRQHandler = DefaultHandler       /* DMA2 Stream 0                */
#pragma weak DMA2_Stream1_IRQHandler = DefaultHandler       /* DMA2 Stream 1                */
#pragma weak DMA2_Stream2_IRQHandler = DefaultHandler       /* DMA2 Stream 2                */
#pragma weak DMA2_Stream3_IRQHandler = DefaultHandler       /* DMA2 Stream 3                */
#pragma weak DMA2_Stream4_IRQHandler = DefaultHandler       /* DMA2 Stream 4                */
#pragma weak ETH_IRQHandler = DefaultHandler                /* Ethernet                     */
#pragma weak ETH_WKUP_IRQHandler = DefaultHandler           /* Ethernet Wakeup through EXTI line */
#pragma weak CAN2_TX_IRQHandler = DefaultHandler            /* CAN2 TX                      */
#pragma weak CAN2_RX0_IRQHandler = DefaultHandler           /* CAN2 RX0                     */
#pragma weak CAN2_RX1_IRQHandler = DefaultHandler           /* CAN2 RX1                     */
#pragma weak CAN2_SCE_IRQHandler = DefaultHandler           /* CAN2 SCE                     */
#pragma weak OTG_FS_IRQHandler = DefaultHandler             /* USB OTG FS                   */
#pragma weak DMA2_Stream5_IRQHandler = DefaultHandler       /* DMA2 Stream 5                */
#pragma weak DMA2_Stream6_IRQHandler = DefaultHandler       /* DMA2 Stream 6                */
#pragma weak DMA2_Stream7_IRQHandler = DefaultHandler       /* DMA2 Stream 7                */
#pragma weak USART6_IRQHandler = DefaultHandler             /* USART6                       */
#pragma weak I2C3_EV_IRQHandler = DefaultHandler            /* I2C3 event                   */
#pragma weak I2C3_ER_IRQHandler = DefaultHandler            /* I2C3 error                   */
#pragma weak OTG_HS_EP1_OUT_IRQHandler = DefaultHandler     /* USB OTG HS End Point 1 Out   */
#pragma weak OTG_HS_EP1_IN_IRQHandler = DefaultHandler      /* USB OTG HS End Point 1 In    */
#pragma weak OTG_HS_WKUP_IRQHandler = DefaultHandler        /* USB OTG HS Wakeup through EXTI */
#pragma weak OTG_HS_IRQHandler = DefaultHandler             /* USB OTG HS                   */
#pragma weak DCMI_IRQHandler = DefaultHandler               /* DCMI                         */
#pragma weak CRYP_IRQHandler = DefaultHandler               /* CRYP crypto                  */
#pragma weak HASH_RNG_IRQHandler = DefaultHandler           /* Hash and Rng                 */

[[gnu::used, gnu::section(".isr_vector")]] static constexpr void (*const p_sc_InterruptVectorTable[])(void) = {
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemoryManagement_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    SVC_Handler,
    DebugMon_Handler,
    nullptr,
    PendSV_Handler,
    SysTick_Handler,
    WWDG_IRQHandler,               /* Window WatchDog              */
    PVD_IRQHandler,                /* PVD through EXTI Line detection */
    TAMP_STAMP_IRQHandler,         /* Tamper and TimeStamps through the EXTI line */
    RTC_WKUP_IRQHandler,           /* RTC Wakeup through the EXTI line */
    FLASH_IRQHandler,              /* FLASH                        */
    RCC_IRQHandler,                /* RCC                          */
    EXTI0_IRQHandler,              /* EXTI Line0                   */
    EXTI1_IRQHandler,              /* EXTI Line1                   */
    EXTI2_IRQHandler,              /* EXTI Line2                   */
    EXTI3_IRQHandler,              /* EXTI Line3                   */
    EXTI4_IRQHandler,              /* EXTI Line4                   */
    DMA1_Stream0_IRQHandler,       /* DMA1 Stream 0                */
    DMA1_Stream1_IRQHandler,       /* DMA1 Stream 1                */
    DMA1_Stream2_IRQHandler,       /* DMA1 Stream 2                */
    DMA1_Stream3_IRQHandler,       /* DMA1 Stream 3                */
    DMA1_Stream4_IRQHandler,       /* DMA1 Stream 4                */
    DMA1_Stream5_IRQHandler,       /* DMA1 Stream 5                */
    DMA1_Stream6_IRQHandler,       /* DMA1 Stream 6                */
    ADC_IRQHandler,                /* ADC1, ADC2 and ADC3s         */
    CAN1_TX_IRQHandler,            /* CAN1 TX                      */
    CAN1_RX0_IRQHandler,           /* CAN1 RX0                     */
    CAN1_RX1_IRQHandler,           /* CAN1 RX1                     */
    CAN1_SCE_IRQHandler,           /* CAN1 SCE                     */
    EXTI9_5_IRQHandler,            /* External Line[9:5]s          */
    TIM1_BRK_TIM9_IRQHandler,      /* TIM1 Break and TIM9          */
    TIM1_UP_TIM10_IRQHandler,      /* TIM1 Update and TIM10        */
    TIM1_TRG_COM_TIM11_IRQHandler, /* TIM1 Trigger and Commutation and TIM11 */
    TIM1_CC_IRQHandler,            /* TIM1 Capture Compare         */
    TIM2_IRQHandler,               /* TIM2                         */
    TIM3_IRQHandler,               /* TIM3                         */
    TIM4_IRQHandler,               /* TIM4                         */
    I2C1_EV_IRQHandler,            /* I2C1 Event                   */
    I2C1_ER_IRQHandler,            /* I2C1 Error                   */
    I2C2_EV_IRQHandler,            /* I2C2 Event                   */
    I2C2_ER_IRQHandler,            /* I2C2 Error                   */
    SPI1_IRQHandler,               /* SPI1                         */
    SPI2_IRQHandler,               /* SPI2                         */
    USART1_IRQHandler,             /* USART1                       */
    USART2_IRQHandler,             /* USART2                       */
    USART3_IRQHandler,             /* USART3                       */
    EXTI15_10_IRQHandler,          /* External Line[15:10]s        */
    RTC_Alarm_IRQHandler,          /* RTC Alarm (A and B) through EXTI Line */
    OTG_FS_WKUP_IRQHandler,        /* USB OTG FS Wakeup through EXTI line */
    TIM8_BRK_TIM12_IRQHandler,     /* TIM8 Break and TIM12         */
    TIM8_UP_TIM13_IRQHandler,      /* TIM8 Update and TIM13        */
    TIM8_TRG_COM_TIM14_IRQHandler, /* TIM8 Trigger and Commutation and TIM14 */
    TIM8_CC_IRQHandler,            /* TIM8 Capture Compare         */
    DMA1_Stream7_IRQHandler,       /* DMA1 Stream7                 */
    FSMC_IRQHandler,               /* FSMC                         */
    SDIO_IRQHandler,               /* SDIO                         */
    TIM5_IRQHandler,               /* TIM5                         */
    SPI3_IRQHandler,               /* SPI3                         */
    UART4_IRQHandler,              /* UART4                        */
    UART5_IRQHandler,              /* UART5                        */
    TIM6_DAC_IRQHandler,           /* TIM6 and DAC1&2 underrun errors */
    TIM7_IRQHandler,               /* TIM7                         */
    DMA2_Stream0_IRQHandler,       /* DMA2 Stream 0                */
    DMA2_Stream1_IRQHandler,       /* DMA2 Stream 1                */
    DMA2_Stream2_IRQHandler,       /* DMA2 Stream 2                */
    DMA2_Stream3_IRQHandler,       /* DMA2 Stream 3                */
    DMA2_Stream4_IRQHandler,       /* DMA2 Stream 4                */
    ETH_IRQHandler,                /* Ethernet                     */
    ETH_WKUP_IRQHandler,           /* Ethernet Wakeup through EXTI line */
    CAN2_TX_IRQHandler,            /* CAN2 TX                      */
    CAN2_RX0_IRQHandler,           /* CAN2 RX0                     */
    CAN2_RX1_IRQHandler,           /* CAN2 RX1                     */
    CAN2_SCE_IRQHandler,           /* CAN2 SCE                     */
    OTG_FS_IRQHandler,             /* USB OTG FS                   */
    DMA2_Stream5_IRQHandler,       /* DMA2 Stream 5                */
    DMA2_Stream6_IRQHandler,       /* DMA2 Stream 6                */
    DMA2_Stream7_IRQHandler,       /* DMA2 Stream 7                */
    USART6_IRQHandler,             /* USART6                       */
    I2C3_EV_IRQHandler,            /* I2C3 event                   */
    I2C3_ER_IRQHandler,            /* I2C3 error                   */
    OTG_HS_EP1_OUT_IRQHandler,     /* USB OTG HS End Point 1 Out   */
    OTG_HS_EP1_IN_IRQHandler,      /* USB OTG HS End Point 1 In    */
    OTG_HS_WKUP_IRQHandler,        /* USB OTG HS Wakeup through EXTI */
    OTG_HS_IRQHandler,             /* USB OTG HS                   */
    DCMI_IRQHandler,               /* DCMI                         */
    CRYP_IRQHandler,               /* CRYP crypto                  */
    HASH_RNG_IRQHandler            /* Hash and Rng                 */
};

void Reset_Handler() {
  extern const uint32_t sdata;
  extern const uint32_t edata;
  extern const uint32_t szero;
  extern const uint32_t ezero;
  extern const uint32_t efixed;
  extern const uint32_t init_array_start;
  extern const uint32_t init_array_end;

  // Initialize data section
  uint32_t *poolValues = const_cast<uint32_t *>(&efixed);
  uint32_t *poolMemory = const_cast<uint32_t *>(&sdata);

  while (poolMemory < &edata) {
    *(poolMemory++) = *(poolValues++);
  }

  // Initialize bss section
  poolMemory = const_cast<uint32_t *>(&szero);
  while (poolMemory < &ezero) {
    *(poolMemory++) = 0UL;
  }

  // Call static constructors
  poolMemory = const_cast<uint32_t *>(&init_array_start);
  while (poolMemory < &init_array_end) {
    reinterpret_cast<void (*)()>((*(poolMemory++)))();
  }

  main();
}

void HardFault_Handler() {
  struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
  } *stack_ptr;

  asm("TST lr, #4 \n"
      "ITE EQ \n"
      "MRSEQ %[ptr], MSP  \n"
      "MRSNE %[ptr], PSP  \n"
      : [ptr] "=r"(stack_ptr));

  while (true) {
    asm("NOP");
  }
}

extern "C" void DefaultHandler() {
  while (true) {
    asm("NOP");
  }
}

extern "C" {
void _close(void) {}
void _lseek(void) {}
void _read(void) {}
void _write(void) {}
}