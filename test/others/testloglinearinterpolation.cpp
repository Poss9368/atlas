#include <gtest/gtest.h>
#include <atlas/others/interpolations/loglinearinterpolation.hpp>
#include <cmath>

using namespace Atlas;

TEST(LogLinearInterpolation, BasicTest) {
    std::vector<double> x_ = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> y_ = {1.0, 4.0, 9.0, 16.0};

    double x        = 2.5;
    double expected = std::sqrt(4.0 * 9.0);

    LogLinearInterpolator<double> interpol(x_, y_);
    double actual = interpol(x);

    EXPECT_NEAR(actual, expected, 1e-9);
}

TEST(LogLinearInterpolation, SinglePoint) {
    std::vector<double> x_ = {1.0};
    std::vector<double> y_ = {2.0};
    double x               = 1.0;

    LogLinearInterpolator<double> interpol(x_, y_);

    EXPECT_THROW(interpol(x), std::out_of_range);
}

TEST(LogLinearInterpolation, ExtrapolationEnabled) {
    std::vector<double> x_ = {1, 2, 3};
    std::vector<double> y_ = {10, 100, 1000};

    LogLinearInterpolator<double> interpol(x_, y_, true);

    // Test extrapolation on the left side
    double left_extrapolation = 10 * std::exp((0 - 1) * (std::log(100) - std::log(10)) / (2 - 1));
    EXPECT_DOUBLE_EQ(left_extrapolation, interpol(0.0));

    // Test extrapolation on the right side
    double right_extrapolation = 1000 * std::exp((4 - 3) * (std::log(1000) - std::log(100)) / (3 - 2));
    EXPECT_DOUBLE_EQ(right_extrapolation, interpol(4.0));
}

TEST(LogLinearInterpolation, ExtrapolationDisabled) {
    std::vector<double> x_ = {1, 2, 3};
    std::vector<double> y_ = {10, 100, 1000};

    LogLinearInterpolator<double> interpol(x_, y_, false);

    // Test extrapolation on the left side

    EXPECT_THROW(interpol(0.0), std::out_of_range);

    // Test extrapolation on the right side
    EXPECT_THROW(interpol(4.0), std::out_of_range);
}