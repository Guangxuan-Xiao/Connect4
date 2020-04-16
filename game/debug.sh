g++ -Wall -std=c++11 -O2 -fpic -shared -D OUTPUT ./Strategy/Judge.cpp ./Strategy/Strategy.cpp ./Strategy/MCTree.cpp ./Strategy/Phase.hpp -o ./so/Strategy.so.d
g++ -std=c++11 ./Compete/*.cpp -o ./Compete/Compete -ldl -lpthread -g -fnon-call-exceptions -Wall
compete() {
    ./Compete/Compete so/Strategy.so.d so/Strategy.so ./debug/$1.txt 5 | tee ./debug_log/$1.log
}
N=10
(
for((bot=0;bot<$N;bot++)); do
    ((i=i%N)); ((i++==0)) && wait
    compete $bot &
done
wait
)
