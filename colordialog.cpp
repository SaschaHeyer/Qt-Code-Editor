#include <QtGui>
#include "colordialog.h"
#include "colorwizard.h"

ColorDialog::ColorDialog(const QColor &foreground, const QColor &background,
    const QVector<QColor> &colors, const QStringList &names, QWidget *parent)
    : QDialog(parent)
{
    bg = background;
    fg = foreground;
    colorList = colors;
    mapper = new QSignalMapper(this);

    QGroupBox *buttonGroupBox = new QGroupBox(tr("Highlighting colors"));
    QGridLayout *buttonLayout = new QGridLayout;

    for (int i = 0; i < colors.size(); ++i) {
        QLabel *label = new QLabel(names[i]);
        QToolButton *button = new QToolButton;
        button->setText(tr("Choose..."));
        button->setIcon(createIcon(button->iconSize(), colors[i]));
        connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(button, i);

        buttonLayout->addWidget(label, i, 0);
        buttonLayout->addWidget(button, i, 1);
    }

    buttonGroupBox->setLayout(buttonLayout);

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(selectColor(int)));

    QGroupBox *autoColorGroupBox = new QGroupBox(tr("Base colors"));

    QLabel *foregroundLabel = new QLabel(tr("Foreground:"));
    QToolButton *foregroundButton = new QToolButton;
    foregroundButton->setText(tr("Choose..."));
    foregroundButton->setIcon(createIcon(foregroundButton->iconSize(), fg));
    QLabel *backgroundLabel = new QLabel(tr("Background:"));
    QToolButton *backgroundButton = new QToolButton;
    backgroundButton->setText(tr("Choose..."));
    backgroundButton->setIcon(createIcon(backgroundButton->iconSize(), bg));
    QPushButton *createButton = new QPushButton(tr("Create New Colors"));

    connect(foregroundButton, SIGNAL(clicked()), this, SLOT(setForeground()));
    connect(backgroundButton, SIGNAL(clicked()), this, SLOT(setBackground()));
    connect(createButton, SIGNAL(clicked()), this, SLOT(createNewColors()));

    QGridLayout *autoColorLayout = new QGridLayout;
    autoColorLayout->addWidget(foregroundLabel, 0, 0);
    autoColorLayout->addWidget(foregroundButton, 0, 1);
    autoColorLayout->addWidget(backgroundLabel, 1, 0);
    autoColorLayout->addWidget(backgroundButton, 1, 1);
    autoColorLayout->addWidget(createButton, 2, 0, 1, 2);
    autoColorGroupBox->setLayout(autoColorLayout);

    QDialogButtonBox *buttons = new QDialogButtonBox(Qt::Horizontal);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->addButton(QDialogButtonBox::Cancel);

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(buttonGroupBox, 0, 0, 2, 1);
    mainLayout->addWidget(autoColorGroupBox, 0, 1);
    mainLayout->addWidget(buttons, 2, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("Configure Highlighting Colors"));
}

QColor ColorDialog::background()
{
    return bg;
}

QVector<QColor> ColorDialog::colors()
{
    return colorList;
}

QIcon ColorDialog::createIcon(const QSize &size, const QColor &color)
{
    QPixmap pixmap(size);
    pixmap.fill(color);
    return QIcon(pixmap);
}

void ColorDialog::createNewColors()
{
    const int noColors = 6;
    QVector<QColor> colors = ColorWizard::highlight(bg, fg, noColors);
    for (int i = 0; i < noColors; i++)
        setColorButton(i, colors[i]);
}

QColor ColorDialog::foreground()
{
    return fg;
}

void ColorDialog::selectColor(int index)
{
    QColor color = QColorDialog::getColor(colorList[index]);
    if (color.isValid())
        setColorButton(index, color);
}

void ColorDialog::setBackground()
{
    QColor color = QColorDialog::getColor(bg);
    if (color.isValid()) {
        QToolButton *button = static_cast<QToolButton *>(sender());
        button->setIcon(createIcon(button->iconSize(), color));
        bg = color;
    }
}

void ColorDialog::setForeground()
{
    QColor color = QColorDialog::getColor(fg);
    if (color.isValid()) {
        QToolButton *button = static_cast<QToolButton *>(sender());
        button->setIcon(createIcon(button->iconSize(), color));
        fg = color;
    }
}

void ColorDialog::setColorButton(int index, const QColor &color)
{
    QToolButton *button = static_cast<QToolButton *>(mapper->mapping(index));
    button->setIcon(createIcon(button->iconSize(), color));
    colorList[index] = color;
}
