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



PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent)
{
  this->setWindowTitle("Preferences");

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
      this->_fontBox = new QFontComboBox(this);
      this->_fontBox->setCurrentFont(Preferences::get()->font());
      hbox->addWidget(this->_fontBox);

      // Assemble size selection box.
      this->_fontSizeBox = new QSpinBox(this);
      this->_fontSizeBox->setRange(6, 24);
      this->_fontSizeBox->setValue(Preferences::get()->font().pointSize());
      hbox->addWidget(this->_fontSizeBox);

      vbox->addWidget(gbox);
    }

    /*
     * Assemble tabsize config
     */
    {
      QGroupBox *gbox = new QGroupBox("Tab Size", this);
      QHBoxLayout *hbox = new QHBoxLayout();
      gbox->setLayout(hbox);

      this->_tabSizeBox = new QSpinBox(this);
      this->_tabSizeBox->setRange(0, 10);
      this->_tabSizeBox->setValue(Preferences::get()->tabSize());
      hbox->addWidget(this->_tabSizeBox);

      vbox->addWidget(gbox);
    }

    // Add "editor frame" to tabwidget:
    tabs->addTab(editorFrame, "Editor");
  }

  {
    QFrame *preFrame = new QFrame(tabs);
    QVBoxLayout *vbox = new QVBoxLayout();
    preFrame->setLayout(vbox);

    this->_preambleBox = new QTextEdit(this);
    PythonHighlighter *highlighter = new PythonHighlighter(this->_preambleBox);
    this->_preambleBox->document()->setPlainText(Preferences::get()->preamble());
    vbox->addWidget(this->_preambleBox);

    // add preamble frame to tabs.
    tabs->addTab(preFrame, "Preamble");
  }

  this->layout()->addWidget(tabs);


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


QFont
PreferencesDialog::font()
{
  QFont font(this->_fontBox->currentFont());
  font.setPointSize(this->_fontSizeBox->value());

  return font;
}


QString
PreferencesDialog::preamble()
{
  return this->_preambleBox->document()->toPlainText();
}


int
PreferencesDialog::tabSize()
{
  return this->_tabSizeBox->value();
}