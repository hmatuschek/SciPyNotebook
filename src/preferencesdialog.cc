/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011, 2012 Hannes Matuschek <hmatuschek AT gmail DOT com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#include "preferencesdialog.hh"
#include "preferences.hh"
#include "pythonhighlighter.hh"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFontComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QTabWidget>
#include <QLabel>



PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent)
{
  setWindowTitle("Preferences");

  QVBoxLayout *layout = new QVBoxLayout(this);
  QTabWidget *tabs = new QTabWidget(this);

  /*
   * Assemble "Editor" tab.
   */
  {
    QFrame *editorFrame = new QFrame(tabs);
    QVBoxLayout *vbox = new QVBoxLayout();
    editorFrame->setLayout(vbox);

    /*
     * Font configuration
     */
    {
      QGroupBox *gbox = new QGroupBox("Font Settings", this);
      QHBoxLayout *hbox = new QHBoxLayout();
      gbox->setLayout(hbox);

      // Assemble font selection:
      _fontBox = new QFontComboBox(this);
      _fontBox->setCurrentFont(Preferences::get()->font());
      hbox->addWidget(_fontBox);

      // Assemble size selection box.
      _fontSizeBox = new QSpinBox(this);
      _fontSizeBox->setRange(6, 24);
      _fontSizeBox->setValue(Preferences::get()->font().pointSize());
      hbox->addWidget(_fontSizeBox);

      vbox->addWidget(gbox);
    }

    /*
     * Assemble tabsize config
     */
    {
      QGroupBox *gbox = new QGroupBox("Tab Size", this);
      QHBoxLayout *hbox = new QHBoxLayout();
      gbox->setLayout(hbox);

      _tabSizeBox = new QSpinBox(this);
      _tabSizeBox->setRange(0, 10);
      _tabSizeBox->setValue(Preferences::get()->tabSize());
      hbox->addWidget(_tabSizeBox);

      vbox->addWidget(gbox);
    }

    /*
     * Assemble auto-completion config
     */
    {
      QGroupBox *gbox = new QGroupBox("Auto Completion", this);
      QBoxLayout *hbox = new QVBoxLayout();
      gbox->setLayout(hbox);

      _autoComplEnabledBox = new QCheckBox("Auto completion enabled", this);
      _autoComplEnabledBox->setTristate(false);
      if (Preferences::get()->autoCompletion()) {
        _autoComplEnabledBox->setCheckState(Qt::Checked);
      } else {
        _autoComplEnabledBox->setCheckState(Qt::Unchecked);
      }
      hbox->addWidget(_autoComplEnabledBox);

      QBoxLayout *thres_box = new QHBoxLayout();
      _autoComplThresBox = new QSpinBox(this);
      _autoComplThresBox->setRange(3, 100);
      _autoComplThresBox->setValue(Preferences::get()->autoCompletionThreshold());
      thres_box->addWidget(new QLabel("Auto completion threshold", this));
      thres_box->addWidget(_autoComplThresBox);
      hbox->addLayout(thres_box);

      vbox->addWidget(gbox);
    }

    // Add "editor frame" to tabwidget:
    tabs->addTab(editorFrame, "Editor");
  }

  {
    QFrame *preFrame = new QFrame(tabs);
    QVBoxLayout *vbox = new QVBoxLayout();
    preFrame->setLayout(vbox);

    _preambleBox = new QTextEdit(this);
    new PythonHighlighter(_preambleBox);
    _preambleBox->document()->setPlainText(Preferences::get()->preamble());
    vbox->addWidget(_preambleBox);

    // add preamble frame to tabs.
    tabs->addTab(preFrame, "Preamble");
  }

  this->layout()->addWidget(tabs);

  // Create default buttons
  QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                   Qt::Horizontal, this);

  // Connect to dialog
  QObject::connect(buttons, SIGNAL(accepted()), this, SLOT(onAccept()));
  QObject::connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

  // Add buttons to layout
  layout->addWidget(buttons);

  // Set layout of dialog
  setLayout(layout);
}


void
PreferencesDialog::onAccept()
{
  // Get preferences:
  Preferences *preferences = Preferences::get();

  // Save font;
  QFont font(_fontBox->currentFont());
  font.setPointSize(_fontSizeBox->value());
  preferences->setFont(font);

  // Save preamble:
  preferences->setPreamble(_preambleBox->document()->toPlainText());

  // Save tab size:
  preferences->setTabSize(_tabSizeBox->value());

  // Save autocompletion:
  preferences->setAutoCompletion(_autoComplEnabledBox->isChecked());
  preferences->setAutoCompletionThreshold(_autoComplThresBox->value());

  accept();
}
