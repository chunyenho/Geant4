#!/bin/bash
echo class start
class_start=`sed -n '/^class[ ].*[^;]$/=' $1`
echo $class_start
echo class end
class_end=`sed -n '/^}\;/=' $1`
echo $class_end

#TMP=${1%.*}".cu"
TMP=$1_bk
#echo $TMP


if [[ -z "$class_start" ]];
then
echo "--------------------------$1--------------------------" >> exception_log.txt
echo "$1 doesn't has a class." >> exception_log.txt    
else
sed -re "/^[ \t]*\/\/.*/! { /^[ \t]*(if|else|for|case|while|goto|sizeof|return|\{)./! { /^.* new [A-Za-z0-9_~=!&]+( *)\(.*\).*$/! {$class_start,$class_end s/^.* [A-Za-z0-9_~=!&]+( *)\(.*$/__device__ __host__ &/gm}}}" $1 > $TMP
#sed -i "/^[ \t]*\/\/.*/! { /^[ \t]*(if|else|for|case|while|goto|sizeof|return)./! { /^.* new [A-Za-z0-9_~=!&]+( *)\(.*\).*$/! {$class_start,$class_end s/^.* [A-Za-z0-9_~=!&]+( *)\(.*\).*$/__device__ __host__ &/gm}}}" $1
mv $TMP ${TMP%_*}
fi
#sed -re "/^[ \t]*\/\/.*/! {$class_start,$class_end s/^.* [A-Za-z0-9_~]*\(.*\).*$/__device__ __host__ &/gm}" $1
