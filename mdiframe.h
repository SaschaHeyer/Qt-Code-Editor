#ifndef MDIFRAME_H
#define MDIFRAME_H

#include "codeeditor.h"

#include <QAbstractItemModel>
#include <QCompleter>

class CodeEditor;

class MdiFrame : public CodeEditor
{
    Q_OBJECT

public:
    MdiFrame();

    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool print();
    bool saveFile(const QString &fileName);
    void setCurType(const QString &fileType);

     QString type;

    QString userSpecCurFile();
    QString currentFile()
    {
        return curFile;
    }

    QString currentType()
    {
        return type;
    }

     QCompleter *completer;
      bool maybeSave();


protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void documentWasModified();

private:

    void setCurFile(const QString &fileName);

    QString strippedName(const QString &fullFileName);

    QString curFile;
    bool isUntitled;

    QAbstractItemModel *modelFromFile(const QString& fileName);


};

#endif // MDIFRAME_H
