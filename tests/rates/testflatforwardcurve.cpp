
#include "../pch.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

using namespace Atlas;

TEST(FlatForwardCurve, BasicTest) {
    double rate  = 0.05;
    Date refDate = Date(1, Month::Jan, 2020);
    FlatForwardCurveStrategy<double> curve(refDate, rate);

    QuantLib::Settings::instance().evaluationDate() = refDate;
    DayCounter dayCounter                           = Actual360();
    QuantLib::FlatForward qlCurve(0, QuantLib::NullCalendar(), rate, dayCounter, Compounding::Simple, Frequency::Annual);

    Date date       = Date(1, Month::Jan, 2021);
    double expected = qlCurve.discount(date);
    double actual   = curve.discount(date);

    EXPECT_NEAR(actual, expected, 1e-9);

    EXPECT_THROW(curve.discount(Date(1, Month::Jan, 2019)), std::invalid_argument);
}
