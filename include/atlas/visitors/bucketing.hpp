/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef BE8C518D_A946_4AF5_9A44_F329F12150DE
#define BE8C518D_A946_4AF5_9A44_F329F12150DE

#include <atlas/data/buckets.hpp>
#include <atlas/instruments/instruments.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    template <class T>
    class PaymentAtMaturity : public ConstVisitor {
       public:
        PaymentAtMaturity(std::vector<Profile<T>>& profiles) : profiles_(profiles){};
        void visit(const DAP& dap) override {
            for (auto& profile : profiles_) {
                Date alive = profile.referenceDate();
                if (dap.isAlive(alive)) {
                    for (auto& bucket : *profile.buckets()) {
                        if (dap.leg().endDate() >= bucket.start_ && dap.leg().endDate() <= bucket.end_) {
                            double v = dap.faceAmount();
                            bucket.value_ += v;
                        }
                    }
                }
            }
        };
        std::vector<Profile<T>> profile() { return profiles_; }

       private:
        std::vector<Profile<T>>& profiles_;
    };

    class ConstantRollover : public ConstVisitor {
        void visit(const DAP& dap){};
    };

}  // namespace atlas

#endif /* BE8C518D_A946_4AF5_9A44_F329F12150DE */
