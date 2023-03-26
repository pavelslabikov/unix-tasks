make
cd target 2> /dev/null || mkdir target && cd target 2> /dev/null
echo 123 > fileA 
for f in stats_*; do
    if [[ -f "$f" ]]; then
        rm $f
    fi
done

pids=()   

function lockfile {
    success=0
    failed=0
    while true;
do
    ../task.out -s 10 fileA > /dev/null
    if [ $? = 0 ]; then
        (( success = success + 1 ))
    else
        (( failed = failed + 1 ))
    fi
    echo "${1}      ${success}         ${failed}" > stats_${1}
done
}
for j in {1..10}; do
    lockfile $j &
    pids+=($!)
done
sleep 20
for jj in ${pids[@]}; do
    kill $jj
done

echo "Job    Success    Failed" > stats
for f in stats_*; do
    if [[ -f "$f" ]]; then
        cat $f >> stats
        rm $f
    fi
    
done