#include "CefWnd.h"
#include "QCefWidget.h"
#include "QCefOpenGLWidget.h"
#include <QtWidgets>

CefWnd::CefWnd(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , framelessWindow_(false)
    , translucentWindowBackground_(false)
    , usingGLWidget_(false)
    , contextMenuEnabled_(false)
    , autoAddDevToolsContextMenu_(false)
    , osrEnabled_(false)
    , maximumFps_(25)
    , windowBkColor_(255, 255, 255, 255)
    , browserBkColor_(255, 255, 255, 255)
    , pCefWidget_(nullptr)
    , pCefGLWidget_(nullptr) {}

CefWnd::~CefWnd() {}

void CefWnd::setupUi() {
  if (framelessWindow_) {
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::FramelessWindowHint);
  }

  if (translucentWindowBackground_) {
    setAttribute(Qt::WA_TranslucentBackground);
  }

  this->setObjectName("CefWnd");
  this->setWindowIcon(QIcon(":/QCefWidgetTest/images/logo.png"));

  QString windowQss = QString("background-color: rgba(%1,%2,%3,%4);")
    .arg(windowBkColor_.red()).arg(windowBkColor_.green()).arg(windowBkColor_.blue()).arg(windowBkColor_.alpha());
  this->setStyleSheet(windowQss);

  if (usingGLWidget_) {
    pCefGLWidget_ = new QCefOpenGLWidget(initUrl_);
    pCefGLWidget_->setObjectName("cefWidget");
    pCefGLWidget_->setOsrEnabled(osrEnabled_);
    pCefGLWidget_->setContextMenuEnabled(contextMenuEnabled_);
    pCefGLWidget_->setAutoShowDevToolsContextMenu(autoAddDevToolsContextMenu_);
    pCefGLWidget_->setBrowserBackgroundColor(browserBkColor_);

    connect(pCefGLWidget_, &QCefOpenGLWidget::invokeMethodNotify, this, &CefWnd::onInvokeMethodNotify);
  }
  else {
    pCefWidget_ = new QCefWidget(initUrl_);
    pCefWidget_->setObjectName("cefWidget");
    pCefWidget_->setOsrEnabled(osrEnabled_);
    pCefWidget_->setContextMenuEnabled(contextMenuEnabled_);
    pCefWidget_->setAutoShowDevToolsContextMenu(autoAddDevToolsContextMenu_);
    pCefWidget_->setBrowserBackgroundColor(browserBkColor_);

    connect(pCefWidget_, &QCefWidget::invokeMethodNotify, this, &CefWnd::onInvokeMethodNotify);
  }

  QHBoxLayout *hlMain = new QHBoxLayout();
  hlMain->setContentsMargins(0, 0, 0, 0);
  hlMain->setSpacing(0);
  if (usingGLWidget_)
    hlMain->addWidget(pCefGLWidget_);
  else
    hlMain->addWidget(pCefWidget_);

  this->setLayout(hlMain);

  this->resize(sizeHint());
}

bool CefWnd::usingGLWidget() { return usingGLWidget_; }

void CefWnd::setUsingGLWidget(bool b) { usingGLWidget_ = b; }

bool CefWnd::framelessWindow() { return framelessWindow_; }

void CefWnd::setFramelessWindow(bool b) { framelessWindow_ = b; }

bool CefWnd::translucentWindowBackground() { return translucentWindowBackground_; }

void CefWnd::setTranslucentWindowBackground(bool b) { translucentWindowBackground_ = b; }

bool CefWnd::osrEnabled() { return osrEnabled_; }

void CefWnd::setOsrEnabled(bool b) { osrEnabled_ = b; }

bool CefWnd::contextMenuEnabled() { return contextMenuEnabled_; }

void CefWnd::setContextMenuEnabled(bool b) { contextMenuEnabled_ = b; }

bool CefWnd::autoAddDevToolsContextMenu() { return autoAddDevToolsContextMenu_; }

void CefWnd::setAutoAddDevToolsContextMenu(bool b) { autoAddDevToolsContextMenu_ = b; }

QSize CefWnd::initSize() { return initSize_; }

void CefWnd::setInitSize(QSize s) { initSize_ = s; }

QString CefWnd::initUrl() { return initUrl_; }

void CefWnd::setInitUrl(QString u) { initUrl_ = u; }

int CefWnd::maximumFps() { return maximumFps_; }

void CefWnd::setMaximumFps(int f) { maximumFps_ = f; }

QColor CefWnd::windowBkColor() { return windowBkColor_; }

void CefWnd::setWindowBkColor(QColor c) { windowBkColor_ = c; }

QColor CefWnd::browserBkColor() { return browserBkColor_; }

void CefWnd::setBrowserBkColor(QColor c) { browserBkColor_ = c; }

QSize CefWnd::sizeHint() const { return initSize_; }

void CefWnd::onNavigateToUrl(QString url) {
  if (usingGLWidget_) {
    if (pCefGLWidget_)
      pCefGLWidget_->navigateToUrl(url);
  }
  else {
    if (pCefWidget_)
      pCefWidget_->navigateToUrl(url);
  }
}

void CefWnd::onReload() {
  if (usingGLWidget_) {
    if (pCefGLWidget_)
      pCefGLWidget_->reloadBrowser();
  }
  else {
    if (pCefWidget_)
      pCefWidget_->reloadBrowser();
  }
}

void CefWnd::onShowDevTools() {
  if (usingGLWidget_) {
    if (pCefGLWidget_)
      pCefGLWidget_->showDevTools();
  }
  else {
    if (pCefWidget_)
      pCefWidget_->showDevTools();
  }
}

void CefWnd::onCloseDevTools() {
  if (usingGLWidget_) {
    if (pCefGLWidget_)
      pCefGLWidget_->closeDevTools();
  }
  else {
    if (pCefWidget_)
      pCefWidget_->closeDevTools();
  }
}

void CefWnd::onInvokeMethodNotify(int browserId, int frameId, const QString &method, const QVariantList &arguments) {
  QString str = QString("[InvokeMethodNotify] browserId: %1, frameId: %2, method: %3\r\n").arg(browserId).arg(frameId).arg(method);
  QString strArgs;
  for (int i = 0; i < arguments.size(); i++) {
    strArgs += QString("[%1]: %2\r\n").arg(i).arg(arguments[i].toString());
  }
  str += strArgs;
  qDebug() << str;
}
