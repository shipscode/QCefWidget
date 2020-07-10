#include "QCefBrowserApp.h"
#include "QCefGlobalSetting.h"
#include <include/cef_browser.h>
#include <include/cef_command_line.h>
#include <include/wrapper/cef_helpers.h>
#include <string>

QCefBrowserApp::QCefBrowserApp() {}

QCefBrowserApp::~QCefBrowserApp() {}

void QCefBrowserApp::OnBeforeCommandLineProcessing(const CefString &process_type,
                                                      CefRefPtr<CefCommandLine> command_line) {
  // All of switch can find from these:
  //   base/base_switches.cc
  //   cef/libcef/common/cef_switches.cc
  //   chrome/common/chrome_switches.cc (not all apply)
  //   content/public/common/content_switches.cc

  command_line->AppendSwitch("no-proxy-server");
  command_line->AppendSwitch("disable-web-security");
  command_line->AppendSwitch("allow-outdated-plugins");
  command_line->AppendSwitchWithValue("enable-npapi", "1");
  command_line->AppendSwitchWithValue("plugin-policy", "allow");
  command_line->AppendSwitch("allow-file-access-from-files");
  command_line->AppendSwitch("allow-universal-access-from-files");
  command_line->AppendSwitch("disable-spell-checking");

  // Can not disable GPU in D3D mode.
  // D3D mode not be support in CEF 2623 version.
  //
  if (QCefGlobalSetting::gpu_enabled) {
    command_line->AppendSwitch("enable-gpu");
    command_line->AppendSwitch("enable-gpu-compositing");
  }
  else {
    command_line->AppendSwitch("disable-gpu");
    command_line->AppendSwitch("disable-gpu-compositing");
  }

  if (QCefGlobalSetting::flush_plugin_path.length() > 0 &&
      QCefGlobalSetting::flush_plugin_ver.length() > 0) {
    command_line->AppendSwitchWithValue("ppapi-flash-path", QCefGlobalSetting::flush_plugin_path);
    command_line->AppendSwitchWithValue("ppapi-flash-version", QCefGlobalSetting::flush_plugin_ver);
  }
}

void QCefBrowserApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) {}

CefRefPtr<CefResourceBundleHandler> QCefBrowserApp::GetResourceBundleHandler() {
  return nullptr;
}

CefRefPtr<CefBrowserProcessHandler> QCefBrowserApp::GetBrowserProcessHandler() { return this; }

CefRefPtr<CefRenderProcessHandler> QCefBrowserApp::GetRenderProcessHandler() { return nullptr; }

//////////////////////////////////////////////////////////////////////////
void QCefBrowserApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  // Register cookieable schemes with the global cookie manager.
  CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(nullptr);
  DCHECK(manager.get());
  manager->SetSupportedSchemes(cookieable_schemes_, true, nullptr);
}

void QCefBrowserApp::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) {}

void QCefBrowserApp::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) {
  CEF_REQUIRE_IO_THREAD();
}

CefRefPtr<CefPrintHandler> QCefBrowserApp::GetPrintHandler() { return nullptr; }

void QCefBrowserApp::OnScheduleMessagePumpWork(int64 delay_ms) {}