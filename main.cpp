#include <iostream>
#include <fstream>
#include <vector>

#define MAX(a, b) ((a > b) ? (a) : (b))

struct DiEdge {
    int from;
    int to;
};

class DiGraph {
public:
    std::vector<std::vector<int>> vertices;

    DiGraph(std::vector<DiEdge> const &edges, int vertexCount)
    {
        vertices.resize(vertexCount);
        for (const DiEdge &edge: edges) {
            vertices[edge.from].push_back(edge.to);
        }
    }
};

struct DFSContext {
    DiGraph const &dfsGraph;
    bool* onPath;
    int verticesOnPath;
    int paths;

    DFSContext(DiGraph const &graph) : dfsGraph(graph) {
        onPath = (bool*) calloc(graph.vertices.size(), sizeof(bool));
        verticesOnPath = 0;
        paths = 0;
    }

    ~DFSContext() {
        free(onPath);
    }
};

void DFSPaths(DFSContext &ctx, int start, int end) {
    if (start == end) {
        ctx.paths += (1 << (ctx.verticesOnPath - 1)); /* -1 since you need to visit the first node */
    }
    else {
        ctx.onPath[start] = true;
        ctx.verticesOnPath++;
        std::vector<int> neighbours = ctx.dfsGraph.vertices[start];
        for (std::vector<int>::iterator next = neighbours.begin(); next != neighbours.end(); ++next) {
            if (!ctx.onPath[*next]) {
                DFSPaths(ctx, *next, end);
            }
        }
        ctx.onPath[start] = false;
        ctx.verticesOnPath--;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "ERROR: Wrong number of arguments! \n";
        return -1;
    }

    std::vector<DiEdge> edges{};
    int vertexCount = 0;

    {   /* Read input file */
        std::string path = argv[1];
        std::ifstream fin(path);

        if (!fin.is_open()) {
            std::cout << "ERROR: Failed to open " << path << "\n";
            return -2;
        }

        size_t delimiterPos = 0;
        int fromInt = 0;
        int toInt = 0;
        int lineNo = 1;

        for (std::string line; std::getline(fin, line, '\n'); ++lineNo) {
            delimiterPos = line.find(',');
            if (delimiterPos == std::string::npos) {
                std::cout << "ERROR: Delimiter not found! line " << lineNo << "\n";
                return -3;
            }
            if (!std::isdigit(line[0]) || !std::isdigit(line[delimiterPos + 1])) {
                std::cout << "ERROR: Vertex not a digit! line " << lineNo << "\n";
                return -3;
            }
            fromInt = std::stoi(line);
            toInt = std::stoi(line.substr(delimiterPos + 1));
            if (fromInt == toInt) {
                std::cout << "ERROR: Self loops are not supported! line " << lineNo << "\n";
                return -3;
            }
            edges.push_back(DiEdge{.from = fromInt, .to = toInt});
            vertexCount = MAX(vertexCount, MAX(fromInt, toInt));
        }
        vertexCount += 1;
        fin.close();
    }

    DiGraph graph(edges, vertexCount);
    DFSContext ctx(graph);

    int start = atoi(argv[2]);
    int end = atoi(argv[3]);

    if ((start < 0) || (end < 0) || (start >= vertexCount) || (end >= vertexCount)) {
        std::cout << "ERROR: Vertex out of range! Number of vertices: " << vertexCount << "\n";
        return -2;
    }

    DFSPaths(ctx, start, end);
    std::cout << "Total number of paths: " << ctx.paths << "\n";
    return 0;
}
