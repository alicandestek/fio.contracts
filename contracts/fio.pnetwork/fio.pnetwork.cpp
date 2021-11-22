#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>
#include <eosio/symbol.hpp>
#include <eosio/transaction.hpp>
#include <eosio/print.hpp>
#include <string>
#include <map>

using namespace eosio;

  class [[eosio::contract]] PEosOnEthVault : public eosio::contract {

    public:
      using eosio::contract::contract;

      [[eosio::action]]
      void pegin(name sender, name tokenContract, asset quantity, std::string destinationAddr, std::vector<char> userData) {
        require_auth(permission_level{get_self(), "active"_n}); // this action must be called by a local entrypoint
      }

      void pegInRequest(
        const eosio::name sender,
        const eosio::name receiver,
        const eosio::asset quantity,
        const std::string memo
      ){
	       if (sender == get_self()) return; //pegout, nothing to do here
	       name tokenContract = _first_receiver;

	       eosio::check(receiver == get_self(),  "Reject: invalid receiver");

        eosio::check(quantity.amount > 0, "Reject: insufficient value");
        eosio::check(SUPPORTED_TOKENS.find(tokenContract) != SUPPORTED_TOKENS.end(), "Reject: unsupported token contract");
        eosio::check(SUPPORTED_TOKENS.find(tokenContract)->second == quantity.symbol, "Reject: unsupported token symbol");

        action(
          permission_level{get_self(), "active"_n},
          get_self(),
          "pegin"_n,
	  std::make_tuple(sender, tokenContract, quantity, memo, NULL)
        ).send();
      };

    private:
      std::map<eosio::name,eosio::symbol> SUPPORTED_TOKENS = {{"fio.token"_n, eosio::symbol(eosio::symbol_code("FIO"), 9)}};
  };