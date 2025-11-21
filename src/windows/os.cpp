// Copyright (c) Leon Freist <freist@informatik.uni-freiburg.de>
// This software is part of HWBenchmark

#include <hwinfo/platform.h>

#ifdef HWINFO_WINDOWS
#include <Windows.h>
#include <hwinfo/os.h>
#include <hwinfo/utils/stringutils.h>
#include <hwinfo/utils/wmi_wrapper.h>

#include <sstream>
#include <string>

namespace hwinfo {

// _____________________________________________________________________________________________________________________
OS::OS() {
  {
    // Get endian. This is platform independent...
    char16_t dummy = 0x0102;
    _bigEndian = ((char*)&dummy)[0] == 0x01;
    _littleEndian = ((char*)&dummy)[0] == 0x02;
  }
  utils::WMI::_WMI wmi;
  const std::wstring query_string(L"SELECT Caption, OSArchitecture, BuildNumber, Version FROM Win32_OperatingSystem");
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return;
  }
  ULONG u_return = 0;
  Microsoft::WRL::ComPtr<IWbemClassObject> obj;
  HRESULT hr = wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
  if (FAILED(hr)) {
    return;
  }
  VARIANT vt_prop;
  hr = obj->Get(L"Caption", 0, &vt_prop, nullptr, nullptr);
  if (SUCCEEDED(hr) && (V_VT(&vt_prop) == VT_BSTR)) {
    _name = utils::wstring_to_std_string(vt_prop.bstrVal);
  }
  hr = obj->Get(L"OSArchitecture", 0, &vt_prop, nullptr, nullptr);
  if (SUCCEEDED(hr) && (V_VT(&vt_prop) == VT_BSTR)) {
    _64bit = utils::wstring_to_std_string(vt_prop.bstrVal).find("64") != std::string::npos;
    _32bit = !_64bit;
  }
  hr = obj->Get(L"BuildNumber", 0, &vt_prop, nullptr, nullptr);
  if (SUCCEEDED(hr) && (V_VT(&vt_prop) == VT_BSTR)) {
    _version = utils::wstring_to_std_string(vt_prop.bstrVal);
  }
  hr = obj->Get(L"Version", 0, &vt_prop, nullptr, nullptr);
  if (SUCCEEDED(hr) && (V_VT(&vt_prop) == VT_BSTR)) {
    _kernel = utils::wstring_to_std_string(vt_prop.bstrVal);
  }
  VariantClear(&vt_prop);
}

}  // namespace hwinfo

#endif  // HWINFO_WINDOWS
