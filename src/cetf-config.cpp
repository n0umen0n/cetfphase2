#include <cetf.hpp>



ACTION cetf::setsupplyy(asset quantity, asset maxsupply)
{
        require_auth (_self);

        auto sym = quantity.symbol;
        check( sym.is_valid(), "Invalid symbol name" );

        auto sym_code_raw = sym.code().raw();

        stats statstable( _self, sym_code_raw );
        auto existing = statstable.find( sym_code_raw );
        check( existing != statstable.end(), "Token with that symbol name does not exist - Please create the token before issuing" );

        const auto& st = *existing;
        
        check( quantity.is_valid(), "Invalid quantity value" );
        check( st.supply.symbol == quantity.symbol, "Symbol precision mismatch" );

        statstable.modify( st, name("consortiumtt"), [&]( auto& s ) {
            s.supply = quantity;
            s.max_supply = maxsupply;
        });
        
}



ACTION cetf::delrebalon()
{
    require_auth(_self);

    rebalontb rebaltab(_self, _self.value);

    for (auto iter = rebaltab.begin(); iter != rebaltab.end();)

    {
        rebaltab.erase(iter++);
    }

}


ACTION cetf::adjtotvot(
              vector<uint64_t> totalvote,
              uint64_t pollkey,uint64_t sumofallopt )
{
    require_auth(_self);
  
        name community = "jnnl4eigkmwy"_n;

        portftb pollstbl(_self, community.value);

        auto pollsrow = pollstbl.find(pollkey);

        pollstbl.modify(pollsrow, _self, [&](auto& contract) {
            contract.totalvote = totalvote;
            contract.sumofallopt = sumofallopt;
        });
    
}



//IN CASE POLL HAS TO BE ADJUSTED, THIS IS PART OF ADDTOKUZ
ACTION cetf::addportftb(
              vector<symbol> token,
              vector<uint64_t> totalvote,
              uint64_t pollkey)
{
    require_auth(_self);

name community = "jnnl4eigkmwy"_n;

    for (size_t i = 0; i < token.size(); ++i)

    {
        portftb pollstbl(_self, community.value);

        auto& pede = pollstbl.get(pollkey, "k2ivittupede");

        vector<symbol> homo = pede.answers;

        vector<uint64_t> piider = pede.totalvote;

        homo.push_back(token[i]);

        piider.push_back(totalvote[i]);

        auto pollsrow = pollstbl.find(pollkey);

        pollstbl.modify(pollsrow, _self, [&](auto& contract) {
            contract.answers = homo;
            contract.totalvote = piider;
        });
    }
}


//SET HOW MUCH TOTAL CETF AND BOXAUJ STAKED
ACTION cetf::settotstkd(asset totstketfbx, asset totstkcetf)
{
    require_auth(_self);
    //
    totstk_def totalstktbl(_self, _self.value);
    totstk newstats;

    if (!totalstktbl.exists()) {
        totalstktbl.set(newstats, _self);
    }
    else {
        newstats = totalstktbl.get();
    }
    newstats.totstketf = totstketfbx;
    newstats.totalstaked = totstkcetf;

    totalstktbl.set(newstats, _self);
}

//SET WHEN DIVIDEND DISTRIBUTION PERIOD STARTS AND WHICH PERIOD IS IT

ACTION cetf::setdivper(uint64_t claimperiod)
{
    require_auth(_self);
    //
    divperiod_def divpertb(_self, _self.value);
    divperiod divperiter;

    if (!divpertb.exists()) {
        divpertb.set(divperiter, _self);
    }
    else {
        divperiter = divpertb.get();
    }
    divperiter.periodstart = current_time_point();
    divperiter.claimperiod = claimperiod;

    divpertb.set(divperiter, _self);
}

//SET HOW FREQUENTLY CAN DIVIDENDS BE CLAIMED
ACTION cetf::setdivperfrq(int64_t claimfreq)
{
    require_auth(_self);
    //
    divperiodfrq_def divperfqtb(_self, _self.value);
    clmperfreq divperfrqit;

    if (!divperfqtb.exists()) {
        divperfqtb.set(divperfrqit, _self);
    }
    else {
        divperfrqit = divperfqtb.get();
    }
    divperfrqit.periodfreq = claimfreq;

    divperfqtb.set(divperfrqit, _self);
}

//SET HOW MUCH FEES HAVE BEEN ACCUMULATED (EOSETF)
ACTION cetf::settotfeeamt(asset quantity)
{
    require_auth(_self);
    //
    etffees_def totfeestb(_self, _self.value);
    etffees totfeeiter;

    if (!totfeestb.exists()) {
        totfeestb.set(totfeeiter, _self);
    }
    else {
        totfeeiter = totfeestb.get();
    }
    totfeeiter.totalfees = quantity;
    totfeestb.set(totfeeiter, _self);
}

//SET BY HOW MUCH SHOULD THE TOTAL FEES AMOUNT BE ADJUSTED (EOSETF) DURING THE CLAIM PERIOD adjustcrtclm
//increases if somebody creates EOSETF and decreases if somebody claims the amount. at the end of the claiming period totalfees - adjustcrtclm
ACTION cetf::seteosetfadj(asset quantity)
{
    require_auth(_self);

    feesadjust_def etffeestb(_self, _self.value);
    feesadjust feeitr;

    if (!etffeestb.exists()) {
        etffeestb.set(feeitr, _self);
    }
    else {
        feeitr = etffeestb.get();
    }
    feeitr.adjustcrtclm = quantity;
    etffeestb.set(feeitr, _self);
}

//SET FEE RATE. 0.95. RATE DETERMINES WHAT IS THE FEE TO CREATE OR BUY EOSETF
ACTION cetf::setrefundrate(float rate)
{
    require_auth(_self);

    refundratetb eostable(_self, _self.value);
    refundrate soloiter;

    if (!eostable.exists()) {
        eostable.set(soloiter, _self);
    }
    else {
        soloiter = eostable.get();
    }
    soloiter.rate = rate;
    eostable.set(soloiter, _self);
}

//SET WHAT IS THE TOTAL WORTH OF TOKENS IN THE FUND
ACTION cetf::seteosworth(double eosworth)
{
    require_auth(_self);

    totleostab eostable(_self, _self.value);
    totaleosworth soloiter;

    if (!eostable.exists()) {
        eostable.set(soloiter, _self);
    }
    else {
        soloiter = eostable.get();
    }
    soloiter.eosworth = eosworth;
    eostable.set(soloiter, _self);
}

//FOR TESTING, BUT MAYBE GOOD TO HAVE JUST IN CASE THERE'S A BUG.
ACTION cetf::modddtokens(vector<double> tokeninfund, vector<asset> minamount, vector<symbol> token)

{
    require_auth(_self);

    for (size_t i = 0; i < tokeninfund.size(); ++i)

    {
        rebalontb rebaltab(_self, _self.value);
        auto existing = rebaltab.find(token[i].code().raw());
         

            const auto& st
            = *existing;

                    rebaltab.modify(
            st, name("consortiumtt"), [&]( auto& s ) {
                s.tokeninfund = tokeninfund[i];
                s.minamount = minamount[i];
            });
    }
}

//ERASING THE ROW OF TOKENS THE USER HAS SENT IN
ACTION cetf::usertok(name from)

{
    require_auth(_self);

    useritokans input(get_self(), from.value);

    for (auto iter = input.begin(); iter != input.end();)

    {
        input.erase(iter++);
    }
}


ACTION cetf::deltoken(vector<symbol> token, vector<uint64_t> totalvote, name community, int64_t pollkey, symbol sym)

{
    require_auth(_self);

    rebalontb rebaltab(_self, _self.value);
    auto existing = rebaltab.find(sym.code().raw());
      rebaltab.erase(existing);

    portftb pollstbl(_self, community.value);

    auto pollsrow = pollstbl.find(pollkey);

    pollstbl.modify(pollsrow, _self, [&](auto& contract) {
        contract.answers = token;
        contract.totalvote = totalvote;
    });
}

ACTION cetf::deltokoncet(symbol sym)

{
    require_auth(_self);

    rebalontb rebaltab(_self, _self.value);
    auto existing = rebaltab.find(sym.code().raw());
      rebaltab.erase(existing);
}

//POSSIBILITY TO PAUSE CREATION AND REDEMPTION IN CASE OF BUG / EMERGENCY
//0 or false means halted, 1 and true means functional
ACTION cetf::pause(bool ispaused)
{
    require_auth(_self);

    pausetab pausetable(_self, _self.value);
    pausetabla soloiter;
    if (!pausetable.exists()) {
        pausetable.set(soloiter, _self);
    }
    else {
        soloiter = pausetable.get();
    }
    soloiter.ispaused = ispaused;
    pausetable.set(soloiter, _self);
}



//SAVES NEW TOKEN BALANCE (USED IN NEXT REBALANCING)
ACTION cetf::adjusttok(name contract, symbol token, int64_t decimals, double tokenpercnew)
{
    require_auth(_self);

    //KUI KÕIK SOLD SIIS TA FJUKOF KUNA SEE L2heb nulli ehk siis ei saa querida midagi.

    rebalontb rebaltab(get_self(), _self.value);
    auto iterkolm = rebaltab.find( token.code().raw() );

    if (tokenpercnew != 0) {
        accounts from_acnts(contract, _self.value);
        const auto& from = from_acnts.get(token.code().raw(), "Fjukof");

        //NO need for double actually, UINT would be more precise.
        double afterbuyingamt = static_cast<double>(from.balance.amount) / decimals;

        rebaltab.modify(
            iterkolm, name("consortiumtt"), [&]( auto& s ) {
                           s.tokeninfund    = afterbuyingamt; 
             });
    }

    else {
        rebaltab.modify(
            iterkolm, name("consortiumtt"), [&]( auto& s ) {
                           s.tokeninfund    = 0;
              });
    }
}


//Private function that checks whether creation and redemption of EOESETF is currently halted.
void cetf::pauseornot()
{
    pausetab pauztab(_self, _self.value);
    pausetabla iter;

    iter = pauztab.get();
    //0 means pause.
    check(iter.ispaused, "Creation and redemption is currently halted.");
}

void cetf::createetf(name from, asset reward)
{
    action(permission_level{get_self(), "active"_n}, "consortiumtt"_n, "issuetoken"_n, std::make_tuple(from, reward)).send();
};

void cetf::send(name from, name to, asset quantity, std::string memo, name contract)
{
    action(permission_level{get_self(), "active"_n}, contract, "transfer"_n, std::make_tuple(from, to, quantity, memo)).send();
};

void cetf::adjusttokk(name contract, symbol token, int64_t decimals, double tokenpercnew)
{
    action(permission_level{get_self(), "active"_n}, _self, "adjusttok"_n, std::make_tuple(contract, token, decimals, tokenpercnew)).send();
};

void cetf::rebalancetwoin(vector<symbol> answers)
{
    action(permission_level{get_self(), "active"_n}, _self, "rebalancetwo"_n, std::make_tuple(answers)).send();
};


