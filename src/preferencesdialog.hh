#ifndef PREFERENCESDIALOG_HH
#define PREFERENCESDIALOG_HH

#include <QDialog>
#include <QFontComboBox>
#include <QSpinBox>
#include <QTextEdit>



class PreferencesDialog : public QDialog
{
  Q_OBJECT

protected:
  QFontComboBox *_fontBox;
  QSpinBox *_fontSizeBox;
  QTextEdit *_preambleBox;
  QSpinBox *_tabSizeBox;


public:
    explicit PreferencesDialog(QWidget *parent = 0);

    QFont font();
    QString preamble();
    int tabSize();
};

#endif // PREFERENCESDIALOG_HH
