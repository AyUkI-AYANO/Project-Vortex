#include "ImageController.h"

#include <QBuffer>
#include <QCoreApplication>
#include <QDir>
#include <QTransform>

ImageController::ImageController(QObject* parent) : QObject(parent) {
    const QString modulesPath = QCoreApplication::applicationDirPath() + "/modules";
    const QStringList loaded = modules_.loadFromDirectory(modulesPath);
    emit statusMessage(QString("已加载模块数量: %1").arg(loaded.size()));
}

bool ImageController::openImage(const QString& path) {
    QImage loaded(path);
    if (loaded.isNull()) {
        emit statusMessage("打开图像失败");
        return false;
    }

    image_ = loaded.convertToFormat(QImage::Format_RGBA8888);
    publishImage();
    emit statusMessage("图像已打开");
    return true;
}

bool ImageController::saveImage(const QString& path) {
    if (image_.isNull()) {
        emit statusMessage("没有可保存的图像");
        return false;
    }

    if (!image_.save(path)) {
        emit statusMessage("保存失败");
        return false;
    }

    emit statusMessage("保存成功");
    return true;
}

void ImageController::rotate90Clockwise() {
    if (image_.isNull()) {
        return;
    }

    image_ = image_.transformed(QTransform().rotate(90));
    publishImage();
    emit statusMessage("已顺时针旋转 90°");
}

void ImageController::crop(int x, int y, int width, int height) {
    if (image_.isNull() || width <= 0 || height <= 0) {
        return;
    }

    const QRect validRect = QRect(0, 0, image_.width(), image_.height()).intersected(QRect(x, y, width, height));
    if (validRect.isEmpty()) {
        emit statusMessage("裁切区域无效");
        return;
    }

    image_ = image_.copy(validRect);
    publishImage();
    emit statusMessage("裁切完成");
}

void ImageController::adjustExposure(double factor) {
    if (image_.isNull()) {
        return;
    }

    if (factor < 0.0) {
        factor = 0.0;
    }

    for (int y = 0; y < image_.height(); ++y) {
        auto* row = image_.scanLine(y);
        for (int x = 0; x < image_.width(); ++x) {
            const int i = x * 4;
            row[i] = static_cast<uchar>(qBound(0, static_cast<int>(row[i] * factor), 255));
            row[i + 1] = static_cast<uchar>(qBound(0, static_cast<int>(row[i + 1] * factor), 255));
            row[i + 2] = static_cast<uchar>(qBound(0, static_cast<int>(row[i + 2] * factor), 255));
        }
    }

    publishImage();
    emit statusMessage("曝光调整完成");
}

QStringList ImageController::listModules() {
    return modules_.moduleNames();
}

bool ImageController::applyModule(const QString& moduleId) {
    if (image_.isNull()) {
        emit statusMessage("请先加载图像");
        return false;
    }

    VortexImage buffer{image_.width(), image_.height(), 4, image_.bits()};
    QString error;
    if (!modules_.applyModule(moduleId, &buffer, error)) {
        emit statusMessage(error);
        return false;
    }

    publishImage();
    emit statusMessage(QString("模块执行成功: %1").arg(moduleId));
    return true;
}

void ImageController::publishImage() {
    if (image_.isNull()) {
        return;
    }

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    image_.save(&buffer, "PNG");
    const QString dataUrl = QString("data:image/png;base64,%1").arg(QString::fromLatin1(bytes.toBase64()));
    emit imageUpdated(dataUrl, image_.width(), image_.height());
}
