make
cd target 2> /dev/null || mkdir target && cd target 2> /dev/null
echo 123 > fileA 
sleep 1 && rm fileA.lck &
../task.out -s 5 fileA 