/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtGui/QIcon>

#include "Config.h"
#include "core/Common.h"
#include "core/Resources.h"
#include "core/settings/Settings.h"

QString Tano::Resources::path(const QString &file)
{
    QString path;
    if (resource(file).isEmpty())
        path = QFileInfo(file).absolutePath();
    else
        path = QFileInfo(resource(file)).absolutePath();

    return path;
}

QString Tano::Resources::recordings()
{
    return settingsPath() + "recordings.xml";
}

QString Tano::Resources::resource(const QString &file)
{
    QString path;

    if (QFileInfo(file).exists())
        path = QFileInfo(file).absoluteFilePath();

    // Try application exe working path
    else if (QFileInfo(QDir::currentPath() + "/" + file).exists())
        path = QFileInfo(QDir::currentPath() + "/" + file).absoluteFilePath();

    // Try application exe directory
    else if (QFileInfo(QCoreApplication::applicationDirPath() + "/" + file).exists())
        path = QFileInfo(QCoreApplication::applicationDirPath() + "/" + file).absoluteFilePath();

    // Try application exe directory without src for development
    else if (QFileInfo(QCoreApplication::applicationDirPath().replace("/src", "") + "/" + file).exists())
        path = QFileInfo(QCoreApplication::applicationDirPath().replace("/src", "") + "/" + file).absoluteFilePath();

#if defined(Q_OS_UNIX)
    else if (QFileInfo("/usr/bin/" + file).exists())
        path = QFileInfo("/usr/bin/" + file).absoluteFilePath();
#endif

#if defined(DATA_DIR)
    else if (QFileInfo(QString(DATA_DIR) + "/" + file).exists())
        path = QFileInfo(QString(DATA_DIR) + "/" + file).absoluteFilePath();
#endif

    return path;
}

QString Tano::Resources::settingsPath()
{
    QScopedPointer<QSettings> settings(
            new QSettings(QSettings::IniFormat,
                          QSettings::UserScope,
                          Tano::nameGroup(),
                          Tano::nameShort()));
    QString path = settings->fileName().replace(Tano::nameShort() + ".ini", "");

    return path;
}

void Tano::Resources::setIconName()
{
    QScopedPointer<Settings> settings(new Settings());
    if (!settings->icons().isEmpty())
        QIcon::setThemeName(settings->icons());

    qDebug() << "Icon theme:" << QIcon::themeName();
}

void Tano::Resources::setIconPaths()
{
    QStringList paths = QIcon::themeSearchPaths();
    paths.prepend(QCoreApplication::applicationDirPath() + "/" + "icons");
#if defined(DATA_DIR)
    paths.prepend(QString(DATA_DIR) + "/" + "icons");
#endif

    QIcon::setThemeSearchPaths(paths);

    qDebug() << "Icon theme search paths:" << QIcon::themeSearchPaths();
}
