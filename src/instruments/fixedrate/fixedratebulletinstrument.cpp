#include <ql/time/schedule.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FixedRateBulletInstrument::FixedRateBulletInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional,
                                                         const InterestRate& rate, std::shared_ptr<CurveContext> discountCurveContext)
    : FixedRateInstrument(startDate, endDate, rate, notional) {
        Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(freq);

        Date firstDate = Date();
        for (const auto& lastDate : schedule.dates()) {
            if (firstDate != Date()) {
                FixedRateCoupon coupon(firstDate, lastDate, notional, rate, discountCurveContext);
                leg_.addCoupon(coupon);
            }
            firstDate = lastDate;
        }

        Redemption redemption(schedule.endDate(), notional, discountCurveContext);
        leg_.addRedemption(redemption);

        disbursement_ = Cashflow(startDate, -notional, discountCurveContext);
    }

}  // namespace Atlas