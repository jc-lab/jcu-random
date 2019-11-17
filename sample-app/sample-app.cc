#include <iostream>

#include <jcu-random/secure-random-factory.h>
#include <jcu-random/secure-random.h>
#include <jcu-random/well512-random.h>

int main(int argc, char *argv[]) {
    setbuf(stdout, 0);

    jcu::random::Well512Random wrandom;
    std::unique_ptr<jcu::random::SecureRandom> srandom = jcu::random::getSecureRandomFactory()->create();

    std::cout << "wrandom int : " << wrandom.nextInt() << std::endl;
    std::cout << "wrandom int : " << wrandom.nextInt() << std::endl;
    std::cout << "wrandom int : " << wrandom.nextInt() << std::endl;
    std::cout << "wrandom int : " << wrandom.nextInt() << std::endl;
    std::cout << "wrandom float : " << wrandom.nextFloat() << std::endl;
    std::cout << "wrandom float : " << wrandom.nextFloat() << std::endl;
    std::cout << "wrandom double : " << wrandom.nextDouble() << std::endl;
    std::cout << "wrandom double : " << wrandom.nextDouble() << std::endl;
    std::cout << "wrandom 1 : " << wrandom.next(1) << std::endl;
    std::cout << "wrandom 1 : " << wrandom.next(1) << std::endl;
    std::cout << "wrandom 2 : " << wrandom.next(2) << std::endl;
    std::cout << "wrandom 2 : " << wrandom.next(2) << std::endl;
    std::cout << "wrandom 3 : " << wrandom.next(3) << std::endl;
    std::cout << "wrandom 3 : " << wrandom.next(3) << std::endl;
    std::cout << "wrandom 4 : " << wrandom.next(4) << std::endl;
    std::cout << "wrandom 4 : " << wrandom.next(4) << std::endl;

    std::cout << std::endl;

    std::cout << "urandom int : " << srandom->nextInt() << std::endl;
    std::cout << "urandom int : " << srandom->nextInt() << std::endl;
    std::cout << "urandom int : " << srandom->nextInt() << std::endl;
    std::cout << "urandom int : " << srandom->nextInt() << std::endl;
    std::cout << "urandom float : " << srandom->nextFloat() << std::endl;
    std::cout << "urandom float : " << srandom->nextFloat() << std::endl;
    std::cout << "urandom double : " << srandom->nextDouble() << std::endl;
    std::cout << "urandom double : " << srandom->nextDouble() << std::endl;
    std::cout << "urandom 1 : " << srandom->next(1) << std::endl;
    std::cout << "urandom 1 : " << srandom->next(1) << std::endl;
    std::cout << "urandom 2 : " << srandom->next(2) << std::endl;
    std::cout << "urandom 2 : " << srandom->next(2) << std::endl;
    std::cout << "urandom 3 : " << srandom->next(3) << std::endl;
    std::cout << "urandom 3 : " << srandom->next(3) << std::endl;
    std::cout << "urandom 4 : " << srandom->next(4) << std::endl;
    std::cout << "urandom 4 : " << srandom->next(4) << std::endl;

    return 0;
}
