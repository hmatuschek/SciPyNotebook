#include "resultcell.hh"
#include <iostream>


ResultCell::ResultCell(QWidget *parent) :
    QTextEdit(parent)
{
    // Please, no line wraps...
    this->setLineWrapMode(QTextEdit::NoWrap);

    // Disable editing...
    this->setReadOnly(true);

    this->_text_size = this->document()->size().toSize();
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QObject::connect(this, SIGNAL(textChanged()),
                     this, SLOT(onTextChanged()));

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set background color.
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, Qt::lightGray);
    this->setPalette(pal);
    this->setAutoFillBackground(true);

    // Disable widget, will be activated once the code runs    
    this->setFrameShape(QFrame::NoFrame);

    // Connect to CellInputStream events:
    QObject::connect(&(this->_stdout_stream), SIGNAL(newData(QString)),
                     this, SLOT(onStdoutText(QString)));
    QObject::connect(&(this->_stderr_stream), SIGNAL(newData(QString)),
                     this, SLOT(onStderrText(QString)));
}


QSize
ResultCell::minimumSizeHint() const
{
    return this->document()->size().toSize();
}


QSize
ResultCell::sizeHint() const
{
    QSize csize(QTextEdit::sizeHint());
    csize.setHeight(this->document()->size().toSize().height());

    return csize;
}


void
ResultCell::onTextChanged()
{
    this->_text_size = this->document()->size().toSize();
    this->updateGeometry();
}


CellInputStream *
ResultCell::getStdoutStream()
{
    return &(this->_stdout_stream);
}


CellInputStream *
ResultCell::getStderrStream()
{
    return &(this->_stderr_stream);
}


void
ResultCell::onStdoutText(const QString &data)
{
    this->textCursor().insertText(data);
}

void
ResultCell::onStderrText(const QString &data)
{
    this->textCursor().insertText(data);
}

