//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// Performance profiling entry
// a RAII class that generates profiling information
// for the code between its construction and destruction
#pragma once

namespace gdproto
{

    class PerfProfiler;

    class PerfProfilerSection
    {
    public:
        // note that the tag, although logically a string, 
        // is never compared by its value, but always by address
        PerfProfilerSection(const char* tag, size_t profilerId = 0);
        ~PerfProfilerSection();

    private:
        PerfProfiler* m_profiler;
    };

}
