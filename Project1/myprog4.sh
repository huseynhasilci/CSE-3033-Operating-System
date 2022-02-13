#! /bin/bash

mkdir largest   # creating the largest file 
mkdir smallest  # creating the smallest file 
a=( `ls -Srq` )  # listing the all files outputs as array
len=${#a[@]}
declare -a FILE_ARRAY=()  # declare file array to save the the largest and smalles files  

echo $len
for (( index = 0;index<$len;index++ )) ;
do 
	if [[ -d ${a[index]} ]];  # checking the file is directory
	then
		continue
	elif [[ -f ${a[index]} ]];  #  checking the file is file and add into to the file array
	then
		FILE_ARRAY+=(${a[index]})
	else
		echo "Something went wrong"
		exit 1
	fi
done

file_array_length=${#FILE_ARRAY[@]}
 	
`mv ${FILE_ARRAY[0]} smallest`  # moving the smallest array into smallest directory
`mv ${FILE_ARRAY[file_array_length-1]} largest` # moving the largest array into largest directory

