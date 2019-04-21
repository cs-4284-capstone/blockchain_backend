#!/bin/bash
export LC_ALL=C.UTF-8
export LANG=C.UTF-8

python3 /contracts/server.py &

nodeos -e -p eosio \
    --plugin eosio::producer_plugin \
    --plugin eosio::chain_api_plugin \
    --plugin eosio::http_plugin \
    --plugin eosio::history_plugin \
    --plugin eosio::history_api_plugin \
    --access-control-allow-origin='*' \
    --contracts-console \
    --http-validate-host=false >> /dev/null 2>&1