export PATH=/c/msys64/mingw$ABI/bin:/c/projects/mpir/bin/:$PATH
cd /c/projects/mpir
./configure ABI=$ABI --with-system-yasm
make
make check
