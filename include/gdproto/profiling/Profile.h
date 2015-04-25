//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// Main profiling macros
#pragma once

#define GDPROTO_PERF_PROFILING_ON 1

#if GDPROTO_PERF_PROFILING_ON

#include "PerfProfilerSection.h"

// profiles a function
// use this macro at the beginning of a function
#   define GDPROTO_PROFILE_FUNC() ::gdproto::PerfProfilerSection __gdprotoProfilerSection(__FUNCTION__)
#   define GDPROTO_PROFILE_FUNC_FOR(ProfilerId) ::gdproto::PerfProfilerSection __gdprotoProfilerSection(__FUNCTION__, ProfilerId)

// profiles a scope with a label
#   define GDPROTO_PROFILE_SCOPE(Label) ::gdproto::PerfProfilerSection __gdprotoProfilerSection ## __LINE__(Label)
#   define GDPROTO_PROFILE_SCOPE_FOR(Label, ProfilerId) ::gdproto::PerfProfilerSection __gdprotoProfilerSection ## __LINE__(Label, ProfilerId)
#else
#
#endif