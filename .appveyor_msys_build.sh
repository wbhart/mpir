export PATH=/c/msys64/mingw$ABI/bin:$PATH
cd /c/projects/mpir
./configure ABI=$ABI
make
if [ "$ABI" = "32" ]
then
    rm yasm/Makefile
fi
make check
