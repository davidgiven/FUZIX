#export PICO_SDK_PATH = /home/dg/src/pico/pico-sdk
export PICO_SDK_PATH = /home/pi/pico/pico-sdk
#set PICO_BOARD_KIND to either pico or vgaboard
PICO_BOARD_KIND = vgaboard
#PICO_BOARD_KIND = pico
ifeq ($(PICO_BOARD_KIND), pico)
export SD_PIO_SPI_SET = 1
else
ifeq ($(PICO_BOARD_KIND), vgaboard)
export SD_PIO_SPI_SET = 2
else
$(error board not supported)
endif
endif
build/fuzix.elf: ../version.c build/Makefile
	$(MAKE) -C build

../version.c:
	$(MAKE) -C .. TARGET=rpipico version.c

build/Makefile: CMakeLists.txt $(wildcard ../*.[chS]) $(wildcard ../*/*.[chS])
	mkdir -p build
	(cd build && cmake -D"PICO_BOARD=$(PICO_BOARD_KIND)" .. )
#	(cd build && cmake -D"PICO_BOARD=vgaboard" ..)
#	(cd build && cmake ..)

image:: world uf2conv
	./update-flash.sh
	../../Standalone/mkftl -s 1952 -e 0x1000 -g 10 filesystem.img -o filesystem.ftl
	arm-none-eabi-objcopy -I binary -O elf32-littlearm --change-section-vma .data=0x10018000 filesystem.ftl filesystem.elf
	./uf2conv filesystem.ftl filesystem.uf2 0x10018000

clean:
	rm -rf build
	$(MAKE) -C ../../Library/libs -f Makefile.armm0 clean
	$(MAKE) -C ../../Applications/util -f Makefile.armm0 clean
	$(MAKE) -C ../../Applications/V7/cmd -f Makefile.armm0 clean
	$(MAKE) -C ../../Applications/V7/cmd/sh -f Makefile.armm0 clean
	$(MAKE) -C ../../Applications/games -f Makefile.armm0 clean
	$(MAKE) -C ../../Applications/levee -f Makefile.armm0 clean
	$(MAKE) -C ../../Applications/cave -f Makefile.armm0 clean
	$(MAKE) -C ../../Applications/cursesgames -f Makefile.armm0 clean
	$(MAKE) -C ../../Standalone clean

world: build/fuzix.elf
	$(MAKE) -C ../../Library/libs -f Makefile.armm0
	$(MAKE) -C ../../Applications/util -f Makefile.armm0
	$(MAKE) -C ../../Applications/V7/cmd -f Makefile.armm0
	$(MAKE) -C ../../Applications/V7/cmd/sh -f Makefile.armm0
	$(MAKE) -C ../../Applications/games -f Makefile.armm0
	$(MAKE) -C ../../Applications/levee -f Makefile.armm0
	$(MAKE) -C ../../Applications/cave -f Makefile.armm0
	$(MAKE) -C ../../Applications/cursesgames -f Makefile.armm0
	$(MAKE) -C ../../Standalone

uf2conv: tools/uf2conv.c
	cc -O -g -o $@ $<

