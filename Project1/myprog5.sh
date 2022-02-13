#! /bin/bash

input_number=${1}

declare -a MOD_ARRAY=() # declare the array for the every digit

sum=0

while [ $input_number -gt 0 ]  # in here taking the digits one by one and adding into the array
do
	mod=$((input_number % 10))  
	MOD_ARRAY+=($mod)
	#echo $mod
	input_number=$((input_number / 10))
	
done

len=${#MOD_ARRAY[@]}

for (( index = 0;index<$len;index++ )) ;
do 
	#echo $index
	if [ $((index+1)) == $len ]
	then
		continue
	else
		new_number=$(( ${MOD_ARRAY[index]} * 10 ))  # calculating the all values from the array and adidng sum into it
		new_number2=${MOD_ARRAY[index+1]}   # calculating the all values from the array and adidng sum into it
		real_number=$((new_number + new_number2))  # get the real number
		(( sum += $real_number ))  # adding into the sum
	fi
	
	
	#echo $new_number2
	#new_number=$((${MOD_ARRAY[index]} + ${MOD_ARRAY[index+1]}))
	
	#echo $sum
done

echo $sum  #printing the sum

