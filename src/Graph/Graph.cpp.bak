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
            m_Vec.erase(m_Vec.begin(), std::remove(m_Vec.begin(), m_Vec.end(), vertex));
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

    /*
    std::vector<int> WeightedGraph::Djikstra(const int srcVertex, const int destVertex) const 
    {
        std::vector<int> fast(m_Vec.size(), std::numeric_limits<int>::max());
        fast[srcVertex] = 0;
        
        std::function<void(const int, std::vector<int>&)> recv = [&](const int srcVertex, std::vector<int>& fast)
        {
            if (fast[srcVertex] == INT_MAX)
                return;

            for (const auto[x, w]& : m_Vec[srcVertex])
            {
                int add = fast[srcVertex] + w;
                if (add < fast[x])
                {
                    fast[x] = add;
                    recv(x, fast);
                }
            }
        };

        recv(srcVertex, fast);
        return fast;
    }
    */
} // namespace my
