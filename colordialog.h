#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QColor>
#include <QVector>
#include <QDialog>

class QSignalMapper;

class ColorDialog : public QDialog
{
    Q_OBJECT

public:
    ColorDialog(const QColor &fg, const QColor &bg, const QVector<QColor> &colors,
                const QStringList &names, QWidget *parent = 0);
    QVector<QColor> colors();
    QColor background();
    QColor foreground();

private slots:
    void createNewColors();
    void selectColor(int index);
    void setBackground();
    void setForeground();

private:
    QIcon createIcon(const QSize &size, const QColor &color);
    void setColorButton(int index, const QColor &color);

    QColor bg;
    QColor fg;
    QVector<QColor> colorList;
    QSignalMapper *mapper;
};

#endif
