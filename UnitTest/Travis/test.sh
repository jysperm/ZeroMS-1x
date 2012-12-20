#!/bin/bash

cd UnitTest/
./ZeroMSUnitTestTest | tee output
exit `grep '^FAIL!' output | wc -l`