// main.cpp

#include <gtest/gtest.h>
#include <iostream>

// Google Test test cases are created using a C++ preprocessor macro
// Here, a "test suite" name and a specific "test name" are provided.
TEST(module_name_leon, test_name_v1) {
    std::cout << "Hello world!" << std::endl;
    // Google Test will also provide macros for assertions.
    ASSERT_EQ(1+1, 2);
}

// Google Test can be run manually from the main() function
// or, it can be linked to the gtest_main library for an already
// set-up main() function primed to accept Google Test test cases.
// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);

//     return RUN_ALL_TESTS();
// }

// Build command: g++ main.cpp -lgtest