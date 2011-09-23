#ifndef IOPool_TFileAdaptor_TFileAdaptor_h
#define IOPool_TFileAdaptor_TFileAdaptor_h

#include <map>
#include <string>
#include <vector>

class TPluginManager;

namespace edm {
  class ActivityRegistry;
  class ConfigurationDescriptions;
  class ParameterSet;
}

// Driver for configuring ROOT plug-in manager to use TStorageFactoryFile.
class TFileAdaptor {
public:
  TFileAdaptor(edm::ParameterSet const& pset, edm::ActivityRegistry& ar);

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  // Write current Storage statistics on a ostream
  void termination(void) const;

  void stats(std::ostream &o) const;

  void statsXML(std::map<std::string, std::string> &data) const;

private:
  static void addType(TPluginManager* mgr, char const* type);
  bool native(char const* proto) const;

  bool enabled_;
  bool doStats_;
  std::string cacheHint_;
  std::string readHint_;
  std::string tempDir_;
  double minFree_;
  unsigned int timeout_;
  std::vector<std::string> native_;

};

namespace edm {
  namespace service {
    inline
    bool isProcessWideService(TFileAdaptor const*) {
      return true;
    }
  }
}
#endif
