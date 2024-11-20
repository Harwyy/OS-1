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

using namespace std;
using namespace chrono;

struct Edge {
  int to, weight;
};

vector<int> dijkstra(int start, int target, const vector<vector<Edge>>& graph) {
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
  return path;
}

void loadDijkstra(int vertexCount, int targetVertex, int iterations) {
  vector<vector<Edge>> graph(vertexCount);
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> distWeight(1, 100);

  for (int i = 0; i < vertexCount; ++i) {
    for (int j = 0; j < 5; ++j) {
      int to = rand() % vertexCount;
      int weight = distWeight(gen);
      graph[i].push_back({to, weight});
    }
  }

  for (int i = 0; i < iterations; ++i) {
    auto start = high_resolution_clock::now();
    auto path = dijkstra(0, targetVertex, graph);
    auto end = high_resolution_clock::now();
    cout << "Iteration " << i + 1 << ": "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
  }
}

void loadDisk(const string& filename, size_t blockSize, size_t totalBlocks) {
  vector<char> buffer(blockSize, 'A');

  ofstream file(filename, ios::binary);
  if (!file) {
    cerr << "Error opening file!" << endl;
    return;
  }

  auto start = high_resolution_clock::now();
  for (size_t i = 0; i < totalBlocks; ++i) {
    file.write(buffer.data(), blockSize);
    if (!file) {
      cerr << "Error writing to file!" << endl;
      return;
    }
  }
  auto end = high_resolution_clock::now();

  file.close();

  auto duration = duration_cast<milliseconds>(end - start);
  double speed = (blockSize * totalBlocks) / (duration.count() * 1024.0 * 1024.0);

  cout << "Write speed: " << speed << " MB/s\n";
}

int main(int argc, char* argv[]) {
  if (argc != 7) {
    cerr << "Usage: " << argv[0] << " <vertexCount> <targetVertex> <iterations> <filename> <blockSize> <totalBlocks>\n";
    return 1;
  }

  int vertexCount = stoi(argv[1]);
  int targetVertex = stoi(argv[2]);
  int iterations = stoi(argv[3]);
  string filename = argv[4];
  size_t blockSize = static_cast<size_t>(stoi(argv[5]));
  size_t totalBlocks = static_cast<size_t>(stoi(argv[6]));

  thread dijkstraThread(loadDijkstra, vertexCount, targetVertex, iterations);
  thread diskThread(loadDisk, filename, blockSize, totalBlocks);

  dijkstraThread.join();
  diskThread.join();

  return 0;
}
