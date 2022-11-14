#ifndef A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D
#define A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D

#include <ql/interestrate.hpp>
#include <atlas/cashflows/leg.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {
    class FixedRateCoupon;
    class Redemption;

    // general porpuse class
    class FixedRateProduct : public Instrument {
       public:
        FixedRateProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                         const FixedRateLeg& leg = FixedRateLeg());

        const FixedRateLeg& constLeg() const { return leg_; };

        FixedRateLeg& leg() { return leg_; };

        void calculateNotionals(const std::vector<QuantLib::Date>& dates,
                                const QuantLib::InterestRate& rate);

        double notional() const { return notional_; };

        void rate(QuantLib::InterestRate rate);

        void rate(double rate);

        QuantLib::InterestRate rate() const { return rate_; };

       protected:
        void calculateFaceAmount();

        FixedRateLeg leg_;
        QuantLib::InterestRate rate_;
        double notional_ = 0.0;
    };
}  // namespace Atlas

#endif /* A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D */
