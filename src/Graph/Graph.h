#ifndef MY_GRAPH_H
#define MY_GRAPH_H

#include <algorithm>
#include <cstdint>
#include <unordered_set>
#include <vector>

#include <CommonDef.h>

namespace my {
    class Graph
    {
    private:
        std::vector<std::vector<int>> m_Vec;

    public:
        Graph() = default;
        explicit Graph(const size_t len);

    private:
        constexpr bool IsVertexValid(const int vertex) const noexcept { return m_Vec.size() < vertex && vertex >= 0; }

    public:
        void             AddVertex(const int vertex);
        void             RemoveVertex(const int vertex);
        void             RemoveEdge(const int srcVertex, const int destVertex);
        void             AddEdge(const int srcVertex, const int destVertex);
        void             Print() const;
        void             DFS(const int currentVertex, const int destVertex, std::unordered_set<int>& visited,
                             std::vector<int>& path, std::vector<int>& shortest_path) const;
        std::vector<int> GetShortestPath(const int srcVertex, const int destVertex) const;
        std::vector<int> GetNeighbours(const int vertex) const;
        bool             HasEdge(const int srcVertex, const int destVertex) const;
    };
} // namespace my

#endif // MY_GRAPH_H
