export PATH=/c/msys64/mingw$ABI/bin:$PATH
cd /c/projects/mpir
./configure ABI=$ABI
make
make check
