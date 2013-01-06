#ifndef SYNTAXHIGHCOLORDIALOG_H
#define SYNTAXHIGHCOLORDIALOG_H

#include <QVector>
#include <QColor>
#include <QDialog>
#include <QSignalMapper>

class QColor;
class QDialog;
class QSignalMapper;

class SyntaxHighColorDialog : public QDialog
{
public:
    SyntaxHighColorDialog();

    QVector<QColor> colors();
    QColor background();
    QColor foreground();

private slots:
  //  void createNewColors();
   // void selectColor(int index);
    //void setBackground();
    //void setForeground();

private:
    QIcon createIcon(const QSize &size, const QColor &color);
    void setColorButton(int index, const QColor &color);

    QColor bg;
    QColor fg;
    QVector<QColor> colorList;
    QSignalMapper *mapper;
};

#endif // SYNTAXHIGHCOLORDIALOG_H
