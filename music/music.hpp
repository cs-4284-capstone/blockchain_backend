#include <string>
#include <vector>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/name.hpp>
#include <math.h>

using std::vector;
using std::map;
using std::string;

using namespace eosio;

namespace types {
  struct st_transfer {
      name from;
      name to;
      asset        quantity;
      string       memo;
  };
}
