// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <template/some.hpp>

#include <iostream>

int main()
{
    template::some s;

    if (s.some_method())
    {
        std::cout << "Hello template!" << std::endl;
    }

    return 0;
}
