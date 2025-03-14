// see bottom of page for license and references

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define LO 0
#define HI 100000000

typedef uint64_t u64;
typedef uint32_t u32;

u32 powmod(u32 base, u32 exponent, u32 m) {
  u32 power = base % m;
  u32 result = 1;
  while (exponent) {
    if (exponent & 1)
      result = (u64)result * power % m;

    power = (u64)power * power % m;
    exponent >>= 1;
  }

  return result;
}

// checks whether n is a strong probable prime to base
// assumes base is prime
bool is_strong_probable_prime(u32 base, u32 n) {
  if (n <= 2)
    return n == 2;

  if (!(n & 1))
    return false;

  // if n is prime and coprime to base, then by Fermat's little theorem, base^(n-1) % n = 1
  // if n is prime, then by the notes at the bottom, if x*x % n = 1, then x = 1 or x = n - 1
  //
  // what we're doing is we're repeatedly taking the square root of base^(n-1)
  // before repeatedly squaring that back into base^(n-1) to ensure both properties hold

  // since base is assumed to be prime,
  // only this check is needed to know whether n is coprime to base
  if (n % base == 0)
    return n == base;

  // power of 2 in prime factorization of n-1
  unsigned e = __builtin_ctzl(n - 1);

  // this is the furthest we can go with square roots while keeping it an integer
  u32 power = powmod(base, (n - 1) >> e, n);

  // if we start out with a 1, then it'll stay 1 when squared, so there's no need to continue
  if (power == 1)
    return true;

  // if not, then the only way to get both properties to hold is to find a -1
  // since we need to end with a 1 (property 1),
  // and the only valid way to get to a 1 is through a -1 (property 2)
  for (unsigned i=0; i < e; ++i) {
    if (power == n - 1)
      return true;

    power = (u64)power * power % n;
  }

  return false;
}

bool is_prime(u32 n) {
  // handle 0, 1, and 2
  if (n <= 2)
    return n == 2;

  // filter out evens
  if ((n & 1) == 0)
    return false;

  // filter out multiples of smaller primes
  for (u32 i=3; i <= 61; i += 2) {
    if (n == i)
      return true;

    if ((n % i) == 0)
      return false;
  }

  // checking the bases 2, 7, and 61 is enough for u32s
  return is_strong_probable_prime( 2, n)
      && is_strong_probable_prime( 7, n)
      && is_strong_probable_prime(61, n);
}

int main() {
  u32 count = LO <= HI && is_prime(HI);
  for (u32 i=LO; i < HI; ++i)
    count += is_prime(i);

  printf("%u\n", count);
}

/*
take x*x % n = 1
=> n divides x*x - 1
=> n divides (x-1)(x+1)

since n is prime,
by Euclid's lemma, n divides at least one of (x-1) or (x+1)

so x-1 = kn for some k
=> x % n = 1

or x+1 = kn for some k
=> x % n = n - 1
*/


/*
references

is_prime, is_strong_probable_prime:
  https://en.wikipedia.org/w/index.php?title=Strong_pseudoprime&oldid=1257753142
  https://en.wikipedia.org/w/index.php?title=Miller%E2%80%93Rabin_primality_test&oldid=1274372807
  https://en.wikipedia.org/w/index.php?title=Baillie%E2%80%93PSW_primality_test&oldid=1278177569

powmod:
  https://en.wikipedia.org/w/index.php?title=Modular_exponentiation&oldid=1215248118
  https://en.wikipedia.org/w/index.php?title=Exponentiation_by_squaring&oldid=1277105037

count_trailing_zeros: https://en.wikipedia.org/w/index.php?title=Find_first_set&oldid=1279131881
*/

// This work is licensed under CC BY-SA 4.0
// https://creativecommons.org/licenses/by-sa/4.0/
