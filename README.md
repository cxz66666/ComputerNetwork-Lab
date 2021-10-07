# ComputerNetwork-Lab
Lab repo for ZJU Computer Network course (linux base)

### How to use with Docker?

~~~bash
git clone https://github.com/cxz66666/ComputerNetwork-Lab
cd /path/to/ComputerNetwork-Lab
docker build -t network-lab:0.0.1
docker-compose up -d
~~~


### How to use with linux?
~~~bash
#make sure you already install g++ cmake make libstdc++
git clone https://github.com/cxz66666/ComputerNetwork-Lab
cd /path/to/ComputerNetwork-Lab/cmake-build-debug
cmake ..
make
~~~


**Warning**:docker use `git clone` to get source code, so you change local code will not apply until you push to github!   