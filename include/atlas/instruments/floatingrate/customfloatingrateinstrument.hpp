#ifndef DFF76924_C230_47B5_BBF1_EC18E72E8DC4
#define DFF76924_C230_47B5_BBF1_EC18E72E8DC4

#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for custom floating rate instruments.
     * @ingroup FloatingRateInstruments
     */
    template <typename adouble>
    class CustomFloatingRateInstrument : public FloatingRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Custom Floating Rate Instrument object
         *
         * @param dates  dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         */
        CustomFloatingRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, adouble spread,
                                     const Context<RateIndex<adouble>>& forecastCurveContext)
        : FloatingRateInstrument<adouble>(dates.front(), dates.back(), 0, spread) {
            this->notional_    = std::reduce(redemptions.begin(), redemptions.end());
            double outstanding = this->notional_;
            for (size_t i = 0; i < redemptions.size(); i++) {
                Redemption<adouble> redemption(dates.at(i + 1), redemptions.at(i));
                this->leg().addRedemption(redemption);

                FloatingRateCoupon<adouble> coupon(dates.at(i), dates.at(i + 1), outstanding, spread, forecastCurveContext);
                this->leg().addCoupon(coupon);
                outstanding -= redemptions.at(i);
            }

            adouble disbursement = -this->notional_;
            this->disbursement(Cashflow<adouble>(dates.front(), disbursement));
        };
        ;
        /**
         * @brief Construct a new Custom Floating Rate Instrument object
         *
         * @param dates  dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        CustomFloatingRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, adouble spread,
                                     const Context<RateIndex<adouble>>& forecastCurveContext, const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : CustomFloatingRateInstrument(dates, redemptions, spread, forecastCurveContext) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement_.discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* DFF76924_C230_47B5_BBF1_EC18E72E8DC4 */
