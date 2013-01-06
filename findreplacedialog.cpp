#include "findreplacedialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include <QDebug>
#include <QTextCharFormat>


FindReplaceDialog::FindReplaceDialog(QWidget *parent)
    :QDialog(parent)
{
    label = new QLabel(tr("Finden:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);

    caseCheckBox = new QCheckBox(tr("Groß-/Kleinschreibung"));
    fromStartCheckBox = new QCheckBox(tr("Von oben beginnen"));
    fromStartCheckBox->setChecked(true);

    findButton = new QPushButton(tr("&Finden"));
    findButton->setDefault(true);
    findButton->setEnabled(false);

    moreButton = new QPushButton(tr("&Mehr Optionen"));
    moreButton->setCheckable(true);
    moreButton->setAutoDefault(false);

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(findButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(moreButton, QDialogButtonBox::ActionRole);

    extension = new QWidget;

    wholeWordsCheckBox = new QCheckBox(tr("&Nur ganze Wörter suchen"));
    backwardCheckBox = new QCheckBox(tr("Top Down suchen"));
    searchSelectionCheckBox = new QCheckBox(tr("suche se&lection"));

    connect(moreButton, SIGNAL(toggled(bool)), extension, SLOT(setVisible(bool)));
    connect(this,SIGNAL(finished(int)),this,SLOT(test()));

    QVBoxLayout *extensionLayout = new QVBoxLayout;
    extensionLayout->setMargin(0);
    extensionLayout->addWidget(wholeWordsCheckBox);
    extensionLayout->addWidget(backwardCheckBox);
    extensionLayout->addWidget(searchSelectionCheckBox);
    extension->setLayout(extensionLayout);

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(caseCheckBox);
    leftLayout->addWidget(fromStartCheckBox);
    leftLayout->addStretch(1);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(leftLayout, 0, 0);
    mainLayout->addWidget(buttonBox, 0, 1);
    mainLayout->addWidget(extension, 1, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("Suchen"));
    extension->hide();

    connect(lineEdit, SIGNAL(textChanged(QString)),this,SLOT(findTextChanged()));
    connect(findButton, SIGNAL(clicked()),this, SLOT(find()));
}

void FindReplaceDialog::test(){
    qDebug()<<"CLosseeeeddd";
    QTextDocument *document = mTextEdit;
  QTextCursor cursor(document);

    QTextCharFormat plainFormat(cursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;

    QColor lineColor = QColor(Qt::white);
        //colorFormat.setForeground(Qt::red);
        colorFormat.setBackground(lineColor);

        cursor.mergeCharFormat(colorFormat);

        while (!cursor.isNull() && !cursor.atEnd()) {

            if (!cursor.isNull()) {
                cursor.movePosition(QTextCursor::WordRight,
                                       QTextCursor::KeepAnchor);

                cursor.mergeCharFormat(colorFormat);
            }
        }
}

void FindReplaceDialog::setTextEdit(QTextDocument *textEdit)
{
    qDebug()<<"Param"+textEdit->toPlainText();
    mTextEdit = textEdit;

    qDebug()<<"Var"+mTextEdit->toPlainText();
}

void FindReplaceDialog::find(){
    find(backwardCheckBox->isChecked());
}

void FindReplaceDialog::find(bool next)
{
    bool back= !next;
    const QString &toSearch =lineEdit->text();
    bool result = false;

    QTextCursor cursor;

    qDebug()<<mTextEdit->toPlainText();

    QTextDocument::FindFlags flags;

   // if(back)
      //  flags |= QTextDocument::FindBackward;
   // if(caseCheckBox)
      //  flags |= QTextDocument::FindCaseSensitively;
   // if(wholeWordsCheckBox)
      //  flags |= QTextDocument::FindWholeWords;

   // qDebug()<<mTextEdit->document()->toPlainText();

    //cursor = mTextEdit->find(toSearch, flags);
    QTextDocument *document = mTextEdit;
    //QTextCursor cursor(mTextEdit->textCursor());
       // cursor.movePosition(QTextCursor::Start);



        QTextCharFormat plainFormat(cursor.charFormat());
        QTextCharFormat colorFormat = plainFormat;
        QColor lineColor = QColor(Qt::green).lighter(160);
            //colorFormat.setForeground(Qt::red);
            colorFormat.setBackground(lineColor);
        QTextCursor newCursor(document);

        while (!newCursor.isNull() && !newCursor.atEnd()) {
            newCursor = document->find(toSearch, newCursor);

            if (!newCursor.isNull()) {
                newCursor.movePosition(QTextCursor::WordRight,
                                       QTextCursor::KeepAnchor);

                newCursor.mergeCharFormat(colorFormat);
            }

        }


}

void FindReplaceDialog::findTextChanged()
{
    if(lineEdit->text().size() > 0)
        findButton->setEnabled(true);
}





