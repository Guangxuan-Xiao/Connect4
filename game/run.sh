g++ -Wall -std=c++11 -O2 -fpic -shared ./Strategy/Judge.cpp ./Strategy/Strategy.cpp ./Strategy/MCTree.cpp ./Strategy/Phase.hpp -o ./so/Strategy.so_random
g++ -std=c++11 ./Compete/*.cpp -o ./Compete/Compete -ldl -lpthread -fnon-call-exceptions -Wall 
./Compete/Compete  so/Strategy.so_smart so/Strategy.so_random result.txt 1