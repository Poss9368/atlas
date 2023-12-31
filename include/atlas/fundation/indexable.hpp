#ifndef D41E427D_25EB_4AB1_89A1_415A80BBF4BF
#define D41E427D_25EB_4AB1_89A1_415A80BBF4BF

namespace Atlas {

    /**
     * @class Indexable
     * @brief The Indexable class represents an object that can be indexed in multiple dimensions. It has three index attributes: discount factor
     * index, forward index, and currency index. Inspired in A. Savine book "Modern computational finance: AAD and parallel simulation"
     */
    class Indexable {
       public:
        virtual ~Indexable() = default;

        /**
         * @brief Returns the index of the discount factor.
         * @return The index of the discount factor.
         */
        inline size_t dfIdx() const { return idx_[0]; }

        /**
         * @brief Sets the index of the discount factor.
         * @param idx The index of the discount factor to set.
         */
        inline void dfIdx(size_t idx) { idx_[0] = idx; }

        /**
         * @brief Returns the index of the forward rate.
         * @return The index of the forward rate.
         */
        inline size_t fwdIdx() const { return idx_[1]; }

        /**
         * @brief Sets the index of the forward rate.
         * @param idx The index of the forward rate to set.
         */
        inline void fwdIdx(size_t idx) { idx_[1] = idx; }

        /**
         * @brief Returns the index of the related price.
         * @return The index of the price.
         */
        inline size_t priceIdx() const { return idx_[2]; }

        /**
         * @brief Sets the index of the price.
         * @param idx The index of the price to set.
         */
        inline void priceIdx(size_t idx) { idx_[2] = idx; }

        /**
         * @brief Returns the index of the fx price.
         * @return The index of the fx price.
         */
        inline size_t fxIdx() const { return idx_[3]; }

        /**
         * @brief Sets the index of the fx price.
         * @param idx The index of the fx price to set.
         */
        inline void fxIdx(size_t idx) { idx_[3] = idx; }

        /**
         * @brief Returns true if the object is indexed.
         * @return True if the object is indexed.
         */
        inline bool isIndexed() const { return isIndexed_; }

        /**
         * @brief Sets the object as indexed.
         * @param isIndexed True if the object is indexed.
         */
        inline void isIndexed(bool isIndexed) { isIndexed_ = isIndexed; }

       private:
        bool isIndexed_ = false;
        size_t idx_[4]  = {SIZE_MAX, SIZE_MAX, SIZE_MAX, SIZE_MAX};
    };

}  // namespace Atlas

#endif /* D41E427D_25EB_4AB1_89A1_415A80BBF4BF */
