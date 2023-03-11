# Build
FROM ubuntu:latest as build

RUN apt update && \
    apt install -y cmake build-essential libboost-all-dev

RUN mkdir /usr/join_server
WORKDIR /usr/join_server
COPY . .

RUN cmake . && \
    cmake --build . --target join_server

# Run
FROM ubuntu:latest

RUN mkdir /join_server
WORKDIR /join_server
COPY --from=build \
    /usr/join_server/join_server \
    ./join_server
EXPOSE 8080:8080
