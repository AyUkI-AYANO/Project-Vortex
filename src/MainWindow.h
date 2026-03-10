#pragma once

#include "ImageController.h"

#include <QMainWindow>

class QWebEngineView;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QWebEngineView* view_{nullptr};
    ImageController controller_;
};
