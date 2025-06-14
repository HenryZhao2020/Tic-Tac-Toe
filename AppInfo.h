#pragma once

#include <QString>
#include <QIcon>

/**
 * @brief Stores metadata about the application.
 */
class AppInfo {
public:
    static QString name();
    static const QIcon &icon();
    static QString version();
    static QString developer();
    static QString description();
    static QString github();

private:
    AppInfo() = delete;     // Prevent instantiation
};
