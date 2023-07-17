#ifndef C2B2159C_6F91_45A1_BF6E_0727152BBB0E
#define C2B2159C_6F91_45A1_BF6E_0727152BBB0E

#include <atlas/cashflows/legs/makeleg.hpp>
#include <atlas/instruments/instrument.hpp>
#include <atlas/instruments/mixins/twolegmixin.hpp>

namespace Atlas {
    /**
     * @defgroup Derivatives Derivatives
     * @ingroup Instruments
     */

    /**
     * @brief A class for fixed-float swap instruments.
     * @ingroup Derivatives
     */
    template <typename adouble = double>
    class FixFloatSwap : public Instrument<adouble>, public TwoLegMixin<FixedRateLeg<adouble>, FloatingRateLeg<adouble>> {
       public:
        FixFloatSwap(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble> rate, adouble spread, Frequency fixFreq,
                     const Context<RateIndex<adouble>>& floatIndex, Side side)
        : TwoLegMixin<FixedRateLeg<adouble>, FloatingRateLeg<adouble>>() {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->notional_  = notional;
            this->side_      = side;

            this->firstLeg_ = MakeLeg<FixedRateLeg, adouble>()
                                  .startDate(startDate)
                                  .endDate(endDate)
                                  .notional(notional)
                                  .rate(rate)
                                  .paymentFrequency(fixFreq)
                                  .createRedemptions(false)
                                  .build();

            this->secondLeg_ = MakeLeg<FloatingRateLeg, adouble>()
                                   .startDate(startDate)
                                   .endDate(endDate)
                                   .notional(notional)
                                   .rateIndexContext(&floatIndex)
                                   .spread(spread)
                                   .createRedemptions(false)
                                   .build();

            fixFreq_   = fixFreq;
            floatFreq_ = floatIndex.object().fixingFrequency();
            rate_      = rate;
            spread_    = spread;
        };

        FixFloatSwap(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble> rate, adouble spread, Frequency fixFreq,
                     const Context<RateIndex<adouble>>& floatIndex, Side side, const Context<YieldTermStructure<adouble>>& discountCurve)
        : FixFloatSwap(startDate, endDate, notional, rate, spread, fixFreq, floatIndex, side) {
            this->firstLeg().discountCurveContext(discountCurve);
            this->secondLeg().discountCurveContext(discountCurve);
        }

        void accept(Visitor<adouble>& visitor) override { visitor.visit(*this); }

        void accept(ConstVisitor<adouble>& visitor) const override { visitor.visit(*this); }

        inline Frequency fixPaymentFrequency() const { return fixFreq_; }

        inline Frequency floatPaymentFrequency() const { return floatFreq_; }

        inline InterestRate<adouble> rate() const { return rate_; }

        inline adouble spread() const { return spread_; }

       private:
        Frequency fixFreq_;
        Frequency floatFreq_;
        InterestRate<adouble> rate_;
        adouble spread_;
    };

}  // namespace Atlas

#endif /* C2B2159C_6F91_45A1_BF6E_0727152BBB0E */
