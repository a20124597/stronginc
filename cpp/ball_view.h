#ifndef CPP_BALLVIEW_H_
#define CPP_BALLVIEW_H_
#include "cpp/edge.h"
#include "cpp/vertex.h"
#include<unordered_set>
#include<unordered_map>
class Ball_View{
public:
    Ball_View();

    ~Ball_View();

    Ball_View(std::unordered_set<VertexID> &vertices,std::unordered_set<Edge>  &edges);

    std::unordered_set<VertexID>&  GetAllVerticesID();

    std::unordered_set<Edge>& GetAllEdges();

    std::unordered_set<VertexID>&  GetParentsID(const VertexID w);

    std::unordered_set<VertexID>&  GetChildrenID(const VertexID w);

    bool ExistEdge(VertexID src, VertexID dst);

private:
     std::unordered_set<VertexID> vertices_;
     std::unordered_set<Edge>  edges_;
     std::unordered_map<VertexID,std::unordered_set<VertexID>> childrens_;
     std::unordered_map<VertexID,std::unordered_set<VertexID>> parents_;

};
#endif //CPP_BALLVIEW_H_
using namespace std;