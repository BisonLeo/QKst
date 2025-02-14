/***************************************************************************
 *                                                                         *
 *   copyright : (C) 2008 The University of Toronto                        *
 *                   netterfield@astro.utoronto.ca                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "labelcreator.h"

#include "application.h"
#include "objectstore.h"
#include "QKst.h"
#include "document.h"
#include "geticon.h"

#include "applicationsettings.h"
#include "dialogdefaults.h"


namespace Kst {

LabelCreator::LabelCreator(QWidget *parent)
  : QDialog(parent) {

  setupUi(this);

  setWindowTitle(tr("Create Label Dialog"));
  int h = fontMetrics().lineSpacing();
  _bold->setFixedWidth(h);
  _bold->setFixedHeight(h);
  _bold->setIcon(KstGetIcon("kst_bold"));
  _italic->setFixedWidth(h);
  _italic->setFixedHeight(h);
  _italic->setIcon(KstGetIcon("kst_italic"));
  _labelColor->setFixedWidth(h);
  _labelColor->setFixedHeight(h);
  _labelText->setObjectStore(kstApp->mainWindow()->document()->objectStore());

  QFont font;
  font.fromString(dialogDefaults().value("label/font",font.toString()).toString());
  _family->setCurrentFont(font);
  _bold->setChecked(font.bold());
  _italic->setChecked(font.italic());

  _labelColor->setColor(dialogDefaults().value("label/color",QColor(Qt::black)).value<QColor>());
  _labelFontScale->setValue(dialogDefaults().value("label/fontScale",12).toDouble());
  if (dialogDefaults().value("label/fixLeft",true).toBool()) {
    _left->setChecked(true);
  } else {
    _right->setChecked(true);
  }
  _lockPosToData->setChecked(dialogDefaultsLockPosToData("label"));
  _saveAsDefault->show();

}


LabelCreator::~LabelCreator() {
}


QString LabelCreator::labelText() {
  return _labelText->labelText();
}


qreal LabelCreator::labelScale() const { 
  return _labelFontScale->value(); 
}


QColor LabelCreator::labelColor() const { 
  return _labelColor->color();
}


QFont LabelCreator::labelFont() const {
  QFont font(_family->currentFont());
  font.setItalic(_italic->isChecked());
  font.setBold(_bold->isChecked());
  return font;
}

bool LabelCreator::fixLeft() const {
  return _left->isChecked();
}
bool LabelCreator::lockPosToData() const {
  return _lockPosToData->isChecked();
}

}

// vim: ts=2 sw=2 et
