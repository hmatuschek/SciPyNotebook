#include "aboutdialog.hh"
#include <QTextEdit>

static QString content = \
"<html>" \
"<head></head>" \
"<body>" \
"<h1>About SciPy Notebook</h1>" \
"</body>" \
"</html>";


AboutDialog::AboutDialog(QWidget *parent) :
    QMainWindow(parent)
{
  QTextEdit *view = new QTextEdit(this);
  view->setReadOnly(true);
  view->setHtml(content);
  this->setCentralWidget(view);
}
