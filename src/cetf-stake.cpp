#include <cetf.hpp>

ACTION cetf::stakecetf(name user, asset quantity, uint64_t id)
{
    require_auth(user);

    auto sym = quantity.symbol.code();
    auto symcetf = symbol("CETF", 4);

    check(quantity.is_valid(), "invalid quantity");
    check(quantity.amount > 0, "must stake positive quantity");
    check(quantity.symbol == symcetf, "Only possible to stake CETF.");

    accounts from_acnts(_self, user.value);
    const auto& from = from_acnts.get(quantity.symbol.code().raw(), "no balance object found");

    check(from.balance.amount >= quantity.amount, "Staking more CETF than you have.");

    divperiod_def divpertb(_self, _self.value);
    divperiod divperiter;
    divperiter = divpertb.get();

    perzonstkd personstktbl(_self, user.value);
    auto userrow = personstktbl.find(id);
    if (userrow == personstktbl.end() )
         
        {
            personstktbl.emplace(_self, [&](auto& s) {
                s.id = id;
                s.staked = quantity;
                s.staketime = current_time_point();
                s.stakeperiod = divperiter.claimperiod;
                   
            });
        }
    if (userrow != personstktbl.end() )
          { check(false, "This ID already in use, please try staking again."); }

for
     (auto iter = personstktbl.begin(); iter != personstktbl.end(); iter++)
    {
        perstotlskd perstottb(_self, _self.value);
        auto totrow = perstottb.find(user.value);

        if (totrow == perstottb.end() )
             
            {
                perstottb.emplace(_self, [&](auto& s) {
                    s.indtotstaked = iter->staked;
                    s.staker = user;
                });
            }
        if (totrow != perstottb.end() )
             
            {
                perstottb.modify(
                    totrow, name("consortiumtt"), [&]( auto& s ) {
                        s.indtotstaked += iter->staked;
                           
                    });
            }
    }

perstotlskd indtotstk(_self, _self.value);
const auto & pede = indtotstk.get(user.value, "Individual has not staked." );

check(from.balance.amount >= pede.indtotstaked.amount, "Trying to stake more than available CETF.");

totstk_def totalstktbl(_self, _self.value);
totstk newstats;

newstats = totalstktbl.get();
newstats.totalstaked.amount += quantity.amount;
totalstktbl.set(newstats, _self);

auto totalrow = indtotstk.find(user.value);
indtotstk.modify(
    totalrow, name("consortiumtt"), [&]( auto& s ) {
        s.indtotstaked.amount = 0;
           
    });
}

ACTION cetf::unstakecetf(name user, vector<asset> quantity, vector<uint64_t> id, name clmspecifier)
{
    require_auth(user);

    check(clmspecifier == "eosetfeosetf"_n, "Wrong claiming specifier");
    claimtimetb claimtab(_self, clmspecifier.value);
    const auto& claimiter = claimtab.get(user.value, "Claim at least once to unstake.");

    divperiod_def divpertb(_self, _self.value);
    divperiod divperiter;
    divperiter = divpertb.get();

    check(claimiter.claimperiod != divperiter.claimperiod, "Please don't claim next period, then you will be able to unstake.");

      for (int i = 0; i < quantity.size(); i++)
    {
        auto sym = quantity[i].symbol.code();
        stats statstable(_self, sym.raw());
        const auto& st = statstable.get(sym.raw());

        check(quantity[i].is_valid(), "invalid quantity");
        check(quantity[i].amount > 0, "must ustake positive quantity");
        check(quantity[i].symbol == st.supply.symbol, "symbol precision mismatch while staking");

        accounts from_acnts(_self, user.value);
        const auto& from = from_acnts.get(quantity[i].symbol.code().raw(), "no balance object found");

        perzonstkd personstktbl(_self, user.value);

        auto userrow = personstktbl.find(id[i]);

        const auto & iterone = personstktbl.get(id[i], "No such staking ID(1)." );

        check(iterone.staked.amount >= quantity[i].amount, "Unstaking too much CETF.");

        personstktbl.modify(
            userrow, name("consortiumtt"), [&]( auto& s ) {
                s.staked.amount -= quantity[i].amount;
                   
            });

        const auto & itertwo = personstktbl.get(id[i], "No such staking ID(2)." );

        if (itertwo.staked.amount == 0) {
            personstktbl.erase(userrow);
        }

        totstk_def totalstktbl(_self, _self.value);
        totstk newstats;

        newstats = totalstktbl.get();

        newstats.totalstaked.amount -= quantity[i].amount;
        totalstktbl.set(newstats, _self);
    }
}

ACTION cetf::unstakeetf(name user, vector<asset> quantity, vector<uint64_t> id, name clmspecifier)

{
    require_auth(user);

    check(clmspecifier == "cetfcetfcetf"_n, "Wrong claiming specifier");
    claimtimetb claimtab(_self, clmspecifier.value);
    const auto& claimiter = claimtab.get(user.value, "Claim at least once to unstake.");

    divperiod_def divpertb(_self, _self.value);
    divperiod divperiter;
    divperiter = divpertb.get();

    check(claimiter.claimperiod != divperiter.claimperiod, "Please don't claim next period, then you will be able to unstake.");

    for (int i = 0; i < quantity.size(); i++) {
        indstkdetftb personstktbl(_self, user.value);

        auto userrow = personstktbl.find(id[i]);

        const auto & iterone = personstktbl.get(id[i], "No such staking ID(1)." );

        check(iterone.staked.amount >= quantity[i].amount, "Unstaking too much BOXAUJ.");

        personstktbl.modify(
            userrow, name("consortiumtt"), [&]( auto& s ) {
                s.staked.amount -= quantity[i].amount;
                   
            });

        send("consortiumtt"_n, user, quantity[i], "Returning LP tokens", "lptoken.defi"_n);

        const auto & itertwo = personstktbl.get(id[i], "No such staking ID(2)." );

        if (itertwo.staked.amount == 0) {
            personstktbl.erase(userrow);
        }

        totstk_def totalstktbl(_self, _self.value);
        totstk newstats;

        newstats = totalstktbl.get();

        newstats.totstketf.amount -= quantity[i].amount;
        totalstktbl.set(newstats, _self);
    }
}