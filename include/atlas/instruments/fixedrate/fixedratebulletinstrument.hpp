#ifndef D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE
#define D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for fixed, single-legged, bullet instruments.
     *
     */
    template <typename adouble>
    class FixedRateBulletInstrument : public FixedRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Fixed Rate Bullet Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         */
        FixedRateBulletInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate<adouble>& rate)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, notional) {
            Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(freq);

            Date firstDate = Date();
            for (const auto& lastDate : schedule.dates()) {
                if (firstDate != Date()) {
                    FixedRateCoupon<adouble> coupon(firstDate, lastDate, notional, rate);
                    this->leg_.addCoupon(coupon);
                }
                firstDate = lastDate;
            }

            Redemption<adouble> redemption(schedule.endDate(), notional);
            this->leg_.addRedemption(redemption);

            this->disbursement_ = Cashflow<adouble>(startDate, -notional);
        };

        /**
         * @brief Construct a new Fixed Rate Bullet Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        FixedRateBulletInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate<adouble>& rate,
                                  const CurveContext& discountCurveContext)
        : FixedRateBulletInstrument(startDate, endDate, freq, notional, rate) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement_.discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE */