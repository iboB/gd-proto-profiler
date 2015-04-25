//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// Profiling library configuration
#pragma once

namespace gdproto
{
    enum ProfilingConfig
    {
        Profiling_PerfNodePoolPageSize = 1024, // size of a node pool page in the perf profiler
        Profiling_PerfNodeMaxChildren = 32, // max number of children of a perf profiler node
    };
}
