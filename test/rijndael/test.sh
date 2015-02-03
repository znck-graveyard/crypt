echo ./.__tmp < rijndael_key_expansion_input.txt > .output
./.__tmp < rijndael_key_expansion_input.txt > .output
result=`diff -q .output rijndael_key_expansion_output.txt`
echo $result
if [ -z "$result" ]
then 
    echo "                       -- Passed"; 
else 
    echo "                       -- Failed"; 
fi
