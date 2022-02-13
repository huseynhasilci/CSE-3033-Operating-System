#! /bin/bash

while IFS= read -r line; do   # reading the file line by line until coming to the last line
	#printf "\n"
	for((i=1; i<=line; i++))
	do 
		echo -n "*"  # printing the stars from the readed number
	done
printf "\n"
done < $1  #read the file 

