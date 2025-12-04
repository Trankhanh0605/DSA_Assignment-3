#include "KnowledgeGraph.h"

// =============================================================================
// Class Edge Implementation
// =============================================================================

template <class T>
Edge<T>::Edge(VertexNode<T> *from, VertexNode<T> *to, float weight)
{
    this->from = from;
    this->to = to;
    this->weight = weight;
}

// TODO: Implement other methods of Edge:
template<class T>
bool Edge<T>:: equals(Edge<T> *edge) {
return (this->from==edge->from && this->to==edge->to);
}

template<class T>
bool Edge<T>:: edgeEQ(Edge<T> *&edge1, Edge<T> *&edge2) {
    if (edge1->equals(edge2)) return true;
    else return false;
}

template <class T>
string Edge<T>::toString()
{
    string fromStr, toStr;
    
    // Kiểm tra vertex2str có null không
    if (from->vertex2str != nullptr) {
        fromStr = from->vertex2str(from->getVertex());
    } else {
        // Nếu null, chuyển đổi mặc định
        ostringstream oss;
        oss << from->getVertex();
        fromStr = oss.str();
    }
    
    if (to->vertex2str != nullptr) {
        toStr = to->vertex2str(to->getVertex());
    } else {
        ostringstream oss;
        oss << to->getVertex();
        toStr = oss.str();
    }
    
    return "E(" + fromStr + "," + toStr + "," + to_string(static_cast<int>(weight)) + ")";
}




// =============================================================================
// Class VertexNode Implementation
// =============================================================================

template <class T>
VertexNode<T>::VertexNode(T vertex, bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
{
    this->vertex = vertex;
    this->vertexEQ = vertexEQ;
    this->vertex2str = vertex2str;
    this->inDegree_ = 0;
    this->outDegree_ = 0;
}

template <class T>
VertexNode<T>::~VertexNode()
{
    // Giải phóng tất cả các edge trong adList
    for (Edge<T>* edge : adList) {
        delete edge;
    }
    adList.clear();
}

template<class T>
T & VertexNode<T>:: getVertex() {
    return vertex;
}

template <class T>
void VertexNode<T>::connect(VertexNode<T> *to, float weight)
{
    // TODO: Connect this vertex to the 'to' vertex
    Edge<T>* existingEdge = getEdge(to);
    if (existingEdge != nullptr) {
        existingEdge->weight = weight;
    } else {
        Edge<T> *newEdge = new Edge(this, to, weight);
        adList.push_back(newEdge);
        this->outDegree_++;
        to->inDegree_++;
    }
}

template<class T>
Edge<T> * VertexNode<T>:: getEdge(VertexNode<T> *to) {
    for (Edge<T>* cur : adList) {
        if (cur->to==to) return cur;
    }
    return nullptr;
}

template<class T>
bool VertexNode<T>:: equals(VertexNode<T> *node) {
    if (node == nullptr) return false;
    if (!vertexEQ) {
        if (this->getVertex()==node->getVertex()) return true;
        else return false;
    }
    else return vertexEQ(this->getVertex(), node->getVertex());
}

template<class T>
void VertexNode<T>:: removeTo(VertexNode<T> *to) {
    // Duyệt qua danh sách cạnh đi ra
    for (auto it = adList.begin(); it != adList.end(); ++it) {
        Edge<T>* edge = *it;
        if (edge->to == to) {
            outDegree_--;
            to->inDegree_--;
            delete edge;
            adList.erase(it);
            return;
        }
    }
}

template<class T>
int VertexNode<T>:: inDegree()  {
    return inDegree_;
}

template<class T>
int VertexNode<T>:: outDegree() {
    return outDegree_;
}

template<class T>
string VertexNode<T>::toString()
{
    string vertexStr;
    
    if (vertex2str != nullptr) {
        vertexStr = vertex2str(getVertex());
    } else {
        // Chuyển đổi mặc định
        ostringstream oss;
        oss << getVertex();
        vertexStr = oss.str();
    }
    
    return "V(" + vertexStr +
           ", in: " + to_string(inDegree()) +
           ", out: " + to_string(outDegree()) + ")";
}



// =============================================================================
// Class DGraphModel Implementation
// =============================================================================

template <class T>
DGraphModel<T>::DGraphModel(bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
{
    this->vertexEQ = vertexEQ;
    this->vertex2str = vertex2str;
}

template <class T>
DGraphModel<T>::~DGraphModel()
{
    // TODO: Clear all vertices and edges to avoid memory leaks
    clear();
}

// TODO: Implement other methods of DGraphModel:
template<class T>
VertexNode<T> * DGraphModel<T>:: getVertexNode(T &vertex) {
    for (VertexNode<T>* cur : nodeList) {
        if (vertexEQ != nullptr) {
            if (vertexEQ(cur->getVertex(), vertex)) {
                return cur;
            }
        } else {
            if (cur->getVertex() == vertex) {
                return cur;
            }
        }
    }
    return nullptr;
}

template<class T>
string DGraphModel<T>:: vertex2Str(VertexNode<T> &node) {
    T vertex = node.getVertex();
    return getVertexString(vertex);
}

template<class T>
string DGraphModel<T>::edge2Str(Edge<T> &edge) {
    return edge.toString();
}

template <class T>
void DGraphModel<T>::add(T vertex)
{
    // TODO: Add a new vertex to the graph
    if (contains(vertex)) return;
    VertexNode<T>* newNode = new VertexNode<T>(vertex, vertexEQ, vertex2str);
    nodeList.push_back(newNode);
}

template<class T>
bool DGraphModel<T>:: contains(T vertex) {
   return getVertexNode(vertex) != nullptr;
}

template<class T>
float DGraphModel<T>::weight(T from, T to) {
    VertexNode<T>* fromNode = getVertexNode(from);
    if (!fromNode) {
        throw VertexNotFoundException("Vertex (" + getVertexString(from) + "): is not found");
    }
    VertexNode<T>* toNode = getVertexNode(to);
    if (!toNode) {
        throw VertexNotFoundException("Vertex (" + getVertexString(to) + "): is not found");
    }
    Edge<T>* edge = fromNode->getEdge(toNode);
    if (!edge) {
        throw EdgeNotFoundException("Edge E(" + getVertexString(from) + "," + getVertexString(to)+ "): is not found");
    }
    return edge->weight;
}

template<class T>
vector<T> DGraphModel<T>::getOutwardEdges(T from) {
    VertexNode<T>* fromNode = getVertexNode(from);
    if (!fromNode) {
   throw VertexNotFoundException("Vertex (" + getVertexString(from) + "): is not found");
    }
    vector<T> result;
    for (Edge<T>* edge : fromNode->adList) {
        result.push_back(edge->to->getVertex());
    }
    return result;
}

template <class T>
void DGraphModel<T>::connect(T from, T to, float weight)
{
    // TODO: Connect two vertices 'from' and 'to'
    VertexNode<T>* fromNode = getVertexNode(from);
    if (!fromNode) throw VertexNotFoundException("Vertex (" + getVertexString(from) + "): is not found");
    VertexNode<T>* toNode = getVertexNode(to);
    if (!toNode) throw VertexNotFoundException("Vertex (" + getVertexString(to) + "): is not found");
    fromNode->connect(toNode,weight);
}

template<class T>
void DGraphModel<T>:: disconnect(T from, T to) {
    VertexNode<T>* fromNode = getVertexNode(from);
    if (!fromNode)   throw VertexNotFoundException("Vertex (" + getVertexString(from) + "): is not found");
    VertexNode<T>* toNode = getVertexNode(to);
    if (!toNode)  throw VertexNotFoundException("Vertex (" + getVertexString(to) + "): is not found");
    fromNode->removeTo(toNode);
}

template<class T>
bool DGraphModel<T>:: connected(T from, T to) {
    VertexNode<T>* fromNode = getVertexNode(from);
    if (!fromNode) throw VertexNotFoundException("Vertex (" + getVertexString(from) + "): is not found");
    VertexNode<T>* toNode = getVertexNode(to);
    if (!toNode) throw VertexNotFoundException("Vertex (" + getVertexString(to) + "): is not found");
    Edge<T>* edge = fromNode->getEdge(toNode);
    return (edge != nullptr);
}

template<class T>
int DGraphModel<T>:: size() {
    return nodeList.size();
}

template<class T>
bool DGraphModel<T>:: empty() {
    return (size()==0);
}

template <class T>
void DGraphModel<T>::clear() {
    for (VertexNode<T>* node : nodeList) {
        delete node;  // Destructor tự xóa edges
    }
    nodeList.clear();
}

template<class T>
int DGraphModel<T>:: inDegree(T vertex) {
    VertexNode<T>* node = getVertexNode(vertex);
    if (!node) throw VertexNotFoundException("Vertex (" + getVertexString(vertex) + "): is not found");
    return node->inDegree();
}

template<class T>
int DGraphModel<T>:: outDegree(T vertex) {
    VertexNode<T>* node=getVertexNode(vertex);
    if (!node) throw VertexNotFoundException("Vertex (" + getVertexString(vertex) + "): is not found");
    return node->outDegree();
}

template<class T>
vector<T> DGraphModel<T>:: vertices() {
    vector<T> result;
    for (VertexNode<T> * cur: nodeList) {
        result.push_back(cur->getVertex());
    }
    return result;
}

template<class T>
string DGraphModel<T>::toString() {
    string result="==================================================\n";
    result += "Vertices:\n";

    // Gọi toString() của từng VertexNode
    for (VertexNode<T>* node : nodeList) {
        result += node->toString() + "\n";
    }

    result += "------------------------------\n";
    result += "Edges:\n";

    // Gọi toString() của từng Edge trong adList của mỗi VertexNode
    for (VertexNode<T>* node : nodeList) {
        for (Edge<T>* edge : node->adList) {
            result += edge->toString() + "\n";
        }
    }
    result+="==================================================\n";
    return result;
}

// TODO: BFS use Queue and DFS use stack
template <class T>
class Queue
{
private:
    vector<T> data;
    int frontIndex;
    int rearIndex;

public:
    Queue() : frontIndex(0), rearIndex(-1) {}
    // TODO
    void push(const T& value) {
        data.push_back(value);
        rearIndex++;
    }

    void pop() {
        if (!empty()) frontIndex++;
    }

    T& front() {
        return data[frontIndex];
    }

    bool empty() const {
        return frontIndex > rearIndex;
    }
};

template <class T>
class Stack
{
private:
    vector<T> data;

public:
    Stack() = default;
    // TODO
    void push(const T& value) {
        data.push_back(value);
    }

    void pop() {
        if (!empty()) data.pop_back();
    }

    T& top() {
        return data.back();
    }

    bool empty() const {
        return data.empty();
    }
};

template<class T>
string DGraphModel<T>::BFS(T start) {
    VertexNode<T>* startNode = getVertexNode(start);
    if (!startNode) {
         throw VertexNotFoundException("Vertex (" + getVertexString(start) + "): is not found");
    }

    string result;
    Queue<VertexNode<T>*> q;
    vector<bool> visited(nodeList.size(), false);

    auto getIndex = [&](VertexNode<T>* node) -> int {
        for (int i = 0; i < nodeList.size(); ++i) {
            if (nodeList[i]->equals(node)) return i;
        }
        return -1;
    };

    q.push(startNode);
    visited[getIndex(startNode)] = true;

    vector<T> order; // lưu thứ tự duyệt

    while (!q.empty()) {
        VertexNode<T>* current = q.front();
        q.pop();

        order.push_back(current->getVertex());

        for (Edge<T>* edge : current->adList) {
            VertexNode<T>* neighbor = edge->to;
            int idx = getIndex(neighbor);
            if (!visited[idx]) {
                visited[idx] = true;
                q.push(neighbor);
            }
        }
    }
    // Ghép chuỗi mà không có khoảng trắng thừa
    for (size_t i = 0; i < order.size(); ++i) {
        result += getVertexString(order[i]);
        if (i + 1 < order.size()) result += " ";
    }
    return result;
}

template<class T>
string DGraphModel<T>::DFS(T start) {
    VertexNode<T>* startNode = getVertexNode(start);
    if (!startNode) {
        throw VertexNotFoundException("Vertex (" + getVertexString(start) + "): is not found");
    }

    string result;
    Stack<VertexNode<T>*> stack;
    vector<bool> visited(nodeList.size(), false);

    auto getIndex = [&](VertexNode<T>* node) -> int {
        for (int i = 0; i < nodeList.size(); ++i) {
            if (nodeList[i]->equals(node)) return i;
        }
        return -1;
    };

    stack.push(startNode);
    vector<T> order; // lưu thứ tự duyệt

    while (!stack.empty()) {
        VertexNode<T>* current = stack.top();
        stack.pop();

        int idx = getIndex(current);
        if (visited[idx]) continue;

        visited[idx] = true;
        order.push_back(current->getVertex());

        // Đẩy hàng xóm vào stack (đảo ngược để giữ thứ tự duyệt đúng)
        for (int i = current->adList.size() - 1; i >= 0; --i) {
            VertexNode<T>* neighbor = current->adList[i]->to;
            int nIdx = getIndex(neighbor);
            if (!visited[nIdx]) {
                stack.push(neighbor);
            }
        }
    }

    // Ghép chuỗi mà không có khoảng trắng thừa
    for (size_t i = 0; i < order.size(); ++i) {
        result += getVertexString(order[i]);
        if (i + 1 < order.size()) result += " ";
    }

    return result;
}

// =============================================================================
// Class KnowledgeGraph Implementation
// =============================================================================

KnowledgeGraph::KnowledgeGraph() {
}

void KnowledgeGraph::addEntity(string entity) {
    // TODO: Add a new entity to the Knowledge Graph
    if (graph.contains(entity)) {
        throw EntityExistsException("Entity (" + entity + ") already exists!" );
    }
    else {
        graph.add(entity);
        entities.push_back(entity);
    }
}

void KnowledgeGraph::addRelation(string from, string to, float weight) {
    // TODO: Add a directed relation
    if (!graph.contains(from)) {
        throw EntityNotFoundException("Entity ("+from+") not found!");
    }
    if (!graph.contains(to)) {
        throw EntityNotFoundException("Entity ("+to+") not found!");
    }
    graph.connect(from, to, weight);
}

// TODO: Implement other methods of KnowledgeGraph:

vector<string> KnowledgeGraph:: getAllEntities() {
    return entities;
}

vector<string> KnowledgeGraph:: getNeighbors(string entity) {
if (!graph.contains(entity)) {
    throw EntityNotFoundException("Entity (" + entity + ") not found!");
}
return graph.getOutwardEdges(entity);
}

string KnowledgeGraph:: bfs(string start) {
    if (!graph.contains(start)) {
        throw EntityNotFoundException("Entity ("+start+") not found!");
    }
    return graph.BFS(start);
}

string KnowledgeGraph:: dfs(string start) {
    if (!graph.contains(start)) {
        throw EntityNotFoundException ("Entity ("+start+") not found!");
    }
    return graph.DFS(start);
}

bool KnowledgeGraph:: isReachable(string from, string to) {
    if (!graph.contains(from)) {
        throw EntityNotFoundException("Entity ("+ from +") not found!");
    }
    if (!graph.contains(to)) {
        throw EntityNotFoundException("Entity ("+ to +") not found!");
    }
    // Trường hợp đặc biệt: from == to
    if (from == to) {
        return true;
    }
    // BFS để tìm đường đi
    Queue<string> q;
    vector<bool> visited(entities.size(), false);
    auto getIndex = [&](string v) -> int {
        for (int i = 0; i < entities.size(); ++i) {
            if (entities[i] == v) return i;
        }
        return -1;
    };
    q.push(from);
    visited[getIndex(from)] = true;
    while (!q.empty()) {
        string current = q.front();
        q.pop();
        // Nếu đến được đích -> có đường đi
        if (current == to) {
            return true;
        }
        vector<string> neighbors = graph.getOutwardEdges(current);
        for (const string& neighbor : neighbors) {
            int idx = getIndex(neighbor);
            if (!visited[idx]) {
                visited[idx] = true;
                q.push(neighbor);
            }
        }
    }
    return false;
}

string KnowledgeGraph:: toString() {
    return graph.toString();
}

vector<string> KnowledgeGraph:: getRelatedEntities(string entity, int depth) {
if (!graph.contains(entity)) {
    throw EntityNotFoundException("Entity (" + entity +") not found!");
}
vector <string> result;
Queue<pair<string,int>>q;
vector<bool> visited(entities.size(),false);
auto getIndex = [&] (string v) ->int {
    for (int i=0; i<entities.size(); ++i) {
        if (entities[i]==v) return i;
    }
    return -1;
};
q.push({entity,0});
visited[getIndex(entity)]=true;
while (!q.empty()) {
auto[current,level]=q.front();
q.pop();
if (current != entity) {
    result.push_back(current);
}
if (level<depth) {
    for (string neighbor: graph.getOutwardEdges(current)) {
        int idx=getIndex(neighbor);
        if (!visited[idx]) {
            visited[idx]=true;
            q.push({neighbor,level+1});
        }
    }
}
}
return result;
}

string KnowledgeGraph:: findCommonAncestors(string entity1, string entity2) {
    if (!graph.contains(entity1)) {
        throw EntityNotFoundException("Entity (" + entity1 +") not found!");
    }
    if (!graph.contains(entity2)) {
        throw EntityNotFoundException("Entity (" + entity2 +") not found!");
    }
    if (entity1==entity2) return entity1;
    // Kiểm tra xem entity1 có phải ancestor của entity2 không
    if (isReachable(entity1, entity2)) {
        return entity1;
    }
    // Kiểm tra xem entity2 có phải ancestor của entity1 không
    if (isReachable(entity2, entity1)) {
        return entity2;
    }
    struct AncestorInfo {
        string entity;
        int distance;
    };
    auto findAncestorWithDistance= [&] (string start) -> vector <AncestorInfo> {
        vector <AncestorInfo> ancestors;
        for (const string& candidate: entities) {
            if (candidate==start) {
                continue;
            }
            Stack<pair<string, int>> stack;
            vector<string> visited;
            stack.push({candidate,0});
            bool found=false;
            int distance = -1;
            while (!stack.empty() && !found) {
                auto current= stack.top();
                stack.pop();
                string currentEntity=current.first;
                int currentDist=current.second;
                bool alreadyVisited=false;
                for (const string & v: visited) {
                    if (v==currentEntity) {
                        alreadyVisited=true;
                        break;
                    }
                }
                if (alreadyVisited) continue;
                visited.push_back(currentEntity);
                if (currentEntity==start) {
                    found=true;
                    distance=currentDist;
                    break;
                }
                vector <string> neighbors=graph.getOutwardEdges (currentEntity);
                for (const string & next : neighbors) {
                    bool nextVisited=false;
                    for (const string& v: visited) {
                        if (v==next) {
                            nextVisited=true;
                            break;
                        }
                    }
                    if (!nextVisited) {
                        stack.push({next, currentDist+1});
                    }
                }
            }
            if (found) ancestors.push_back({candidate,distance});
        }
        return ancestors;
    };
    vector<AncestorInfo> ancestors1=findAncestorWithDistance(entity1);
    vector<AncestorInfo> ancestors2=findAncestorWithDistance(entity2);
    string lca="No common ancestor";
    int minSum=2147483647;
    for (const AncestorInfo& anc1 : ancestors1) {
        for (const AncestorInfo& anc2 : ancestors2) {
            if (anc1.entity==anc2.entity) {
                int totalDist=anc1.distance+anc2.distance;
                if (totalDist<minSum) {
                    minSum=totalDist;
                    lca=anc1.entity;
                }
                // Nếu bằng nhau, chọn theo thứ tự từ điển
                else if (totalDist==minSum && anc1.entity < lca){
                    lca=anc1.entity;
                }
                break;
            }
        }
    }
    return lca;
}

// =============================================================================
// Explicit Template Instantiation
// =============================================================================

template<class T>
string DGraphModel<T>::getVertexString(T& vertex) {
    if (vertex2str != nullptr) {
        return vertex2str(vertex);
    } else {
        ostringstream oss;
        oss << vertex;
        return oss.str();
    }
}



template class Edge<string>;
template class Edge<int>;
template class Edge<float>;
template class Edge<char>;

template class VertexNode<string>;
template class VertexNode<int>;
template class VertexNode<float>;
template class VertexNode<char>;

template class DGraphModel<string>;
template class DGraphModel<int>;
template class DGraphModel<float>;
template class DGraphModel<char>;
