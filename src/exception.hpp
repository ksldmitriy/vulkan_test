#include <iostream>
#include <string>
#include <memory>

using namespace std;

class Exception{
private:
  shared_ptr<Exception> sub_exception;
public:
  Exception() = default;
  virtual operator string() = 0;
};
