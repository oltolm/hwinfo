#include <hwinfo/utils/wmi_wrapper.h>

#ifdef HWINFO_WINDOWS

#include <hwinfo/utils/stringutils.h>

#include <vector>

namespace hwinfo {
namespace utils {
namespace WMI {

_WMI::_WMI() {
  if (SUCCEEDED(CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT,
                                     RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr))) {
    if (SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) {
      if (SUCCEEDED(CoCreateInstance(__uuidof(WbemLocator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&locator)))) {
        if (SUCCEEDED(locator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, nullptr, 0, nullptr, nullptr,
                                             &service))) {
          if (SUCCEEDED(CoSetProxyBlanket(service.Get(), RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
                                          RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE)))
            return;
        }
      }
      CoUninitialize();
    }
  }
  throw std::runtime_error("error initializing WMI");
}

_WMI::~_WMI() { CoUninitialize(); }

bool _WMI::execute_query(const std::wstring& query) {
  if (service == nullptr) return false;
  return SUCCEEDED(service->ExecQuery(_bstr_t(L"WQL"), _bstr_t(std::wstring(query.begin(), query.end()).c_str()),
                                      WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator));
}

}  // namespace WMI
}  // namespace utils
}  // namespace hwinfo

#endif  // HWINFO_WINDOWS
