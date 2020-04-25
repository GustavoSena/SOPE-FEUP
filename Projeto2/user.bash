#!/bin/bash
echo "CHANGE FLAGS IN BASH"
cd src/User
echo "----------------------------"
# $? = 0 se compilou bem
# $? = 2 otherwise
make 
if [ $? -eq 0 ] ; then
  ./Un $@
  make clean
else
  echo "COMPILATION ERROR";
fi
