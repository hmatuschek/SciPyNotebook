#ifndef PREAMBLECELL_HH
#define PREAMBLECELL_HH

#include <QFrame>
#include <QStackedLayout>

#include "cell.hh"


class PreambleCell : public QFrame
{
    Q_OBJECT

protected:
  QStackedLayout *_layout;
  Cell *cell;


public:
  explicit PreambleCell(QWidget *parent = 0);


public slots:
  void showCell();
  void hideCell();

};

#endif // PREAMBLECELL_HH
