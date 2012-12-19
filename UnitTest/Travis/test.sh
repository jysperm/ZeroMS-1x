#!/bin/bash

cd UnitTest/
./ZeroMSUnitTestTest | tee output
grep "^FAIL!" output 
exit $?