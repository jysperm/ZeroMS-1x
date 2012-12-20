#!/bin/bash

qmake ZeroMS.pro
make
cd UnitTest/
qmake ZeroMSUnitTest.pro
make
