#ifndef REDDEERNOTE_H
#define REDDEERNOTE_H

#include <QtGui/QMainWindow>
#include <QMenu>
#include <QLabel>
#include <QAction>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QListWidget>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QCloseEvent>

#include "mdiframe.h"
#include "findreplacedialog.h"
#include "chatwidget.h"
#include "connection.h"
#include "syntaxhtmlhighlighter.h"
#include "syntaxqthighlighter.h"
#include "ColorDialog.h"

class QMenu;
class QLabel;
class QAction;
class QMdiArea;
class QMdiSubWindow;
class MdiFrame;
class QSignalMapper;
class QListWidget;
class QFileSystemModel;
class QAbstractItemModel;
class QCompleter;
class QProgressBar;
class QComboBox;

class RedDeerNote : public QMainWindow
{
    Q_OBJECT

public:
    RedDeerNote(QWidget *parent = 0);
    ~RedDeerNote();

    enum ProgLang {
        CPLUSPLUS,
        CSS,
        HTML,
        JAVA,
        JAVASCRIPT,
        LISP,
        PASCAL,
        PERL,
        PHP,
        TEX,
        QT
    };

signals:
    void newFileSignal(const QString);           // 553
    void signalUpdateDocumentDock();
    void fileToWatcher(QString);

private Q_SLOTS:
    void newFile();
    void insertDocument(const QString);          // 553


    void updateDocumentDock();

    MdiFrame *createMdiFrame();

    void showSaveUnderDialog();
    void showPrintDialog();
    void save();
    void about();
    void showFileDialog();
    void findDialog();
    void setActiveSubWindow(QWidget *window);
    void sendFile();
    void buttonChecker();
    void updateStatusBar();
    void incomingFile(const QString &from, const QString& message);     //Eingehendes File von Netzwerk
   // void enableHTMLSyntaxHighlighter();
    // void enableQtSyntaxHighlighter();


    void enableQtHighlighter();
    void enableJavaHighlighter();
    void enableCppHighlighter();
    void enableJavaScriptHighlighter();
    void enableCssHighlighter();
    void enableHtmlHighlighter();
    void enableLispHighlighter();
    void enablePascalHighlighter();
    void enablePerlHighlighter();
    void enablePhpHighlighter();
    void enablePhytonHighlighter();
    void enableTexHighlighter();
    void enableXmlHighlighter();
    void enableVbHighlighter();


    void openRecentFile();
    void openWebRedDeer();
    void setCurrentFileViewWithModelIndex(QModelIndex);
    void treeViewClicked(QModelIndex);

    void configureHighlighting();




    void fileChangedHandler(QString);
    void addFileToWatcher(QString);



protected:
    void closeEvent(QCloseEvent *event);


private:
    // CreateMethoden für die GUI Elemente
    void createMenus();
    void createActions();
    void addActionsToMenus();
    void createToolBars();
    void createStatusBars();
    void createMdiArea();
    void createWidgetStack();
    void createDockWindows();
    void updateRecentFiles();
    void setRecentFile(const QString &fileName);

    // Menus
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* findMenu;
    QMenu* progLangMenu;
    QMenu* networkMenu;
    QMenu* windowsMenu;
    QMenu* settingsMenu;
    QMenu* settingsDockMenu;
    QMenu* settingsTollbarMenu;
    QMenu* settingsSyntaxMenu;
    QMenu* helpMenu;

    // Status Bar Labels
    QLabel* firstLabel;
    QLabel* secondLabel;
    QLabel* thirdLabel;
    QLabel* codierungsLabel;
    QLabel* maxLinesLabel;

    // Settings Actions
    QAction* settingsSyntaxAction;

    // FileMenu Actions
    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileSaveUnderAction; 
    QAction* printAction;
    QAction* closeRedDeerAction;

    // MDI Actions
    QAction* cascadeAction;
    QAction* titledAction;
    QAction* nextAction;
    QAction* prevAction;
    QAction* closeAction;
    QAction* closeAllAction;

    // Edit Actions
    //QAction* jumpToRowAction; // unused

    // MDI
    QMdiArea *mdiArea;
    QMdiSubWindow *getMdi(const QString &fileName);
    MdiFrame *activeMdiChild();
    QSignalMapper *windowMapper;

    // Find Actions
    QAction* findAction;

    // Dialogs
    FindReplaceDialog *mFindReplaceDialog;
    ChatWidget* chatWidget;

    // Network Actions
    QAction* sendFileToUserAction;
    QAction* syncFileAction;
    QAction* chatAction;
    QAction* lockUnlockAction;


    Connection* connection;

    // Syntax Actions
    QAction* htmlSyntaxAction;
    QAction* cplusplusSyntaxAction;
    QAction* csharpSyntaxAction;
    QAction* cssSyntaxAction;
    QAction* javaSyntaxAction;
    QAction* javaScriptSyntaxAction;
    QAction* lispSyntaxAction;
    QAction* perlSyntaxAction;
    QAction* pascalSyntaxAction;
    QAction* phpSyntaxAction;
    QAction* phytonSyntaxAction;
    QAction* sqlSyntaxAction;
    QAction* texSyntaxAction;
    QAction* vbSyntaxAction;
    QAction* xmlSyntaxAction;
    QAction* qtSyntaxAction;

    // About Actions
    QAction* aboutAction;
    QAction* qtAboutAction;
    QAction* webRedDeerAction;

    // Highlighter Klassen
    SyntaxHtmlHighlighter* htmlHighlighter;
    SyntaxQtHighlighter* qtHighlighter;

    // Recent Files
    QAction *separatorAct;
    QMenu* recentFilesMenu;
    enum { MaxRecentFiles = 5 };
    QAction *recentFilesAction[MaxRecentFiles];

    // Document Browser
    QListWidget *documentList;

    // File Browser
    QFileSystemModel *dirModel;

    //Code compeltion
    QAbstractItemModel *modelFromFile(const QString& fileName);
   // QCompleter *completer;

   // void setupEditor();

    ColorDialog* colorDialog;

    QFileSystemWatcher* watcher;
    bool mBlockSignals;
};

#endif // REDDEERNOTE_H
