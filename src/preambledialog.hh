#ifndef PREAMBLEDIALOG_HH
#define PREAMBLEDIALOG_HH

#include <QDialog>
#include <QTextEdit>
#include <QSyntaxHighlighter>


/**
 * A simple dialog to edit the preamble source code of a notebook.
 */
class PreambleDialog : public QDialog
{
  Q_OBJECT

protected:
  QTextEdit *editor;
  QSyntaxHighlighter *highlighter;


public:
  explicit PreambleDialog(QWidget *parent = 0);

  QString sourceCode();
  void setSourceCode(const QString &code);
};

#endif // PREAMBLEDIALOG_HH
