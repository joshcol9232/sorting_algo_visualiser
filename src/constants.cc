

namespace constants {
namespace runtime {

bool disableSleeps(const bool disable) {
  static bool doDisable = disable;
  return doDisable;
}

}  // runtime
}  // constants
