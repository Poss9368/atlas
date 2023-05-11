#include "../testsetup.hpp"
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/forwardrateforecaster.hpp>
#include <atlas/visitors/indexer.hpp>

TEST(ForwardForecaster, FloatingRateInstrument) {
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;
    Indexer<double> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    ForwardRateForecaster<double> forwardForecaster(marketData);
    forwardForecaster.visit(instrument);

    for (size_t i = 0; i < instrument.leg().coupons().size(); ++i) {
        double atlasAmount = instrument.leg().coupons()[i].amount();
        double qlAmount    = qlBond.cashflows()[i]->amount();
        EXPECT_NEAR(atlasAmount, qlAmount, 1e-6);
    }
}