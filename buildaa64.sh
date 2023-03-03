export PATH=${PATH}:$(pwd)/../buildroot-2022.02.10/output/host/bin/
#export

#cd core
#make -f Makefile.aa64 clean
#make -f Makefile.aa64
#cd ..

cd gui/sdl1
make -f Makefile.aa64 clean
make -f Makefile.aa64 -j32
cd ../..
