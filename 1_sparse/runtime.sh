#!/usr/bin/env
echo "Начало скрипта"
make
cd target || mkdir target && cd target 2> /dev/null
../generator.out
../task.out fileA fileB
gzip -kf fileA fileB
gzip -ckfd fileB.gz | ../task.out fileC
../task.out -b 100 fileA fileD
stat fileA fileA.gz fileB fileB.gz fileC fileD
echo "Окончание скрипта"