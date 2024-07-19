#include "diskio.h"

void INIT_SPI(void);
// Port Controls (Platform dependent)
#define SELECT()    PORTB &= ~(1<<SD_CS)        // MMC CS = L
#define    DESELECT()    PORTB |=  (1<<SD_CS)        // MMC CS = H
#define    MMC_SEL        !(PORTB & (1<<SD_CS))    // MMC CS status (true:selected)
#ifdef _MEGA64_INCLUDED_
#define SD_DI   2
#define SD_DO   3
#define SD_CLK  1
#define SD_CS   0
#else
#define SD_DI   3
#define SD_DO   4
#define SD_CLK  5
#define SD_CS   2
#endif