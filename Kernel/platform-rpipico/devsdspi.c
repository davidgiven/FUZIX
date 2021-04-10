#include <kernel.h>
#include <kdata.h>
#include <printf.h>
#include <timer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <blkdev.h>
#include "dev/devsd.h"
#include "picosdk.h"
#include "globals.h"
#include "config.h"

#include "pio_spi_sdcard.h"

/*
 * REWIRE MAP FOR PICO BREADBOARD to VGABOARD BREADBOARD LAYOUT
 * PPINx = PhysicalPinx
 * UART TX = GREEN, happen to be using GREEN for SCK also
 * WHITE    RXD (U0)PPIN1   ->  (U1)PPIN26
 * GREEN    TXD (U0)PPIN2   ->  (U1)PPIN27
 * RED      3v3
 * YELLOW   MISO    PPIN16  ->  PPIN25
 * BLUE     CS	    PPIN17  ->  PPIN29
 * BLACK    GND
 * GREEN    SCK	    PPIN19  ->  PPIN7
 * ORANGE   MOSI    PPIN20  ->  PPIN24
*/

#if SD_PIO_SPI_SET == 1 /*pico*/
/* Match current hardware spi layout on pico
 * GPIO12   PIN16   MISO
 * GPIO13   PIN17   CS
 * GPIO14   PIN19   SCK
 * GPIO15   PIN20   MOSI
 */
#define PIN_MISO 12
#define PIN_CS   13
#define PIN_SCK  14
#define PIN_MOSI 15
#elif SD_PIO_SPI_SET == 2 /*vgaboard*/
/* With pimoroni version will have to cut tracks GPIO20/GPIO21 for U1
 * GPIO5    PIN7    SCK
 * GPIO18   PIN24   MOSI
 * GPIO19   PIN25   MISO
 * GPIO22   PIN29   CS
 */
#define PIN_MISO 19
#define PIN_CS   22
#define PIN_SCK   5
#define PIN_MOSI 18
#else
#error "Unsupported SD_PIO_SPI_SET take a look at the Makefile"
#endif

    pio_spi_inst_t spi = {
        .pio = pio0,
        .sm = 0,
        .cs_pin = PIN_CS
    };

void sd_rawinit(void)
{
    gpio_init(PIN_CS);
    gpio_put(PIN_CS, 1);
    gpio_set_dir(PIN_CS, GPIO_OUT);

    uint offset = pio_add_program(spi.pio, &spi_cpha0_program);
//    kprintf("Loaded program at %d\n", offset);

    pio_spi_init(spi.pio, spi.sm, offset,
//                 8,       // 8 bits per SPI frame
                 125.0000f,  // 0.25 MHz @ 125 clk_sys
//                 false,   // CPHA = 0
//                 false,   // CPOL = 0
                 PIN_SCK,
                 PIN_MOSI,
                 PIN_MISO
    );
}

void sd_spi_clock(bool go_fast)
{
/*
*  250000Hz = 125.0000f
* 1000000Hz =  31.2500f
* 4000000Hz =   7.8125f
*/
    pio_spi_disable(spi.pio, spi.sm);
    pio_sm_clear_fifos(spi.pio, spi.sm); //not sure if needed

    if (go_fast)
    {
        pio_sm_set_clkdiv(spi.pio, spi.sm, 7.8125f);
    }
    else
    {
        pio_sm_set_clkdiv(spi.pio, spi.sm, 125.0000f);
    }

    pio_spi_enable(spi.pio, spi.sm);

}

void sd_spi_raise_cs(void)
{
    gpio_put(spi.cs_pin, true);
}

void sd_spi_lower_cs(void)
{
    gpio_put(spi.cs_pin, false);
}

void sd_spi_transmit_byte(uint_fast8_t b)
{
    pio_spi_write8_blocking(&spi, (uint8_t*) &b, 1);
}

uint_fast8_t sd_spi_receive_byte(void)
{
    uint8_t b;
    pio_spi_read8_blocking(&spi, (uint8_t*) &b, 1);
    return b;
}

bool sd_spi_receive_sector(void)
{
    pio_spi_read8_blocking(&spi, (uint8_t*) blk_op.addr, 512);
    return 0;
}

bool sd_spi_transmit_sector(void)
{
    pio_spi_write8_blocking(&spi, (uint8_t*) blk_op.addr, 512);
    return 0;
}

/* vim: sw=4 ts=4 et: */

