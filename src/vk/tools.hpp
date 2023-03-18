#pragma once
#include <string>
#include <vector>

using namespace std;

namespace tools {
  vector<const char *> string_vector_to_c_array(vector<string> &array);
}
