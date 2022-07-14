#!/bin/bash

cd $(pwd)/curl-7.64.1/ 

export PATH=$PATH:/home/qihua/myhome/TC/toolchain/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin


./configure --prefix=$(pwd)/../output --host=arm-linux-gnueabihf --without-ssl --disable-verbose --without-zlib  --disable-threaded-resolver LDFLAGS=-lpthread CFLAGS=-fPIC
make && make install
cd -


