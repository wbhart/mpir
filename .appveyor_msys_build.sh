export PATH=/c/msys64/mingw$ABI/bin:/c/projects/mpir/bin/:$PATH
cd /c/projects/mpir
echo && echo build: ./autogen.sh
./autogen.sh
echo && echo build: ./configure ABI=$ABI $LIB
./configure ABI=$ABI $LIB
echo && echo build: make
make
# should work but falsely requires texlive ?!?
#echo && echo build: DISTCHECK_CONFIGURE_FLAGS="ABI=$ABI $LIB" make distcheck
#DISTCHECK_CONFIGURE_FLAGS="ABI=$ABI $LIB" make distcheck
echo && echo build: make check
make check && make -C tests testsuite-all.log
echo && echo build: make dist
make dist
