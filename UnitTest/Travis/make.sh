#!/bin/bash

/opt/qt5/bin/qmake ZeroMS.pro
make
cd UnitTest/
/opt/qt5/bin/qmake ZeroMSUnitTest.pro
make

cd ../Doc
export QT_INSTALL_DOCS=/usr/share/qt5/doc
/opt/qt5/bin/qdoc ZeroMS.qdocconf
