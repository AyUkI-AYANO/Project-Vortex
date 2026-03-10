#pragma once

#include "ModuleManager.h"

#include <QObject>
#include <QImage>

class ImageController : public QObject {
    Q_OBJECT
public:
    explicit ImageController(QObject* parent = nullptr);

public slots:
    bool openImage(const QString& path);
    bool saveImage(const QString& path);
    void rotate90Clockwise();
    void crop(int x, int y, int width, int height);
    void adjustExposure(double factor);
    QStringList listModules();
    bool applyModule(const QString& moduleId);

signals:
    void imageUpdated(const QString& dataUrl, int width, int height);
    void statusMessage(const QString& message);

private:
    void publishImage();

    QImage image_;
    ModuleManager modules_;
};
