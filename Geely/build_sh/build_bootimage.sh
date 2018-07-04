 #!/bin/sh
export WORKSPACE=/home/wxle/workspace
export MYPOC=$PWD
export FASTBOOT=${WORKSPACE}/fastboot
export MYKERNEL=${MYPOC}/kernel
export RELEASE_BIN=${MYPOC}/out/target/product/geelycv/
export PATH=$PATH:${MYPOC}/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/ 
export CROSS_COMPILE=${MYPOC}/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
export ARCH=arm 



cd ${MYKERNEL}

make mrproper 

cd ${MYPOC}

sudo rm -rf ${MYPOC}/out/target/product/geelycv/obj/KERNEL_OBJ

make -j2 bootimage

sudo cp  ${RELEASE_BIN}/boot.img   ${FASTBOOT}/
