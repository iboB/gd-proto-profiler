//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// Profiler manager class
// Singleton
#pragma once

#include <string>
#include <vector>

namespace gdproto
{
    class PerfProfiler;

    class ProfilerManager
    {
    public:
        static ProfilerManager& instance();

        // creates a performance profiler by a (non-unique) name
        PerfProfiler* createPerfProfiler(const std::string& name);

        // gets a performance profiler by name
        // returns nullptr if it doesn't exist
        // it does a linear search in a vector to get it, so it's not a preferred way of getting it
        // there's nothing stopping you from having two profilers with the same name
        // this will return the first match
        PerfProfiler* getPerfProfiler(const std::string& name);

        // gets a performance profiler by id
        // returns nullptr if it doesn't exist
        PerfProfiler* getPerfProfiler(size_t id);

    private:
        ProfilerManager();
        ~ProfilerManager();

        std::vector<PerfProfiler*> m_perfProfilers;
    };
}
