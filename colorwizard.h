#ifndef COLORWIZARD_H
#define COLORWIZARD_H

#include <QPixmap>
#include <QVector>

class QColor;

class ColorWizard
{
public:
    static QVector<QColor> highlight(const QColor &bg, const QColor &fg,
                                     int noColors);
};

#endif
