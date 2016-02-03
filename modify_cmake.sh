#!/bin/bash

#Usage : Rename cc to cu in CMakefiles.

grep -nr "\.cc" -c| grep -v rename | grep -v tags | grep -v "\:0" | sed -e "s/:.*$//g" | xargs -n1 sed -i "s/\.cc/\.cu/g"
