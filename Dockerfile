FROM alpine
WORKDIR /app
EXPOSE 4611
RUN sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories

RUN apk --no-cache add git make cmake g++ libstdc++

RUN git clone  https://github.com.cnpmjs.org/cxz66666/ComputerNetwork-Lab ./lab
WORKDIR lab
RUN cd cmake-build-debug && cmake .. && make
RUN apk del git cmake g++


ENTRYPOINT ["/app/lab/cmake-build-debug/server/server"]


