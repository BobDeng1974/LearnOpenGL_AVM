 #!/bin/sh
export WORKSPACE=/home/wxle/workspace
export MYPOC=$PWD
export FASTBOOT=${WORKSPACE}/fastboot
export MYUBOOT=${MYPOC}/u-boot
export RELEASE_BIN=${MYUBOOT}
export CROSS_COMPILE=${MYPOC}/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
export ARCH=arm 

cd ${MYUBOOT}

make distclean

make dra7xx_geely_android_config 

make

sudo cp  ${RELEASE_BIN}/MLO   ${FASTBOOT}/
sudo cp  ${RELEASE_BIN}/u-boot.img  ${FASTBOOT}/
