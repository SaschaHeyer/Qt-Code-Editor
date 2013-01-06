#ifndef SYNTAXHTMLHIGHLIGHTER_H
#define SYNTAXHTMLHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class QTextDocument;

class SyntaxHtmlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    SyntaxHtmlHighlighter(QTextDocument *parent = 0);
    QVector<QColor> colors();
    void setColors(const QVector<QColor> &colors);
    QStringList colorNames();

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat *format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif
