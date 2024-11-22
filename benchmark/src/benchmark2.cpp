#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <vector>
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

int main(int argc, char *argv[]) {
  //  srand(time(nullptr));
  if (argc < 4) {
    cerr << "Using: " << argv[0] << " <number of vertices> <target vertex> <count>\n";
    return 1;
  }

  int vertex_count = stoi(argv[1]);
  int target_vertex = stoi(argv[2]);
  int count = stoi(argv[3]);


  if (target_vertex >= vertex_count) {
    cerr << "Error: Target vertex exceeds the number of vertices in the graph.\n";
    return 1;
  }

  vector<vector<Edge>> graph(vertex_count);
  for (int i = 0; i < vertex_count; ++i) {
    for (int j = 0; j < 5; ++j) {
      int to = rand() % vertex_count;
      int weight = rand() % 100 + 1;
      graph[i].push_back({to, weight});
    }
  }

  vector<int> shortest_path;
  for (int i = 0; i < count; i++) {
    shortest_path = dijkstra(0, target_vertex, graph);
  }

  if (shortest_path.empty()) {
    cout << "No path exists to vertex " << target_vertex << ".\n";
  } else {
    cout << "Shortest path to vertex " << target_vertex << ": ";
    for (int node : shortest_path) {
      cout << node << " ";
    }
    cout << "\n";
  }

  return 0;
}
