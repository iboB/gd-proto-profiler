//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "PerfProfilerNode.h"

#include <cassert>

namespace gdproto
{
    PerfProfilerNode::PerfProfilerNode(const char* tag)
        : m_tag(tag)
    {
    }

    void PerfProfilerNode::addChild(PerfProfilerNode* child)
    {
        child->m_parent = this;

        if (m_numChildren >= Profiling_PerfNodeMaxChildren)
        {
            // max children has been reached
            // update the config file 
            assert(false);

            // hacky way to try to save the crash as much as we can
            // we put the current child as last
            // thus if there are millions of calls to it, it won't eat up the memory
            // by calling getNewChild constantly
            --m_numChildren;
        }
        
        m_children[m_numChildren++] = child;
    }

    void PerfProfilerNode::enter()
    {
        ++m_timesEntered;
        m_enterTime = high_res_clock::now();
    }

    void PerfProfilerNode::leave()
    {
        auto duration = high_res_clock::now() - m_enterTime;

        m_nsTime += std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    }

    void PerfProfilerNode::calcAdditionalData()
    {
        uint64_t childrenTime = 0;
        for (size_t i = 0; i < m_numChildren; ++i)
        {
            childrenTime += m_children[i]->m_nsTime;
        }

        m_unprofiledTime = m_nsTime - childrenTime;

        m_meanTime = m_nsTime / m_timesEntered;
    }

    void PerfProfilerNode::append(const PerfProfilerNode& other)
    {
        m_nsTime += other.m_nsTime;
        m_timesEntered += other.m_timesEntered;
        m_unprofiledTime += other.m_unprofiledTime;

        // cannot merge in other meaningful way
        m_meanTime = m_nsTime / m_timesEntered;
    }
}
