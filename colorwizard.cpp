#include "colorwizard.h"

#include <QColor>
#include <QPixmap>
#include <QVector>

QVector<QColor> ColorWizard::highlight(const QColor &bg, const QColor &fg,
                                        int noColors)
{
    QVector<QColor> colors;
    const int HUE_BASE = (bg.hue() == -1) ? 90 : bg.hue();
    int h, s, v;

    for (int i = 0; i < noColors; i++)
    {
        h = int(HUE_BASE + (360.0 / noColors * i)) % 360;
        s = 240;
        v = int(qMax(bg.value(), fg.value()) * 0.85);

        // take care of corner cases
        const int M = 35;
        if (   (h < bg.hue() + M &&h > bg.hue() - M)
                || (h < fg.hue() + M &&h > fg.hue() - M))
        {
            h = ((bg.hue() + fg.hue()) / (i + 1)) % 360;
            s = ((bg.saturation() + fg.saturation() + 2 * i) / 2) % 256;
            v = ((bg.value() + fg.value() + 2 * i) / 2) % 256;
        }

        colors.append(QColor::fromHsv(h, s, v));
    }

    return colors;
}
