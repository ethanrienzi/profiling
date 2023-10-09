#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>
#include <cstdlib>  // for rand()
#include <cmath>    // for pow()
#include <functional>

using namespace std;

struct Config {
  int num_threads;
  int num_loops;
  bool search_primes;
  bool calculate_factorial;
  bool generate_fibonacci;
  bool add_large_random_numbers;
  bool inter_thread_communication;
  bool exponentiation;
  bool matrix_multiplication;
  bool hash_calculation;
  bool bit_manipulation;
  bool custom_task;
};

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

      if (key == "num_threads") {
        config.num_threads = stoi(value);
      } else if (key == "num_loops") {
        config.num_loops = stoi(value);
      } else if (key == "search_primes") {
        config.search_primes = (value == "true");
      } else if (key == "calculate_factorial") {
        config.calculate_factorial = (value == "true");
      } else if (key == "generate_fibonacci") {
        config.generate_fibonacci = (value == "true");
      } else if (key == "add_large_random_numbers") {
        config.add_large_random_numbers = (value == "true");
      } else if (key == "inter_thread_communication") {
        config.inter_thread_communication = (value == "true");
      } else if (key == "exponentiation") {
        config.exponentiation = (value == "true");
      } else if (key == "matrix_multiplication") {
        config.matrix_multiplication = (value == "true");
      } else if (key == "hash_calculation") {
        config.hash_calculation = (value == "true");
      } else if (key == "bit_manipulation") {
        config.bit_manipulation = (value == "true");
      } else if (key == "custom_task") {
        config.custom_task = (value == "true");
      } else {
        throw invalid_argument("Unknown configuration key: " + key);
      }
    }
  } else {
    throw runtime_error("Error: Could not open config file " + filename);
  }

  return config;
}

bool is_prime(int n) {
  if (n <= 1) return false;
  if (n == 2) return true;

  for (int i = 2; i * i <= n; ++i) {
    if (n % i == 0) {
      return false;
    }
  }
  return true;
}

int factorial(int n) {
  if (n == 0 || n == 1) return 1;

  int result = 1;
  for (int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

int fibonacci(int n) {
  if (n <= 1) return n;

  int a = 0, b = 1, temp;
  for (int i = 2; i <= n; ++i) {
    temp = a + b;
    a = b;
    b = temp;
  }
  return b;
}

void matrix_multiply(const vector<vector<int>>& matrix1,
                     const vector<vector<int>>& matrix2,
                     vector<vector<int>>& result) {
  int rows1 = matrix1.size();
  int cols1 = matrix1[0].size();
  int cols2 = matrix2[0].size();

  // Initialize result matrix with appropriate dimensions
  result.resize(rows1);
  for (int i = 0; i < rows1; ++i) {
    result[i].resize(cols2, 0);
  }

  for (int i = 0; i < rows1; ++i) {
    for (int j = 0; j < cols2; ++j) {
      for (int k = 0; k < cols1; ++k) {
        result[i][j] += matrix1[i][k] * matrix2[k][j];
      }
    }
  }
}

// Hash function (example: simple hash)
unsigned int hash_function(const string& input) {
  unsigned int hash = 0;
  for (char c : input) {
    hash = (hash * 31) + c;
  }
  return hash;
}

// Custom CPU-intensive task (example: calculating a large expression)
double custom_cpu_intensive_task(double input) {
  double result = 1.0;
  for (int i = 1; i <= 100000; ++i) {
    result *= (input * i) / (i + 1);
  }
  return result;
}

void perform_intensive_task(const Config& config, vector<int>& thread_data, int thread_id) {
  cout << "Thread " << thread_id << " starting intensive tasks..." << endl;

  for (int i = 0; i < config.num_loops; ++i) {
    int random_number = rand() % 1000000;  // Generate a random number

    // Inter-thread communication: Send the random number to other threads for processing
    if (config.inter_thread_communication) {
      for (int j = 0; j < config.num_threads; ++j) {
        if (j != thread_id) {
          thread_data[j] = random_number;
        }
      }
    }

    if (config.search_primes) {
      for (int num = 0; num < 10000; ++num) {
        if (is_prime(num)) {
          // Do something with the prime number
        }
      }
    }

    if (config.calculate_factorial) {
      int result = factorial(10); // Calculate factorial of 10 as an example
    }

    if (config.generate_fibonacci) {
      int result = fibonacci(10); // Generate Fibonacci sequence up to 10 terms as an example
    }

    if (config.add_large_random_numbers) {
      // Generate and add large random numbers
      long long result = 0;
      for (int j = 0; j < 1000000; ++j) {
        result += rand() % 1000000000;  // Add random number up to 1,000,000,000
      }
    }

    if (config.exponentiation) {
      double result = pow(random_number, 2.0); // Perform exponentiation
    }

    if (config.matrix_multiplication) {

      //we can add more later iif neeeded
      vector<vector<int>> matrix1 = {{1, 2}, {3, 4}};
  vector<vector<int>> matrix2 = {{5, 6}, {7, 8}};
  vector<vector<int>> result;

  // Perform matrix multiplication
  matrix_multiply(matrix1, matrix2, result);
    }

    if (config.hash_calculation) {
      string input = to_string(random_number);
      unsigned int result = hash_function(input); // Perform hash calculation
    }

    if (config.bit_manipulation) {
      int result = random_number ^ (random_number << 1); // Perform bit manipulation
    }

    if (config.custom_task) {
      double result = custom_cpu_intensive_task(random_number); // Perform custom CPU-intensive task
    }
  }

  cout << "Thread " << thread_id << " finished intensive tasks." << endl;
}

int main() {
  Config config;

  try {
    config = read_config("config.ini");

    // Create a vector to store the threads and their data
    vector<thread> threads;
    vector<int> thread_data(config.num_threads);

    // Start the threads
    for (int i = 0; i < config.num_threads; ++i) {
      threads.push_back(thread(perform_intensive_task, ref(config), ref(thread_data), i));
    }

    // Join the threads
    for (auto& thread : threads) {
      thread.join();
    }

  } catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }

  return 0;
}
