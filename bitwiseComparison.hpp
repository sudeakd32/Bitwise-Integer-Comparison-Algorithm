
#include <bitset>
#include <type_traits>
#include <cstdint>

/**
 * @brief Compares two integers without using conditional statements or direct comparison operators.
 * 
 * DO NOT use: `if`, `?:`, `switch`, `<`, `>`, `<=`, `>=`, `-`, `+`
 * You MAY use: `~`, `&`, `|`, `^`, `>>`, `<<`, `sizeof`
 * 
 * HINT:
 * - Consider the most significant differing bit.
 * - Think about how subtraction works in two’s complement.
 * - Use bitwise operations to extract and interpret the sign of the difference.
 * 
 * @tparam T Must be an integral type.
 * @param a First integer.
 * @param b Second integer.
 * @return -1 if a < b, 0 if a == b, 1 if a > b.
 */
template <typename T>
int compare_integral(T a, T b) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    if constexpr (std::is_signed<T>::value) {

        T intMinValue = T(1) << sizeof(T)*8-1;
        T isAEqualToMinValue = !(a ^ intMinValue);
        T isBEqualToMinValue = !(b ^ intMinValue);

        T resultMinCase =isAEqualToMinValue & ~isBEqualToMinValue & T(-1) | ~isAEqualToMinValue & isBEqualToMinValue & T(1);

        T msbOfA = a >> sizeof(T)*8-1;
        T msbOfB = b >> sizeof(T)*8-1;
        T signDifferentiate = msbOfA ^ msbOfB;

        T resultDiffSign = msbOfA << 1 | 1;

        T reverseB = ~b;
        T carry = 1;

        reverseB = reverseB ^ carry;
        carry = (reverseB & carry) << 1;
        reverseB = reverseB ^ carry;
        carry = (reverseB & carry) << 1;
        reverseB = reverseB ^ carry;
        carry = (reverseB & carry) << 1;
        reverseB = reverseB ^ carry;
        carry = (reverseB & carry) << 1;
        reverseB = reverseB ^ carry;
        carry = (reverseB & carry) << 1;
        reverseB = reverseB ^ carry;
        carry = (reverseB & carry) << 1;
        reverseB = reverseB ^ carry;

        T diff = a ^ reverseB;
        carry = (a & reverseB) << 1;

        diff = diff ^ carry;
        carry = (diff & carry) << 1;
        diff = diff ^ carry;
        carry = (diff & carry) << 1;
        diff = diff ^ carry;
        carry = (diff & carry) << 1;
        diff = diff ^ carry;
        carry = (diff & carry) << 1;
        diff = diff ^ carry;
        carry = (diff & carry) << 1;
        diff = diff ^ carry;
        carry = (diff & carry) << 1;
        diff = diff ^ carry;
        carry = (diff & carry) << 1;
        diff = diff ^ carry;

        T anyBitSet = diff;
        anyBitSet |= anyBitSet >> 1;
        anyBitSet |= anyBitSet >> 2;
        anyBitSet |= anyBitSet >> 4;
        T typeMask = T(1) << ((T(1) << 1) << 2)*2-1;

        //Using bitwise shifting instead of direct comparison
        anyBitSet |= (typeMask >> 8) & (anyBitSet >> 8);
        anyBitSet |= (typeMask >> 16) & (anyBitSet >> 16);
        T isZero = ~anyBitSet & 1;

        T signDiff = diff >> sizeof(T)*8-1;
        T resultForSameSign = (~isZero & ((signDiff & T(-1)) | (~signDiff & T(1))));

        T resultForNonMin = (signDifferentiate & resultDiffSign) | (~signDifferentiate & resultForSameSign);
        T result = resultMinCase | (~isAEqualToMinValue & ~isBEqualToMinValue & resultForNonMin);

        return result;

        // TODO: Implement signed comparison using only bitwise operations.
        return 0;  // Placeholder
    } else {
        T diff = a ^ b;
        T isEqual = ~(diff | -diff) >> sizeof(T)*8-1;

        //Isolate the most significant differing Bit
        T shifted = diff;
        shifted |= shifted >> 1;
        shifted |= shifted >> 2;
        shifted |= shifted >> 4;
        shifted |= shifted >> 8;
        shifted |= shifted >> 16;
        shifted |= shifted >> 32;
        shifted &= ~(shifted >> 1);

        //Determine which number is larger
        T AIsHigher = (a & shifted) & ~(b & shifted);
        T AGreaterThan = (AIsHigher | -AIsHigher) >> sizeof(T)*8-1 & 1;

        //Compute final result
        T resultPart = (AGreaterThan << 1) - 1;
        T result = (isEqual & 0) | (~isEqual & resultPart);

        return result;

        // TODO: Implement unsigned comparison using only bitwise operations.
        return 0;  // Placeholder
    }    
}


