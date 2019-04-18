#!/bin/bash

WALLET="soundbin"

set -x

cd /contracts

cleos wallet create -n ${WALLET} -f ${WALLET}_wallet.txt
cleos wallet open -n ${WALLET}
cat ${WALLET}_wallet.txt | cleos wallet unlock -n ${WALLET}
cleos wallet create_key -n soundbin | awk 'BEGIN { FS = "\"" } ; { print $2 }' > pubkey.txt
cat key.txt | cleos wallet import -n ${WALLET}

USER_KEY=$(cat pubkey.txt)

cleos create account eosio eosio.token ${USER_KEY}
cleos create account eosio music ${USER_KEY}
cleos create account eosio bob ${USER_KEY}
cleos create account eosio alice ${USER_KEY}

cleos set contract eosio.token /contracts/eosio.contracts/eosio.token --abi eosio.token.abi -p eosio.token@active
cleos set contract music /contracts/music -p music@active

cleos push action eosio.token create '[ "eosio", "1000000000.0000 SYS"]' -p eosio.token@active
cleos push action eosio.token issue '[ "alice", "100.0000 SYS", "memo" ]' -p eosio@active

cleos wallet lock -n $WALLET