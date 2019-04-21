#!/bin/bash
MONGO="mongodb://$MONGO_UNAME:$MONGO_PASS@$MONGO_URI"
echo "Using mongodb: $MONGO"

nodeos \
    --plugin eosio::mongo_db_plugin -m $MONGO \
    --mongodb-store-blocks=false --mongodb-store-transactions=false \
    --mongodb-store-transaction-traces=false \
    --mongodb-filter-on=music:transfer: \
    --p2p-listen-endpoint 127.0.0.1:9877 \
    --p2p-peer-address $PEER_HOST \
    --abi-serializer-max-time-ms=999900000 \
    --plugin eosio::chain_plugin \
    --read-mode=read-only \
    --plugin eosio::chain_api_plugin \
    --plugin eosio::http_plugin \
    --http-server-address=127.0.0.1:$HTTP_PORT \
    --config-dir node2 \
    --data-dir node2 >> /dev/null 2>&1