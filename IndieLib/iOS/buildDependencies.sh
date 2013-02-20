#bin/sh
DEPSDIR="../common/dependencies"

#build dependencies
echo "DEPENDENCIES RELATIVE DIR: $DEPSDIR"

cd $DEPSDIR

#FreeImage
cd ./FreeImage
make clean
make -f Makefile.iphone

#SDL
cd ../SDL-2.0/build-scripts/
MIN_OS_VERSION=5.1 ./iosbuild.sh configure-armv7
MIN_OS_VERSION=5.1 ./iosbuild.sh make-armv7