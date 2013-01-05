#!/bin/bash

cd ../UnitTest/
./ZeroMSUnitTest | tee output
exit `grep '^FAIL!' output | wc -l`
