//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "PerfProfilerSection.h"

#include "ProfilerManager.h"
#include "PerfProfiler.h"

#include <cassert>

namespace gdproto
{
    PerfProfilerSection::PerfProfilerSection(const char* tag, size_t profilerId)
    {
        m_profiler = ProfilerManager::instance().getPerfProfiler(profilerId);
        assert(m_profiler);
        m_profiler->enter(tag);
    }

    PerfProfilerSection::~PerfProfilerSection()
    {
        m_profiler->leave();
    }
}
