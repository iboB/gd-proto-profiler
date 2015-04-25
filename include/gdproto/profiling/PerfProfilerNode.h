//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// A node in the performance profiler
#pragma once

#include "Config.h"
#include "internal/high_res_clock.h"

#include <array>

namespace gdproto
{
    class PerfProfilerNode
    {
    public:
        PerfProfilerNode(const char* tag);

        const char* tag() const { return m_tag; }

        // node extra data
        // use it to associate some data of your own for the node
        intptr_t extraData() const { return m_extraData; }
        void setExtraData(intptr_t data) { m_extraData = data; }

        // total time spent in this node in nanoseconds
        uint64_t time() const { return m_nsTime; }

        // number of times this node has been entered
        unsigned timesEntered() const { return m_timesEntered; }

        // call to calculate additional profiling data for the node
        void calcAdditionalData();

        // the following properties are evaluated only after a call to calcAdditionalData

        // time spent in the node, that hasn't been calculated for its children
        uint64_t unprofiledTime() const { return m_unprofiledTime; }

        // mean time spent in this node
        uint64_t meanTime() const { return m_meanTime; }

        // merges the data of both nodes into this one
        // the tag of the other node is ignored
        void append(const PerfProfilerNode& other);

    private:
        friend class PerfProfiler;
        const char* m_tag;

        // node extra data
        // use it to associate some data of your own for the node
        intptr_t m_extraData = 0;

        void enter();
        void leave();

        void addChild(PerfProfilerNode* child);

        std::array<PerfProfilerNode*, Profiling_PerfNodeMaxChildren> m_children;
        size_t m_numChildren = 0; // since we can't use boost::static_vector, we'll write it like it's 1969

        PerfProfilerNode* m_parent = nullptr;

        high_res_clock::time_point m_enterTime;

        // performance profiling data
        uint64_t m_nsTime = 0;
        unsigned m_timesEntered = 0;

        // additional data
        uint64_t m_unprofiledTime = 0;
        uint64_t m_meanTime = 0;

    };
}
