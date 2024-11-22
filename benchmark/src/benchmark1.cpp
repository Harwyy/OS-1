#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include "string"

using namespace std;
using namespace chrono;

void measureWriteSpeed(const string& filename, size_t blockSize, size_t totalBlocks) {

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

  cout << "================" << endl;
  cout << "Total time: " << duration.count() << " ms" << endl;
  cout << "Write speed: " << speed << " MB/s" << endl;
  cout << "================" << endl;
}

int main(int argc, char* argv[]) {
  if (argc != 5) {
    cerr << "Usage: " << argv[0] << " <filename> <blockSize> <totalBlocks> <count>\n";
    return 1;
  }

  string filename = argv[1];
  size_t blockSize = static_cast<size_t>(stoi(argv[2]));
  size_t totalBlocks = static_cast<size_t>(stoi(argv[3]));
  int count = stoi(argv[4]);

  for (int i = 0; i < count; i++) {
    measureWriteSpeed(filename, blockSize, totalBlocks);
  }

  return 0;
}



