#include "reddeernote.h"

#include "colordialog.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QFileInfo>
#include <QFileDialog>
#include <QDockWidget>
#include <QPrintDialog>
#include <QPrinter>
#include <QListWidget>
#include <QToolBar>
#include <QMessageBox>
#include <QSettings>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QList>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QStringListModel>

#include <QFileSystemWatcher>




RedDeerNote::RedDeerNote(QWidget *parent)
    : QMainWindow(parent), fileMenu(0), editMenu(0), findMenu(0),progLangMenu(0),
      networkMenu(0),windowsMenu(0), helpMenu(0), firstLabel(0), secondLabel(0), fileNewAction(0),
      fileOpenAction(0), fileSaveAction(0), fileSaveUnderAction(0), closeRedDeerAction(0),
      mdiArea(0), windowMapper(0), findAction(0), mFindReplaceDialog(0)
{
    setWindowIcon(QIcon(QString(":/icons/rdicon.ico")));

    // Network
    connection = new Connection();
    chatWidget = new ChatWidget(this);

    // FileSystemWatcher
   // watcher = new QFileSystemWatcher(this);


    // MDI Area setzen
    createMdiArea();



/*
    // Code Completer
    completer = new QCompleter(this);
   // completer->setModel(modelFromFile(":/apis/qt.api"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(true);
    completer->setMaxVisibleItems(10);
    */

    // Mapping
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));

    // Connects
    connect(mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(buttonChecker()));
    connect(mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(updateStatusBar()));
    connect(chatWidget, SIGNAL(sendCodeToMdiFrame(QString,QString)),this, SLOT(incomingFile(QString,QString)));
    connect(this,SIGNAL(newFileSignal(QString)),this,SLOT(insertDocument(QString)));


    connect(this,SIGNAL(signalUpdateDocumentDock()),this,SLOT(updateDocumentDock()));



   // connect(watcher,SIGNAL(fileChanged(QString)),this,SLOT(fileChangedHandler(QString)));

   // connect(this,SIGNAL(fileToWatcher(QString)),this,SLOT(addFileToWatcher(QString)));

    // GUI-Elemente (Bereiche)
    createMenus();
    createActions();
    addActionsToMenus();
    createToolBars();
    createStatusBars();
    createDockWindows();

    //Dialogs
    mFindReplaceDialog = new FindReplaceDialog(this);
    mFindReplaceDialog->setModal(false);
}

void RedDeerNote::fileChangedHandler(QString file){
   // qDebug()<<"fileChanged:  "+file;
    qDebug()<<watcher->files();

    QMessageBox msgBox;
    msgBox.setText(tr("Datei wurde auserhalb geändert!"));
    msgBox.setInformativeText(tr("Soll die Datei neu geladen werden?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Yes:
           // frame = createMdiFrame();
           // frame->newFile();
           // frame->show();
           // fileSaveUnderAction->setEnabled(true);
           // activeMdiChild()->document()->setPlainText(code);
            break;
        case QMessageBox::No:
            break;
        default:
            break;
    }
}

void RedDeerNote::addFileToWatcher(QString file){
    watcher->addPath(file);
}


void RedDeerNote::closeEvent(QCloseEvent *event)
{
    qDebug()<<"closed";

}

void RedDeerNote::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("Datei"));
    editMenu = menuBar()->addMenu(tr("Bearbeiten"));
    findMenu = menuBar()->addMenu(tr("Suchen"));
    progLangMenu = menuBar()->addMenu(tr("Sprache (Programmier)"));
    networkMenu = menuBar()->addMenu(tr("Netzwerk"));
    windowsMenu = menuBar()->addMenu(tr("Fenster"));

    settingsMenu = menuBar()->addMenu(tr("Einstellungen"));
    settingsDockMenu = settingsMenu->addMenu(tr("Fenster andocken..."));
    settingsTollbarMenu = settingsMenu->addMenu(tr("Werkzeugleisten..."));
    settingsSyntaxMenu = settingsMenu->addMenu(tr("Syntax Einstellungen..."));

    helpMenu = menuBar()->addMenu(tr("?"));
}

void RedDeerNote::addActionsToMenus()
{
    // File Menu
    fileMenu->addAction(fileNewAction);
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileSaveAction);
    fileMenu->addAction(fileSaveUnderAction);
    fileMenu->addSeparator();
    fileMenu->addAction(printAction);
    fileMenu->addSeparator();
    separatorAct = fileMenu->addSeparator();
         for (int i = 0; i < MaxRecentFiles; ++i)
             fileMenu->addAction(recentFilesAction[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(closeRedDeerAction);
    findMenu->addAction(findAction);

    // Network Menu
    networkMenu->addAction(sendFileToUserAction);
    networkMenu->addAction(syncFileAction);
    networkMenu->addAction(chatAction);
    networkMenu->addAction(lockUnlockAction);

    // Syntax Menu
    progLangMenu->addAction(htmlSyntaxAction);
    progLangMenu->addAction(cplusplusSyntaxAction);
    progLangMenu->addAction(cssSyntaxAction);
    progLangMenu->addAction(javaSyntaxAction);
    progLangMenu->addAction(javaScriptSyntaxAction);
    progLangMenu->addAction(lispSyntaxAction);
    progLangMenu->addAction(pascalSyntaxAction);
    progLangMenu->addAction(perlSyntaxAction);
    progLangMenu->addAction(phpSyntaxAction);
    progLangMenu->addAction(texSyntaxAction);
    progLangMenu->addAction(qtSyntaxAction);

    // Window Menu
    windowsMenu->addAction(nextAction);
    windowsMenu->addAction(prevAction);
    windowsMenu->addSeparator();
    windowsMenu->addAction(cascadeAction);
    windowsMenu->addAction(titledAction);
    windowsMenu->addSeparator();
    windowsMenu->addAction(closeAction);
    windowsMenu->addAction(closeAllAction);

    // Help Menu
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(qtAboutAction);
    helpMenu->addAction(webRedDeerAction);

    // Settings / Syntax Menu
    settingsSyntaxMenu->addAction(settingsSyntaxAction);

    updateRecentFiles();
    buttonChecker();


}

void RedDeerNote::createActions()
{
    // File Menu Actions
    fileNewAction = new QAction(QIcon(QString(":/icons/document_new.png")),tr("&Neu..."), this);
    fileNewAction->setShortcuts(QKeySequence::New);
    fileNewAction->setStatusTip(tr("Erstellt ein neues Dokument"));
    connect(fileNewAction, SIGNAL(triggered()), this, SLOT(newFile()));

    fileOpenAction = new QAction(QIcon(QString(":/icons/document_open.png")),tr("&Oeffnen..."), this);
    fileOpenAction->setShortcuts(QKeySequence::Open);
    fileOpenAction->setStatusTip(tr("Eine bereits vorhandene Datei öffnen"));
    connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(showFileDialog()));

    fileSaveAction = new QAction(QIcon(QString(":/icons/document_save_as.png")), tr("&Speichern"), this);
    fileSaveAction->setShortcuts(QKeySequence::Save);
    fileSaveAction->setStatusTip(tr("Aktuell ausgewählte Datei speichern"));
    fileSaveAction->setEnabled(false);
    connect(fileSaveAction, SIGNAL(triggered()), this, SLOT(save()));

    fileSaveUnderAction = new QAction(QIcon(QString(":/icons/document_save_as.png")), tr("&Speichern unter..."), this);
    fileSaveUnderAction->setShortcuts(QKeySequence::SaveAs);
    fileSaveUnderAction->setStatusTip(tr("Datei in einem bestimmten Verzeichnis speichern"));
    fileSaveUnderAction->setEnabled(false);
    connect(fileSaveUnderAction, SIGNAL(triggered()), this, SLOT(showSaveUnderDialog()));   

    printAction = new QAction(QIcon(QString(":/icons/print.png")), tr("&Drucken"), this);
    printAction->setShortcuts(QKeySequence::Print);
    printAction->setStatusTip(tr("Druckt die aktuelle Datei"));
    printAction->setEnabled(false);
    connect(printAction, SIGNAL(triggered()), this, SLOT(showPrintDialog()));

    closeAllAction = new QAction(tr("&Alle offenen Dateien schließen"), this);
    closeAllAction->setStatusTip(tr("Schließt alle offenen Dateien"));
    connect(closeAllAction, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

    for(int i=0;i<MaxRecentFiles; i++){
        recentFilesAction[i] = new QAction(this);
        recentFilesAction[i]->setVisible(false);
        connect(recentFilesAction[i],SIGNAL(triggered()),this,SLOT(openRecentFile()));
    }

    closeRedDeerAction = new QAction(QIcon(QString(":/icons/window_close.png")), tr("&Beenden"), this);
    closeRedDeerAction->setShortcuts(QKeySequence::Close);
    closeRedDeerAction->setStatusTip(tr("Beenden das Programm"));
    connect(closeRedDeerAction, SIGNAL(triggered()), this, SLOT(close()));

    // Find Actions
    findAction = new QAction(QIcon(QString(":/icons/find.png")), tr("&Finden und Ersetzen"), this);
    findAction->setShortcuts(QKeySequence::Find);
    findAction->setStatusTip(tr("Finden und Ersetzen Funktion"));
    connect(findAction, SIGNAL(triggered()), this, SLOT(findDialog()));

    // Network Actions
    sendFileToUserAction = new QAction(QIcon(QString(":/icons/file_send.png")), tr("&Datei senden"), this);
    sendFileToUserAction->setShortcuts(QKeySequence::Find);
    sendFileToUserAction->setEnabled(false);
    sendFileToUserAction->setStatusTip(tr("Sendet die Datei an den in der Liste augewählten User"));
    connect(sendFileToUserAction, SIGNAL(triggered()), this, SLOT(sendFile()));

    syncFileAction = new QAction(QIcon(QString(":/icons/sync.png")), tr("&Datei sync"), this);
    syncFileAction->setEnabled(false);
    syncFileAction->setStatusTip(tr("Synchronisiert eine Datei mit einem anderen User"));
    connect(syncFileAction, SIGNAL(triggered()), this, SLOT(sendFile()));

    chatAction = new QAction(QIcon(QString(":/icons/chat.png")), tr("&Chat"), this);
    chatAction->setStatusTip(tr("Öffnet und startet den Network WorkChat"));
    connect(chatAction, SIGNAL(triggered()), this, SLOT(sendFile()));

    lockUnlockAction = new QAction(QIcon(QString(":/icons/lock.png")), tr("&Datei lock"), this);
    //lockUnlockAction->setStatusTip(tr("Öffnet und startet den Network WorkChat"));
    connect(chatAction, SIGNAL(triggered()), this, SLOT(sendFile()));

    // Syntax / Completiotion Actions
    htmlSyntaxAction = new QAction(QIcon(QString(":/icons/html.png")), tr("&HTML"), this);
    htmlSyntaxAction->setStatusTip(tr("HTML"));
    htmlSyntaxAction->setEnabled(false);
    connect(htmlSyntaxAction, SIGNAL(triggered()), this, SLOT(enableHtmlHighlighter()));

    cplusplusSyntaxAction = new QAction(QIcon(QString(":/icons/cpp.png")), tr("&C++"), this);
    cplusplusSyntaxAction->setStatusTip(tr("Aktiviert das C++ Syntax Highlighting und Autocompletion"));
    cplusplusSyntaxAction->setEnabled(false);
    connect(cplusplusSyntaxAction, SIGNAL(triggered()), this, SLOT(enableCppHighlighter()));

    cssSyntaxAction = new QAction(QIcon(QString(":/icons/css.png")), tr("&CSS"), this);
    cssSyntaxAction->setStatusTip(tr("Aktiviert das CSS Syntax Highlighting und Autocompletion"));
    cssSyntaxAction->setEnabled(false);
    connect(cssSyntaxAction, SIGNAL(triggered()), this, SLOT(enableCssHighlighter()));

    javaSyntaxAction = new QAction(QIcon(QString(":/icons/java.png")), tr("&Java"), this);
    javaSyntaxAction->setStatusTip(tr("Aktiviert das Java Syntax Highlighting und Autocompletion"));
    javaSyntaxAction->setEnabled(false);
    connect(javaSyntaxAction, SIGNAL(triggered()), this, SLOT(enableJavaHighlighter()));

    javaScriptSyntaxAction = new QAction(QIcon(QString(":/icons/javascript.png")), tr("&Java Script"), this);
    javaScriptSyntaxAction->setStatusTip(tr("Aktiviert das Java Script Syntax Highlighting und Autocompletion"));
    javaScriptSyntaxAction->setEnabled(false);
    connect(javaScriptSyntaxAction, SIGNAL(triggered()), this, SLOT(enableJavaScriptHighlighter()));

    lispSyntaxAction = new QAction(QIcon(QString(":/icons/lisp.png")), tr("&Lisp"), this);
    lispSyntaxAction->setStatusTip(tr("Aktiviert das Lisp Syntax Highlighting und Autocompletion"));
    lispSyntaxAction->setEnabled(false);
    connect(lispSyntaxAction, SIGNAL(triggered()), this, SLOT(enableLispHighlighter()));

    pascalSyntaxAction = new QAction(QIcon(QString(":/icons/pascal.png")), tr("&Pascal"), this);
    pascalSyntaxAction->setStatusTip(tr("Aktiviert das Pascal Syntax Highlighting und Autocompletion"));
    pascalSyntaxAction->setEnabled(false);
    connect(pascalSyntaxAction, SIGNAL(triggered()), this, SLOT(enablePascalHighlighter()));

    perlSyntaxAction = new QAction(QIcon(QString(":/icons/perl.png")), tr("&Perl"), this);
    perlSyntaxAction->setStatusTip(tr("Aktiviert das Perl Syntax Highlighting und Autocompletion"));
    perlSyntaxAction->setEnabled(false);
    connect(perlSyntaxAction, SIGNAL(triggered()), this, SLOT(enablePerlHighlighter()));

    phpSyntaxAction = new QAction(QIcon(QString(":/icons/php.png")), tr("&PHP"), this);
    phpSyntaxAction->setStatusTip(tr("Aktiviert das PHP Syntax Highlighting und Autocompletion"));
    phpSyntaxAction->setEnabled(false);
    connect(phpSyntaxAction, SIGNAL(triggered()), this, SLOT(enablePhpHighlighter()));

    texSyntaxAction = new QAction(QIcon(QString(":/icons/tex.png")), tr("&TEX (Latex)"), this);
    texSyntaxAction->setStatusTip(tr("Aktiviert das Tex (Latex) Syntax Highlighting und Autocompletion"));
    texSyntaxAction->setEnabled(false);
    connect(texSyntaxAction, SIGNAL(triggered()), this, SLOT(enableTexHighlighter()));

    qtSyntaxAction = new QAction(QIcon(QString(":/icons/qt.png")), tr("&Qt"), this);
    qtSyntaxAction->setStatusTip(tr("Aktiviert das Qt Syntax Highlighting und Autocompletion"));
    qtSyntaxAction->setEnabled(false);
    connect(qtSyntaxAction, SIGNAL(triggered()), this, SLOT(enableQtHighlighter()));

    xmlSyntaxAction = new QAction(QIcon(QString(":/icons/xml.png")), tr("&XML"), this);
    xmlSyntaxAction->setStatusTip(tr("Aktiviert das XML Syntax Highlighting und Autocompletion"));
    xmlSyntaxAction->setEnabled(false);
    connect(xmlSyntaxAction, SIGNAL(triggered()), this, SLOT(enableXmlHighlighter()));



    // Help Actions
    aboutAction = new QAction(QIcon(QString(":/icons/question.png")), tr("&Über RDNote"), this);
    aboutAction->setStatusTip(tr("HTML"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    webRedDeerAction = new QAction(tr("&RedDeerNote im Web"), this);
    webRedDeerAction->setStatusTip(tr("HTML"));
    connect(webRedDeerAction, SIGNAL(triggered()), this, SLOT(openWebRedDeer()));

    // MDI Options Actions
    closeAction = new QAction(tr("&Schließen"), this);
    closeAction->setStatusTip(tr("Schließt das aktuelle Fenster"));
    connect(closeAction, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAction = new QAction(tr("&Alle Schließen"), this);
    closeAllAction->setStatusTip(tr("Schließt alle Fenster"));
    connect(closeAllAction, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

    cascadeAction = new QAction(tr("&Cascade Fenster"), this);
    cascadeAction->setStatusTip(tr("Schließt alle Fenster"));
    connect(cascadeAction, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    titledAction = new QAction(tr("&Title Fenster"), this);
    titledAction->setStatusTip(tr("Schließt alle Fenster"));
    connect(titledAction, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    nextAction = new QAction(QIcon(QString(":/icons/next.png")), tr("&Nächstes Fenster"), this);
    nextAction->setStatusTip(tr("Wechselt zum nächsten Fenster"));
    nextAction->setShortcut(QKeySequence(Qt::Key_Control+Qt::Key_Right));
    connect(nextAction, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    prevAction = new QAction(QIcon(QString(":/icons/prev.png")), tr("&Vorheriges Fenster"), this);
    prevAction->setStatusTip(tr("Wechselt zum vorherigen Fenster"));
    connect(prevAction, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

    // Settings Actions
    settingsSyntaxAction = new QAction(QIcon(QString(":/icons/prev.png")), tr("&Syntax Farben"), this);
    settingsSyntaxAction->setStatusTip(tr("Legt die Farben für das Highlighting fest"));
    connect(settingsSyntaxAction, SIGNAL(triggered()), this, SLOT(configureHighlighting()));

    qtAboutAction = new QAction(tr("&Über Qt"), this);
    qtAboutAction->setStatusTip(tr("Qt Info"));
    connect(qtAboutAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

}


void RedDeerNote::openWebRedDeer()
{
    QDesktopServices::openUrl(QUrl("http://www.wukat.de/wordpress/rdnote"));
}

// Die Recent Files aus den Settings auslesen und auf visible setzen
void RedDeerNote::updateRecentFiles()
{
     QSettings settings("MySoft", "Star Runner");
     QStringList files = settings.value("recentFileList").toStringList();
     int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

     for (int i = 0; i < numRecentFiles; ++i) {
         QString text = tr("&%1 %2").arg(i + 1).arg(files[i]);
         recentFilesAction[i]->setText(text);
         recentFilesAction[i]->setData(files[i]);
         recentFilesAction[i]->setVisible(true);
     }

     for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
         recentFilesAction[j]->setVisible(false);

     separatorAct->setVisible(numRecentFiles > 0);
}

// Öffnet ein File das aus der RecentFile Liste gewählt wurde
// wenn Datei schon geöffnet dann springe zu diesem Fenster
//
// mdiArea->setActiveSubWindow(existing);
//
// ansonsten erstelle ein neues MDIFrame und zeige es an
void RedDeerNote::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action){
        QMdiSubWindow *existing = getMdi(action->data().toString());
        if (existing) { //Datei schon geöffnet
            mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiFrame *frame = createMdiFrame();
        if (frame->loadFile(action->data().toString())) {
            statusBar()->showMessage(tr("Datei geöffnet"), 2000);
            frame->show();
        } else {
            frame->close();
        }
    }
}

void RedDeerNote::enableQtHighlighter()
{
   // qtHighlighter = new SyntaxQtHighlighter(activeMdiChild()->document());
   // statusBar()->showMessage("[C++/Qt] Syntax Highlighting aktiviert",1000);

    activeMdiChild()->completer->setModel(modelFromFile(":/apis/qt.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("Qt");
}

void RedDeerNote::enableJavaHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/java.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("Java");
}

void RedDeerNote::enableCppHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/cplusplus.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("C++");
}

void RedDeerNote::enableJavaScriptHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/javascript.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("JavaScript");
}

void RedDeerNote::enableCssHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/css.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("CSS");
}

void RedDeerNote::enableHtmlHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/html.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("HTML");
}

void RedDeerNote::enableLispHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/lisp.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("Lisp");
}

void RedDeerNote::enablePascalHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/pascal.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("Pascal");
}

void RedDeerNote::enablePerlHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/perl.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("Perl");
}

void RedDeerNote::enablePhpHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/php.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("PHP");
}

void RedDeerNote::enablePhytonHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/phyton.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("Phyton");
}

void RedDeerNote::enableTexHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/tex.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("Latex");
}

void RedDeerNote::enableXmlHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/xml.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("XML");
}

void RedDeerNote::enableVbHighlighter(){
    activeMdiChild()->completer->setModel(modelFromFile(":/apis/vb.api"));
    activeMdiChild()->setCompleter(activeMdiChild()->completer);
    activeMdiChild()->setCurType("Visual Basic");
}

/*void RedDeerNote::enableSyntaxHighlighting(ProgLang progLang){
    switch (progLang) {
    case CPLUSPLUS:
    case CSS:
    case HTML:
    case JAVA:
    case JAVASCRIPT:
    case LISP:
    case PASCAL:
    case PERL:
    case PHP:
    case TEX:
    case QT:
        activeMdiChild()->setCurType("C++");
        qtHighlighter = new SyntaxQtHighlighter(activeMdiChild()->document());
        statusBar()->showMessage("[C++/Qt] Syntax Highlighting aktiviert",1000);
    default:
        break;
    }
}*/

void RedDeerNote::configureHighlighting()
{

    qtHighlighter = new SyntaxQtHighlighter(activeMdiChild()->document());
    colorDialog = new ColorDialog(activeMdiChild()->palette().color(QPalette::Text),
                                  activeMdiChild()->palette().color(QPalette::Base),
                                  qtHighlighter->colors(), qtHighlighter->colorNames(), this);
    if (colorDialog->exec() == QDialog::Accepted) {
        QPalette palette = activeMdiChild()->palette();
        palette.setColor(QPalette::Text, colorDialog->foreground());
        palette.setColor(QPalette::Base, colorDialog->background());
        activeMdiChild()->setPalette(palette);

        qtHighlighter->setColors(colorDialog->colors());
        qtHighlighter->rehighlight();

       // qtHighlighter = new SyntaxQtHighlighter(activeMdiChild()->document());
    }
}

// Erstellt die Toolbar
// und fügt die Toogle Actions der Toolbars zu den Settings hinzu
void RedDeerNote::createToolBars()
{
    //!!!! qtoolbar evtl. auslagern in header
    //!!!! so wie in beispiel auf
    //!!!! http://doc.trolltech.com/4.1/mainwindows-mdi-mainwindow-cpp.html

    //Datei Toolbar
    QToolBar* toolBarFile = new QToolBar(this);
    toolBarFile ->setOrientation(Qt::Horizontal);
   // toolBarFile->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBarFile->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBarFile->setIconSize(QSize(20,20));
    this->addToolBar(Qt::TopToolBarArea, toolBarFile );
    toolBarFile->setWindowTitle(tr("Datei Toolbar an/aus"));
    toolBarFile->addAction(fileNewAction);
    toolBarFile->addAction(fileOpenAction);
    toolBarFile->addAction(fileSaveAction);
    toolBarFile->addAction(fileSaveUnderAction);
    toolBarFile->addAction(printAction);

    //Datei Toolbar
    QToolBar* toolBarNetwork = new QToolBar(this);
    toolBarNetwork ->setOrientation(Qt::Horizontal);
   // toolBarNetwork->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBarNetwork->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBarNetwork->setIconSize(QSize(20,20));
    this->addToolBar(Qt::TopToolBarArea, toolBarNetwork );
    toolBarNetwork->setWindowTitle(tr("Netzwerk Toolbar an/aus"));
    toolBarNetwork->addAction(sendFileToUserAction);
    toolBarNetwork->addAction(syncFileAction);
    toolBarNetwork->addAction(chatAction);
    toolBarNetwork->addAction(lockUnlockAction);

    //Datei Toolbar
    QToolBar* toolBarSearch = new QToolBar(this);
    toolBarSearch ->setOrientation(Qt::Horizontal);
   // toolBarSearch->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBarSearch->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBarSearch->setIconSize(QSize(20,20));
    this->addToolBar(Qt::TopToolBarArea, toolBarSearch );
    toolBarSearch->setWindowTitle(tr("Suche Toolbar an/aus"));
    toolBarSearch->addAction(findAction);

    settingsTollbarMenu->addAction(toolBarFile->toggleViewAction());
    settingsTollbarMenu->addAction(toolBarNetwork->toggleViewAction());
    settingsTollbarMenu->addAction(toolBarSearch->toggleViewAction());
}

// Erstellt die Statusbar
void RedDeerNote::createStatusBars()
{
    firstLabel = new QLabel(this);
    firstLabel->setAlignment(Qt::AlignHCenter);
    firstLabel->setMinimumSize(firstLabel->sizeHint());

    secondLabel = new QLabel(this);
    secondLabel->setAlignment(Qt::AlignHCenter);
    secondLabel->setMinimumSize(secondLabel->sizeHint());

    thirdLabel = new QLabel(this);
    thirdLabel->setAlignment(Qt::AlignHCenter);
    thirdLabel->setMinimumSize(thirdLabel->sizeHint());

    codierungsLabel = new QLabel(this);
    codierungsLabel->setAlignment(Qt::AlignHCenter);
    codierungsLabel->setMinimumSize(thirdLabel->sizeHint());

    maxLinesLabel = new QLabel(this);
    maxLinesLabel->setAlignment(Qt::AlignHCenter);
    maxLinesLabel->setMinimumSize(thirdLabel->sizeHint());

    statusBar()->addWidget(firstLabel);
    statusBar()->addWidget(secondLabel);
    statusBar()->addWidget(thirdLabel);
    statusBar()->addWidget(codierungsLabel);
    statusBar()->addWidget(maxLinesLabel);
}

// Updatet die Statusbar
// jenachdem welcher Datentyp die Datei ist, wird die Sprache gesetzt
// weitere Anzeigen Zeichen, Zeile, Datentyp, Sprache, Codierung
void RedDeerNote::updateStatusBar()
{
    //Prüfen ob Child aktiv ist
    if(activeMdiChild() != 0){
        QString content = activeMdiChild()->document()->toPlainText();
        QString type= activeMdiChild()->currentType();
        QString lang;
        int length= content.length();
        int rows = activeMdiChild()->document()->lineCount();


        if(type == "cpp" || type == "h")
            lang ="C++";

        if(type == "java")
            lang = "Java";

        if(type == "txt")
            lang = tr("Datei(Textdokument)");

        if(type == "api")
            lang = tr("Api Datei");

        firstLabel->setText(tr("Zeichen: %1   ")
                            .arg(length));
        secondLabel->setText(tr("Zeile: %1   ")
                             .arg(rows));
        thirdLabel->setText(tr("Datentyp: %1   ")
                            .arg(type));
        codierungsLabel->setText(tr("Codierung: %1   ")
                            .arg(type));

        maxLinesLabel->setText(tr("Sprache: %1   ")
                            .arg(lang));
    }
}

void RedDeerNote::createMdiArea()
{
    mdiArea = new QMdiArea;
    // Scrollbar nur anzeigen wenn benötigt
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // Tabs oben ausrichten
    mdiArea->setTabPosition(QTabWidget::North);
    // Dokumente in TabView
    mdiArea->setViewMode(QMdiArea::TabbedView);

    setCentralWidget(mdiArea);

    // Font für mdiArea festlegen
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    mdiArea->setFont(font);
}


void RedDeerNote::newFile()
{
    MdiFrame *frame = createMdiFrame();
    frame->newFile();
    frame->show();

    emit updateDocumentDock();
    connect(frame,SIGNAL(destroyed()),this,SLOT(updateDocumentDock()));

    //mFindReplaceDialog->setTextEdit(activeMdiChild()->document());
}

void RedDeerNote::updateDocumentDock(){
    QList<QMdiSubWindow *> listtest;
    listtest=mdiArea->subWindowList(QMdiArea::CreationOrder);
    documentList->clear();

    for(int i=0;i<listtest.count();i++){
        emit newFileSignal(listtest.at(i)->windowTitle());
    }
}

MdiFrame *RedDeerNote::createMdiFrame()
{
    MdiFrame *frame = new MdiFrame();
    mdiArea->addSubWindow(frame);
    connect(frame, SIGNAL(textChanged()),this, SLOT(updateStatusBar()));
    connect(frame,SIGNAL(textChanged()),this,SLOT(buttonChecker()));

    //frame->setCompleter(completer);

    return frame;
}

void RedDeerNote::setRecentFile(const QString &fileName)
{
    QString curFile = fileName;
    setWindowFilePath(curFile);

    QSettings settings("MySoft", "Star Runner");
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentFiles();
}

void RedDeerNote::about(){
    QMessageBox::about(this, tr("About RedDeerNote"),
                tr("<h4>RDNote v1.0</h4>"
                   "<h4>Informationen:</h4>"
                   "<p>Red Deer Note ist ein Codeeditor auf Basis von Qt. Für verschiedene Programmiersprachen. "
                   "Dieses Programm entstand im Rahmen des Faches Qt - Ein Universelles C++ Framework an der Fachochschule Kaiserslautern.<br>"
                   "Die verschiedenen Funktionen können auf der Internetseite nachgelesen werden."
                   "<br>"
                   "<i>Author: Sascha Heyer</i><br>"
                   "<i>Web: www.saschaheyer.de</i><br>"
                   "<i>E-Mail: mail@saschaheyer.de</i><br><hr />"
                   "</p><h4>Lizenz:</h4>"
                   "<small>GNU General Public Licence<br><br>"
                   "This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version."
                   "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details."
                   "You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.</small>"));
}

MdiFrame *RedDeerNote::activeMdiChild()
{
     if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()){

         return qobject_cast<MdiFrame *>(activeSubWindow->widget());
     }

     return 0;
}


void RedDeerNote::showSaveUnderDialog()
{

    if (activeMdiChild() && activeMdiChild()->saveAs()){
        setRecentFile( activeMdiChild()->currentFile());
        updateRecentFiles();
        statusBar()->showMessage((activeMdiChild()->userSpecCurFile()+tr(" wurde gespeichert")), 2000);
        fileSaveAction->setEnabled(true);  // Jetzt schnellspeichern aktiviert
    }
}



void RedDeerNote::showPrintDialog()
{
    QTextDocument *document = activeMdiChild()->document();
    QPrinter printer;

    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() != QDialog::Accepted)
        return;

    document->print(&printer);


       // noch in MDI Frame auslagern
  // if (activeMdiChild() && activeMdiChild()->print())
        //     statusBar()->showMessage((activeMdiChild()->userSpecCurFile()+tr(" wurde gedruckt")), 2000);

}

void RedDeerNote::buttonChecker()
{
    bool hasMdiFrame = (activeMdiChild() != 0);

    bool hasMdiFrameContent = false;
    if(hasMdiFrame)
        hasMdiFrameContent = (activeMdiChild()->document()->toPlainText().count() != 0);

    // Aktivieren wenn Datei vorhanden
    titledAction->setEnabled(hasMdiFrame);
    cascadeAction->setEnabled(hasMdiFrame);
    nextAction->setEnabled(hasMdiFrame);
    prevAction->setEnabled(hasMdiFrame);
    prevAction->setEnabled(hasMdiFrame);
    closeAction->setEnabled(hasMdiFrame);
    closeAllAction->setEnabled(hasMdiFrame);
    htmlSyntaxAction->setEnabled(hasMdiFrame);
    fileSaveUnderAction->setEnabled(hasMdiFrame);
    chatAction->setEnabled(hasMdiFrame);
    lockUnlockAction->setEnabled(hasMdiFrame);

    cplusplusSyntaxAction->setEnabled(hasMdiFrame);
    cssSyntaxAction->setEnabled(hasMdiFrame);
    javaSyntaxAction->setEnabled(hasMdiFrame);
    javaScriptSyntaxAction->setEnabled(hasMdiFrame);
    lispSyntaxAction->setEnabled(hasMdiFrame);
    pascalSyntaxAction->setEnabled(hasMdiFrame);
    perlSyntaxAction->setEnabled(hasMdiFrame);
    phpSyntaxAction->setEnabled(hasMdiFrame);
    texSyntaxAction->setEnabled(hasMdiFrame);
    qtSyntaxAction->setEnabled(hasMdiFrame);

    // Aktivieren wenn Inhalt in Datei
    sendFileToUserAction->setEnabled(hasMdiFrameContent);
    syncFileAction->setEnabled(hasMdiFrameContent);
    printAction->setEnabled(hasMdiFrameContent);
    findAction->setEnabled(hasMdiFrameContent);

}

void RedDeerNote::save(){
    if (activeMdiChild() && activeMdiChild()->save()){
             statusBar()->showMessage(tr("gespeichert"), 2000);
    }
}

void RedDeerNote::showFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()) {
            QMdiSubWindow *existing = getMdi(fileName);
            if (existing) {
                mdiArea->setActiveSubWindow(existing);
                return;
            }

            MdiFrame *frame = createMdiFrame();
            if (frame->loadFile(fileName)) {
                statusBar()->showMessage(tr("Datei geöffnet"), 2000);
                frame->show();
                setRecentFile( activeMdiChild()->currentFile());
                emit newFileSignal(activeMdiChild()->documentTitle());
                emit updateDocumentDock();
                //emit fileToWatcher(fileName);
                connect(frame,SIGNAL(destroyed()),this,SLOT(updateDocumentDock()));
            } else {
                frame->close();
            }
        }
}



void RedDeerNote::setActiveSubWindow(QWidget *window)
 {
     if (!window){
             sendFileToUserAction->setEnabled(false);
         return;
     }

     mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
     mFindReplaceDialog->setTextEdit(activeMdiChild()->document());

}

QMdiSubWindow *RedDeerNote::getMdi(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        MdiFrame *mdi = qobject_cast<MdiFrame *>(window->widget());
        if (mdi->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

void RedDeerNote::findDialog()
{
    mFindReplaceDialog->setTextEdit(activeMdiChild()->document());
    mFindReplaceDialog->show();
}

// Eingehende Dateien verwalten
// Parameter:
//      user -> von wem kommt die Datei
//      code -> Code / Datei die übertragen wurde
//
// Wenn die Datei angenommen wird in neuem MDIFrame öffnen
void RedDeerNote::incomingFile(const QString &user,const QString &code)
{
    QMessageBox msgBox;
    msgBox.setText(user+tr(" hat die eine Datei geschickt. Möchtest du diese Datei öffnen?"));
    msgBox.setInformativeText(tr("Öffne die Vorschau um mehr zu erfahren"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setDetailedText(code);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();

    MdiFrame *frame;

    switch (ret) {
        case QMessageBox::Yes:
            frame = createMdiFrame();
            frame->newFile();
            frame->show();
            fileSaveUnderAction->setEnabled(true);
            activeMdiChild()->document()->setPlainText(code);
            break;
        case QMessageBox::No:
            break;
        default:
            break;
    }
}

void RedDeerNote::sendFile()
{
    chatWidget->sendCode(activeMdiChild()->document()->toPlainText());
}

void RedDeerNote::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Network Chat"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(chatWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    settingsDockMenu->addAction(dock->toggleViewAction());


    QTreeView * tree = new QTreeView;
    QString sPath = "C:/";
    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath(sPath);
    tree->setModel(dirModel);

    QDockWidget *dock3 = new QDockWidget(tr("Datei Browser"), this);
    dock3->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock3->setWidget(tree);
    addDockWidget(Qt::LeftDockWidgetArea, dock3);
    settingsDockMenu->addAction(dock3->toggleViewAction());

    connect(tree,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(treeViewClicked(QModelIndex)));

    QDockWidget *documentDock = new QDockWidget(tr("Dokumentenliste"), this);
    documentDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    documentList = new QListWidget(documentDock);
    documentDock->setWidget(documentList);
    addDockWidget(Qt::LeftDockWidgetArea, documentDock);
    connect(documentList,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setCurrentFileViewWithModelIndex(QModelIndex)));
    settingsDockMenu->addAction(documentDock->toggleViewAction());
}


// File im DateiBrowser wurde angeklickt
// Parameter:
//      index -> QModelIndex der Laufwerke / Datenträger
//
// EMIT:
//      Statusbar updaten
//
// wenn Item geklicked wurde dann die Datei öffnen (neues MDIFrame)
void RedDeerNote::treeViewClicked(QModelIndex index)
{
   QString sPath= dirModel->fileInfo(index).absoluteFilePath();
   QFileInfo fileInfo(sPath);
   QString ext = fileInfo.completeSuffix();

   if(fileInfo.isFile()){

           QMdiSubWindow *existing = getMdi(sPath);
           if (existing) {
               mdiArea->setActiveSubWindow(existing);
               return;
           }

       MdiFrame *frame = createMdiFrame();
       if (frame->loadFile(sPath)) {
           statusBar()->showMessage(tr("Datei geöffnet"), 2000);
           frame->show();
           setRecentFile( activeMdiChild()->currentFile());
           activeMdiChild()->type=ext;
           emit updateStatusBar();

            emit newFileSignal(activeMdiChild()->documentTitle());
            emit updateDocumentDock();
            connect(frame,SIGNAL(destroyed()),this,SLOT(updateDocumentDock()));
       } else {
           frame->close();
       }
   }
}

void RedDeerNote::setCurrentFileViewWithModelIndex(QModelIndex index)
{
    QList<QMdiSubWindow *> listtest;
    listtest=mdiArea->subWindowList(QMdiArea::CreationOrder);
    mdiArea->setActiveSubWindow(listtest.at(index.row()));
}


void RedDeerNote::insertDocument(const QString document)
{

    qDebug()<<document;
    if(document.isEmpty())
        return;

    documentList->addItem(document);
}



RedDeerNote::~RedDeerNote()
{

}

QAbstractItemModel *RedDeerNote::modelFromFile(const QString& fileName)
{
    QCompleter* completer = new QCompleter();

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

//#ifndef QT_NO_CURSOR
    //QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

//#ifndef QT_NO_CURSOR
   // QApplication::restoreOverrideCursor();
//#endif
    return new QStringListModel(words, completer);

}
