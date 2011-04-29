#include "preferencesdialog.hh"
#include <QVBoxLayout>
#include <QDialogButtonBox>


PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

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
