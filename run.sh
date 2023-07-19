#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

while true
do
   rm -f $1
   $SCRIPT_DIR/jh-limiter $1 $2 $3
   sleep 60
done
