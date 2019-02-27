#include <eosiolib/eosio.hpp>
#include <eosiolib/symbol.hpp>

using namespace eosio;

class [[eosio::contract("music")]] music : public eosio::contract {

public:
  using contract::contract;
  
  music(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
  void addsong(std::string song_name, std::uint16_t price) {
    //require_auth(_self);
    std::hash<std::string> hasher;
    song_index songs(_code, _code.value); 
    auto iterator = songs.find(hasher(song_name));
    eosio_assert(iterator == songs.end(), "Song already exists");
    songs.emplace(_self, [&]( auto& row ) {
      row.key = hasher(song_name);
      row.song_name = song_name;
      row.price = price;
    });
  }

  [[eosio::action]]
  void erase(std::string song_name) {
    //require_auth(_self);
    song_index songs(_self, _code.value);
    std::hash<std::string> hasher;
    auto iterator = songs.find(hasher(song_name));
    eosio_assert(iterator != songs.end(), "Song doesn't exist");
    songs.erase(iterator);
  }

  [[eosio::action]]
  void buysong(name from, std::string song_name, std::string email) {
    require_auth(_self);
    print(email + "\n\n");
    action { permission_level{_self, "active"_n},
      "eosio.token"_n, "transfer"_n, {
         from, _self, asset(1, string_to_symbol(4, "SYS")), "idk"}}.send(); 
  }

private:
  struct [[eosio::table]] song {
    uint64_t key;
    std::string song_name;
    uint16_t price; 
    uint64_t primary_key() const { return key; }
  };
  typedef eosio::multi_index<"songs"_n, song> song_index;

};

EOSIO_DISPATCH( music, (addsong)(erase)(buysong))
