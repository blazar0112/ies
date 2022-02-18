#include "gtest/gtest.h"

namespace testing {

//! @brief This is used to reduce print out when test cases are successful. only print 2 line per suite.
//! @note this is used for console executing mostly, in qtcreator integrated Test remove such need
//! also qtcreator Test cannot recognize this flow.
//! @code
//!     ::testing::InitGoogleTest(&argc, argv);
//!
//!     auto &listeners = testing::UnitTest::GetInstance()->listeners();
//!     auto listener = listeners.Release(listeners.default_result_printer());
//!     listeners.Append(new testing::FailurePrinter(listener));
//!
//!     return RUN_ALL_TESTS();
class FailurePrinter : public TestEventListener
{
public:

    FailurePrinter(TestEventListener* listener)
    : l(listener)
    {}

    void OnTestProgramStart(const UnitTest& unit_test) override
    {
        l->OnTestProgramStart(unit_test);
    }

    void OnTestIterationStart(const UnitTest& unit_test, int iteration) override
    {
        l->OnTestIterationStart(unit_test, iteration);
    }

    void OnEnvironmentsSetUpStart(const UnitTest& unit_test) override
    {
        l->OnEnvironmentsSetUpStart(unit_test);
    }

    void OnEnvironmentsSetUpEnd(const UnitTest& unit_test) override
    {
        l->OnEnvironmentsSetUpEnd(unit_test);
    }

    void OnTestCaseStart(const TestCase& /*test_case*/) override
    {
        //l->OnTestCaseStart(test_case);
    }

    void OnTestStart(const TestInfo& test_info) override
    {
        if (test_info.result()->Failed())
        {
            l->OnTestStart(test_info);
        }
    }

    void OnTestPartResult(const TestPartResult& result) override
    {
        if (result.failed())
        {
            printf("\n");
            l->OnTestPartResult(result);
        }
    }

    void OnTestEnd(const TestInfo& test_info) override
    {
        if (test_info.result()->Failed())
        {
            l->OnTestEnd(test_info);
        }
    }

    void OnTestCaseEnd(const TestCase& test_case) override
    {
        l->OnTestCaseEnd(test_case);
    }

    void OnEnvironmentsTearDownStart(const UnitTest& unit_test) override
    {
        l->OnEnvironmentsTearDownStart(unit_test);
    }

    void OnEnvironmentsTearDownEnd(const UnitTest& unit_test) override
    {
        l->OnEnvironmentsTearDownEnd(unit_test);
    }

    void OnTestIterationEnd(const UnitTest& unit_test, int iteration) override
    {
        l->OnTestIterationEnd(unit_test, iteration);
    }

    void OnTestProgramEnd(const UnitTest& unit_test) override
    {
        l->OnTestProgramEnd(unit_test);
    }

protected:
    TestEventListener* l;
};

}

int
main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    auto &listeners = testing::UnitTest::GetInstance()->listeners();
    auto* listener = listeners.Release(listeners.default_result_printer());
    listeners.Append(new testing::FailurePrinter(listener));

    return RUN_ALL_TESTS();
}
