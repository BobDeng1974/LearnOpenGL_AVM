#!/bin/sh
export MYDROID=$PWD
cd ${MYDROID}/device/ti/proprietary-open/jacinto6/sgx_src/eurasia_km/eurasiacon/build/linux2/omap_android
export RELEASE_BIN=${MYDROID}/device/ti/proprietary-open/jacinto6/sgx_src/eurasia_km/eurasiacon/binary2_omap_android_release/target/kbuild
export CROSS_COMPILE=${MYDROID}/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
export KERNEL_CROSS_COMPILE=${MYDROID}/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
export KERNELDIR=${MYDROID}/kernel
#export KERNELDIR=/home/jianyi/work/git_work/poc/android/kernel
export ARCH=arm

make TARGET_PRODUCT="jacinto6evm" BUILD=release
sudo cp  ${RELEASE_BIN}/pvrsrvkm.ko ${MYDROID}/pvrsrvkm.ko
