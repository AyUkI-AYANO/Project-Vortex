#include "ModuleManager.h"

#include <QDir>
#include <QFileInfo>

QStringList ModuleManager::loadFromDirectory(const QString& directoryPath) {
    modules_.clear();

    QDir dir(directoryPath);
    QStringList loaded;
    if (!dir.exists()) {
        return loaded;
    }

#if defined(_WIN32)
    const QStringList filters{"*.dll"};
#elif defined(__APPLE__)
    const QStringList filters{"*.dylib"};
#else
    const QStringList filters{"*.so"};
#endif

    const QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
    for (const QFileInfo& file : files) {
        LoadedModule mod;
        mod.filePath = file.absoluteFilePath();
        mod.library.setFileName(mod.filePath);

        if (!mod.library.load()) {
            continue;
        }

        auto registerFn = reinterpret_cast<RegisterModuleFn>(mod.library.resolve("register_module"));
        if (!registerFn) {
            mod.library.unload();
            continue;
        }

        const ModuleDescriptor* desc = registerFn();
        if (!desc || !desc->id || !desc->apply) {
            mod.library.unload();
            continue;
        }

        mod.descriptor = desc;
        loaded << QString::fromUtf8(desc->name ? desc->name : desc->id);
        modules_.push_back(std::move(mod));
    }

    return loaded;
}

QStringList ModuleManager::moduleNames() const {
    QStringList names;
    for (const auto& module : modules_) {
        names << QString::fromUtf8(module.descriptor->name ? module.descriptor->name : module.descriptor->id);
    }
    return names;
}

bool ModuleManager::applyModule(const QString& moduleId, VortexImage* image, QString& errorMessage) {
    for (auto& module : modules_) {
        const QString id = QString::fromUtf8(module.descriptor->id);
        const QString name = QString::fromUtf8(module.descriptor->name ? module.descriptor->name : module.descriptor->id);
        if (moduleId == id || moduleId == name) {
            if (!module.descriptor->apply(image)) {
                errorMessage = QString("模块执行失败: %1").arg(name);
                return false;
            }
            return true;
        }
    }

    errorMessage = QString("未找到模块: %1").arg(moduleId);
    return false;
}
