#! /bin/bash

declare -a FILE_ARRAY=()  # declare a array to save the listed outputs
declare -a FILE_TO_DELETE=() # declare a array to delete the listed outputs
for entry in `ls $1*/`  # listing the files with command line argumets
do
	FILE_ARRAY+=($entry) # adding into the file array
done
len=${#FILE_ARRAY[@]}

for (( index = 0;index<$len;index++ )) ;
do 
	if [[ -d ${FILE_ARRAY[index]} ]];  # checking the file is subdirectory
	then
		continue  # if it is continue
	
	elif [[ "${FILE_ARRAY[index]}" == *.c ]]; # checking the file is endswith .c
	then
		continue  # if it is continue
	elif [[ "${FILE_ARRAY[index]}" == *.h ]]; # checking the file is endswith .h
	then
		continue # if it is continue
	elif [[ "${FILE_ARRAY[index]}" == *.sh ]]; # checking the file is endswith .sh write this to not to delete the sh files 
	then
		continue # if it is continue
	elif [[ "${FILE_ARRAY[index]}" == "Makefile" ]]; # checking the file name is Makefile
	then
		continue # if it is continue
	elif [[ "${FILE_ARRAY[index]}" == "makefile" ]]; # checking the file name is Makefile
	then
		continue # if it is continue
	else
		FILE_TO_DELETE+=(${FILE_ARRAY[index]})  # adding the files into the deletion array
	fi
done

d_len=${#FILE_TO_DELETE[@]}
get_pwd=`pwd`

for (( i = 0; i<$d_len;i++ ));
do
	`rm /$get_pwd/$1/${FILE_TO_DELETE[i]}`   # deleting the files 
done    

