#include <hwinfo/utils/wmi_wrapper.h>

#ifdef HWINFO_WINDOWS

#include <hwinfo/utils/stringutils.h>
#include <wrl/client.h>

#include <vector>

namespace hwinfo {
namespace utils {
namespace WMI {
using Microsoft::WRL::ComPtr;

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

template <>
std::vector<long> query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter) {
  std::vector<long> result;
  _WMI wmi;
  std::wstring filter_string;
  if (!filter.empty()) {
    filter_string.append(L" WHERE " + filter);
  }
  std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class + filter_string);
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return {};
  }

  ComPtr<IWbemClassObject> obj;
  HRESULT hr;
  do {
    ULONG u_return = 0;
    hr = wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
    if (FAILED(hr)) {
      break;
    }
    VARIANT vt_prop;
    HRESULT hr = obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      result.push_back(vt_prop.intVal);
    }
    VariantClear(&vt_prop);
  } while (hr == WBEM_S_NO_ERROR);
  return result;
}

template <>
std::vector<int> query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter) {
  std::vector<int> result;
  for (const auto& v : query<long>(wmi_class, field, filter)) {
    result.push_back(static_cast<int>(v));
  }
  return result;
}

template <>
std::vector<bool> query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter) {
  std::vector<bool> result;
  _WMI wmi;
  std::wstring filter_string;
  if (!filter.empty()) {
    filter_string.append(L" WHERE " + filter);
  }
  std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class + filter_string);
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return {};
  }

  ComPtr<IWbemClassObject> obj;
  HRESULT hr;
  do {
    ULONG u_return = 0;
    hr = wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
    if (FAILED(hr)) {
      break;
    }
    VARIANT vt_prop;
    HRESULT hr = obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      result.push_back(vt_prop.boolVal);
    }
    VariantClear(&vt_prop);
  } while (hr == WBEM_S_NO_ERROR);
  return result;
}

template <>
std::vector<unsigned> query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter) {
  std::vector<unsigned> result;
  _WMI wmi;
  std::wstring filter_string;
  if (!filter.empty()) {
    filter_string.append(L" WHERE " + filter);
  }
  std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class + filter_string);
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return {};
  }

  ComPtr<IWbemClassObject> obj;
  HRESULT hr;
  do {
    ULONG u_return = 0;
    hr = wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
    if (FAILED(hr)) {
      break;
    }
    VARIANT vt_prop;
    HRESULT hr = obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      result.push_back(vt_prop.uintVal);
    }
    VariantClear(&vt_prop);
  } while (hr == WBEM_S_NO_ERROR);
  return result;
}

template <>
std::vector<unsigned short> query(const std::wstring& wmi_class, const std::wstring& field,
                                  const std::wstring& filter) {
  std::vector<unsigned short> result;
  _WMI wmi;
  std::wstring filter_string;
  if (!filter.empty()) {
    filter_string.append(L" WHERE " + filter);
  }
  std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class + filter_string);
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return {};
  }

  ComPtr<IWbemClassObject> obj;
  HRESULT hr;
  do {
    ULONG u_return = 0;
    hr = wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
    if (FAILED(hr)) {
      break;
    }
    VARIANT vt_prop;
    HRESULT hr = obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      result.push_back(vt_prop.uiVal);
    }
    VariantClear(&vt_prop);
  } while (hr == WBEM_S_NO_ERROR);
  return result;
}

template <>
std::vector<long long> query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter) {
  std::vector<long long> result;
  _WMI wmi;
  std::wstring filter_string;
  if (!filter.empty()) {
    filter_string.append(L" WHERE " + filter);
  }
  std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class + filter_string);
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return {};
  }

  ComPtr<IWbemClassObject> obj;
  HRESULT hr;
  do {
    ULONG u_return = 0;
    hr = wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
    if (FAILED(hr)) {
      break;
    }
    VARIANT vt_prop;
    HRESULT hr = obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      result.push_back(vt_prop.llVal);
    }
    VariantClear(&vt_prop);
  } while (hr == WBEM_S_NO_ERROR);
  return result;
}

template <>
std::vector<unsigned long long> query(const std::wstring& wmi_class, const std::wstring& field,
                                      const std::wstring& filter) {
  std::vector<unsigned long long> result;
  _WMI wmi;
  std::wstring filter_string;
  if (!filter.empty()) {
    filter_string.append(L" WHERE " + filter);
  }
  std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class + filter_string);
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return {};
  }

  ComPtr<IWbemClassObject> obj;
  HRESULT hr;
  do {
    ULONG u_return = 0;
    hr = wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
    if (FAILED(hr)) {
      break;
    }
    VARIANT vt_prop;
    HRESULT hr = obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      result.push_back(vt_prop.ullVal);
    }
    VariantClear(&vt_prop);
  } while (hr == WBEM_S_NO_ERROR);
  return result;
}

template <>
std::vector<std::string> query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter) {
  _WMI wmi;
  std::wstring filter_string;
  if (!filter.empty()) {
    filter_string.append(L" WHERE " + filter);
  }
  std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class + filter_string);
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return {};
  }
  std::vector<std::string> result;

  ComPtr<IWbemClassObject> obj;
  HRESULT hr;
  do {
    ULONG u_return = 0;
    hr = wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
    if (FAILED(hr)) {
      break;
    }
    VARIANT vt_prop;
    HRESULT hr = obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      result.push_back(wstring_to_std_string(vt_prop.bstrVal));
    }
    VariantClear(&vt_prop);
  } while (hr == WBEM_S_NO_ERROR);
  return result;
}

}  // namespace WMI
}  // namespace utils
}  // namespace hwinfo

#endif  // HWINFO_WINDOWS
