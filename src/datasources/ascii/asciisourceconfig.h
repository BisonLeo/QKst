/***************************************************************************
 *                                                                         *
 *   Copyright : (C) 2010 The University of Toronto                        *
 *   email     : netterfield@astro.utoronto.ca                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KST_ASCII_SOURCE_CONFIG_H
#define KST_ASCII_SOURCE_CONFIG_H

#include "namedparameter.h"

#include <QDomElement>
#include <QDateTime>


class AsciiSourceConfig {

    // TODO translate keys?
    static const char Key_fileNamePattern[];
    static const char Key_delimiters[];
    static const char Key_indexVector[];
    static const char Key_indexInterpretation[];
    static const char Key_timeAsciiFormatString[];
    static const char Key_columnType[];
    static const char Key_columnDelimiter[];
    static const char Key_columnWidth[];
    static const char Key_columnWidthIsConst[];
    static const char Key_dataLine[];
    static const char Key_readFields[];
    static const char Key_readUnits[];
    static const char Key_useDot[];
    static const char Key_fieldsLine[];
    static const char Key_unitsLine[];
    static const char Key_limitFileBuffer[];
    static const char Key_limitFileBufferSize[];
    static const char Key_useThreads[];
    static const char Key_dataRate[];
    static const char Key_offsetDateTime[];
    static const char Key_offsetFileDate[];
    static const char Key_offsetRelative[];
    static const char Key_dateTimeOffset[];
    static const char Key_relativeOffset[];
    static const char Key_nanValue[];

  public:
    AsciiSourceConfig();

    bool operator==(const AsciiSourceConfig&) const;
    bool operator!=(const AsciiSourceConfig& rhs) const;
    bool isUpdateNecessary(const AsciiSourceConfig& rhs) const;

    void saveGroup(QSettings& cfg, const QString& fileName = QString()) const;
    void readGroup(QSettings& cfg, const QString& fileName = QString());

    void save(QXmlStreamWriter& s);
    void parseProperties(QXmlStreamAttributes &properties);

    void load(const QDomElement& e);

    enum Interpretation { Unknown = 0, NoInterpretation, CTime, Seconds, FormattedTime, FixedRate, IntEnd = 0xffff };
    enum ColumnType { Whitespace = 0, Fixed, Custom, ColEnd = 0xffff };

    NamedParameter<QString, Key_delimiters> _delimiters;
    NamedParameter<QString, Key_indexVector> _indexVector;
    NamedParameter<int, Key_indexInterpretation> _indexInterpretation;
    NamedParameter<QString, Key_timeAsciiFormatString> _timeAsciiFormatString;
    NamedParameter<QString, Key_fileNamePattern> _fileNamePattern;
    NamedParameter<int, Key_columnType> _columnType;
    NamedParameter<QString, Key_columnDelimiter> _columnDelimiter;
    NamedParameter<int, Key_columnWidth> _columnWidth;
    NamedParameter<bool, Key_columnWidthIsConst> _columnWidthIsConst;
    NamedParameter<int, Key_dataLine> _dataLine;
    NamedParameter<bool, Key_readFields> _readFields;
    NamedParameter<bool, Key_readUnits> _readUnits;
    NamedParameter<int, Key_fieldsLine> _fieldsLine;
    NamedParameter<int, Key_unitsLine> _unitsLine;
    NamedParameter<bool, Key_useDot> _useDot;
    NamedParameter<bool, Key_limitFileBuffer> _limitFileBuffer;
    NamedParameter<qint64, Key_limitFileBufferSize> _limitFileBufferSize;
    NamedParameter<int, Key_useThreads> _useThreads;
    NamedParameter<double, Key_dataRate> _dataRate;
    NamedParameter<bool, Key_offsetDateTime> _offsetDateTime;
    NamedParameter<bool, Key_offsetFileDate> _offsetFileDate;
    NamedParameter<bool, Key_offsetRelative> _offsetRelative;
    NamedParameter<QDateTime, Key_dateTimeOffset> _dateTimeOffset;
    NamedParameter<double, Key_relativeOffset> _relativeOffset;
    NamedParameter<int, Key_nanValue> _nanValue;

  private:
    void save(QSettings& cfg) const;
    void read(QSettings& cfg);
    QString typeKey() const;
};

Q_DECLARE_METATYPE(AsciiSourceConfig::Interpretation)
Q_DECLARE_METATYPE(AsciiSourceConfig::ColumnType)

#endif
