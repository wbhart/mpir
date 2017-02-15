export PATH=/c/msys64/mingw$ABI/bin:/c/projects/mpir/bin/:$PATH
cd /c/projects/mpir
./autogen.sh
./configure ABI=$ABI $LIB
make
make check
