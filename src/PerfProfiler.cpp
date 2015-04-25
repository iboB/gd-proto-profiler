//
// gd-proto
// Copyright (c) 2015 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "PerfProfiler.h"
#include "Config.h"

#include <map>
#include <cassert>

namespace gdproto
{
    PerfProfiler::PerfProfiler(const std::string& name, size_t id)
        : m_name(name)
        , m_id(id)
        , m_nodePool(1)
    {
        m_nodePool[0].reserve(Profiling_PerfNodePoolPageSize);
        
        m_curNode = getNewNode("root");
    }

    PerfProfiler::~PerfProfiler()
    {
    }

    void PerfProfiler::start()
    {
        // don't call start/stop when you've entered some nodes
        // this could be fixed (making start/stop work as a sort of "pause"
        // for the performance profiling, but we won't support this feature for now
        assert(m_curNode->m_parent == nullptr);
        m_curNode->enter();
    }

    void PerfProfiler::stop()
    {
        // don't call start/stop when you've entered some nodes
        assert(m_curNode->m_parent == nullptr);
        m_curNode->leave();
    }

    void PerfProfiler::reset()
    {
        // now this is not STRICTLY an error
        // but the user just shouldn't reset the profiler
        // unless for every call of enter there has been a call of leave
        // getting here quite likely means a premature reset 
        // not all nodes that have been entered have been left
        assert(m_curNode->m_parent == nullptr);

        for (auto& page : m_nodePool)
        {
            page.clear();
        }

        m_curNodePoolPage = 0;

        m_curNode = getNewNode("root");
    }

    PerfProfilerNode* PerfProfiler::getNewNode(const char* tag)
    {
        if (m_nodePool[m_curNodePoolPage].size() == Profiling_PerfNodePoolPageSize)
        {
            // reached end of page, increment cur page index
            ++m_curNodePoolPage;

            // we don't remove pages, once we've created them
            // could be that there are free pages after this one
            // if not, create a new one
            if (m_curNodePoolPage == m_nodePool.size())
            {
                m_nodePool.resize(m_nodePool.size() + 1);
                m_nodePool.back().reserve(Profiling_PerfNodePoolPageSize);
            }
        }

        auto& curPage = m_nodePool[m_curNodePoolPage];
        curPage.emplace_back(tag);
        return &curPage.back();
    }

    void PerfProfiler::updateCurNodeForEnter(const char* tag)
    {
        for (size_t i = 0; i < m_curNode->m_numChildren; ++i)
        {
            if (m_curNode->m_children[i]->m_tag == tag)
            {
                m_curNode = m_curNode->m_children[i];
                return;
            }
        }

        // no existing child with this tag has been found
        auto child = getNewNode(tag);
        m_curNode->addChild(child);
        m_curNode = child;
    }

    void PerfProfiler::enter(const char* tag)
    {
        updateCurNodeForEnter(tag);
        m_curNode->enter();
    }

    void PerfProfiler::leave()
    {
        m_curNode->leave();

        // leaving the root node
        // a programming error
        assert(m_curNode->m_parent);
        m_curNode = m_curNode->m_parent;
    }

    PerfProfilerNode& PerfProfiler::rootNode()
    {
        return m_nodePool[0][0];
    }

    const PerfProfilerNode& PerfProfiler::rootNode() const
    {
        return m_nodePool[0][0];
    }

    bool PerfProfiler::traverseNodesDFS(PerfProfilerNode& node, PerfProfiler::NodeTraverser& traverser)
    {
        if (!traverser.traverse(node))
        {
            return false;
        }

        for (size_t i = 0; i < node.m_numChildren; ++i)
        {
            traverser.down();
            if (!traverseNodesDFS(*node.m_children[i], traverser))
            {
                return false;
            }

            traverser.up();
        }

        return true;
    }

    bool PerfProfiler::traverseNodesDFS(PerfProfiler::NodeTraverser& traverser)
    {
        return traverseNodesDFS(rootNode(), traverser);
    }

    void PerfProfiler::prepareNodesForReport()
    {
        simpleTraverseNodesDFS([](PerfProfilerNode& node) -> void
        {
            node.calcAdditionalData();
        });
    }

    struct FlatReportGenerator : public PerfProfiler::NodeTraverser
    {
        bool traverse(PerfProfilerNode& node) override
        {
            auto check = flatNodes.emplace(node.tag(), node.tag());

            check.first->second.append(node);

            return true;
        }

        std::map<const char*, PerfProfilerNode> flatNodes;
    };

    std::vector<PerfProfilerNode> PerfProfiler::generateFlatReport()
    {
        FlatReportGenerator gen;
        traverseNodesDFS(gen);

        std::vector<PerfProfilerNode> ret;
        ret.reserve(gen.flatNodes.size());
        for (const auto& n : gen.flatNodes)
        {
            ret.push_back(n.second);
        }

        return ret;
    }
}
