#ifndef PREFERENCES_HH
#define PREFERENCES_HH

#include <QObject>
#include <QFont>
#include <QFile>


class Preferences : public QObject
{
    Q_OBJECT

protected:
  static Preferences *_instance;


protected:
  QFont _font;
  QFile _configfile;
  QString _preamble;
  int _tabsize;

protected:
  explicit Preferences(QObject *parent = 0);


public:  
    QFont font() const;
    QString preamble() const;
    int tabSize();

public:
    static Preferences *get();

signals:
    void fontChanged();
    void tabSizeChanged();

public slots:
    void setFont(const QFont &font);
    void setPreamble(const QString &text);
    void setTabSize(int size);

    void save();
};

#endif // PREFERENCES_HH
