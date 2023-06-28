#ifndef F01E58DD_C649_4B27_B416_BE9F8F03E002
#define F01E58DD_C649_4B27_B416_BE9F8F03E002

#include "config.hpp"
#include <atlas/cashflows/cashflow.hpp>
#include <atlas/cashflows/coupon.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <atlas/cashflows/legs/leg.hpp>
#include <atlas/fundation/indexable.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

namespace Aux {

    class PyCoupon : public Coupon<NumType> {
       public:
        using Coupon<NumType>::Coupon;

        DayCounter dayCounter() const override {
            PYBIND11_OVERRIDE_PURE(DayCounter,      /* Return type */
                                   Coupon<NumType>, /* Parent class */
                                   dayCounter,      /* Name of function in C++ (must match Python name) */
                                                    /* Argument(s) */
            );
        }

        double accruedPeriod(const Date& start, const Date& end) const override {
            PYBIND11_OVERRIDE_PURE(double,          /* Return type */
                                   Coupon<NumType>, /* Parent class */
                                   accruedPeriod,   /* Name of function in C++ (must match Python name) */
                                   start,           /* Argument(s) */
                                   end);
        }

        NumType accruedAmount(const Date& start, const Date& end) const override {
            PYBIND11_OVERRIDE_PURE(NumType,         /* Return type */
                                   Coupon<NumType>, /* Parent class */
                                   accruedAmount,   /* Name of function in C++ (must match Python name) */
                                   start,           /* Argument(s) */
                                   end);
        }
    };

}  // namespace Aux

void py_couponlegs(py::module& m) {
    py::class_<Indexable>(m, "Indexable")
        .def("dfIdx", py::overload_cast<>(&Indexable::dfIdx, py::const_))
        .def("dfIdx", py::overload_cast<size_t>(&Indexable::dfIdx))
        .def("fwdIdx", py::overload_cast<>(&Indexable::fwdIdx, py::const_))
        .def("fwdIdx", py::overload_cast<size_t>(&Indexable::fwdIdx))
        .def("fxIdx", py::overload_cast<>(&Indexable::fxIdx, py::const_))
        .def("fxIdx", py::overload_cast<size_t>(&Indexable::fxIdx));

    py::class_<Cashflow<NumType>, Indexable>(m, "Cashflow")
        .def(py::init<>())
        .def(py::init<const Context<YieldTermStructure<NumType>>&>())
        .def(py::init<const Date&, NumType>())
        .def(py::init<const Date&, NumType, const Context<YieldTermStructure<NumType>>&>())
        .def("paymentDate", &Cashflow<NumType>::paymentDate)
        .def("amount", py::overload_cast<>(&Cashflow<NumType>::amount, py::const_))
        .def("hasOcurred", &Cashflow<NumType>::hasOcurred)
        .def("discountCurveContext", &Cashflow<NumType>::discountCurveContext)
        .def("hasDiscountContext", &Cashflow<NumType>::hasDiscountContext)
        .def("discountContextIdx", &Cashflow<NumType>::discountContextIdx)
        .def("currency", &Cashflow<NumType>::currency)
        .def("currencyCode", &Cashflow<NumType>::currencyCode)
        .def("applyCcy", py::overload_cast<>(&Cashflow<NumType>::applyCcy, py::const_))
        .def("applyCcy", py::overload_cast<bool>(&Cashflow<NumType>::applyCcy));

    py::class_<Coupon<NumType>, Aux::PyCoupon, Cashflow<NumType>>(m, "Coupon")
        .def(py::init<const Date&, const Date&, double>())
        .def(py::init<const Date&, const Date&, double, const Context<YieldTermStructure<NumType>>&>())
        .def("notional", &Coupon<NumType>::notional)
        .def("startDate", &Coupon<NumType>::startDate)
        .def("endDate", &Coupon<NumType>::endDate)
        .def("dayCounter", &Coupon<NumType>::dayCounter)
        .def("accruedPeriod", &Coupon<NumType>::accruedPeriod)
        .def("accruedAmount", &Coupon<NumType>::accruedAmount);

    py::class_<FixedRateCoupon<NumType>, Coupon<NumType>>(m, "FixedRateCoupon")
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&>())
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, const Context<YieldTermStructure<NumType>>&>())
        .def("rate", py::overload_cast<>(&FixedRateCoupon<NumType>::rate, py::const_))
        .def("rate", py::overload_cast<const InterestRate<NumType>&>(&FixedRateCoupon<NumType>::rate))
        .def("accruedPeriod", &FixedRateCoupon<NumType>::accruedPeriod)
        .def("accruedAmount", &FixedRateCoupon<NumType>::accruedAmount)
        .def("dayCounter", &FixedRateCoupon<NumType>::dayCounter);

    py::class_<FloatingRateCoupon<NumType>, Coupon<NumType>>(m, "FloatingRateCoupon")
        .def(py::init<const Date&, const Date&, double, NumType, const Context<RateIndex<NumType>>&>())
        .def(py::init<const Date&, const Date&, double, NumType, const Context<RateIndex<NumType>>&, const Context<YieldTermStructure<NumType>>&>())
        .def("spread", py::overload_cast<>(&FloatingRateCoupon<NumType>::spread, py::const_))
        .def("spread", py::overload_cast<NumType>(&FloatingRateCoupon<NumType>::spread))
        .def("fixing", py::overload_cast<>(&FloatingRateCoupon<NumType>::fixing, py::const_))
        .def("fixing", py::overload_cast<NumType>(&FloatingRateCoupon<NumType>::fixing))
        .def("rateIndexContext", &FloatingRateCoupon<NumType>::rateIndexContext)
        .def("accruedAmount", &FloatingRateCoupon<NumType>::accruedAmount)
        .def("dayCounter", &FloatingRateCoupon<NumType>::dayCounter)
        .def("accruedPeriod", &FloatingRateCoupon<NumType>::accruedPeriod)
        .def("rateIndexContextIdx", &FloatingRateCoupon<NumType>::rateIndexContextIdx)
        .def("hasRateIndexContext", &FloatingRateCoupon<NumType>::hasRateIndexContext);

    // Legs
    py::class_<Leg<NumType>>(m, "Leg")
        .def(py::init<std::vector<Redemption<NumType>>&, bool>())
        .def("redemptions", py::overload_cast<>(&Leg<NumType>::redemptions, py::const_), py::return_value_policy::reference)
        .def("redemptions", py::overload_cast<>(&Leg<NumType>::redemptions), py::return_value_policy::reference)
        .def("addRedemption", &Leg<NumType>::addRedemption)
        .def("sortRedemptions", &Leg<NumType>::sortRedemptions);

    py::class_<FixedRateLeg<NumType>, Leg<NumType>>(m, "FixedRateLeg")
        .def(py::init<std::vector<FixedRateCoupon<NumType>>&, std::vector<Redemption<NumType>>&, bool>())
        .def("coupons", py::overload_cast<>(&FixedRateLeg<NumType>::coupons, py::const_), py::return_value_policy::reference)
        .def("coupons", py::overload_cast<>(&FixedRateLeg<NumType>::coupons), py::return_value_policy::reference)
        .def("addCoupon", &FixedRateLeg<NumType>::addCoupon)
        .def("sort", &FixedRateLeg<NumType>::sort)
        .def("discountCurveContext", &FixedRateLeg<NumType>::discountCurveContext);

    py::class_<FloatingRateLeg<NumType>, Leg<NumType>>(m, "FloatingRateLeg")
        .def(py::init<std::vector<FloatingRateCoupon<NumType>>&, std::vector<Redemption<NumType>>&, bool>())
        .def("coupons", py::overload_cast<>(&FloatingRateLeg<NumType>::coupons, py::const_), py::return_value_policy::reference)
        .def("coupons", py::overload_cast<>(&FloatingRateLeg<NumType>::coupons), py::return_value_policy::reference)
        .def("addCoupon", &FloatingRateLeg<NumType>::addCoupon)
        .def("sort", &FloatingRateLeg<NumType>::sort)
        .def("discountCurveContext", &FloatingRateLeg<NumType>::discountCurveContext)
        .def("rateIndexContext", &FloatingRateLeg<NumType>::rateIndexContext);
};

#endif /* F01E58DD_C649_4B27_B416_BE9F8F03E002 */
