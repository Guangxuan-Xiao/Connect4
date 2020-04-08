g++ -Wall -std=c++11 -O2 -D OUTPUT -fpic -shared ./Strategy/Judge.cpp ./Strategy/Strategy.cpp ./Strategy/MCTree.cpp ./Strategy/Phase.hpp -o ./so/Strategy.so
g++ -std=c++11 ./Compete/*.cpp -o ./Compete/Compete -ldl -lpthread -fnon-call-exceptions -Wall 
./Compete/Compete  so/Strategy.so TestCases/100.so result.txt 5