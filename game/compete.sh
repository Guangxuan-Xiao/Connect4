MY_BOT=./so/Strategy.so
TESTCASES=./TestCases/
compete() {
    echo Compete with "$1"
    ./Compete/Compete $MY_BOT $TESTCASES$1.so ./results/$1.txt 5 | tee ./log/$1.log
}

N=25
(
for((bot=100;bot>0;bot-=2)); do
    ((i=i%N)); ((i++==0)) && wait
    compete $bot &
done
wait
)