/**
 * @file	random.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2019/11/15
 * @copyright Copyright (C) 2019 jichan.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef __JCU_RANDOM_RANDOM_H__
#define __JCU_RANDOM_RANDOM_H__

#include <stdint.h>

#define JCU_RANDOM_NEXT_MASK ((1LL << 48) - 1)

namespace jcu {
    namespace random {

        class Random {
        public:
            virtual ~Random() {}

            /**
             * Generates the next pseudorandom number.
             *
             * The general contract of next is that it returns an int value and if the argument bits is between 1 and 32 (inclusive),
             * then that many low-order bits of the returned value will be (approximately) independently chosen bit values,
             * each of which is (approximately) equally likely to be 0 or 1.
             * The method next is implemented by class Random by atomically updating the seed to the algorithm.
             *
             * @param bits random bits
             * @return generated random value
             */
            virtual int next(int bits) = 0;

            /**
             * Sets the seed of this random number generator using a single 64bit seed.
             *
             * @param seed
             */
            virtual void setSeed(uint64_t seed) = 0;

            /**
             * Generates random bytes and places them into a user-supplied byte array.
             *
             * @param buf
             * @param length
             */
            virtual void nextBytes(void *buf, size_t length);

            /**
             * Returns the next pseudorandom, uniformly distributed boolean value from this random number generator's sequence.
             *
             * @return
             */
            virtual bool nextBoolean();

            /**
             * Returns the next pseudorandom, uniformly distributed float value between 0.0 and 1.0 from this random number generator's sequence.
             *
             * @return generated random value
             */
            virtual double nextDouble();

            /**
             * Returns the next pseudorandom, uniformly distributed float value between 0.0 and 1.0 from this random number generator's sequence.
             *
             * @return generated random value
             */
            virtual float nextFloat();

            /**
             * Returns a pseudorandom, uniformly distributed int value between 0 (inclusive) and the specified value (exclusive), drawn from this random number generator's sequence.
             *
             * @param bound the upper bound (exclusive). Must be positive.
             * @return generated random value
             */
            virtual int nextInt(int bound = 0);

            /**
             * Returns a pseudorandom, uniformly distributed int value between 0 (inclusive) and the specified value (exclusive), drawn from this random number generator's sequence.
             *
             * @param bound
             * @return the upper bound (exclusive). Must be positive.
             */
            virtual int32_t nextInt32(int bound = 0);

            /**
             * Returns a pseudorandom, uniformly distributed int value between 0 (inclusive) and the specified value (exclusive), drawn from this random number generator's sequence.
             *
             * @param bound
             * @return the upper bound (exclusive). Must be positive.
             */
            virtual int64_t nextInt64();
        };

    }
}

#endif //__JCU_RANDOM_RANDOM_H__
