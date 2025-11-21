#pragma once

#include <hwinfo/platform.h>

#include "hwinfo/utils/stringutils.h"

#ifdef HWINFO_WINDOWS

#include <WbemIdl.h>
#include <comdef.h>
#include <wrl/client.h>

#include <iostream>
#include <string>
#include <vector>
#pragma comment(lib, "wbemuuid.lib")

namespace hwinfo {

namespace utils {
namespace WMI {

struct _WMI {
  _WMI();
  ~_WMI();
  bool execute_query(const std::wstring& query);

  Microsoft::WRL::ComPtr<IWbemLocator> locator;
  Microsoft::WRL::ComPtr<IWbemServices> service;
  Microsoft::WRL::ComPtr<IEnumWbemClassObject> enumerator;
};

template <typename T>
std::vector<T> query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter = L"") {
  _WMI wmi;
  std::wstring filter_string;
  if (!filter.empty()) {
    filter_string.append(L" WHERE " + filter);
  }
  std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class + filter_string);
  if (!wmi.execute_query(query_string)) {
    return {};
  }
  std::vector<T> result;

  HRESULT hr;
  do {
    ULONG u_return = 0;
    Microsoft::WRL::ComPtr<IWbemClassObject> obj;
    hr = wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
    if (FAILED(hr)) {
      break;
    }
    VARIANT vt_prop;
    if (SUCCEEDED(obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr))) {
      if constexpr (std::is_same_v<T, std::string>) {
        result.push_back(wstring_to_std_string(vt_prop.bstrVal));
      } else if constexpr (std::is_same_v<T, unsigned long long>) {
        result.push_back(vt_prop.ullVal);
      } else if constexpr (std::is_same_v<T, long long>) {
        result.push_back(vt_prop.llVal);
      } else if constexpr (std::is_same_v<T, unsigned short>) {
        result.push_back(vt_prop.uiVal);
      } else if constexpr (std::is_same_v<T, unsigned int>) {
        result.push_back(vt_prop.uintVal);
      } else if constexpr (std::is_same_v<T, bool>) {
        result.push_back(vt_prop.boolVal);
      } else if constexpr (std::is_same_v<T, int>) {
        result.push_back(vt_prop.intVal);
      } else if constexpr (std::is_same_v<T, long>) {
        result.push_back(vt_prop.lVal);
      }
    }
    VariantClear(&vt_prop);
  } while (hr == WBEM_S_NO_ERROR);
  return result;
}

}  // namespace WMI
}  // namespace utils
}  // namespace hwinfo

#endif
