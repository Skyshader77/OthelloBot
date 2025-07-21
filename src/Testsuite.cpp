#include "TestSuite.h"

void assert(bool condition, string message)
{
    if (!condition)
    {
        throw AssertionFail(message);
    }
}

bool TestCase::execute()
{
    try
    {
        to_execute();
        return true;
    }
    catch (exception &e)
    {
        failure = e.what();
        return false;
    }
}

void TestSuite::require(string testName, double numberPoints, function<void()> to_execute)
{
    TestCase test(testName, numberPoints, to_execute);
    tests.push_back(test);
}

void TestSuite::activateColorAnsiWindows()
{
    // Needed to display colors in Windows sometimes, because it is not always activated by default
#if defined(_WIN32) && !defined(__CYGWIN__)
    HANDLE hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modeActuel = 0;
    GetConsoleMode(hConOut, &modeActuel);
    SetConsoleMode(hConOut, modeActuel | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void TestSuite::showResults()
{
    double totalPoints = 0;
    double obtainedPoints = 0;
    int i = 1;
    for (auto test : tests)
    {
        totalPoints += test.numberPoints;
        cout << "Test (" << i << " sur " << tests.size() << ") ";
        if (test.execute())
        {
            obtainedPoints += test.numberPoints;
            cout << "\033[1;32mOK: \033[0m\t" << test.testName << endl;
        }
        else
        {
            cout << "\033[1;31mechec:\033[0m\t" << test.testName << ": " << endl;
            cout << "\033[1;33m" << test.failure << "\033[0m" << endl;
        }
        ++i;
    }

    cout << "Point obtenu: " << obtainedPoints << "/" << totalPoints << endl;
    cout << "Note obtenue: " << int((obtainedPoints / totalPoints) * 100) << "%" << endl;
}