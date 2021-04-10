cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(fuzix C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(PICO_COPY_TO_RAM 1)

# Makefile modified to set up SD_PIO_SPI_SET of GPIO's
# depends on choosing between 'pico' or 'vgaboard'
# see Makefile
add_definitions(-DSD_PIO_SPI_SET=$(SD_PIO_SPI_SET))

pico_sdk_init()
include_directories(
	.
	..
	../dev
	../include
	../cpu-armm0
)



add_executable(fuzix)

pico_generate_pio_header(fuzix ${CMAKE_CURRENT_LIST_DIR}/spi_sdcard.pio)


target_sources(fuzix PRIVATE
	devices.c
	devflash.c
	devsdspi.c
	pio_spi_sdcard.c
	pio_spi_sdcard.h
	devtty.c
	elf.c
	main.c
	misc.c
	rawflash.c
	swapper.c
	tricks.S
	core1.c
	usbdescriptors.c
	../dev/blkdev.c
	../dev/mbr.c
	../dev/devsd_discard.c
	../dev/devsd.c
	../lib/dhara/error.c
	../lib/dhara/journal.c
	../lib/dhara/map.c
	../lib/hexdump.c
	../devio.c
	../devsys.c
	../filesys.c
	../inode.c
	../kdata.c
	../lowlevel-armm0.c
	../malloc.c
	../mm.c
	../process.c
	../start.c
	../swap.c
	../syscall_execelf32.c
	../syscall_fs.c
	../syscall_fs2.c
	../syscall_fs3.c
	../syscall_other.c
	../syscall_proc.c
	../timer.c
	../tty.c
	../usermem.c
	../version.c
)

target_link_libraries(fuzix
	pico_stdlib
	pico_multicore
	hardware_flash
#	hardware_spi
	hardware_uart
#       hardware_timer
        hardware_pio
	tinyusb_device_unmarked
	tinyusb_board
)

pico_set_float_implementation(fuzix none)
pico_set_double_implementation(fuzix none)
pico_add_extra_outputs(fuzix)

