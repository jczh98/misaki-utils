#include <string>
#include <vector>

namespace misaki::string {

extern std::vector<std::string> tokenize(const std::string &string,
                                         const std::string &delim = ", ",
                                         bool include_empty = false);

extern std::string indent(const std::string &string, size_t amount = 2);

}  // namespace misaki::string