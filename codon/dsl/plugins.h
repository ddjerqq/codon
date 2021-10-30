#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "codon/dsl/dsl.h"
#include "codon/sir/util/iterators.h"
#include "llvm/Support/DynamicLibrary.h"

namespace codon {

/// Plugin metadata
struct Plugin {
  /// the associated DSL
  std::unique_ptr<DSL> dsl;
  /// plugin information
  DSL::Info info;
  /// library handle
  llvm::sys::DynamicLibrary handle;

  Plugin(std::unique_ptr<DSL> dsl, DSL::Info info, llvm::sys::DynamicLibrary handle)
      : dsl(std::move(dsl)), info(std::move(info)), handle(std::move(handle)) {}
};

/// Manager for loading, applying and unloading plugins.
class PluginManager {
private:
  /// vector of loaded plugins
  std::vector<std::unique_ptr<Plugin>> plugins;

public:
  /// Constructs a plugin manager
  PluginManager() : plugins() {}

  /// @return iterator to the first plugin
  auto begin() { return ir::util::raw_ptr_adaptor(plugins.begin()); }
  /// @return iterator beyond the last plugin
  auto end() { return ir::util::raw_ptr_adaptor(plugins.end()); }
  /// @return const iterator to the first plugin
  auto begin() const { return ir::util::const_raw_ptr_adaptor(plugins.begin()); }
  /// @return const iterator beyond the last plugin
  auto end() const { return ir::util::const_raw_ptr_adaptor(plugins.end()); }

  /// Loads the plugin at the given load path.
  /// @param path path to plugin directory containing "plugin.toml" file
  /// @param errMsg where to store potential error messages, if non-null
  /// @return plugin pointer if successful, null otherwise
  Plugin *load(const std::string &path, std::string *errMsg = nullptr);
};

} // namespace codon
