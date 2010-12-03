/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** No Commercial Usage
**
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef FAKEMETAOBJECT_H
#define FAKEMETAOBJECT_H

#include "languageutils_global.h"
#include "componentversion.h"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QHash>

namespace LanguageUtils {

class LANGUAGEUTILS_EXPORT FakeMetaEnum {
    QString m_name;
    QStringList m_keys;
    QList<int> m_values;

public:
    FakeMetaEnum(const QString &name);

    QString name() const;

    void addKey(const QString &key, int value);
    QString key(int index) const;
    int keyCount() const;
    QStringList keys() const;
};

class LANGUAGEUTILS_EXPORT FakeMetaMethod {
public:
    enum {
        Signal,
        Slot,
        Method
    };

    enum {
        Private,
        Protected,
        Public
    };

public:
    FakeMetaMethod(const QString &name, const QString &returnType = QString());

    QString methodName() const;
    QStringList parameterNames() const;
    QStringList parameterTypes() const;
    void addParameter(const QString &name, const QString &type);

    int methodType() const;
    void setMethodType(int methodType);

    int access() const;

private:
    QString m_name;
    QString m_returnType;
    QStringList m_paramNames;
    QStringList m_paramTypes;
    int m_methodTy;
    int m_methodAccess;
};

class LANGUAGEUTILS_EXPORT FakeMetaProperty {
    QString m_propertyName;
    QString m_type;
    bool m_isList;
    bool m_isWritable;
    bool m_isPointer;

public:
    FakeMetaProperty(const QString &name, const QString &type, bool isList, bool isWritable, bool isPointer);

    QString name() const;
    QString typeName() const;

    bool isList() const;
    bool isWritable() const;
    bool isPointer() const;
};

class LANGUAGEUTILS_EXPORT FakeMetaObject {
    Q_DISABLE_COPY(FakeMetaObject);

public:
    class Export {
    public:
        QString package;
        QString type;
        ComponentVersion version;
        QString packageNameVersion;
    };

private:
    QList<Export> m_exports;
    const FakeMetaObject *m_super;
    QString m_superName;
    QList<FakeMetaEnum> m_enums;
    QHash<QString, int> m_enumNameToIndex;
    QList<FakeMetaProperty> m_props;
    QHash<QString, int> m_propNameToIdx;
    QList<FakeMetaMethod> m_methods;
    QString m_defaultPropertyName;

public:
    FakeMetaObject();

    void addExport(const QString &name, const QString &package, ComponentVersion version);
    QList<Export> exports() const;

    void setSuperclassName(const QString &superclass);
    QString superclassName() const;
    void setSuperclass(FakeMetaObject *superClass);
    const FakeMetaObject *superClass() const;

    void addEnum(const FakeMetaEnum &fakeEnum);
    int enumeratorCount() const;
    int enumeratorOffset() const;
    FakeMetaEnum enumerator(int index) const;
    int enumeratorIndex(const QString &name) const;

    void addProperty(const FakeMetaProperty &property);
    int propertyCount() const;
    int propertyOffset() const;
    FakeMetaProperty property(int index) const;
    int propertyIndex(const QString &name) const;

    void addMethod(const FakeMetaMethod &method);
    int methodCount() const;
    int methodOffset() const;
    FakeMetaMethod method(int index) const;

    QString defaultPropertyName() const;
    void setDefaultPropertyName(const QString defaultPropertyName);
};

} // namespace LanguageUtils

#endif // FAKEMETAOBJECT_H
