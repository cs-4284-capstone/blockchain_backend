FROM ubuntu:bionic

RUN apt update && apt-get install -yq wget python3-pip

RUN wget https://github.com/EOSIO/eos/releases/download/v1.7.0/eosio_1.7.0-1-ubuntu-18.04_amd64.deb
RUN apt install -yq ./eosio_1.7.0-1-ubuntu-18.04_amd64.deb

RUN wget https://github.com/EOSIO/eosio.cdt/releases/download/v1.6.1/eosio.cdt_1.6.1-1_amd64.deb
RUN apt install -yq ./eosio.cdt_1.6.1-1_amd64.deb

RUN pip3 install flask

RUN mkdir contracts
COPY . /contracts

WORKDIR /contracts
RUN cd eosio.contracts/eosio.token && eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
RUN eosio-cpp -o music.wasm music/music.cpp --abigen
RUN mv music.* music

COPY nodeos.sh /nodeos.sh
COPY nodeos_mirror.sh /nodeos_mirror.sh

ENTRYPOINT [ "sh", "-c" ]