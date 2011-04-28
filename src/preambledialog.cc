#include "preambledialog.hh"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include "pythonhighlighter.hh"



PreambleDialog::PreambleDialog(QWidget *parent) :
    QDialog(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  // Create editor and add to layout
  this->editor = new QTextEdit(this);
  this->highlighter = new PythonHighlighter(this->editor);
  this->highlighter->setDocument(this->editor->document());
  layout->addWidget(this->editor);

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


void
PreambleDialog::setSourceCode(const QString &code)
{
  this->editor->document()->setPlainText(code);
}


QString
PreambleDialog::sourceCode()
{
  return this->editor->document()->toPlainText();
}
