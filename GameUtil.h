#pragma once

#include <QString>
#include <QIcon>
#include <QMap>

/**
 * @brief Utility functions for reading files.
 */
class FileUtil {
public:
    /**
     * @brief Reads the entire contents of a text file.
     *
     * @param path The file path to read from.
     * @return The file content as a QString. Returns an empty string
     * if the file cannot be opened.
     */
    static QString readAll(const QString &path);
};

/**
 * @brief Utility functions for loading and processing icons with caching.
 */
class IconUtil {
public:
    /**
     * @brief Loads an icon from the given file path with caching.
     *
     * If the icon has been loaded previously, the cached version is returned.
     *
     * @param path The file path of the icon.
     * @return A const reference to the loaded (and possibly cached) QIcon.
     */
    static const QIcon &load(const QString &path);

    /**
     * @brief Clears icon cache to free up memory.
     */
    static void clearCache();

private:
    static QMap<QString, QIcon> cache;
};
