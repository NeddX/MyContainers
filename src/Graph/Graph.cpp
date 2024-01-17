#include "Graph.h"

#include <algorithm>
#include <exception>
#include <iostream>

namespace my {
    Graph::Graph(const size_t len) : m_Vec(len, std::vector<int>())
    {
    }

    void Graph::AddVertex(const int vertex)
    {
        if (vertex < m_Vec.size())
            return;
        m_Vec.emplace_back(std::vector<int>());
    }

    void Graph::RemoveVertex(const int vertex)
    {
        if (IsVertexValid(vertex))
        {
            m_Vec[vertex].clear();
            for (auto& e : m_Vec)
                e.erase(e.begin(), std::remove(e.begin(), e.end(), vertex));
        }
        else
            throw std::out_of_range("Vertex does not exist.");
    }

    void Graph::RemoveEdge(const int srcVertex, const int destVertex)
    {
        if (IsVertexValid(srcVertex) && IsVertexValid(destVertex))
        {
            auto& e = m_Vec[srcVertex];
            e.erase(e.begin(), std::remove(e.begin(), e.end(), destVertex));
        }
        else
            throw std::out_of_range("Vertex does not exist.");
    }

    void Graph::AddEdge(const int srcVertex, const int destVertex)
    {
        if (IsVertexValid(srcVertex) && IsVertexValid(destVertex))
        {
            auto it = std::find(m_Vec[srcVertex].begin(), m_Vec[srcVertex].end(), destVertex);
            if (it == m_Vec[srcVertex].end())
                m_Vec[srcVertex].push_back(destVertex);
        }
        else
            throw std::invalid_argument("Vertex (or verticies) are(is) invalid.");
    }

    void Graph::Print() const
    {
        for (auto i = 0; i < m_Vec.size(); ++i)
        {
            std::cout << "[" << i << "]: { ";
            for (auto j = 0; j < m_Vec[i].size(); ++i)
                std::cout << m_Vec[i][j] << ", ";
            std::cout << " }" << std::endl;
        }
    }

    void Graph::DFS(const int currentVertex, const int destVertex, std::unordered_set<int>& visited,
                    std::vector<int>& path, std::vector<int>& shortest_path) const
    {
        visited.insert(currentVertex);
        path.push_back(currentVertex);
        if (currentVertex == destVertex)
        {
            if (shortest_path.empty() || shortest_path.size() > path.size())
                shortest_path = path;
        }
        else
        {
            for (const auto& e : m_Vec[currentVertex])
            {
                if (visited.find(e) == visited.end())
                    DFS(e, destVertex, visited, path, shortest_path);
            }
        }
        path.pop_back();
        visited.clear();
    }

    std::vector<int> Graph::GetShortestPath(const int srcVertex, const int destVertex) const
    {
        std::unordered_set<int> visited;
        std::vector<int>        path;
        std::vector<int>        shortest_path;
        DFS(srcVertex, destVertex, visited, path, shortest_path);
        return shortest_path;
    }

    std::vector<int> Graph::GetNeighbours(const int vertex) const
    {
        if (IsVertexValid(vertex))
            return m_Vec[vertex];
        else
            throw std::out_of_range("Provided vertex is non existent.");
    }

    bool Graph::HasEdge(const int srcVertex, const int destVertex) const
    {
        if (IsVertexValid(srcVertex) && IsVertexValid(destVertex))
        {
            auto& e = m_Vec[srcVertex];
            return std::find(e.begin(), e.end(), destVertex) != e.end();
        }
        else
            throw std::invalid_argument("Vertex (or verticies) are(is) invalid.");
    }
} // namespace my
