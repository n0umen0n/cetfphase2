#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <cmath>
#include <eosio/singleton.hpp>
#include <numeric>

using namespace std;
using namespace eosio;

CONTRACT cetf : public contract
{
public:
    using contract::contract;

    //Actions related to claiming tokens
    ACTION getdiv(name user, name clmspecifier);
    ACTION getcetf(name user, name clmspecifier);
    ACTION issuetoken (name owner, asset touser, asset tosupply);
    ACTION transferdiv ( name owner, asset div );

    //Actions related to configuring the fund
    ACTION setsupplyy(asset quantity, asset maxsupply);
    ACTION delrebalon();
    ACTION adjtotvot(vector<uint64_t> totalvote, uint64_t pollkey, uint64_t sumofallopt);
    ACTION addportftb(vector<symbol> token, vector<uint64_t> totalvote, uint64_t pollkey);
    ACTION settotstkd(asset totstketfbx, asset totstkcetf);
    ACTION setdivper(uint64_t claimperiod);
    ACTION setdivperfrq(int64_t claimfreq);
    ACTION settotfeeamt(asset quantity);
    ACTION seteosetfadj(asset quantity);
    ACTION setrefundrate(float rate);
    ACTION seteosworth(double eosworth);
    ACTION modddtokens(vector<double> tokeninfund, vector<asset> minamount, vector<symbol> token);
    ACTION usertok(name from);
    ACTION deltoken(vector<symbol> token, vector<uint64_t> totalvote, name community, int64_t pollkey, symbol sym);
    ACTION deltokoncet(symbol sym);
    ACTION pause(bool ispaused);
    ACTION adjusttok(name contract, symbol token, int64_t decimals, double tokenpercnew);
    //Action related to rebalancing of the EOSETF
    ACTION rebalance(name user, uint64_t pollkey, name community);
    ACTION rebalancetwo(vector<symbol> answers);
    ACTION newratiof(symbol answers);
    //Actions related to staking
    ACTION stakecetf(name user, asset quantity, uint64_t id);
    ACTION unstakecetf(name user, vector<asset> quantity, vector<uint64_t> id, name clmspecifier);
    ACTION unstakeetf(name user, vector<asset> quantity, vector<uint64_t> id, name clmspecifier);
    //Actions related to eosio.token
    ACTION create(const name& issuer, const asset& maximum_supply);
    ACTION issue(const name& to, const asset& quantity, const string& memo);
    ACTION transfer(name from, name to, asset quantity, std::string memo);
    //Action that adds new token to the list of potential tokens to be included in EOSETF. Managers will have the option to vote in the token after that function.
    ACTION addtokuz(vector<double> tokeninfund, vector<double> tokenwortheos, vector<double> tokenperold,
        vector<double> tokenpercnew, vector<int64_t> decimals, vector<uint64_t> pairid, vector<string> strpairid, vector<symbol> token,
        vector<name> contract, vector<double> ratio, vector<asset> minamount, vector<uint64_t> totalvote, name community,
        uint64_t pollkey);
    ACTION cetfvote(vector<uint64_t> usersvote, uint64_t pollkey, name community, name voter);
    ACTION addportfel(name community, name creator, uint64_t pollkey);
    ACTION whiteaccs(vector<name> accounts, name community);
    ACTION delmanager(name community, name manager);

    //All on-notify actions
    [[eosio::on_notify("lptoken.defi::transfer")]] void saveetfstk(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("eosio.token::transfer")]] void captureeos(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("tethertether::transfer")]] void issueetfusdt(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("dappservices::transfer")]] void issueetfdapp(name from, name to, asset quantity, const string memo);
    [[eosio::on_notify("core.ogx::transfer")]] void issueetfogx(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("everipediaiq::transfer")]] void issueetfiq(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("vig111111111::transfer")]] void issueetfvig(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("token.defi::transfer")]] void issueetfbox(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("pizzatotoken::transfer")]] void issueetfpizza(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("token.newdex::transfer")]] void issueetfdex(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("eosiotptoken::transfer")]] void issueetftpt(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("boidcomtoken::transfer")]] void issueetfbd(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("prospectorsg::transfer")]] void issueetfdpg(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("xsovxsovxsov::transfer")]] void issueetfxv(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("thezeostoken::transfer")]] void issueetzs(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("dop.efi::transfer")]] void issueetfdop(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("btc.ptokens::transfer")]] void issueetfbtc(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("eth.ptokens::transfer")]] void issueetfeth(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("emanateoneos::transfer")]] void issueetfemt(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("chexchexchex::transfer")]] void issueetfchex(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("dmd.efi::transfer")]] void issueetfdmd(name from, name to, asset quantity, std::string memo);
    [[eosio::on_notify("bbsbbsbbseos::transfer")]] void issueetfbbs(name from, name to, asset quantity, std::string memo);

private:
    //Checks whether creation and redemption of EOSETF is on pause
    void pauseornot();

    //Triggering actions public actions of contracts
    void createetf(name from, asset touser, asset tosupply);
    void creatediv(name from, asset div);
    void send(name from, name to, asset quantity, std::string memo, name contract);
    void adjusttokk(name contract, symbol token, int64_t decimals, double tokenpercnew);
    void rebalancetwoin(vector<symbol> answers);

    //Part of rebalancing
    void votersnulli(name community, uint64_t pollkey);
    void newratio(symbol answers);

    //Refunding tokens for sending back EOSETF
    void refund_tokens_back(name from, name to, asset quantity, std::string memo);

    //Creating EOSETF
    void savetokens(name from, asset quantity, name to);
    void checkratuus(name from, asset quantity);

    //standard eosio.token
    void sub_balance(name owner, asset value);
    void add_balance(name owner, asset value, name ram_payer);
    void retire(asset quantity, std::string memo);

    // EOSIO-TOKEN TABLES
    TABLE account
    {
        asset balance;

        uint64_t primary_key() const { return balance.symbol.code().raw(); }
    };
    typedef eosio::multi_index<name("accounts"), account> accounts;

    TABLE currency_stats
    {
        asset supply;
        asset max_supply;
        name issuer;

        uint64_t primary_key() const { return supply.symbol.code().raw(); }
    };
    typedef eosio::multi_index<"stat"_n, currency_stats> stats;

    //EOSETF CREATION TABLES

    //Stores the amounts of tokens user sends in when creating EOSETF.
    TABLE usertokens
    {
        asset token;
        double ratio;

        uint64_t primary_key() const { return token.symbol.code().raw(); }
    };
    typedef eosio::multi_index<name("usertokens"), usertokens> useritokans;

    //Stores the fee percentage that gets charged while creating or puchasing EOSETF.
    TABLE refundrate
    {

        float rate;
    };
    typedef eosio::singleton<"refundrate"_n, refundrate> refundratetb;

    //Table stores the base token that is used to check the ratios when creation the EOSETF.
    TABLE basetok
    {

        symbol base;
    };
    typedef eosio::singleton<"basetok"_n, basetok> basetoktab;

    /*Stores all the information in regards to token that can be included in the fund. In order for managers to add a 
  token to the fund first it has to be added to this table. Contains key information of each token in the fund. */
    TABLE rebalon
    {

        double tokeninfund;

        double tokenwortheos;

        double tokenperold;

        double tokenpercnew;

        int64_t decimals;

        uint64_t pairid;

        string strpairid;

        symbol token;

        name contract;

        double ratio;

        asset minamount;

        string image;

        auto primary_key() const { return token.code().raw(); }
    };

    typedef eosio::multi_index<"rebalon"_n, rebalon> rebalontb;

    //Stores info on whether the creation and redemption of EOSETF is paused.
    TABLE pausetabla
    {

        bool ispaused;
    };
    typedef eosio::singleton<"pauze"_n, pausetabla> pausetab;

    //STAKING TABLES and FEE CLAIMING TABLES

    //Person total staked table, shows how much CETF has user staked in total. It is always reset to zero.
    TABLE prstotstkd
    {

        name staker;

        asset indtotstaked;

        auto primary_key() const { return staker.value; }
    };

    typedef eosio::multi_index<name("prstotstkd"), prstotstkd> perstotlskd;

    //Person total staked table, shows how much BOXAUJ has user staked in total. It is always reset to zero.
    TABLE prstotstkdbx
    {

        name staker;

        asset indtotstaked;

        auto primary_key() const { return staker.value; }
    };

    typedef eosio::multi_index<name("prstotstkdbx"), prstotstkdbx> perstotstbx;

    //Person staked table, shows on each row separately how much CETF has user staked.
    TABLE perzonstaked
    {

        uint64_t id;

        asset staked;

        time_point_sec staketime;

        uint64_t stakeperiod;

        auto primary_key() const { return id; }
    };

    typedef eosio::multi_index<name("persznstaked"), perzonstaked> perzonstkd;

    //Person staked table, shows on each row separately how much BOXAUJ has user staked.
    TABLE indstkdetf
    {

        uint64_t id;

        asset staked;

        time_point_sec staketime;

        uint64_t stakeperiod;

        auto primary_key() const { return id; }
    };

    typedef eosio::multi_index<name("indstkdetf"), indstkdetf> indstkdetftb;

    //Shows in which period user staked last time.
    TABLE claimtime
    {

        name user;

        uint64_t claimperiod;

        auto primary_key() const { return user.value; }
    };

    typedef eosio::multi_index<name("claimtime"), claimtime> claimtimetb;

    //Shows how much BOXAUJ and CETF is staked in total.
    TABLE totstk
    {

        asset totalstaked = { int64_t(00000), symbol("CETF", 4) };

        asset totstketf = { int64_t(0), symbol("BOXAUJ", 0) };
    };
    typedef eosio::singleton<"totstk"_n, totstk> totstk_def;

    //Shows current distribution period and when it started. Separate scope for CETF and EOSETF.
    TABLE divperiod
    {

        time_point_sec periodstart;

        uint64_t claimperiod;
    };

    typedef eosio::singleton<"divperiod"_n, divperiod> divperiod_def;

    //Shows how frequently the distribution period occurs.
    TABLE clmperfreq
    {

        int64_t periodfreq;
    };
    typedef eosio::singleton<"clmperfreq"_n, clmperfreq> divperiodfrq_def;

    /*Shows by how much should be the accumulated fees/profit adjusted. Amount is positive if
fees accumulated during the period were larger than the amount claimed by CETF holders.*/
    TABLE feesadjust
    {

        asset adjustcrtclm = { int64_t(00000), symbol("EOSETF", 4) };
        ;
    };
    typedef eosio::singleton<"feesadjust"_n, feesadjust> feesadjust_def;

    //Shows accumulated fees in EOSETF, available to be claimed.
    TABLE etffees
    {

        asset totalfees = { int64_t(00000), symbol("EOSETF", 4) };
    };
    typedef eosio::singleton<"etffees"_n, etffees> etffees_def;

    //REBALANCING TABLES

    //Table that stores what is the EOS worth of tokens that were sold during the rebalancing. Used to calculate how much of each token should be bought.
    TABLE eoscapt
    {

        asset capturedeos = { int64_t(00000), symbol("EOS", 4) };
    };
    typedef eosio::singleton<"eoscapt"_n, eoscapt> eoscaptura;

    //Table stores total EOS worth of tokens to be bought. That amount is required in order to calculate percentage from eos  Why not to use eoscapt table?
    TABLE totalbuy
    {

        double amountbuy;
    };
    typedef eosio::singleton<"totalbuy"_n, totalbuy> totalbuy_tab;

    //Table stores total EOS worth of each token to be bought.
    TABLE eosworthbuy
    {

        double eosworthbuy;

        symbol token;

        auto primary_key() const { return token.code().raw(); }
    };
    typedef eosio::multi_index<name("eosworthbuy"), eosworthbuy> eosworthbuytb;

    //Table stores number of tokens currently included in the fund.
    TABLE etfsize
    {

        int8_t size;
    };
    typedef eosio::singleton<"etfsize"_n, etfsize> etfsizetab;

    //Table stores value in EOS of all the tokens in the fund.
    TABLE totaleosworth
    {

        double eosworth;
    };
    typedef eosio::singleton<"totleosworth"_n, totaleosworth> totleostab;

    //Stores number of managers for the fund.
    TABLE mngtab
    {

        uint64_t nrofmanagers;

        name community;

        auto primary_key() const { return community.value; }
    };

    typedef eosio::multi_index<name("mngtab"), mngtab> nrofmngtab;

    //TABLES USED IN VOTING FOR PORTFOLIO ALLOCATION

    //Table for a poll. Poll that contains in the "vector <symbol> answers" the potential tokens to be included in the EOSETF.
    TABLE portfolios
    {

        uint64_t pollkey;

        name community;

        name creator;

        vector<uint64_t> totalvote;

        vector<symbol> answers;

        uint8_t nrofvoters = 0;

        uint64_t sumofallopt = 0;

        auto primary_key() const { return pollkey; }

        uint64_t by_secondary() const { return community.value; }
    };

    typedef eosio::multi_index<"portfolios"_n, portfolios,
        eosio::indexed_by<"bycomjus"_n, eosio::const_mem_fun<portfolios, uint64_t, &portfolios::by_secondary> > > portftb;

    TABLE usersvote
    {

        uint64_t pollkey;

        vector<uint64_t> usersvote;

        auto primary_key() const { return pollkey; }
    };
    typedef eosio::multi_index<name("usersvote"), usersvote> usersvote_tab;

    //Stores info on who is fund manager
    TABLE white
    {

        name accounts;

        auto primary_key() const { return accounts.value; }
    };

    typedef eosio::multi_index<name("approvedaccs"), white> approvedaccs;

    //TABLES OF OTHER CONTRACTS
    struct token {
        name contract;
        symbol symbol;
    };

    //Defibox table for the rebalancing action
    TABLE pair
    {

        uint64_t id;

        token token0;

        token token1;

        asset reserve0;

        asset reserve1;

        uint64_t liquidity_token;

        double price0_last;

        double price1_last;

        uint64_t price0_cumulative_last;

        uint64_t price1_cumulative_last;

        time_point_sec block_time_last;

        auto primary_key() const { return id; }
    };

    typedef eosio::multi_index<"pairs"_n, pair> pairs;
};