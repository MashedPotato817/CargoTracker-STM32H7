Build started: Project: test1
*** Using Compiler 'V6.21', folder: 'C:\Keil_v5MDK\ARM\ARMCLANG\Bin'
Build target 'test1'
assembling startup_stm32h7a3xxq.s...
compiling gpio.c...
compiling usart.c...
../Core/Src/freertos.c(21): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
10 errors generated.
compiling freertos.c...
compiling stm32h7xx_it.c...
compiling stm32h7xx_hal_msp.c...
compiling stm32h7xx_hal_timebase_tim.c...
../Core/Src/main.c(21): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
10 errors generated.
compiling main.c...
compiling stm32h7xx_hal_cortex.c...
compiling stm32h7xx_hal_gpio.c...
compiling stm32h7xx_hal_hsem.c...
compiling stm32h7xx_hal_flash.c...
compiling stm32h7xx_hal_flash_ex.c...
compiling stm32h7xx_hal_rcc.c...
compiling stm32h7xx_hal_dma_ex.c...
compiling stm32h7xx_hal_rcc_ex.c...
compiling stm32h7xx_hal_tim_ex.c...
compiling stm32h7xx_hal_pwr.c...
compiling stm32h7xx_hal.c...
compiling stm32h7xx_hal_pwr_ex.c...
compiling stm32h7xx_hal_dma.c...
compiling stm32h7xx_hal_mdma.c...
../Middlewares/Third_Party/FreeRTOS/Source/portable/Common/mpu_wrappers_v2.c(40): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
10 errors generated.
compiling mpu_wrappers_v2.c...
../Middlewares/Third_Party/FreeRTOS/Source/croutine.c(29): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
10 errors generated.
compiling croutine.c...
../Middlewares/Third_Party/FreeRTOS/Source/list.c(37): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
10 errors generated.
compiling list.c...
../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c(38): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c(246): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  246 |             portYIELD_WITHIN_API();
      |             ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c(246): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c(398): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  398 |             portYIELD_WITHIN_API();
      |             ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c(398): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c(508): error: call to undeclared function 'ulPortRaiseBASEPRI'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  508 |     uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
      |                              ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c(508): note: did you mean 'vPortRaiseBASEPRI'?
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): note: 'vPortRaiseBASEPRI' declared here
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |                                  ^
15 errors generated.
compiling event_groups.c...
compiling stm32h7xx_hal_i2c_ex.c...
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(37): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(329): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  329 |                         queueYIELD_IF_USING_PREEMPTION();
      |                         ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(92): note: expanded from macro 'queueYIELD_IF_USING_PREEMPTION'
   92 |     #define queueYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
      |                                                 ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(329): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(92): note: expanded from macro 'queueYIELD_IF_USING_PREEMPTION'
   92 |     #define queueYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
      |                                                 ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(991): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  991 |                             queueYIELD_IF_USING_PREEMPTION();
      |                             ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(92): note: expanded from macro 'queueYIELD_IF_USING_PREEMPTION'
   92 |     #define queueYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
      |                                                 ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(991): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(92): note: expanded from macro 'queueYIELD_IF_USING_PREEMPTION'
   92 |     #define queueYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
      |                                                 ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(1004): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1004 |                         queueYIELD_IF_USING_PREEMPTION();
      |                         ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(92): note: expanded from macro 'queueYIELD_IF_USING_PREEMPTION'
   92 |     #define queueYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
      |                                                 ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(1004): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(92): note: expanded from macro 'queueYIELD_IF_USING_PREEMPTION'
   92 |     #define queueYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
      |                                                 ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(1073): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1073 |                     portYIELD_WITHIN_API();
      |                     ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(1073): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(1130): error: call to undeclared function 'ulPortRaiseBASEPRI'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1130 |     uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
      |                              ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
../Middlewares/Third_Party/FreeRTOS/Source/queue.c(1130): note: did you mean 'vPortRaiseBASEPRI'?
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): note: 'vPortRaiseBASEPRI' declared here
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |                                  ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
20 errors generated.
compiling queue.c...
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(39): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(834): error: call to undeclared function 'ulPortRaiseBASEPRI'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  834 |             prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken );
      |             ^
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(208): note: expanded from macro 'prvSEND_COMPLETE_FROM_ISR'
  208 |     sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) )
      |     ^
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(178): note: expanded from macro 'sbSEND_COMPLETE_FROM_ISR'
  178 |         uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();                     \
      |                                  ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(834): note: did you mean 'vPortRaiseBASEPRI'?
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(208): note: expanded from macro 'prvSEND_COMPLETE_FROM_ISR'
  208 |     sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) )
      |     ^
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(178): note: expanded from macro 'sbSEND_COMPLETE_FROM_ISR'
  178 |         uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();                     \
      |                                  ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): note: 'vPortRaiseBASEPRI' declared here
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |                                  ^
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(1085): error: call to undeclared function 'ulPortRaiseBASEPRI'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1085 |             prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken );
      |             ^
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(131): note: expanded from macro 'prvRECEIVE_COMPLETED_FROM_ISR'
  131 |     sbRECEIVE_COMPLETED_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) )
      |     ^
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(101): note: expanded from macro 'sbRECEIVE_COMPLETED_FROM_ISR'
  101 |         uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();                  \
      |                                  ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(1223): error: call to undeclared function 'ulPortRaiseBASEPRI'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1223 |     uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
      |                              ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c(1254): error: call to undeclared function 'ulPortRaiseBASEPRI'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1254 |     uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
      |                              ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
14 errors generated.
compiling stream_buffer.c...
../Middlewares/Third_Party/FreeRTOS/Source/timers.c(37): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
../Middlewares/Third_Party/FreeRTOS/Source/timers.c(672): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  672 |                         portYIELD_WITHIN_API();
      |                         ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/timers.c(672): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
12 errors generated.
compiling timers.c...
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(39): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(1100): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1100 |             taskYIELD_IF_USING_PREEMPTION();
      |             ^
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(67): note: expanded from macro 'taskYIELD_IF_USING_PREEMPTION'
   67 |     #define taskYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
      |                                                ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(1100): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(67): note: expanded from macro 'taskYIELD_IF_USING_PREEMPTION'
   67 |     #define taskYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
      |                                                ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(1204): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1204 |                 portYIELD_WITHIN_API();
      |                 ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(1204): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(1290): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1290 |             portYIELD_WITHIN_API();
      |             ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(1290): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(1337): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1337 |             portYIELD_WITHIN_API();
      |             ^
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(1337): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(861): note: expanded from macro 'portYIELD_WITHIN_API'
  861 |     #define portYIELD_WITHIN_API    portYIELD
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(1504): error: call to undeclared function 'ulPortRaiseBASEPRI'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1504 |         uxSavedInterruptState = portSET_INTERRUPT_MASK_FROM_ISR();
      |                                 ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c(1504): note: did you mean 'vPortRaiseBASEPRI'?
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(114): note: expanded from macro 'portSET_INTERRUPT_MASK_FROM_ISR'
  114 |     #define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
      |                                                       ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): note: 'vPortRaiseBASEPRI' declared here
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |                                  ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
20 errors generated.
compiling tasks.c...
compiling stm32h7xx_hal_exti.c...
compiling system_stm32h7xx.c...
compiling stm32h7xx_hal_uart_ex.c...
compiling stm32h7xx_hal_tim.c...
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c(24): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c(770): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  770 |     taskYIELD();
      |     ^
../Middlewares/Third_Party/FreeRTOS/Source/include\task.h(201): note: expanded from macro 'taskYIELD'
  201 | #define taskYIELD()                        portYIELD()
      |                                            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c(770): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/include\task.h(201): note: expanded from macro 'taskYIELD'
  201 | #define taskYIELD()                        portYIELD()
      |                                            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c(943): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  943 |       portYIELD_FROM_ISR (yield);
      |       ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(103): note: expanded from macro 'portYIELD_FROM_ISR'
  103 |     #define portYIELD_FROM_ISR( x )                     portEND_SWITCHING_ISR( x )
      |                                                         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(102): note: expanded from macro 'portEND_SWITCHING_ISR'
  102 |     #define portEND_SWITCHING_ISR( xSwitchRequired )    do { if( xSwitchRequired != pdFALSE ) portYIELD(); } while( 0 )
      |                                                                                               ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c(943): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(103): note: expanded from macro 'portYIELD_FROM_ISR'
  103 |     #define portYIELD_FROM_ISR( x )                     portEND_SWITCHING_ISR( x )
      |                                                         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(102): note: expanded from macro 'portEND_SWITCHING_ISR'
  102 |     #define portEND_SWITCHING_ISR( xSwitchRequired )    do { if( xSwitchRequired != pdFALSE ) portYIELD(); } while( 0 )
      |                                                                                               ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c(1487): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1487 |       portYIELD_FROM_ISR (yield);
      |       ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(103): note: expanded from macro 'portYIELD_FROM_ISR'
  103 |     #define portYIELD_FROM_ISR( x )                     portEND_SWITCHING_ISR( x )
      |                                                         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(102): note: expanded from macro 'portEND_SWITCHING_ISR'
  102 |     #define portEND_SWITCHING_ISR( xSwitchRequired )    do { if( xSwitchRequired != pdFALSE ) portYIELD(); } while( 0 )
      |                                                                                               ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c(1487): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(103): note: expanded from macro 'portYIELD_FROM_ISR'
  103 |     #define portYIELD_FROM_ISR( x )                     portEND_SWITCHING_ISR( x )
      |                                                         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(102): note: expanded from macro 'portEND_SWITCHING_ISR'
  102 |     #define portEND_SWITCHING_ISR( xSwitchRequired )    do { if( xSwitchRequired != pdFALSE ) portYIELD(); } while( 0 )
      |                                                                                               ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c(1526): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1526 |       portYIELD_FROM_ISR (pdTRUE);
      |       ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(103): note: expanded from macro 'portYIELD_FROM_ISR'
  103 |     #define portYIELD_FROM_ISR( x )                     portEND_SWITCHING_ISR( x )
      |                                                         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(102): note: expanded from macro 'portEND_SWITCHING_ISR'
  102 |     #define portEND_SWITCHING_ISR( xSwitchRequired )    do { if( xSwitchRequired != pdFALSE ) portYIELD(); } while( 0 )
      |                                                                                               ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c(1526): error: call to undeclared function '__isb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(103): note: expanded from macro 'portYIELD_FROM_ISR'
  103 |     #define portYIELD_FROM_ISR( x )                     portEND_SWITCHING_ISR( x )
      |                                                         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(102): note: expanded from macro 'portEND_SWITCHING_ISR'
  102 |     #define portEND_SWITCHING_ISR( xSwitchRequired )    do { if( xSwitchRequired != pdFALSE ) portYIELD(); } while( 0 )
      |                                                                                               ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(96): note: expanded from macro 'portYIELD'
   96 |         __isb( portSY_FULL_READ_WRITE );                           \
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c(1990): error: call to undeclared function '__dsb'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
 1990 |         portYIELD_FROM_ISR (yield);
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(103): note: expanded from macro 'portYIELD_FROM_ISR'
  103 |     #define portYIELD_FROM_ISR( x )                     portEND_SWITCHING_ISR( x )
      |                                                         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(102): note: expanded from macro 'portEND_SWITCHING_ISR'
  102 |     #define portEND_SWITCHING_ISR( xSwitchRequired )    do { if( xSwitchRequired != pdFALSE ) portYIELD(); } while( 0 )
      |                                                                                               ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(95): note: expanded from macro 'portYIELD'
   95 |         __dsb( portSY_FULL_READ_WRITE );                           \
      |         ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
20 errors generated.
compiling cmsis_os2.c...
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c(45): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
10 errors generated.
compiling heap_4.c...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c(34): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include\FreeRTOS.h(98): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/include/portable.h(53): warning: In file included from...
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(172): error: unknown type name '__forceinline'
  172 |     static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(175): error: expected 'volatile', 'inline', 'goto', or '('
  175 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(185): error: unknown type name '__forceinline'
  185 |     static portFORCE_INLINE void vPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(190): error: expected 'volatile', 'inline', 'goto', or '('
  190 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(202): error: unknown type name '__forceinline'
  202 |     static portFORCE_INLINE void vPortClearBASEPRIFromISR( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(205): error: expected 'volatile', 'inline', 'goto', or '('
  205 |         {
      |         ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: unknown type name '__forceinline'
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(216): error: expected ';' after top level declarator
  216 |     static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI( void )
      |                                     ^
      |                                     ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: unknown type name '__forceinline'
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |            ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(167): note: expanded from macro 'portFORCE_INLINE'
  167 |         #define portFORCE_INLINE    __forceinline
      |                                     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F\portmacro.h(236): error: expected ';' after top level declarator
  236 |     static portFORCE_INLINE BaseType_t xPortIsInsideInterrupt( void )
      |                                       ^
      |                                       ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c(38): error: This port can only be used when the project options are configured to enable hardware floating point support.
   38 |     #error This port can only be used when the project options are configured to enable hardware floating point support.
      |      ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c(246): error: expected '(' after 'asm'
  246 | __asm void vPortSVCHandler( void )
      |       ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c(246): error: expected ';' after top-level asm block
  246 | __asm void vPortSVCHandler( void )
      |      ^
      |      ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c(249): error: use of undeclared identifier 'PRESERVE8'
  249 |     PRESERVE8
      |     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c(266): error: expected '(' after 'asm'
  266 | __asm void prvStartFirstTask( void )
      |       ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c(266): error: expected ';' after top-level asm block
  266 | __asm void prvStartFirstTask( void )
      |      ^
      |      ;
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c(269): error: use of undeclared identifier 'PRESERVE8'
  269 |     PRESERVE8
      |     ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c(297): error: expected '(' after 'asm'
  297 | __asm void prvEnableVFP( void )
      |       ^
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c(297): error: expected ';' after top-level asm block
  297 | __asm void prvEnableVFP( void )
      |      ^
      |      ;
fatal error: too many errors emitted, stopping now [-ferror-limit=]
20 errors generated.
compiling port.c...
compiling stm32h7xx_hal_uart.c...
compiling stm32h7xx_hal_i2c.c...
"test1\test1.axf" - 177 Error(s), 0 Warning(s).
Target not created.
Build Time Elapsed:  00:00:02