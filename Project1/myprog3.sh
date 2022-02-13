#! /bin/bash
sed -i "s/$2/$3/g" $1 
count=$(grep -o -i $3 $1 | wc -l)  #counting the changed strings 
echo "All ${count} occurrences of $2 in $1 has changed with $3" #printing the count number 


