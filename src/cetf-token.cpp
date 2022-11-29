#include <cetf.hpp>

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <cmath>
#include <eosio/singleton.hpp>
#include <numeric>

using namespace std;
using namespace eosio;

ACTION cetf::issuetoken ( name owner, asset touser, asset tosupply )
    
{
            require_auth (_self);
            require_recipient( owner );

            
        auto sym = tosupply.symbol;
            check( sym.is_valid(), "Invalid symbol name" );

            auto sym_code_raw = sym.code().raw();

            stats statstable( _self, sym_code_raw );
            auto existing = statstable.find( sym_code_raw );
            check( existing != statstable.end(), "Token with that symbol name does not exist - Please create the token before issuing" );

            const auto& st = * existing;
            
        check( tosupply.is_valid(), "Invalid quantity value" );
            check( st.supply.symbol == tosupply.symbol, "Symbol precision mismatch" );
            check( st.max_supply.amount - st.supply.amount >= tosupply.amount, "Quantity value cannot exceed the available supply" );

            statstable.modify( st, name("cet.f"), [&]( auto& s ) {
            s.supply += tosupply;
         });
            
        
       add_balance( owner, touser, name("cet.f"));
        
}

ACTION cetf::transferdiv ( name owner, asset div )
    
{
            require_auth (_self);
            require_recipient( owner );
            
       add_balance( owner, div, name("cet.f"));
        
}

ACTION cetf::create(const name& issuer, const asset& maximum_supply)
{
    require_auth(_self);

    auto sym = maximum_supply.symbol;
    check(sym.is_valid(), "invalid symbol name");
    check(maximum_supply.is_valid(), "invalid supply");
    check(maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable(_self, sym.code().raw());
    auto existing = statstable.find(sym.code().raw());
    check(existing == statstable.end(), "token with symbol already exists");

    statstable.emplace(_self, [&](auto& s) {
        s.supply.symbol = maximum_supply.symbol;
        s.max_supply = maximum_supply;
        s.issuer = issuer;
    });
}

ACTION cetf::issue(const name& to, const asset& quantity, const string& memo)
{
    auto sym = quantity.symbol;
    check(sym.is_valid(), "invalid symbol name");
    check(memo.size() <= 256, "memo has more than 256 bytes");

    stats statstable(_self, sym.code().raw());
    auto existing = statstable.find(sym.code().raw());
    check(existing != statstable.end(), "token with symbol does not exist, create token before issue");
    const auto& st = *existing;
    check(to == st.issuer, "tokens can only be issued to issuer account");

    require_auth(st.issuer);
    check(quantity.is_valid(), "invalid quantity");
    check(quantity.amount > 0, "must issue positive quantity");

    check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
    check(quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify(st, same_payer, [&](auto& s) { s.supply += quantity; });

    add_balance(st.issuer, quantity, st.issuer);
}

ACTION cetf::transfer(name from, name to, asset quantity, std::string memo)
{
    check(from != to, "cannot transfer to self");
    require_auth(from);
    check(is_account(to), "to account does not exist");
    auto sym = quantity.symbol.code();
    stats statstable(_self, sym.raw());
    const auto& st = statstable.get(sym.raw());

    require_recipient(from);
    require_recipient(to);

    check(quantity.is_valid(), "invalid quantity");
    check(quantity.amount > 0, "must transfer positive quantity");
    check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
    check(memo.size() <= 256, "memo has more than 256 bytes");

    auto payer = has_auth(to) ? to : from;

    auto separator_pos = memo.find(':');

    if (to == get_self()) {
        //check(false, "This action will be activated when CETF distribution ends or latest on 31.04.2021");

        refund_tokens_back(from, to, quantity, memo);

        sub_balance(from, quantity);
        //add_balance(to, quantity, payer);

        retire(quantity, memo);
    }

    //IN CASE SOMEBODY PURCHASES FROM DEFIBOX THEY GET 95% FROM THE PURCHASED AMOUNT 5% GOES TO CETF HOLDERS.
    if (from == "swap.defi"_n && memo != "Defibox: withdraw" && quantity.symbol == symbol("EOSETF", 4)) {
        feesadjust_def etffeestb(_self, _self.value);
        feesadjust feeitr;

        feeitr = etffeestb.get();

        refundratetb eostable(_self, _self.value);
        refundrate soloiter;
        soloiter = eostable.get();

        //ADD TO THE SINGLETON THAT CALCULATES HOW MUCH FEE WAS ACCUMULATED DURING A PERIOD

        feeitr.adjustcrtclm.amount += (quantity.amount * (1 - soloiter.rate));
        etffeestb.set(feeitr, _self);

        int64_t intadjquan = static_cast<double>(quantity.amount) * soloiter.rate;

        struct asset adjquantity = { int64_t(intadjquan), symbol("EOSETF", 4) };

        sub_balance(from, quantity);

        add_balance(to, adjquantity, payer); //*feerate
    }

    if (from != "swap.defi"_n && to != get_self()) {

        sub_balance(from, quantity);
        add_balance(to, quantity, payer);
    }
}

//CRUCIAL FUNCTION THAT CHECKS IF CORRECT AMOUNTS OF TOKENS ARE SENT IN
//ON_NOTIFY CAPTURES ALL THE INCOMING TOKENS AND SAVES THEM INTO USERITOKANS TABLE
void cetf::savetokens(name from, asset quantity, name to)
{
    if
         (to  != "cet.f"_n) return;

    pauseornot();

    rebalontb sinput(get_self(), _self.value);
          auto secinput = sinput.find( quantity.symbol.code().raw() );

    useritokans input(get_self(), from.value);
          auto newinput = input.find( quantity.symbol.code().raw() );

    check(quantity.symbol == secinput->token, "Incorrect symbol.");

    //SHOULD THERE BE CHECK FOR MAX AMOUNT?

    check(quantity.amount >= secinput->minamount.amount, "Minimum creation threshold is 1 EOSETF.");

    //THIS NEEDED IN ORDER TO SAVE ONLY THE TOKENS THAT ARE INCLUDED IN THE FUND
    if (secinput->tokenpercnew > 0)

    {
             if ( newinput == input.end() ) 
        {
                     input.emplace( name("cet.f"), [&]( auto& a ) {
                            a.token = quantity;
                a.ratio = secinput->ratio;
                         
            });

                 
        }
                 else
        {
                        input.modify(
                newinput, name("cet.f"), [&]( auto& a ) {
                                a.token += quantity;
                    a.ratio = secinput->ratio;
                            
                });
                  
        }
    }

    checkratuus(from, quantity);
}

void cetf::checkratuus(name from, asset quantity)
{
    basetoktab basetable(_self, _self.value);
    basetok baseiter;

    baseiter = basetable.get();

    useritokans input(get_self(), from.value);

    auto size = std::distance(input.cbegin(), input.cend());

    etfsizetab sizetable(_self, _self.value);
    etfsize soloiter;

    soloiter = sizetable.get();

    //if size is smaller will not issue CETF/EOSETF, just saves the token values in the table.
    if (size == soloiter.size) {
        const auto& basetokrow = input.find(baseiter.base.code().raw());

        //LOOP THAT CHECKS THE RATIO FOR EACH TOKEN
        for (auto iter = input.begin(); iter != input.end();) {
            //ADDING TOKENS TO FUND, NEEDED FOR REBALANCING PURPOSES.
            rebalontb rebaldab(get_self(), _self.value);

            const auto & rebaliter = rebaldab.get(iter->token.symbol.code().raw() , "No such token in rebal table" );

            double addtofund = static_cast<double>(iter->token.amount) / rebaliter.decimals;

            auto otsiexisting = rebaldab.find(iter->token.symbol.code().raw() );
            rebaldab.modify(
                otsiexisting, name("cet.f"), [&]( auto& s ) {
                                s.tokeninfund    += addtofund;
                            
                });

            //CHECKING IF RATIOS CORRECT
            check(iter->token.amount != 0, "Doggy Afuera!");

            //THIS ONE CRUCIAL, CHECKS IF INCOMING TOKENS PRODUCE THE PREDETERMINED RATIO (CALCULATED DURING THE REBELANCE FUNCTION). IF PRECISELY THE SAME (RATIO IS A DOUBLE) THEN
            check((static_cast<double>(iter->token.amount) / basetokrow->token.amount == iter->ratio), "Incorrect token ratios.");

            input.erase(iter++);
        }

        rebalontb rebaltab(get_self(), _self.value);
        auto iteraator = rebaltab.find( baseiter.base.code().raw());

        feesadjust_def etffeestb(_self, _self.value);
        feesadjust feeitr;
        feeitr = etffeestb.get();

        refundratetb eostable(_self, _self.value);
        refundrate soloiter;
        soloiter = eostable.get();

        //struct asset numberofetfs = {int64_t ((basetokrow->token.amount/iteraator->minamount.amount)*10000), symbol ("EOSETF", 4)};
         struct asset numberofetfs = { int64_t((basetokrow->token.amount / iteraator->minamount.amount * (soloiter.rate)) * 10000), symbol("EOSETF", 4) };

                struct asset addtocrtclm = { int64_t((basetokrow->token.amount / iteraator->minamount.amount * (1 - soloiter.rate)) * 10000), symbol("EOSETF", 4) };

                struct asset addsupply = { int64_t(numberofetfs.amount + addtocrtclm.amount), symbol("EOSETF", 4) };
        //ADD TO THE SINGLETON THAT CALCULATES HOW MUCH FEE WAS ACCUMULATED DURING A PERIOD
        feeitr.adjustcrtclm.amount += addtocrtclm.amount;
        etffeestb.set(feeitr, _self);

        //ISSUE ETF
        createetf(from, numberofetfs, addsupply);
        /* ADD IN CASE WANT TO REWARD USERS FOR CREATING EOSETF
auto sym = symbol ("CETF", 4);


stats statstable( _self, sym.code().raw() );
auto existing = statstable.find( sym.code().raw() );
const auto& st = *existing;

//SOME CETF ISSUANCE FOR CREATING EOSETF, TOUCH OF SATOSHI, HIS HALVING METHOD FOR BTC
if (st.supply.amount < 800000000000)

{

const int64_t interval = (200000000000);  

int64_t halvings =  (st.supply.amount / interval);

int64_t rewardint =  (800000);

int64_t divider = pow( 2 , halvings);

int64_t adjrewardint = rewardint/divider;

struct asset reward = {int64_t (adjrewardint*numberofetfs.amount/10000), symbol ("CETF", 4)};

createetf(from, reward );

}
*/
    }
}

void cetf::sub_balance(name owner, asset value)
{
    accounts from_acnts(_self, owner.value);
    const auto& from = from_acnts.get(value.symbol.code().raw(), "no balance object found in accounts");

    check(from.balance >= (value), "sub_balance: from.balance overdrawn balance");

    auto sym = symbol("CETF", 4);

    //IGNORING EOSETF SUBSTRACTIONS
    if (value.symbol == sym)

    {
        perzonstkd personstktbl(_self, owner.value);

        //THIS LOOP CHECKS HOW MUCH HAS USER STAKED IN TOTAL
for
     (auto iter = personstktbl.begin(); iter != personstktbl.end(); iter++)
    {
        perstotlskd perstottb(_self, _self.value);
        auto totrow = perstottb.find(owner.value);

        if (totrow != perstottb.end() )
             
            {
                perstottb.modify(
                    totrow, name("cet.f"), [&]( auto& s ) {
                        s.indtotstaked += iter->staked;
                           
                    });
            }
    }

perstotlskd perstotkaks(_self, _self.value);
auto totrowkaks = perstotkaks.find(owner.value);

if (totrowkaks != perstotkaks.end() )
     
    {
        const auto & tra = perstotkaks.get(owner.value, "No totstaked for user" );

        check(from.balance.amount - tra.indtotstaked.amount >= (value.amount), "sub_balance: unstake CETF to transfer");

        //SETTING TOTAL STAKE TO ZERO AGAIN IN ORDER FOR THE CHECK TO HAPPEN NEXT TIME AGAIN IF TRANSFERRED
        perstotkaks.modify(
            totrowkaks, name("cet.f"), [&]( auto& s ) {
                s.indtotstaked.amount = 0;
                   
            });
    }

    } //CLOSING IF SYM == CETF

    from_acnts.modify(from, owner, [&](auto& a) { a.balance -= value; });
}

void cetf::add_balance(name owner, asset value, name ram_payer)
{
    accounts to_acnts(_self, owner.value);
    auto to = to_acnts.find(value.symbol.code().raw());
    if (to == to_acnts.end()) {
        to_acnts.emplace(ram_payer, [&](auto& a) { a.balance = value; });
    }
    else {
        to_acnts.modify(to, same_payer, [&](auto& a) { a.balance += value; });
    }
}

void cetf::retire(asset quantity, std::string memo)
{
    auto sym = quantity.symbol;
    check(sym.is_valid(), "invalid symbol name");
    check(memo.size() <= 256, "memo has more than 256 bytes");

    stats statstable(_self, sym.code().raw());
    auto existing = statstable.find(sym.code().raw());
    check(existing != statstable.end(), "token with symbol does not exist");
    const auto& st = *existing;

    //require_auth( st.issuer );
    check(quantity.is_valid(), "invalid quantity");
    check(quantity.amount > 0, "must retire positive quantity");

    check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");

    statstable.modify(st, same_payer, [&](auto& s) { s.supply -= quantity; });

    //sub_balance(st.issuer, quantity);
}
