#!/bin/bash

qmake ZeroMS.pro
make
cd UnitTest/
qmake ZeroMSUnitTest.pro
make

cd ../Doc
/opt/qt5/bin/qdoc ZeroMS.qdocconf
