#include "preferencesdialog.hh"
#include "preferences.hh"
#include "pythonhighlighter.hh"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFontComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QTextEdit>



PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  /*
   * Font configuration
   */
  {
    QGroupBox *gbox = new QGroupBox("Font Settings", this);
    QHBoxLayout *hbox = new QHBoxLayout();
    gbox->setLayout(hbox);

    // Assemble font selection:
    this->_fontBox = new QFontComboBox(this);
    this->_fontBox->setCurrentFont(Preferences::get()->font());
    hbox->addWidget(this->_fontBox);

    // Assemble size selection box.
    this->_fontSizeBox = new QSpinBox(this);
    this->_fontSizeBox->setRange(6, 24);
    this->_fontSizeBox->setValue(Preferences::get()->font().pointSize());
    hbox->addWidget(this->_fontSizeBox);

    this->layout()->addWidget(gbox);
  }

  /*
   * Assemble Preamble
   */
  {
    QGroupBox *gbox = new QGroupBox("Default Preamble", this);
    QHBoxLayout *hbox = new QHBoxLayout();
    gbox->setLayout(hbox);

    this->_preambleBox = new QTextEdit(this);
    PythonHighlighter *highlighter = new PythonHighlighter(this->_preambleBox);
    this->_preambleBox->document()->setPlainText(Preferences::get()->preamble());
    hbox->addWidget(this->_preambleBox);

    this->layout()->addWidget(gbox);
  }

  // Create default buttons
  QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                   Qt::Horizontal, this);

  // Connect to dialog
  QObject::connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

  // Add buttons to layout
  layout->addWidget(buttons);

  // Set layout of dialog
  this->setLayout(layout);
}
