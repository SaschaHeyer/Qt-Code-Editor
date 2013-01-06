#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QWidget>
#include <QTextEdit>
#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QDialog>
#include <QPlainTextEdit>

class QWidget;
class QEvent;
class QLabel;
class QGroupBox;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QDialogButtonBox;
class QDialog;
class QPlainTextEdit;

class FindReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    FindReplaceDialog(QWidget *parent = 0);
    void setTextEdit(QTextDocument *textEdit);

public slots:
    void find(bool down);
    void find();
   // void findNext();
    //void findPrev();
   // void replace();
   // void replaceAll();
    void findTextChanged();
    void test();

protected:
  //  void changeEvent(QEvent *event);
    QTextDocument *mTextEdit;

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *fromStartCheckBox;
    QCheckBox *wholeWordsCheckBox;
    QCheckBox *searchSelectionCheckBox;
    QCheckBox *backwardCheckBox;
    QDialogButtonBox *buttonBox;
    QPushButton *findButton;
    QPushButton *moreButton;
    QWidget *extension;
};

#endif // FINDREPLACEDIALOG_H
