#include "../testsetup.hpp"
#include <ql/cashflows/couponpricer.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/instruments/bonds/floatingratebond.hpp>
#include <ql/instruments/makevanillaswap.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptioninstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/newvisitors/fixingvisitor.hpp>
#include <atlas/visitors/newvisitors/indexingvisitor.hpp>
#include <atlas/visitors/newvisitors/npvconstvisitor.hpp>
#include <atlas/visitors/npvcalculator.hpp>
using namespace Atlas;

TEST(NPVCalculator, FixedRateInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    Indexer<double> indexer;
    indexer.visit(instrument);
    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    EXPECT_NEAR(npvCalculator.results(), qlBond.NPV(), 1e-6);
}

TEST(NPVCalculator, EqualPaymentInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& store = vars.store;

    auto context   = store.curveContext("Zero");
    Date startDate = Date(1, Month::Aug, 2017);
    Date endDate   = Date(1, Month::Aug, 2024);
    Frequency freq = Frequency::Monthly;
    EqualPaymentInstrument<double> instrument(startDate, endDate, freq, vars.notional, vars.atlasRate, context, false,
                                              EqualPaymentInstrument<double>::BuildType::Precise);
    EqualPaymentInstrument<double> instrument2(startDate, endDate, freq, vars.notional, vars.atlasRate, context, false,
                                               EqualPaymentInstrument<double>::BuildType::Fast);

    Indexer<double> indexer;
    indexer.visit(instrument);
    indexer.visit(instrument2);
    MarketRequest request;
    indexer.setRequest(request);

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(instrument);
    double npv1 = npvCalculator.results();

    npvCalculator.clear();
    npvCalculator.visit(instrument2);
    double npv2 = npvCalculator.results();

    EXPECT_NEAR(npv1, npv2, 1e-8);
}

TEST(NPVCalculator, FixedRateInstrumentDual) {
    // Create a fixed rate instrument
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    Indexer<dual> indexer;
    indexer.visit(instrument);
    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<dual> model(request, vars.store);
    MarketData<dual> marketData = model.marketData();
    NPVCalculator<dual> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    // for (auto& cashflow : instrument.leg().coupons()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // for (auto& cashflow : instrument.leg().redemptions()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // std::cout << "\n" << std::endl;
    // for (auto& cashflow : qlBond.cashflows()) { std::cout << cashflow->date() << "||" << cashflow->amount() << std::endl; }
    EXPECT_NEAR(val(npvCalculator.results()), qlBond.NPV(), 1e-6);
}

TEST(NPVCalculator, FloatingRateInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;
    Indexer<double> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    // for (auto& cashflow : instrument.leg().coupons()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // for (auto& cashflow : instrument.leg().redemptions()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // std::cout << "\n" << std::endl;
    // for (auto& cashflow : qlBond.cashflows()) { std::cout << cashflow->date() << "||" << cashflow->amount() << std::endl; }

    EXPECT_NEAR(npvCalculator.results(), qlBond.NPV(), 1e-6);
}

TEST(NPVCalculator, FloatingRateInstrumentDual) {
    // Create a fixed rate instrument
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;

    Indexer<dual> indexer;
    indexer.visit(instrument);
    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<dual> model(request, vars.store);
    MarketData<dual> marketData = model.marketData();
    NPVCalculator<dual> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    EXPECT_NEAR(val(npvCalculator.results()), qlBond.NPV(), 1e-6);
}

TEST(NPVCalculator, FxForward) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    double fwdPrice = 825;
    Currency ccy    = CLP();
    double notional = 100'000;
    FxForward instrument(vars.startDate, vars.endDate, fwdPrice, CLP(), USD(), notional, FxForward<double>::BUY, vars.store.curveContext("CLP"));

    Indexer<double> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    //
    auto& clpCurve = vars.store.curveContext("CLP").object();
    auto& usdCurve = vars.store.curveContext("USD").object();
    double spot    = vars.store.exchange(CLP(), USD());
    double proyFx  = spot * usdCurve.discount(vars.endDate) / clpCurve.discount(vars.endDate);
    double df      = clpCurve.discount(vars.endDate);
    double evalNPV = (proyFx - fwdPrice) * df * notional / spot;
    EXPECT_NEAR(npvCalculator.results(), evalNPV, 1e-6);

    double notional2 = notional * fwdPrice;
    FxForward instrument2(vars.startDate, vars.endDate, 1 / fwdPrice, USD(), CLP(), notional2, FxForward<double>::SELL,
                          vars.store.curveContext("USD"));
    Indexer<double> indexer2;
    indexer2.visit(instrument2);
    MarketRequest request2;
    indexer2.setRequest(request2);
    SpotMarketDataModel<double> model2(request2, vars.store);
    MarketData<double> marketData2 = model2.marketData();
    NPVCalculator<double> npvCalculator2(marketData2);
    npvCalculator2.visit(instrument2);

    EXPECT_NEAR(npvCalculator2.results(), evalNPV, 1e-6);
}

TEST(NPVCalculator, FixFloatSwap) {
    TestSetup<double> vars;
    auto& store                = vars.store;
    Frequency paymentFrequency = Frequency::Quarterly;
    FixFloatSwap<double> swap(vars.startDate, vars.endDate, vars.notional, vars.atlasRate, vars.spread, paymentFrequency,
                              store.rateIndexContext("TEST"), FixFloatSwap<double>::Side::PAY);

    swap.firstLeg().discountCurveContext(store.curveContext("TEST"));
    swap.secondLeg().discountCurveContext(store.curveContext("TEST"));

    Indexer<double> indexer;
    indexer.visit(swap);

    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(swap);

    Calendar calendar      = QuantLib::NullCalendar();
    Schedule fixSchedule   = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(paymentFrequency).withCalendar(calendar);
    Schedule floatSchedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.indexFrequency).withCalendar(calendar);
    QuantLib::VanillaSwap qlSwap(QuantLib::VanillaSwap::Payer, vars.notional, fixSchedule, vars.qlRate, vars.dayCounter, floatSchedule, vars.qlIndex,
                                 0, vars.curveDayCounter);

    boost::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::DiscountingSwapEngine(vars.discountingTermStructure));
    qlSwap.setPricingEngine(engine);
    qlSwap.NPV();
    EXPECT_NEAR(npvCalculator.results(), qlSwap.NPV(), 1e-6);
}

// New visitors

TEST(NPVConstVisitor, FixedRateInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    NPVConstVisitor<double> npvVisitor(marketData);
    npvVisitor(instrument);

    EXPECT_NEAR(npvVisitor.getResults(), qlBond.NPV(), 1e-6);
}

TEST(NPVConstVisitor, EqualPaymentInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& store = vars.store;

    auto context   = store.curveContext("Zero");
    Date startDate = Date(1, Month::Aug, 2017);
    Date endDate   = Date(1, Month::Aug, 2024);
    Frequency freq = Frequency::Monthly;
    EqualPaymentInstrument<double> instrument(startDate, endDate, freq, vars.notional, vars.atlasRate, context, false,
                                              EqualPaymentInstrument<double>::BuildType::Precise);
    EqualPaymentInstrument<double> instrument2(startDate, endDate, freq, vars.notional, vars.atlasRate, context, false,
                                               EqualPaymentInstrument<double>::BuildType::Fast);

    IndexingVisitor<double> indexer;
    indexer(instrument);
    indexer(instrument2);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    NPVConstVisitor<double> npvVisitor(marketData);
    npvVisitor(instrument);

    double npv1 = npvVisitor.getResults();

    npvVisitor.reset();

    npvVisitor(instrument2);
    double npv2 = npvVisitor.getResults();

    EXPECT_NEAR(npv1, npv2, 1e-8);
}

TEST(NPVConstVisitor, FixedRateInstrumentDual) {
    // Create a fixed rate instrument
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFixBond;
    auto& qlBond     = *vars.qlFixBond;

    IndexingVisitor<dual> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<dual> model(request, vars.store);
    MarketData<dual> marketData = model.marketData();

    NPVConstVisitor<dual> npvVisitor(marketData);
    npvVisitor(instrument);

    // for (auto& cashflow : instrument.leg().coupons()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // for (auto& cashflow : instrument.leg().redemptions()) { std::cout << cashflow.paymentDate() << "||" << cashflow.amount() << std::endl; }
    // std::cout << "\n" << std::endl;
    // for (auto& cashflow : qlBond.cashflows()) { std::cout << cashflow->date() << "||" << cashflow->amount() << std::endl; }
    EXPECT_NEAR(val(npvVisitor.getResults()), qlBond.NPV(), 1e-6);
}

TEST(NPVConstVisitor, FloatingRateInstrument) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    FixingVisitor fixingVisitor(marketData);
    fixingVisitor(instrument);

    NPVConstVisitor<double> npvVisitor(marketData);
    npvVisitor(instrument);

    EXPECT_NEAR(npvVisitor.getResults(), qlBond.NPV(), 1e-6);
}

TEST(NPVConstVisitor, FloatingRateInstrumentDual) {
    // Create a fixed rate instrument
    TestSetup<dual> vars;
    auto& instrument = *vars.atlasFloatBond;
    auto& qlBond     = *vars.qlFloatBond;

    IndexingVisitor<dual> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<dual> model(request, vars.store);
    MarketData<dual> marketData = model.marketData();

    FixingVisitor fixingVisitor(marketData);
    fixingVisitor(instrument);

    NPVConstVisitor<dual> npvVisitor(marketData);
    npvVisitor(instrument);
    EXPECT_NEAR(val(npvVisitor.getResults()), qlBond.NPV(), 1e-6);
}

TEST(NPVConstVisitor, FxForward) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    double fwdPrice = 825;
    Currency ccy    = CLP();
    double notional = 100'000;
    FxForward instrument(vars.startDate, vars.endDate, fwdPrice, CLP(), USD(), notional, FxForward<double>::BUY, vars.store.curveContext("CLP"));

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    NPVConstVisitor<double> npvVisitor(marketData);
    npvVisitor(instrument);

    //
    auto& clpCurve = vars.store.curveContext("CLP").object();
    auto& usdCurve = vars.store.curveContext("USD").object();
    double spot    = vars.store.exchange(CLP(), USD());
    double proyFx  = spot * usdCurve.discount(vars.endDate) / clpCurve.discount(vars.endDate);
    double df      = clpCurve.discount(vars.endDate);
    double evalNPV = (proyFx - fwdPrice) * df * notional / spot;
    EXPECT_NEAR(npvVisitor.getResults(), evalNPV, 1e-6);

    double notional2 = notional * fwdPrice;
    FxForward instrument2(vars.startDate, vars.endDate, 1 / fwdPrice, USD(), CLP(), notional2, FxForward<double>::SELL,
                          vars.store.curveContext("USD"));

    IndexingVisitor<double> indexer2;
    indexer2(instrument2);
    MarketRequest request2 = indexer2.getResults();

    SpotMarketDataModel<double> model2(request2, vars.store);
    MarketData<double> marketData2 = model2.marketData();

    NPVConstVisitor<double> npvVisitor2(marketData2);
    npvVisitor2(instrument2);

    EXPECT_NEAR(npvVisitor2.getResults(), evalNPV, 1e-6);
}

TEST(NPVConstVisitor, FixFloatSwap) {
    TestSetup<double> vars;
    auto& store                = vars.store;
    Frequency paymentFrequency = Frequency::Quarterly;
    FixFloatSwap<double> swap(vars.startDate, vars.endDate, vars.notional, vars.atlasRate, vars.spread, paymentFrequency,
                              store.rateIndexContext("TEST"), FixFloatSwap<double>::Side::PAY);

    swap.firstLeg().discountCurveContext(store.curveContext("TEST"));
    swap.secondLeg().discountCurveContext(store.curveContext("TEST"));

    Indexer<double> indexer;
    indexer.visit(swap);

    MarketRequest request;
    indexer.setRequest(request);
    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();
    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(swap);

    Calendar calendar      = QuantLib::NullCalendar();
    Schedule fixSchedule   = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(paymentFrequency).withCalendar(calendar);
    Schedule floatSchedule = MakeSchedule().from(vars.startDate).to(vars.endDate).withFrequency(vars.indexFrequency).withCalendar(calendar);
    QuantLib::VanillaSwap qlSwap(QuantLib::VanillaSwap::Payer, vars.notional, fixSchedule, vars.qlRate, vars.dayCounter, floatSchedule, vars.qlIndex,
                                 0, vars.curveDayCounter);

    boost::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::DiscountingSwapEngine(vars.discountingTermStructure));
    qlSwap.setPricingEngine(engine);
    qlSwap.NPV();
    EXPECT_NEAR(npvCalculator.results(), qlSwap.NPV(), 1e-6);
}

TEST(NPVConstVisitor, FixedRateInstrumentApplyCcy) {
    // Create a fixed rate instrument
    TestSetup<double> vars;
    FixedRateBulletInstrument<double>& instrument = *vars.atlasFixBond;

    instrument.currency(CLP());
    for (auto& coupon : instrument.leg().coupons()) { coupon.applyCcy(true); }
    for (auto& redemption : instrument.leg().redemptions()) { redemption.applyCcy(true); }

    IndexingVisitor<double> indexer;
    indexer(instrument);
    MarketRequest request = indexer.getResults();

    SpotMarketDataModel<double> model(request, vars.store);
    MarketData<double> marketData = model.marketData();

    NPVConstVisitor<double> npvVisitor(marketData);
    npvVisitor(instrument);

    // ql
    double npv = 0;
    for (auto& coupon : instrument.leg().coupons()) {
        double fx = marketData.fxs.at(coupon.fxIdx());
        double df = marketData.dfs.at(coupon.dfIdx());
        npv += coupon.amount() * df / fx;
    }

    for (auto& redemption : instrument.leg().redemptions()) {
        double fx = marketData.fxs.at(redemption.fxIdx());
        double df = marketData.dfs.at(redemption.dfIdx());
        npv += redemption.amount() * df / fx;
    }

    EXPECT_NEAR(npvVisitor.getResults(), npv, 1e-6);

    instrument.currency(USD());
    indexer.reset();
    indexer(instrument);

    request = indexer.getResults();
    model.marketRequest(request);
    marketData = model.marketData();

    npvVisitor.reset();
    npvVisitor(instrument);

    npv = 0;
    for (auto& coupon : instrument.leg().coupons()) {
        double fx = marketData.fxs.at(coupon.fxIdx());
        double df = marketData.dfs.at(coupon.dfIdx());
        npv += coupon.amount() * df / fx;
    }

    for (auto& redemption : instrument.leg().redemptions()) {
        double fx = marketData.fxs.at(redemption.fxIdx());
        double df = marketData.dfs.at(redemption.dfIdx());
        npv += redemption.amount() * df / fx;
    }

    EXPECT_NEAR(npvVisitor.getResults(), npv, 1e-6);
}