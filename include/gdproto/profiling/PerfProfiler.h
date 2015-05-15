//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// Performance profiler class
// profiles the call stack in a single thread
#pragma once

#include <string>
#include <vector>
#include <array>

#include "PerfProfilerNode.h"

namespace gdproto
{
    class PerfProfiler
    {
    public:
        // call this to start the timer of the root profiler node
        // if you're profiling multiple subsystems in a single thread
        // you might want to call start and stop for each of those
        void start();
        void stop();

        // resets the profile data
        // it's usually a good idea to call this function at the start of your program
        // or after you want to start a new profiling session
        void reset();

        // enter a profiling section
        // create a profiling node, if needed
        void enter(const char* tag);

        // leave a profiling section
        // calling leave more times than enter will cause an assertion to fail (or a crash)
        void leave();

        const std::string& name() const { return m_name; }
        const size_t id() const { return m_id; }

        // you can inherit from this class to traverse all nodes
        class NodeTraverser
        {
        public:
            // called for each node
            // if it returns false, it breaks the traversal
            virtual bool traverse(PerfProfilerNode& node) = 0;

            // called when moving the traversal from a parent to a child
            virtual void down() {};

            // called when moving the traversal up a level
            virtual void up() {};
            
            virtual ~NodeTraverser() {};
        };

        // DFS traversal of all nodes
        // return true if some node has cut the traversal short
        bool traverseNodesDFS(NodeTraverser& traverser);
        
        // traverse from speficic node
        // return true if some node has cut the traversal short
        static bool traverseNodesDFS(PerfProfilerNode& node, PerfProfiler::NodeTraverser& traverser);

        template <typename Func>
        void simpleTraverseNodesDFS(Func traverser)
        {
            simpleTraverseNodesDFS(rootNode(), traverser);
        }

        template <typename Func>
        static void simpleTraverseNodesDFS(PerfProfilerNode& node, Func traverser)
        {
            traverser(node);

            for (size_t i = 0; i < node.m_numChildren; ++i)
            {
                simpleTraverseNodesDFS(*node.m_children[i], traverser);
            }
        }

        // gets the root node
        PerfProfilerNode& rootNode();
        const PerfProfilerNode& rootNode() const;

        // prepare the nodes for report gathering
        void prepareNodesForReport();

        // generates a "flat" report
        // all data gathered for nodes with the same tag will be present in a single node
        // in the returned vector
        // the order in which they appear in the vector is not determined
        std::vector<PerfProfilerNode> generateFlatReport();

    private:
        friend class ProfilerManager;
        PerfProfiler(const std::string& name, size_t id);
        ~PerfProfiler();

        const std::string m_name;
        const size_t m_id;

        // a pool of profiler nodes
        // it's not a good idea to allocate new memory during the profiling
        // that's why a pool of nodes is being used
        std::vector<std::vector<PerfProfilerNode>> m_nodePool;
        size_t m_curNodePoolPage = 0;
        
        // current node (for entering and leaving)
        PerfProfilerNode* m_curNode;

        PerfProfilerNode* getNewNode(const char* tag);

        // updates the current node to point to its child tag
        // creates the child if needed
        void updateCurNodeForEnter(const char* tag);
    };
}
