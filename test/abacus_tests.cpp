// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdint>
#include <ctime>

#include <abacus/protobuf/metrics.pb.h>
#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char** argv)
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    srand(static_cast<uint32_t>(time(0)));

    testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();

    // Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return result;
}
