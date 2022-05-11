#include "QKst.h"

#include "plotitem.h"
#include "debug.h"
#include "document.h"
#include "scriptserver.h"
#include "commandlineparser.h"

#include <QUndoGroup>
#include <QMessageBox>
#include <QFileDialog>

using namespace Kst;

QKst::QKst(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    _tabWidget = new TabWidget(this);
    _undoGroup = new QUndoGroup(this);

    _tabWidget->createView();

    setCentralWidget(_tabWidget);
    _tabWidget->setAcceptDrops(false);

//    insertPlot();
}

void QKst::insertPlot() {
    CreatePlotForCurve* cmd = new CreatePlotForCurve(_tabWidget->currentView());
    cmd->createItem();

    PlotItem* plotItem = static_cast<PlotItem*>(cmd->item());
    plotItem->view()->resetPlotFontSizes(plotItem);
    plotItem->view()->configurePlotFontDefaults(plotItem); // copy plots already in window
    plotItem->view()->appendToLayout(CurvePlacement::Auto, plotItem, 0);

}

TabWidget* QKst::tabWidget() const {
    return _tabWidget;
}

QUndoGroup* QKst::undoGroup() const {
    return _undoGroup;
}

Document* Kst::QKst::document() const
{
    return _doc;
}

bool Kst::QKst::isTiedTabs()
{
    // not implemented
    return false;
}

/** set the status bar message.  If you are doing this inside a view
 * object paint() call, then set delayed to true, and call ::updateStatusMessage()
 * later (after leaving paint). This is currently done for you in updateViewItems */
void QKst::setStatusMessage(QString message, int timeout, bool delayed) {
    Debug::self()->log(message, Debug::Warning);
    _statusBarMessage = message;
    _statusBarTimeout = timeout;
    //if (!delayed) {
    //    statusBar()->showMessage(message, timeout);
    //}
}

void Kst::QKst::updateStatusMessage()
{
    Debug::self()->log(_statusBarMessage, Debug::Warning);
}

QString Kst::QKst::statusMessage()
{
    return _statusBarMessage;
}

void Kst::QKst::newDoc(bool force)
{
    bool clearApproved = false;
    if (force) {
        clearApproved = true;
    }
    else {
        clearApproved = promptSaveDone();
    }

    if (clearApproved) {
        cleanup();
        delete _dataManager;
        _dataManager = 0;
        delete _doc;
        resetNameIndexes();
        _doc = new Document(this);
        _scriptServer->setStore(_doc->objectStore());
        tabWidget()->clear();
        tabWidget()->createView();
        return;
    }
    else {
        return;
    }
}

bool Kst::QKst::promptSaveDone()
{
    if (!_doc->isChanged()) {
        return true; // No need to ask if there is no unsaved change -> we're done
    }
    else { // Changes registered: ask the user
        int rc = QMessageBox::warning(this, tr("Kst: Save Prompt"), tr("Your document has been modified.\nSave changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
        if (rc == QMessageBox::Save) {
            save();
        }
        else if (rc == QMessageBox::Cancel) {
            return false;
        }
        return true;
    }
}
void Kst::QKst::save() {
    if (_doc->isOpen()) {
        _doc->save();
    }
    else {
        saveAs();
    }
}

void Kst::QKst::openFile(const QString& file) {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    cleanup();
    newDoc(true); // Does all the init stuff, but does not ask for override as it's supposed to be done elsewhere
    bool ok = _doc->open(file);
    // not implemented
    //    updateProgress(100, "");
    QApplication::restoreOverrideCursor();

    if (!ok) {
        QString lastError = _doc->lastError();
        if (lastError.isEmpty())
            lastError = tr("For details see Help->Debug Dialog->Log.");
        QMessageBox::critical(this, tr("Kst"), tr("Error opening document:\n  '%1'\n%2\n").arg(file, lastError));
    }

    _sessionFileName = file;
    // not implemented
    //setKstWindowTitle();
    //updateRecentKstFiles(file);
}

void Kst::QKst::cleanup() {
    if (document() && document()->objectStore()) {
        document()->objectStore()->clear();
    }
}

void Kst::QKst::saveAs() {
    QString fn = QFileDialog::getSaveFileName(this, tr("Kst: Save File"), _doc->fileName(), tr("Kst Sessions (*.kst)"));
    if (fn.isEmpty()) {
        return;
    }
    QString restorePath = QDir::currentPath();
    QString kstfiledir = fn.left(fn.lastIndexOf('/')) + '/';
    QDir::setCurrent(kstfiledir);
    //QString currentP = QDir::currentPath();
    _doc->save(fn);
    QDir::setCurrent(restorePath);
    _sessionFileName = fn;
    // not implemented
    //setKstWindowTitle();
    //updateRecentKstFiles(fn);
}

void QKst::clearDrawingMarker() {
//    _createBoxAct->setChecked(false);
//    _createSharedAxisBoxAct->setChecked(false);
//    _createCircleAct->setChecked(false);
//    _createEllipseAct->setChecked(false);
//    _createLabelAct->setChecked(false);
//    _createLineAct->setChecked(false);
//    _createArrowAct->setChecked(false);
//    _createPictureAct->setChecked(false);
//    _createPlotAct->setChecked(false);
//#ifndef KST_NO_SVG
//    _createSvgAct->setChecked(false);
//#endif
    _tabWidget->currentView()->setMouseMode(View::Default);
}

bool Kst::QKst::initFromCommandLine()
{
    if(_doc != nullptr)
        delete _doc;
    _doc = new Document(this);
    _scriptServer->setStore(_doc->objectStore());

    CommandLineParser P(_doc, this);

    bool ok = _doc->initFromCommandLine(&P);
//    if (!P.pngFile().isEmpty()) {
//        int w = 1280;
//        int h = 1024;
//
//        if (P.pngWidth() > 1) {
//            w = P.pngWidth();
//        }
//        if (P.pngHeight() > 1) {
//            h = P.pngHeight();
//        }
//        exportGraphicsFile(P.pngFile(), "png", w, h, 2, true, 0);
//        ok = false;
//    }
//    if (!P.printFile().isEmpty()) {
//#ifndef KST_NO_PRINTER
//        printFromCommandLine(P.printFile());
//#endif
//        ok = false;
//    }
//    if (!P.kstFileName().isEmpty()) {
//        _sessionFileName = P.kstFileName();
//        setKstWindowTitle();
//    }
    _doc->setChanged(false);
    return ok;
}

void QKst::updateRecentFiles(const QString& key, QMenu* menu, QList<QAction*>& actions, QMenu* submenu, const QString& newfilename, const char* openslot)
{
    // not implemented
}

void QKst::updateRecentDataFiles(const QString& filename)
{
    // not implemented
    //updateRecentFiles("recentDataFileList", _toolsMenu, _bottomRecentDataActions, _recentDataFilesMenu, filename, SLOT(openRecentDataFile()));
    //if (!filename.isEmpty()) {
    //    dialogDefaults().setValue("vector/datasource", filename);
    //}
}

void QKst::updateRecentKstFiles(const QString& filename)
{
    // not implemented
    //updateRecentFiles("recentKstFileList", _fileMenu, _bottomRecentKstActions, _recentKstFilesMenu, filename, SLOT(openRecentKstFile()));
}


void Kst::QKst::setKstWindowTitle()
{
    QString title = "QKst";
    QString server_name = _scriptServer->serverName;
    QString user_name = "--" + kstApp->userName();

    if (server_name.endsWith(user_name)) {
        server_name.remove(server_name.lastIndexOf(user_name), 10000);
    }

    if (!_sessionFileName.isEmpty()) {
        title += " - " + _sessionFileName;
    }
    if (scriptServerNameSet()) {
        title += " -- " + server_name;
    }
    setWindowTitle(title);
}

QString QKst::scriptServerName()
{
    return _scriptServer->serverName;
}

bool QKst::scriptServerNameSet()
{
    return _scriptServer->serverNameSet;
}

void QKst::setScriptServerName(QString server_name)
{
    _scriptServer->setScriptServerName(server_name);
    setKstWindowTitle();
}

void QKst::createActions() {
    // ***************************** File actions ******************************** //
    _openAct = new QAction(tr("&Open..."), this);
    _openAct->setStatusTip(tr("Open a new session"));
    _openAct->setShortcuts(QKeySequence::Open);
//    _openAct->setIcon(KstGetIcon("document-open"));
//    connect(_openAct, SIGNAL(triggered()), this, SLOT(open()));
}