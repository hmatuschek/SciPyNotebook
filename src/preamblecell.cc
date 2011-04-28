#include "preamblecell.hh"

#include <QStackedLayout>
#include <QPushButton>


PreambleCell::PreambleCell(QWidget *parent) :
    QFrame(parent)
{
  this->_layout = new QStackedLayout(this);

  this->cell = new Cell(this);
  this->cell->setCode("#\n#\n#\n");
  this->_layout->addWidget(this->cell);

  QPushButton *label = new QPushButton(tr("Preamble"));
  label->setFlat(true);
  QObject::connect(label, SIGNAL(clicked()), this, SLOT(showCell()));
  this->_layout->addWidget(label);

  // hide cell
  this->_layout->setCurrentIndex(1);

  this->setLayout(this->_layout);
}


void
PreambleCell::showCell()
{
  this->_layout->setCurrentIndex(0);
}


void
PreambleCell::hideCell()
{
  this->_layout->setCurrentIndex(1);
}
