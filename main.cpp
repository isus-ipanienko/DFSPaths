#include <iostream>
#include <vector>

using namespace std;

struct DiEdge {
    int from;
    int to;
};

class DiGraph {
public:
    vector<vector<int>> vertices;

    DiGraph(vector<DiEdge> const &edges, int n)
    {
        vertices.resize(n);
        for (const DiEdge &edge: edges) {
            vertices[edge.from].push_back(edge.to);
        }
    }
};

struct DFSContext {
    bool* onPath;
    int paths;

    DFSContext(DiGraph const &graph) {
        this->onPath = (bool*) calloc(graph.vertices.size(), sizeof(bool));
        this->paths = 0;
    }

    ~DFSContext() {
        free(this->onPath);
    }
};

void DFSPaths(DFSContext &ctx, DiGraph const &graph, int start, int end) {
    if (start == end) {
        ctx.paths++;
    }
    else {
        ctx.onPath[start] = true;
        vector<int> neighbours = graph.vertices[start];
        for (vector<int>::iterator next = neighbours.begin(); next != neighbours.end(); ++next) {
            if (!ctx.onPath[*next]) {
                DFSPaths(ctx, graph, *next, end);
            }
        }
        ctx.onPath[start] = false;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "ERROR: Wrong number of arguments!\n";
        return -1;
    }

    vector<DiEdge> edges {
        {0, 1}, {1, 2}, {1, 3}, {1, 6}, {2, 1}, {2, 3}, {3, 2},
        {3, 4}, {3, 5}, {4, 5}, {4, 8}, {4, 7}, {5, 4}, {5, 1},
        {5, 6}, {5, 9}, {6, 9}, {6, 7}, {7, 5}, {7, 8}, {8, 9},
        {9, 7}
    };
    int vertexCount = 10;
    DiGraph graph(edges, vertexCount);
    DFSContext ctx(graph);

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    if ((start > vertexCount) || (end > vertexCount)) {
        cout << "ERROR: Start or end out of range! Number of vertices: " << vertexCount << "\n";
        return -1;
    }

    DFSPaths(ctx, graph, start, end);
    cout << "Total number of paths: " << ctx.paths << "\n";
    return 0;
}
