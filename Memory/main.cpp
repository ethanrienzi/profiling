#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <thread>
#include <malloc.h>  // Memory-related library for additional memory usage
#include "Player.cpp"

using namespace std;

// Structure to hold game configuration
struct Config {
  int numberOfPlayers;
  int lengthOfSentences;
  int optionNumber;
  int MAX_ACTIONS;
  int MEMORY_ALLOCATION_SIZE;
  int MEMORY_OPERATION_DURATION_SECONDS;
  bool USE_MEMORY_LIBRARIES;
};

// Function to generate and parse config
Config read_config(string filename) {
  Config config;

  ifstream config_file(filename);
  if (config_file.is_open()) {
    string line;
    while (getline(config_file, line)) {
      if (line.empty()) {
        continue;
      }

      stringstream ss(line);
      string key, value;
      getline(ss, key, '=');
      getline(ss, value);

      if (key == "numberOfPlayers") {
        config.numberOfPlayers = stoi(value);
      } else if (key == "lengthOfSentences") {
        config.lengthOfSentences = stoi(value);
      } else if (key == "optionNumber") {
        config.optionNumber = stoi(value);
      } else if (key == "MAX_ACTIONS") {
        config.MAX_ACTIONS = stoi(value);
      } else if (key == "MEMORY_ALLOCATION_SIZE") {
        config.MEMORY_ALLOCATION_SIZE = stoi(value);
      } else if (key == "MEMORY_OPERATION_DURATION_SECONDS") {
        config.MEMORY_OPERATION_DURATION_SECONDS = stoi(value);
      } else if (key == "USE_MEMORY_LIBRARIES") {
        config.USE_MEMORY_LIBRARIES = (value == "true");
      } else {
        throw invalid_argument("Unknown configuration key: " + key);
      }
    }
  } else {
    throw runtime_error("Error: Could not open config file " + filename);
  }

  return config;
}

// Function to perform memory-intensive operations
void perform_memory_intensive_operations(int memory_allocation_size, int duration_seconds, bool use_memory_libraries) {
  auto start_time = chrono::steady_clock::now();
  while (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start_time).count() < duration_seconds) {
    // Allocate a large memory chunk
    char* memory_chunk = new char[memory_allocation_size];
    memset(memory_chunk, 'a', memory_allocation_size);  // Fill memory chunk with 'a'

    // Use additional memory-related libraries if specified
    if (use_memory_libraries) {
      void* more_memory = malloc(memory_allocation_size);
      memset(more_memory, 'b', memory_allocation_size);
    }
  }
}

int main() {
  Config config;
  vector<vector<string>> words;

  try {
    // Read in configuration and set params
    config = read_config("config.ini");

    // Read words from CSV into a 2D array
    ifstream file("wordlist.csv");
    if (file.is_open()) {
      string word;
      while (getline(file, word, ',')) {
        vector<string> row;
        row.push_back(word);
        words.push_back(row);
      }
      file.close();
    } else {
      throw runtime_error("Error: Could not open wordlist file");
    }

    // Validate configuration
    if (config.optionNumber > words.size()) {
      cerr << "Error: optionNumber exceeds the number of words in the wordlist." << endl;
      return 1;
    }

    // Perform memory-intensive operations
    perform_memory_intensive_operations(config.MEMORY_ALLOCATION_SIZE, config.MEMORY_OPERATION_DURATION_SECONDS, config.USE_MEMORY_LIBRARIES);

    // Initialize player arrays
    vector<vector<string>> playerActions(config.numberOfPlayers);

    // Generate n actions for the player to choose from
    for (int i = 0; i < config.MAX_ACTIONS; ++i) {
      for (int j = 0; j < config.numberOfPlayers; ++j) {
        int random_index = rand() % config.optionNumber;
        string action = words[random_index][0];
        playerActions[j].push_back(action);
      }
    }

  } catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }

  return 0;
}

