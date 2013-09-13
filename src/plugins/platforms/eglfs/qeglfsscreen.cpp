/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qeglfscursor.h"
#include "qeglfsscreen.h"
#include "qeglfswindow.h"
#include "qeglfshooks.h"

QT_BEGIN_NAMESPACE

QEglFSScreen::QEglFSScreen(EGLDisplay dpy)
    : m_dpy(dpy)
    , m_surface(EGL_NO_SURFACE)
    , m_cursor(0)
{
#ifdef QEGL_EXTRA_DEBUG
    qWarning("QEglScreen %p\n", this);
#endif

    static int hideCursor = qgetenv("QT_QPA_EGLFS_HIDECURSOR").toInt();
    if (!hideCursor)
        m_cursor = QEglFSHooks::hooks()->createCursor(this);
}

QEglFSScreen::~QEglFSScreen()
{
    delete m_cursor;
}

QRect QEglFSScreen::geometry() const
{
    return QRect(QPoint(0, 0), QEglFSHooks::hooks()->screenSize());
}

int QEglFSScreen::depth() const
{
    return QEglFSHooks::hooks()->screenDepth();
}

QImage::Format QEglFSScreen::format() const
{
    return QEglFSHooks::hooks()->screenFormat();
}

QSizeF QEglFSScreen::physicalSize() const
{
    return QEglFSHooks::hooks()->physicalScreenSize();
}

QDpi QEglFSScreen::logicalDpi() const
{
    return QEglFSHooks::hooks()->logicalDpi();
}

Qt::ScreenOrientation QEglFSScreen::nativeOrientation() const
{
    return QEglFSHooks::hooks()->nativeOrientation();
}

Qt::ScreenOrientation QEglFSScreen::orientation() const
{
    return QEglFSHooks::hooks()->orientation();
}

QPlatformCursor *QEglFSScreen::cursor() const
{
    return m_cursor;
}

void QEglFSScreen::setPrimarySurface(EGLSurface surface)
{
    m_surface = surface;
}

void QEglFSScreen::addWindow(QEglFSWindow *window)
{
    if (!m_windows.contains(window))
        m_windows.append(window);
}

void QEglFSScreen::removeWindow(QEglFSWindow *window)
{
    m_windows.removeOne(window);
}

QEglFSWindow *QEglFSScreen::rootWindow()
{
    Q_FOREACH (QEglFSWindow *window, m_windows) {
        if (window->isRasterRoot())
            return window;
    }
    return 0;
}

QT_END_NAMESPACE
