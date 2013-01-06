#include "syntaxhtmlhighlighter.h"

SyntaxHtmlHighlighter::SyntaxHtmlHighlighter(QTextDocument *parent)
    :QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns <<"<html>" <<"</html>"
                    <<"<head>" <<"</head>"
                    <<"<body>" <<"</body>"
                    <<"<title>" <<"</title>"
                    <<"<script>" <<"</script>"
                    <<"<b>" <<"</b>"
                    <<"<p>" <<"</p>"
                    <<"<i>" <<"</i>"
                    <<"<u>" <<"</u>"
                    <<"<sup>" <<"</sup>"
                    <<"<sub>" <<"</sub>"
                    <<"<h1>" <<"</h1>"
                    <<"<h2>" <<"</h2>"
                    <<"<h3>" <<"</h3>"
                    <<"<h4>" <<"</h4>"
                    <<"<h5>" <<"</h5>"
                    <<"<h6>" <<"</h6>"
                    <<"<small>" <<"</small>"
                    <<"<big>" <<"</big>"
                    <<"<center>" <<"</center>"
                    <<"<nobr>" <<"</nobr>"
                    <<"<blockquote>" <<"</blockquote>"
                    <<"<li>" <<"</li>"
                    <<"<ul>" <<"</ul>"
                    <<"<ol>" <<"</ol>"
                    <<"<dl>" <<"</dl>"
                    <<"<table>" <<"</table>"
                    <<"<td>" <<"</td>"
                    <<"<strike>" <<"</strike>";
    foreach (QString pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = &keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = &classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = &singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = &quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = &functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

QVector<QColor> SyntaxHtmlHighlighter::colors()
{
    QVector<QColor> colors;
    colors << keywordFormat.foreground().color()
           << classFormat.foreground().color()
           << singleLineCommentFormat.foreground().color()
           << multiLineCommentFormat.foreground().color()
           << quotationFormat.foreground().color()
           << functionFormat.foreground().color();
    return colors;
}

QStringList SyntaxHtmlHighlighter::colorNames()
{
    QStringList names;
    names << tr("Keywords")
          << tr("Classes")
          << tr("Single line comments")
          << tr("Multi-line comments")
          << tr("Strings")
          << tr("Functions");
    return names;
}

void SyntaxHtmlHighlighter::highlightBlock(const QString &text)
{
    foreach (HighlightingRule rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, *(rule.format));
            index = text.indexOf(expression, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        int endIndex = text.indexOf(commentEndExpression, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression,
                                                startIndex + commentLength);
    }
}

void SyntaxHtmlHighlighter::setColors(const QVector<QColor> &colors)
{
    keywordFormat.setForeground(colors[0]);
    classFormat.setForeground(colors[1]);
    singleLineCommentFormat.setForeground(colors[2]);
    multiLineCommentFormat.setForeground(colors[3]);
    quotationFormat.setForeground(colors[4]);
    functionFormat.setForeground(colors[5]);
}
