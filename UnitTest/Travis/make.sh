#!/bin/bash

qmake ZeroMS.pro
make
cd UnitTest/
qmake ZeroMSUnitTest.pro
make

cd ../Doc
export QT_INSTALL_DOCS=/usr/share/qt4/doc
/opt/qt5/bin/qdoc ZeroMS.qdocconf
