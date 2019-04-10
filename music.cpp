#include <eosiolib/eosio.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/asset.hpp>
#include "music.hpp"
#include <string>
using namespace eosio;
class [[eosio::contract("music")]] music : public eosio::contract {

public:
  using contract::contract;
  music(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
  void addsong(std::uint64_t song_id, std::uint64_t price) {
    //require_auth(_self);
    song_index songs(get_self(), 100001); 
    auto iterator = songs.find(song_id);
    eosio_assert(iterator == songs.end(), "Song already exists");
    songs.emplace(get_self(), [&]( auto& row ) {
      row.key = song_id;
      row.song_id = song_id;
      row.price = price;
    });
  }

  [[eosio::action]]
  void erase(std::uint64_t song_id) {
    //require_auth(_self);
    song_index songs(get_self(), 100001);
    auto iterator = songs.find(song_id);
    eosio_assert(iterator != songs.end(), "Song doesn't exist");
    songs.erase(iterator);
  }

  [[eosio::action]]
  void transfer(uint64_t sender, uint64_t receiver) {
    auto data = unpack_action_data<types::st_transfer>();
    eosio_assert(data.quantity.is_valid(), "Transaction must be valid");
    int totalcost = 0;
    int last = 0;
    const std::uint64_t song_id = std::stoi(data.memo.substr(0, data.memo.find(";")));
    song_index songs(get_self(), 100001);
    auto x = 0;
    auto iterator = songs.find(song_id);
    eosio_assert(iterator != songs.end(), "Song you attempted to buy doesn't exist!");
    totalcost += iterator->price;
    /*while ((x = memo.string::find(",")) != std::string::npos) {
      auto name = memo.substr(0, x);
      auto itr = songs.find(hasher(name));
      eosio_assert(itr != songs.end(), "Song you attempted to buy doesn't exist!");
      //query for price
      totalcost += itr->price;
    }*/
    eosio_assert(totalcost * 10000 == data.quantity.amount, "Incorrect amount of money sent for transaction");
    eosio::print("transfer complete");
  }

  /*[[eosio::action]]
  void buysong(name from, std::string song_name, std::string email) {
    //require_auth(from);
    action ( permission_level{from, name("active")},
      name("eosio.token"), name("transfer"), std::make_tuple(
         from, get_self(), asset(10000, symbol(std::string("SYS"), 4)), std::string("idk"))).send();*/ 

private:
  struct [[eosio::table]] song {
    uint64_t key;
    uint64_t song_id;
    uint64_t price; 
    uint64_t primary_key() const { return key; }
  };
  typedef eosio::multi_index<"songs"_n, song> song_index;

};

extern "C" {
    void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        if (action == "transfer"_n.value) {
          execute_action(name(receiver), name(code), &music::transfer);
        } 
        else if (code == receiver) {
          switch (action) { EOSIO_DISPATCH_HELPER(music, (addsong)(erase)) }
        }
        eosio_exit(0);
    }
};
