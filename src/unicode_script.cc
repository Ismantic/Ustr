#include "unicode_script.h"
#include "unicode_script_map.h"

#include <unordered_map>

namespace ustr {
namespace unicode_script {
namespace {

template <class Collection>
const typename Collection::value_type::second_type &FindWithDefault(
    const Collection &collection,
    const typename Collection::value_type::first_type &key,
    const typename Collection::value_type::second_type &value) {
    typename Collection::const_iterator it = collection.find(key);
    if (it == collection.end()) {
        return value;
    }
    return it->second;
}

class GetScriptInternal {
 public:
  GetScriptInternal() { InitTable(&smap_); }

  ScriptType GetScript(char32_t c) const {
    return FindWithDefault(smap_, c, ScriptType::U_Common);    
  }
 private:
   std::unordered_map<char32_t, ScriptType> smap_;
};
} // namespace

ScriptType GetScript(char32_t c) {
    static GetScriptInternal sc;
    return sc.GetScript(c);
}

} // namespace unicode_script
} // namespace ustr