#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <random>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include "string"

using namespace std;
using namespace chrono;

struct Edge {
    int to, weight;
};

vector<int> dijkstra(int start, int target, const vector<vector<Edge>>& graph) {
    auto s = high_resolution_clock::now();
    vector<int> dist(graph.size(), numeric_limits<int>::max());
    vector<int> prev(graph.size(), -1);
    dist[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int v = pq.top().second;
        int current_dist = pq.top().first;
        pq.pop();

        if (current_dist > dist[v]) continue;

        for (const Edge& edge : graph[v]) {
            if (dist[v] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[v] + edge.weight;
                prev[edge.to] = v;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }

    vector<int> path;
    for (int at = target; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    if (path.size() == 1 && path[0] != start) {
        path.clear();
    }

    auto e = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(e - s);
    cout << "================" << endl;
    cout << "Total time: " << duration.count() << " ms" << endl;
    cout << "================" << endl;

    return path;
}

void measureWriteSpeed(const string& filename, size_t blockSize, size_t totalBlocks) {
    vector<char> buffer(blockSize, 'A');

    int fd;
    errno_t err = _sopen_s(&fd,
                           filename.c_str(),
                           _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY,
                           _SH_DENYNO,
                           _S_IREAD | _S_IWRITE);

    if (err != 0 || fd == -1) {
        cerr << "Error opening file!" << endl;
        return;
    }

    auto start = chrono::high_resolution_clock::now();

    for (size_t i = 0; i < totalBlocks; ++i) {
        int bytesWritten = _write(fd, buffer.data(), blockSize);
        if (bytesWritten == -1) {
            cerr << "Error writing to file!" << endl;
            _close(fd);
            return;
        }
    }

    auto end = chrono::high_resolution_clock::now();

    _close(fd);

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    double speed = (blockSize * totalBlocks) / (duration.count() * 1024.0);

    cout << "================" << endl;
    cout << "Total time: " << duration.count() / 1000 << " s" << endl;
    cout << "Write speed: " << speed << " MB/s" << endl;
    cout << "================" << endl;
}

int main(int argc, char* argv[]) {
  if (argc != 7) {
    cerr << "Usage: " << argv[0] << " <vertexCount> <targetVertex> <filename> <blockSize> <totalBlocks> <iterations>\n";
    return 1;
  }

  int vertexCount = atoi(argv[1]);
  int targetVertex = atoi(argv[2]);
  string filename = argv[3];
  size_t blockSize = static_cast<size_t>(atoi(argv[4]));
  size_t totalBlocks = static_cast<size_t>(atoi(argv[5]));
  size_t iterations = static_cast<size_t>(atoi(argv[6]));

    vector<vector<Edge>> graph(vertexCount);
    for (int i = 0; i < vertexCount; ++i) {
        for (int j = 0; j < 3; ++j) {
            int to = rand() % vertexCount;
            int weight = rand() % 100 + 1;
            graph[i].push_back({to, weight});
        }
    }

    vector<thread> threads;

    for (int i = 0; i < iterations; i++) {
        threads.push_back(thread([=]() {
            measureWriteSpeed(filename + to_string(i), blockSize, totalBlocks);
        }));
        threads.push_back(thread([=]() {
            dijkstra(0, targetVertex, graph);
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    threads.clear();

  return 0;
}
