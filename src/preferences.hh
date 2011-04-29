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
  QFont font;
  QFile configfile;


protected:
  explicit Preferences(QObject *parent = 0);


public:  
    QFont getFont() const;


public:
    static Preferences *get();

signals:
    void fontChanged();


public slots:
    void setFont(QFont &font);

};

#endif // PREFERENCES_HH
