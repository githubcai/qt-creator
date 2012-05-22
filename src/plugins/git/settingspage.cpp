/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "settingspage.h"
#include "gitsettings.h"
#include "gitplugin.h"
#include "gitclient.h"

#include <vcsbase/vcsbaseconstants.h>
#include <utils/pathchooser.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QTextStream>
#include <QProcessEnvironment>
#include <QMessageBox>

namespace Git {
namespace Internal {

SettingsPageWidget::SettingsPageWidget(QWidget *parent) :
    QWidget(parent)
{
    m_ui.setupUi(this);
#ifdef Q_OS_WIN
    const QByteArray currentHome = qgetenv("HOME");
    const QString toolTip
            = tr("Set the environment variable HOME to '%1'\n(%2).\n"
                 "This causes msysgit to look for the SSH-keys in that location\n"
                 "instead of its installation directory when run outside git bash.").
              arg(QDir::homePath(),
                  currentHome.isEmpty() ? tr("not currently set") :
                                        tr("currently set to '%1'").arg(QString::fromLocal8Bit(currentHome)));
    m_ui.winHomeCheckBox->setToolTip(toolTip);
#else
    m_ui.winHomeCheckBox->setVisible(false);
#endif
    m_ui.repBrowserCommandPathChooser->setExpectedKind(Utils::PathChooser::ExistingCommand);
    m_ui.repBrowserCommandPathChooser->setPromptDialogTitle(tr("Git Repository Browser Command"));
}

GitSettings SettingsPageWidget::settings() const
{
    GitSettings rc;
    rc.setValue(GitSettings::pathKey, m_ui.pathLineEdit->text());
    rc.setValue(GitSettings::logCountKey, m_ui.logCountSpinBox->value());
    rc.setValue(GitSettings::timeoutKey, m_ui.timeoutSpinBox->value());
    rc.setValue(GitSettings::pullRebaseKey, m_ui.pullRebaseCheckBox->isChecked());
    rc.setValue(GitSettings::promptOnSubmitKey, m_ui.promptToSubmitCheckBox->isChecked());
    rc.setValue(GitSettings::winSetHomeEnvironmentKey, m_ui.winHomeCheckBox->isChecked());
    rc.setValue(GitSettings::gitkOptionsKey, m_ui.gitkOptionsLineEdit->text().trimmed());
    rc.setValue(GitSettings::repositoryBrowserCmd, m_ui.repBrowserCommandPathChooser->path().trimmed());
    return rc;
}

void SettingsPageWidget::setSettings(const GitSettings &s)
{
    m_ui.pathLineEdit->setText(s.stringValue(GitSettings::pathKey));
    m_ui.logCountSpinBox->setValue(s.intValue(GitSettings::logCountKey));
    m_ui.timeoutSpinBox->setValue(s.intValue(GitSettings::timeoutKey));
    m_ui.pullRebaseCheckBox->setChecked(s.boolValue(GitSettings::pullRebaseKey));
    m_ui.promptToSubmitCheckBox->setChecked(s.boolValue(GitSettings::promptOnSubmitKey));
    m_ui.winHomeCheckBox->setChecked(s.boolValue(GitSettings::winSetHomeEnvironmentKey));
    m_ui.gitkOptionsLineEdit->setText(s.stringValue(GitSettings::gitkOptionsKey));
    m_ui.repBrowserCommandPathChooser->setPath(s.stringValue(GitSettings::repositoryBrowserCmd));
}

QString SettingsPageWidget::searchKeywords() const
{
    QString rc;
    QLatin1Char sep(' ');
    QTextStream(&rc)
            << sep << m_ui.pathlabel->text()
            << sep << m_ui.winHomeCheckBox->text()
            << sep << m_ui.groupBox->title()
            << sep << m_ui.logCountLabel->text()
            << sep << m_ui.timeoutLabel->text()
            << sep << m_ui.promptToSubmitCheckBox->text()
            << sep << m_ui.promptToSubmitCheckBox->text()
            << sep << m_ui.gitkGroupBox->title()
            << sep << m_ui.gitkOptionsLabel->text()
            << sep << m_ui.repBrowserGroupBox->title()
            << sep << m_ui.repBrowserCommandLabel->text()
               ;
    rc.remove(QLatin1Char('&'));
    return rc;
}

// -------- SettingsPage
SettingsPage::SettingsPage() :
    m_widget(0)
{
    setId(QLatin1String(VcsBase::Constants::VCS_ID_GIT));
    setDisplayName(tr("Git"));
}

QWidget *SettingsPage::createPage(QWidget *parent)
{
    m_widget = new SettingsPageWidget(parent);
    m_widget->setSettings(GitPlugin::instance()->settings());
    if (m_searchKeywords.isEmpty())
        m_searchKeywords = m_widget->searchKeywords();
    return m_widget;
}

void SettingsPage::apply()
{
    const GitSettings newSettings = m_widget->settings();
    // Warn if git cannot be found in path if the widget is on top
    if (m_widget->isVisible()) {
        bool gitFoundOk;
        QString errorMessage;
        newSettings.gitBinaryPath(&gitFoundOk, &errorMessage);
        if (!gitFoundOk)
            QMessageBox::warning(m_widget, tr("Git Settings"), errorMessage);
    }

    GitPlugin::instance()->setSettings(newSettings);
}

bool SettingsPage::matches(const QString &s) const
{
    return m_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}
}
