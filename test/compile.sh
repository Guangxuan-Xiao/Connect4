root_dir="/mnt/d/Documents/2019-2020Spring/Learning/Intro2AI/hw/hw2/ConnectFour/test/"  #your path here
for file in `ls "${root_dir}Sourcecode"`
do
    cd "${root_dir}Sourcecode"
    pwd
    g++ -Wall -std=c++11 -O2 -fpic -shared *.cpp -o "${root_dir}so/${file}.so"
    cd -
done
