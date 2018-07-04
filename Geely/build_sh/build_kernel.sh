 #!/bin/sh
export WORKSPACE=/home/wxle/workspace
export MYPOC=$PWD
export FASTBOOT=${WORKSPACE}/fastboot
export MYKERNEL=${MYPOC}/kernel
export RELEASE_BIN=${MYPOC}/kernel/arch/arm/boot/
export PATH=$PATH:${MYPOC}/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/ 
export CROSS_COMPILE=${MYPOC}/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
export ARCH=arm 

cd ${MYKERNEL}

make mrproper 

make dra72x_geelycv_base_defconfig 
make uImage LOADADDR=0x80008000 
make modules
make dtbs 

sudo cp  ${RELEASE_BIN}/zImage                                 ${FASTBOOT}/
sudo cp  ${RELEASE_BIN}/uImage                                ${FASTBOOT}/
sudo cp  ${RELEASE_BIN}/dts/dra7-geely-board.dtb  ${FASTBOOT}/
