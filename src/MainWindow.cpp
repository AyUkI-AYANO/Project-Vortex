#include "MainWindow.h"

#include <QWebChannel>
#include <QWebEngineView>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    resize(1200, 760);
    setWindowTitle("Project Vortex 0.1.0");

    view_ = new QWebEngineView(this);
    setCentralWidget(view_);

    auto* channel = new QWebChannel(view_->page());
    channel->registerObject(QStringLiteral("imageController"), &controller_);
    view_->page()->setWebChannel(channel);
    view_->setUrl(QUrl("qrc:/frontend/index.html"));
}
