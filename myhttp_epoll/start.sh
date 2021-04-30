#!/bin/bash
make clean
make

cd ./root/cgi
make clean
make
cd -

./myhttpd
