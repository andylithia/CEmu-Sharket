export PATH=${PATH}:$(pwd)/../buildroot-2022.02.10/output/host/bin/
#export

# Building Emulator Core
cd core
make -f Makefile.aa64 clean
make -f Makefile.aa64
cd ..

# Building SDL1.2 Application
cd gui/sdl1
make -f Makefile.aa64 clean
make -f Makefile.aa64 -j32
cd ../..
