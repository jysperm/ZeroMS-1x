#!/bin/bash

qmake ZeroMS.pro
make
cd UnitTest/
qmake ZeroMSUnitTest.pro
make

cd ../Doc
qdoc ZeroMS.qdocconf
