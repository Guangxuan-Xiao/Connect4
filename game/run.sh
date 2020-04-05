g++ -Wall -std=c++11 -O2 -fpic -shared -DDEBUG -g ./Strategy/Judge.cpp ./Strategy/Strategy.cpp ./Strategy/MCTree.cpp ./Strategy/Phase.hpp -o ./so/Strategy.so
g++ -std=c++11 ./Compete/*.cpp -o ./Compete/Compete -ldl -lpthread -g -fnon-call-exceptions -Wall -DDEBUG 
./Compete/Compete TestCases/100.so so/Strategy.so result.txt 1