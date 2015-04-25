//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ProfilerManager.h"

#include "PerfProfiler.h"

#include <algorithm>

namespace gdproto
{

    ProfilerManager& ProfilerManager::instance()
    {
        static ProfilerManager theInstance;
        return theInstance;
    }

    ProfilerManager::ProfilerManager()
        : m_perfProfilers(1, new PerfProfiler("main", 0))
    {
    }

    ProfilerManager::~ProfilerManager()
    {
        for (auto pp : m_perfProfilers)
        {
            delete pp;
        }
    }

    PerfProfiler* ProfilerManager::createPerfProfiler(const std::string& name)
    {
        auto pp = new PerfProfiler(name, m_perfProfilers.size());
        m_perfProfilers.push_back(pp);
        return pp;
    }

    PerfProfiler* ProfilerManager::getPerfProfiler(const std::string& name)
    {
        auto f = std::find_if(m_perfProfilers.begin(), m_perfProfilers.end(),
            [&name](const PerfProfiler* pp) -> bool {
            return pp->name() == name;
        });

        return f == m_perfProfilers.end() ? nullptr : *f;
    }

    PerfProfiler* ProfilerManager::getPerfProfiler(size_t id)
    {
        return id < m_perfProfilers.size() ? m_perfProfilers[id] : nullptr;
    }
}
