#pragma once

#include "module_api.h"

#include <QLibrary>
#include <QString>
#include <QStringList>
#include <vector>

struct LoadedModule {
    QString filePath;
    QLibrary library;
    const ModuleDescriptor* descriptor{nullptr};
};

class ModuleManager {
public:
    QStringList loadFromDirectory(const QString& directoryPath);
    QStringList moduleNames() const;
    bool applyModule(const QString& moduleId, VortexImage* image, QString& errorMessage);

private:
    std::vector<LoadedModule> modules_;
};
