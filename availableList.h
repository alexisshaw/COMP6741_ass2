#include <vector>
#include <algorithm>


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/operator.hpp>
#include <boost/range/iterator_range.hpp>

#include "gen_types.h"


class availableList{
    typedef std::pair<genTypes::Graph::vertex_descriptor, genTypes::Graph::vertex_descriptor> vertexPair;
    typedef std::vector<vertexPair> availableListType;

    typedef std::vector<genTypes::Graph::vertex_descriptor> vertexVect;
    typedef std::vector<vertexVect> vetexVectVect;

    availableListType available;


    availableList(vertexVect X, vertexVectVect V){
        for (const auto& Vi1: V){
            for (const auto& Vi2: V) {
                for (const auto &v1: Vi1) {
                    for(const auto& v2: Vi2) {
                        if(v1 < v2) {
                            available.push_back(make_pair(v1, v2));
                        }
                    }
                }
            }
            for (const auto& v1: Vi1) {
                for (const auto &Xi: X) {
                    available.push_back(make_pair(v1, Xi));
                }
            }
        }

        for (const auto& Xi2: X) {
            for (const auto& Xi1: X) {
                if(Xi1 < Xi2) {
                    available.push_back(make_pair(v1, Xi));
                }
            }
        }
    }

    void updateAvailable(genTypes::Graph::vertex_descriptor u, genTypes::Graph::vertex_descriptor v, genTypes::Graph& g) {
        size_t n = num_vertices(g);

        boost::matrix<int> m_g(n, n);

        for (auto e: iterator_range(edges(g))) {
            m_g(source(e), target(e)) = 1;
            m_g(target(e), source(e)) = 1;
        }

        boost::matrix<int> m_g2(m_g);
        boost::axpy_prod(m_g, m_g, m_g2, false);

        boost::matrix<int> m_g3(m_g2);
        boost::axpy_prod(m_g2, m_g, m_g3, false);

        boost::matrix<int> m_g4(m_g3);
        boost::axpy_prod(m_g3, m_g, m_g4, false);

        availableListType::iterator newEnd = remove_if(available.begin(), available.end(), [&m_g4](vertexPair &p) {
            return m_g4(p.first, p.second) != 0;
        });

        available.resize(newEnd - available.begin());
    }

    size_t size(){
        return available.size();
    }

    vertexPair& operator[](size_type n){
        return available[n];
    }


};