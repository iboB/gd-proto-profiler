//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <iostream>
#include <string>
#include <thread>

#include <gdproto/profiling/Profile.h>
#include <gdproto/profiling/ProfilerManager.h>
#include <gdproto/profiling/PerfProfiler.h>

using namespace std;
using namespace std::chrono;
using namespace gdproto;

class ReportTraverser : public PerfProfiler::NodeTraverser
{
public:
    static const int indent_size = 2;
    void down() override
    {
        for (int i = 0; i < indent_size; ++i)
            indent.push_back(' ');
    }

    void up() override
    {
        for (int i = 0; i < indent_size; ++i)
            indent.pop_back();
    }

    bool traverse(PerfProfilerNode& node) override
    {
        cout << indent
            << node.tag() << ": "
            << double(node.time() / 1000) / 1000 << " ms "
            << "(u: " << double(node.unprofiledTime() / 1000) / 1000 << " ms), "
            << node.timesEntered() << " calls, "
            << double(node.meanTime() / 1000) / 1000 << " ms each"
            << endl;

        return true;
    }

    string indent;
};

struct Test
{
    void sleep(int ms)
    {
        GDPROTO_PROFILE_FUNC();
        this_thread::sleep_for(milliseconds(ms));
    }

    void sleep(float sec)
    {
        GDPROTO_PROFILE_FUNC();
        this_thread::sleep_for(microseconds(int(sec*1000000)));
    }
};

void bar(Test& t)
{
    GDPROTO_PROFILE_FUNC();
    t.sleep(10);
}

void foo(Test& t)
{
    GDPROTO_PROFILE_FUNC();
    t.sleep(0.1f);
    bar(t);
}

int main()
{
    auto profiler = ProfilerManager::instance().getPerfProfiler(0);

    Test test;

    profiler->start();

    for (size_t i = 0; i < 10; ++i)
    {
        GDPROTO_PROFILE_SCOPE("main loop");
        foo(test);
        bar(test);
        test.sleep(0.05f);
    }

    profiler->stop();

    profiler->prepareNodesForReport();
    
    ReportTraverser t;
    profiler->traverseNodesDFS(t);

    cout << endl << endl;

    auto flat = profiler->generateFlatReport();

    for (auto& node : flat)
    {
        t.traverse(node);
    }

    return 0;
}
