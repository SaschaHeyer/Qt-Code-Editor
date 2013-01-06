#include "mdiframe.h"
#include "codeeditor.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QDebug>
#include <QApplication>

MdiFrame::MdiFrame()
    : CodeEditor(0), curFile(""), isUntitled(0),type(tr("nicht festgelegt"))
{

    completer = new QCompleter(this);
   // completer->setModel(modelFromFile(":/apis/qt.api"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(true);
    completer->setMaxVisibleItems(10);

}

void MdiFrame::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("neues Dokument%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

    connect(document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));




}

bool MdiFrame::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("RDNote"),
                             tr("Lesen fehlgeschlagen %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurFile(fileName);

    connect(document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));

    return true;
}

bool MdiFrame::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MdiFrame::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Speichern als"),
                                                    curFile,
                                                    tr("HTML (*.html);;CSS (*.css);;CPP (*.cpp);;Text files (*.txt);;XML files (*.xml)"));

    QFileInfo fileInfo(fileName);
    QString base = fileInfo.suffix();

    qDebug()<<base;

    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MdiFrame::print()
{
   /* QPrinter printer;

        QPrintDialog *dialog = new QPrintDialog(&printer, this);
        dialog->setWindowTitle(tr("Print Document"));
        if (editor->textCursor().hasSelection())
            dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
        if (dialog->exec() != QDialog::Accepted)
            return true;*/

    return true;
       // return "";
}

bool MdiFrame::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("RDNote"),
                             tr("Speichern fehlgeschlagen %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    QApplication::restoreOverrideCursor();

    setCurFile(fileName);
    return true;
}

QString MdiFrame::userSpecCurFile()
{
    return strippedName(curFile);
}

void MdiFrame::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MdiFrame::documentWasModified()
{
    setWindowModified(document()->isModified());
}

bool MdiFrame::maybeSave()
{
    if (document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Nicht gespeicherte Änderungen"),
                     tr("'%1' wurde geändert.\n"
                        "Möchtest du deine Änderungen speichern?")
                     .arg(userSpecCurFile()),
                     QMessageBox::Save | QMessageBox::Discard
                     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MdiFrame::setCurFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userSpecCurFile() + "[*]");
}

void MdiFrame::setCurType(const QString &fileType)
{
    type = fileType;
}

QString MdiFrame::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


