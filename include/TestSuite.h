#pragma once

#include <functional>
#include <vector>
#include <string>
#include <iostream>

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#endif

using namespace std;

void assert(bool condition, string message = "");

class AssertionFail : public std::exception
{
public:
    std::string s;
    AssertionFail(std::string ss) : s(ss) {}
    ~AssertionFail() throw() {}
    const char *what() const throw() { return s.c_str(); }
};

class TestCase
{
public:
    TestCase(string testName, double numberPoints, function<void()> to_execute) : testName(testName), numberPoints(numberPoints), to_execute(to_execute) {}
    string testName;
    string failure;
    double numberPoints;
    function<void()> to_execute;
    bool execute();
};

class TestSuite
{
public:
    TestSuite() { activateColorAnsiWindows(); }
    void require(string testName, double numberPoints, function<void()> to_execute);

    void showResults();

private:
    static void activateColorAnsiWindows();

    vector<TestCase> tests;
};