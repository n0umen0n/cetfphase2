#include <cetf.hpp>
//IMG nah
//ADDS TOKEN TO THE FUND. ALLOCATION 0. SO FUND MANAGERS HAVE TO VOTE TO ACTUALLY INCLUDE THE TOKEN IN THE FUND. ALSO ON_NOTIFY IS REQUIRED FOR EACH ADDED TOKEN.
ACTION cetf::addtokuz(vector<double> tokeninfund,
              vector<double> tokenwortheos,
              vector<double> tokenperold,
              vector<double> tokenpercnew,
              vector<int64_t> decimals,
              vector<uint64_t> pairid,
              vector<string> strpairid,
              vector<symbol> token,
              vector<name> contract,
              vector<double> ratio,
              vector<asset> minamount,
              vector<uint64_t> totalvote,
              name community,
              uint64_t pollkey)
{
    require_auth(_self);

    for (size_t i = 0; i < pairid.size(); ++i)

    {
        rebalontb rebaltab(get_self(), _self.value);
        auto existing = rebaltab.find(token[i].code().raw());
         

            if (existing == rebaltab.end() ) 
        {
            rebaltab.emplace(_self, [&](auto& s) {
                s.tokeninfund = tokeninfund[i];
                s.tokenwortheos = tokenwortheos[i];
                s.tokenperold = tokenperold[i];
                s.tokenpercnew = tokenpercnew[i];
                s.decimals = decimals[i];
                s.pairid = pairid[i];
                s.strpairid = strpairid[i];
                s.token = token[i];
                s.contract = contract[i];
                s.ratio = ratio[i];
                s.minamount = minamount[i];
            });
        }

        if (existing != rebaltab.end() )
              { check(false, "Token already added."); }
    }

    //require_auth ("cet.f"_n);

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






ACTION cetf::cetfvote(vector <uint64_t> usersvote, uint64_t pollkey, name community, name voter) {

require_auth ( voter );

uint64_t sum_of_votes = accumulate(usersvote.begin(), usersvote.end(), 0);

check(sum_of_votes >= 1, "Why do you want to vote with 0 tokens?");

//CHECK IF USER IS PORTFOLIO MANAGER
approvedaccs whitetbl(_self, community.value);
auto whiterow = whitetbl.find(voter.value);
check(whiterow != whitetbl.end(), "Not fund manager's account.");


check( 1005 >= sum_of_votes, "You have only 100 voting credits");

  portftb pollstbl(_self, community.value);
  auto pollsrow = pollstbl.find(pollkey);


  check( pollsrow != pollstbl.end(), "Poll does not exist.");


usersvote_tab userstbl(_self, voter.value);
  const auto& usersrow = userstbl.find(pollkey);

  
   if (usersrow == userstbl.end()) {

pollstbl.modify( pollsrow, voter, [&](auto& contract) {
                        contract.nrofvoters += 1;
                           });
       

for (size_t i = 0; i < usersvote.size(); ++i)
{

  pollstbl.modify( pollsrow, voter, [&](auto& contract) {
            contract.totalvote[i] += usersvote[i];
            contract.sumofallopt += usersvote[i];
            });

    
}
            userstbl.emplace( _self, [&](auto& contractt) {
            contractt.pollkey = pollkey;
            contractt.usersvote = usersvote;

            });

            
}


else {


for (size_t i = 0; i < usersvote.size(); ++i)
{

pollstbl.modify( pollsrow, voter, [&](auto& contract) {
            contract.totalvote[i] += usersvote[i] - usersrow->usersvote[i];
            contract.sumofallopt += usersvote[i] - usersrow->usersvote[i];
            });

}


 userstbl.modify( usersrow, voter, [&](auto& contractt) {
            contractt.usersvote = usersvote;
            });

            
}
        


 }





ACTION cetf::addportfel (name community, name creator, uint64_t pollkey) {
require_auth ( _self );

  portftb pollstbl(_self, community.value);
  auto pollsrow = pollstbl.find(pollkey);
  check( pollsrow == pollstbl.end(), "Choose different ID.");

  pollstbl.emplace(_self, [&](auto& item) {
    item.pollkey = pollkey;
    item.creator = creator;
    item.community = community;

  });


  }






//WHITELISTING FUND MANAGERS, SO ONLY THEY COULD SUBMIT VOTES (CETFVOTE ACTION)
ACTION cetf::whiteaccs (vector <name> accounts, name community) {

require_auth ( _self);

for (size_t i = 0; i < accounts.size(); ++i)

{

approvedaccs whitetbl(_self, community.value);
auto whiterow = whitetbl.find(accounts[i].value);
check(whiterow == whitetbl.end(), "This account has been whitelisted");

whitetbl.emplace(_self, [&](auto& item) {
    item.accounts = accounts[i];
  });

             nrofmngtab managtbl(_self, _self.value);
             const auto& managrow = managtbl.find(community.value);
                  
                   if (managrow == managtbl.end()) {

                  managtbl.emplace( _self, [&](auto& contract) {
                  contract.nrofmanagers = 1;
                  contract.community = community;
                  });    

                  } 

                  else { 

                  managtbl.modify( managrow, _self, [&](auto& contract) {
                   contract.nrofmanagers += 1;
                   });    
                   }

 }


}



ACTION cetf::delmanager(name community, name manager)

{
require_auth(_self);

approvedaccs whitetbl(_self, community.value);
auto whiterow = whitetbl.find(manager.value);
whitetbl.erase(whiterow);

}












