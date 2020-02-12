#include <iostream>
#include <string>
#include <stdexcept>

#include <boost/di.hpp>

using namespace boost;

class TErrorPolicy;
class TUpdater;

template<class ErrorPolicy = TErrorPolicy>
class simple_updater {
public:
  void update() const {
    ErrorPolicy::on("update");
  }
};

template<class Updater = TUpdater>
class example {
public:
  explicit example(const Updater& updater)
    : updater(updater)
  { }

  void update() {
    updater.update();
  }

private:
  const Updater& updater;
};

int main() {
  struct throw_policy {
    static void on(const std::string& str) {
      throw std::runtime_error(str);
    }
  };

  const auto injector = di::make_injector(
    di::bind<TErrorPolicy>.to<throw_policy>(),
    di::bind<TUpdater>.to<simple_updater>()
  );

  int status = -1;
  try {
      injector.create<example>().update();
  } catch(const std::runtime_error& err) {
      std::cout << "ok: " << err.what() << '\n';
      status = 0;
  }
  return status;
}
