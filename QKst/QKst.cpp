#include "QKst.h"

#include "plotitem.h"
#include "debug.h"
#include "document.h"
#include "scriptserver.h"
#include "commandlineparser.h"
#include "debugdialog.h"


#include "getIcon.h"    // kstwidget
#include "applicationsettings.h"
#include "settings.h"
#include "updatemanager.h"  // kstcore
#include "datasourcepluginmanager.h"  // kstcore

#include <QUndoGroup>
#include <QMessageBox>
#include <QFileDialog>

using namespace Kst;

QKst::QKst(QWidget *parent)
    : QMainWindow(parent), _settings(createSettings("application"))
{
    ui.setupUi(this);
    _doc = new Document(this);
    _scriptServer = new ScriptServer(_doc->objectStore());
    _tabWidget = new TabWidget(this);
    _undoGroup = new QUndoGroup(this);

    _debugDialog = new DebugDialog(this);
    Debug::self()->setHandler(_debugDialog);

    createActions();
    createMenus();


    _tabWidget->createView();

    setCentralWidget(_tabWidget);
    _tabWidget->setAcceptDrops(false);

    UpdateManager::self()->setMinimumUpdatePeriod(ApplicationSettings::self()->minimumUpdatePeriod());
    DataObject::init();
    DataSourcePluginManager::init();

    updateRecentKstFiles();
    setAcceptDrops(true);

}

void QKst::createPlot() {
    if (_createPlotAct->isChecked()) {
        clearDrawingMarker();
        _createPlotAct->setChecked(true);
        CreatePlotCommand* cmd = new CreatePlotCommand;
        cmd->createItem();
    }
    else {
        _tabWidget->currentView()->setMouseMode(View::Default);
    }
}

void QKst::insertPlot() {
    CreatePlotForCurve* cmd = new CreatePlotForCurve(_tabWidget->currentView());
    cmd->createItem();

    PlotItem* plotItem = static_cast<PlotItem*>(cmd->item());
    plotItem->view()->resetPlotFontSizes(plotItem);
    plotItem->view()->configurePlotFontDefaults(plotItem); // copy plots already in window
    plotItem->view()->appendToLayout(CurvePlacement::Auto, plotItem, 0);

}

void QKst::createMenus() {
    _fileMenu = menuBar()->addMenu(tr("&File"));
    // exit
    _fileMenu->addAction(_exitAct);
    // recent files
    _fileMenu->addSeparator();


    _editMenu = menuBar()->addMenu(tr("&Edit"));
    //_editMenu->addAction(_undoAct);
    //_editMenu->addAction(_redoAct);
    //_editMenu->addAction(_copyTabAct);
    // Cut/Copy/Paste will come here

    _viewMenu = menuBar()->addMenu(tr("&View"));
    //_viewMenu->addAction(_vectorEditorAct);
    //_viewMenu->addAction(_scalarEditorAct);
    //_viewMenu->addAction(_matrixEditorAct);
    //_viewMenu->addAction(_stringEditorAct);

    _rangeMenu = menuBar()->addMenu(tr("&Range"));
    _rangeMenu->addAction(_backAct);
    //_rangeMenu->addAction(_backSmallAct);
    _rangeMenu->addAction(_forwardAct);
    //_rangeMenu->addAction(_forwardSmallAct);
    _rangeMenu->addSeparator();
    _rangeMenu->addAction(_readFromEndAct);
    _rangeMenu->addAction(_readToEndAct);
    _rangeMenu->addAction(_pauseAct);
    _rangeMenu->addSeparator();
    //_rangeMenu->addAction(_changeDataSampleDialogAct);

    _createMenu = menuBar()->addMenu(tr("&Create"));
    // Containers
    _createMenu->addAction(_insertPlotAct);
    _createMenu->addAction(_createPlotAct);
    //_createMenu->addAction(_createSharedAxisBoxAct);
    _createMenu->addSeparator();
    // Primitives
    //_createMenu->addAction(_newScalarAct);
    //_createMenu->addAction(_newVectorAct);
    //_createMenu->addAction(_newMatrixAct);
    //_createMenu->addAction(_newStringAct);
    _createMenu->addSeparator();
    // Data objects
    //_createMenu->addAction(_newEquationAct);
    //_createMenu->addAction(_newHistogramAct);
    //_createMenu->addAction(_newPSDAct);
    //_createMenu->addAction(_newSpectrogramAct);
    //_createMenu->addAction(_newEventMonitorAct);
    _createMenu->addSeparator();

    // Now, create the dynamic plugin menus
    QMenu* _pluginsMenu = _createMenu->addMenu(tr("Standard P&lugin"));
    QMenu* _fitPluginsMenu = _createMenu->addMenu(tr("Fit Pl&ugin"));
    QMenu* _filterPluginsMenu = _createMenu->addMenu(tr("Fil&ter Plugin"));
    //PluginMenuItemAction* action;
    //foreach(const QString & pluginName, DataObject::dataObjectPluginList()) {
    //    action = new PluginMenuItemAction(pluginName, this);
    //    connect(action, SIGNAL(triggered(QString&)), this, SLOT(showPluginDialog(QString&)));
    //    _pluginsMenu->addAction(action);
    //}
    //foreach(const QString & pluginName, DataObject::fitsPluginList()) {
    //    action = new PluginMenuItemAction(pluginName, this);
    //    connect(action, SIGNAL(triggered(QString&)), this, SLOT(showPluginDialog(QString&)));
    //    _fitPluginsMenu->addAction(action);
    //}
    //foreach(const QString & pluginName, DataObject::filterPluginList()) {
    //    action = new PluginMenuItemAction(pluginName, this);
    //    connect(action, SIGNAL(triggered(QString&)), this, SLOT(showPluginDialog(QString&)));
    //    _filterPluginsMenu->addAction(action);
    //}
    _createMenu->addSeparator();
    // Relations
    //_createMenu->addAction(_newCurveAct);
    //_createMenu->addAction(_newImageAct);
    //_createMenu->addSeparator();
    // Annotation objects
    QMenu* annotations = _createMenu->addMenu(tr("&Annotation"));
    //annotations->addAction(_createLabelAct);
    //annotations->addAction(_createBoxAct);
    //annotations->addAction(_createCircleAct);
    //annotations->addAction(_createEllipseAct);
    //annotations->addAction(_createLineAct);
    //annotations->addAction(_createArrowAct);
    //annotations->addAction(_createPictureAct);
    //annotations->addAction(_createSvgAct);


    _modeMenu = menuBar()->addMenu(tr("&Mode"));
    // Interaction mode
    _modeMenu->addSeparator()->setText(tr("Interaction mode"));
    //_modeMenu->addAction(_standardZoomAct);
    //_modeMenu->addAction(_xOnlyZoomAct);
    //_modeMenu->addAction(_yOnlyZoomAct);
    //_modeMenu->addAction(_layoutModeAct);
    //_standardZoomAct->setChecked(true);
    _modeMenu->addSeparator();
    // Tied zoom options
    //_modeMenu->addAction(_tiedZoomAct);
    //_modeMenu->addAction(_tabTiedAct);
    //_modeMenu->addSeparator();
    // Data mode
    //_modeMenu->addAction(_highlightPointAct);

    _toolsMenu = menuBar()->addMenu(tr("&Tools"));
    //_toolsMenu->addAction(_dataManagerAct);
    //_toolsMenu->addAction(_dataWizardAct);
    //_toolsMenu->addAction(_changeFileDialogAct);
    //_toolsMenu->addAction(_chooseColorDialogAct);
    //_toolsMenu->addAction(_differentiateCurvesDialogAct);
    //_toolsMenu->addAction(_filterMultipleDialogAct);
    _toolsMenu->addSeparator();
    updateRecentDataFiles();

    _settingsMenu = menuBar()->addMenu(tr("&Settings"));
    //_settingsMenu->addAction(_settingsDialogAct);
    //_settingsMenu->addAction(_themeDialogAct);
    //_settingsMenu->addAction(_clearUISettings);
    //_settingsMenu->addAction(_clearDataSourceSettings);
    //_settingsMenu->addAction(_toggleStatusBar);

    menuBar()->addSeparator();


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
    Kst::View* view = _tabWidget->currentView();
    if(view)
        view->setMouseMode(View::Default);
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
    //_openAct->setIcon(KstGetIcon("document-open"));
    connect(_openAct, SIGNAL(triggered()), this, SLOT(open()));

    _saveAct = new QAction(tr("&Save"), this);
    _saveAct->setStatusTip(tr("Save the current session"));
    _saveAct->setShortcuts(QKeySequence::Save);
    //_saveAct->setIcon(KstGetIcon("document-save"));
    connect(_saveAct, SIGNAL(triggered()), this, SLOT(save()));

    _saveAsAct = new QAction(tr("Save &as..."), this);
    _saveAsAct->setStatusTip(tr("Save the current session"));
    //_saveAsAct->setIcon(KstGetIcon("document-save-as"));
    _saveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(_saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    _closeAct = new QAction(tr("Ne&w Session"), this);
    _closeAct->setStatusTip(tr("Close current session and start a new, empty one"));
    //_closeAct->setIcon(KstGetIcon("document-close"));
    _closeAct->setShortcuts(QKeySequence::Close);
    connect(_closeAct, SIGNAL(triggered()), this, SLOT(newDoc()));

    _reloadAct = new QAction(tr("Reload All &Data Sources"), this);
    _reloadAct->setStatusTip(tr("Reload all data sources"));
    //_reloadAct->setIcon(KstGetIcon("kst_reload"));
    connect(_reloadAct, SIGNAL(triggered()), this, SLOT(reload()));

    _printAct = new QAction(tr("&Print..."), this);
    _printAct->setStatusTip(tr("Print the current view"));
    //_printAct->setIcon(KstGetIcon("document-print"));
    _printAct->setShortcuts(QKeySequence::Print);
    connect(_printAct, SIGNAL(triggered()), this, SLOT(print()));
#ifdef KST_NO_PRINTER
    _printAct->setEnabled(false);
#endif

    _exportGraphicsAct = new QAction(tr("&Export as Image(s)..."), this);
    _exportGraphicsAct->setStatusTip(tr("Export graphics to disk"));
    _exportGraphicsAct->setIcon(KstGetIcon("insert-image"));
    connect(_exportGraphicsAct, SIGNAL(triggered()), this, SLOT(showExportGraphicsDialog()));

    _exportVectorsAct = new QAction(tr("Save &Vectors to Disk..."), this);
    _exportVectorsAct->setStatusTip(tr("Export vectors to ascii file"));
    _exportVectorsAct->setIcon(KstGetIcon("save-vectors"));
    connect(_exportVectorsAct, SIGNAL(triggered()), this, SLOT(showExportVectorsDialog()));

    _logAct = new QAction(tr("&Log Entry..."), this);
    _logAct->setStatusTip(tr("Commit a log entry"));
    _logAct->setIcon(KstGetIcon("new-log-event"));
    connect(_logAct, SIGNAL(triggered()), this, SLOT(showLogDialog()));

    _newTabAct = new QAction(tr("&New Tab"), this);
    _newTabAct->setStatusTip(tr("Create a new tab"));
    _newTabAct->setIcon(KstGetIcon("tab-new"));

    connect(_newTabAct, SIGNAL(triggered()), tabWidget(), SLOT(createView()));

    _closeTabAct = new QAction(tr("&Close Tab"), this);
    _closeTabAct->setStatusTip(tr("Close the current tab"));
    _closeTabAct->setIcon(KstGetIcon("tab-close"));
    connect(_closeTabAct, SIGNAL(triggered()), tabWidget(), SLOT(closeCurrentView()));

    _exitAct = new QAction(tr("E&xit"), this);
    _exitAct->setShortcuts(QKeySequence::Quit);
    _exitAct->setStatusTip(tr("Exit the application"));
    _exitAct->setIcon(KstGetIcon("application-exit"));
    connect(_exitAct, SIGNAL(triggered()), this, SLOT(close()));

    // ************************** Edit Actions ******************************* //
    _undoAct = _undoGroup->createUndoAction(this);
    _undoAct->setShortcuts(QKeySequence::Undo);
    _undoAct->setIcon(KstGetIcon("edit-undo"));

    _redoAct = _undoGroup->createRedoAction(this);
    _redoAct->setShortcuts(QKeySequence::Redo);
    _redoAct->setIcon(KstGetIcon("edit-redo"));

    _copyTabAct = new QAction(tr("Copy Tab Image"), this);
    _copyTabAct->setStatusTip("Copy image of the current tab to the desktop clipboard");
    _copyTabAct->setIcon(KstGetIcon("edit-copy"));
    connect(_copyTabAct, SIGNAL(triggered()), this, SLOT(copyTab()));

    // ********************* View Actions ********************************** //
    _vectorEditorAct = new QAction(tr("&Vectors"), this);
    _vectorEditorAct->setStatusTip(tr("Show all vectors in a spreadsheet"));
    connect(_vectorEditorAct, SIGNAL(triggered()), this, SLOT(showVectorEditor()));

    _scalarEditorAct = new QAction(tr("&Scalars"), this);
    _scalarEditorAct->setStatusTip(tr("Show all scalars in a spreadsheet"));
    connect(_scalarEditorAct, SIGNAL(triggered()), this, SLOT(showScalarEditor()));

    _matrixEditorAct = new QAction(tr("&Matrices"), this);
    _matrixEditorAct->setStatusTip(tr("Show all matrices in a spreadsheet"));
    connect(_matrixEditorAct, SIGNAL(triggered()), this, SLOT(showMatrixEditor()));

    _stringEditorAct = new QAction(tr("S&trings"), this);
    _stringEditorAct->setStatusTip(tr("Show all strings in a spreadsheet"));
    connect(_stringEditorAct, SIGNAL(triggered()), this, SLOT(showStringEditor()));

    // ************************ Data Range 1 click Actions ************************** //
    _backAct = new QAction(tr("&Back One Screen"), this);
    _backAct->setStatusTip(tr("Back one screen (Page Up)"));
    _backAct->setToolTip(tr("Back one screen (Page Up)"));
    _backAct->setIcon(KstGetIcon("page-previous"));
    _backAct->setShortcut(Qt::Key_PageUp);
    connect(_backAct, SIGNAL(triggered()), this, SLOT(back()));

    _forwardAct = new QAction(tr("&Forward One Screen"), this);
    _forwardAct->setStatusTip(tr("Forward one screen (Page Down)"));
    _forwardAct->setToolTip(tr("Forward one screen (Page Down)"));
    _forwardAct->setIcon(KstGetIcon("page-next"));
    _forwardAct->setShortcut(Qt::Key_PageDown);
    connect(_forwardAct, SIGNAL(triggered()), this, SLOT(forward()));

    _backSmallAct = new QAction(tr("&Back 1/5th Screen"), this);
    _backSmallAct->setStatusTip(tr("Back 1/5th screen (Shift + Page Up)"));
    _backSmallAct->setToolTip(tr("Back 1/5th screen (Shift + Page Up)"));
    _backSmallAct->setIcon(KstGetIcon("page-previous"));
    _backSmallAct->setShortcut(Qt::SHIFT + Qt::Key_PageDown);
    connect(_backSmallAct, SIGNAL(triggered()), this, SLOT(backSmall()));

    _forwardSmallAct = new QAction(tr("&Forward 1/5th Screen"), this);
    _forwardSmallAct->setStatusTip(tr("Forward 1/5th screen (Page Down)"));
    _forwardSmallAct->setToolTip(tr("Forward 1/5th screen (Page Down)"));
    _forwardSmallAct->setIcon(KstGetIcon("page-next"));
    _forwardSmallAct->setShortcut(Qt::SHIFT + Qt::Key_PageUp);
    connect(_forwardSmallAct, SIGNAL(triggered()), this, SLOT(forwardSmall()));

    _readFromEndAct = new QAction(tr("&Count from End"), this);
    _readFromEndAct->setStatusTip(tr("Count from end mode (End)"));
    _readFromEndAct->setToolTip(tr("Count from end mode (End)"));
    _readFromEndAct->setIcon(KstGetIcon("count-from-end"));
    _readFromEndAct->setShortcut(Qt::Key_End);
    connect(_readFromEndAct, SIGNAL(triggered()), this, SLOT(readFromEnd()));

    _readToEndAct = new QAction(tr("&Read to End"), this);
    _readToEndAct->setStatusTip(tr("Read to end mode"));
    _readToEndAct->setToolTip(tr("Read to end mode (shift+end)"));
    _readToEndAct->setIcon(KstGetIcon("read-to-end"));
    _readToEndAct->setShortcut(Qt::ShiftModifier + Qt::Key_End);
    connect(_readToEndAct, SIGNAL(triggered()), this, SLOT(readToEnd()));

    _pauseAct = new QAction(tr("&Pause"), this);
    _pauseAct->setIcon(KstGetIcon("pause"));
    _pauseAct->setCheckable(true);
    _pauseAct->setShortcut(tr("p", "shortcut for pause"));
    _pauseAct->setStatusTip(tr("Toggle pause updates of data sources (%1)").arg(_pauseAct->shortcut().toString()));
    _pauseAct->setToolTip(tr("Toggle \"Pause Data Sources\" (%1)").arg(_pauseAct->shortcut().toString()));
    connect(_pauseAct, SIGNAL(toggled(bool)), this, SLOT(pause(bool)));

    _changeDataSampleDialogAct = new QAction(tr("Change Data &Sample Range..."), this);
    _changeDataSampleDialogAct->setShortcut(tr("J", "shortcut change data sample dialog"));
    _changeDataSampleDialogAct->setStatusTip(tr("Show Kst's Change Data Sample Range Dialog (%1)").arg(_changeDataSampleDialogAct->shortcut().toString()));
    _changeDataSampleDialogAct->setToolTip(tr("Change Data Sample Range (%1)").arg(_changeDataSampleDialogAct->shortcut().toString()));
    _changeDataSampleDialogAct->setIcon(KstGetIcon("kst_changenpts"));
    connect(_changeDataSampleDialogAct, SIGNAL(triggered()), this, SLOT(showChangeDataSampleDialog()));

    // ************************ Create Actions ************************** //
    _insertPlotAct = new QAction(tr("&Plot"), this);
    _insertPlotAct->setIcon(KstGetIcon("kst_newplot"));
    _insertPlotAct->setShortcut(QString("F11"));
    _insertPlotAct->setStatusTip(tr("Insert a plot in the current view (%1)").arg(_insertPlotAct->shortcut().toString()));
    _insertPlotAct->setToolTip(tr("New plot (%1)").arg(_insertPlotAct->shortcut().toString()));
    _insertPlotAct->setCheckable(false);
    connect(_insertPlotAct, SIGNAL(triggered()), this, SLOT(insertPlot()));

    _createPlotAct = new QAction(tr("&Plot"), this);
    _createPlotAct->setIcon(KstGetIcon("kst_newplot"));
    //_createPlotAct->setShortcut(QString("F11"));
    _createPlotAct->setStatusTip(tr("Create a plot for the current view"));
    _createPlotAct->setCheckable(true);
    connect(_createPlotAct, SIGNAL(triggered()), this, SLOT(createPlot()));

    _newScalarAct = new QAction(tr("&Scalar"), this);
    connect(_newScalarAct, SIGNAL(triggered()), this, SLOT(createScalar()));

    _newVectorAct = new QAction(tr("&Vector"), this);
    connect(_newVectorAct, SIGNAL(triggered()), this, SLOT(createVector()));

    _newMatrixAct = new QAction(tr("&Matrix"), this);
    connect(_newMatrixAct, SIGNAL(triggered()), this, SLOT(createMatrix()));

    _newStringAct = new QAction(tr("Strin&g"), this);
    connect(_newStringAct, SIGNAL(triggered()), this, SLOT(createString()));

    _newCurveAct = new QAction(tr("&Curve"), this);
    connect(_newCurveAct, SIGNAL(triggered()), this, SLOT(createCurve()));

    _newEquationAct = new QAction(tr("&Equation"), this);
    connect(_newEquationAct, SIGNAL(triggered()), this, SLOT(createEquation()));

    _newPSDAct = new QAction(tr("Po&wer Spectrum"), this);
    connect(_newPSDAct, SIGNAL(triggered()), this, SLOT(createPSD()));

    _newHistogramAct = new QAction(tr("&Histogram"), this);
    connect(_newHistogramAct, SIGNAL(triggered()), this, SLOT(createHistogram()));

    _newImageAct = new QAction(tr("&Image"), this);
    connect(_newImageAct, SIGNAL(triggered()), this, SLOT(createImage()));

    _newSpectrogramAct = new QAction(tr("Spectr&ogram"), this);
    connect(_newSpectrogramAct, SIGNAL(triggered()), this, SLOT(createSpectogram()));

    _newEventMonitorAct = new QAction(tr("Eve&nt Monitor"), this);
    connect(_newEventMonitorAct, SIGNAL(triggered()), this, SLOT(createEventMonitor()));

    // Advanced layout
    _createLabelAct = new QAction(tr("&Label"), this);
    _createLabelAct->setIcon(KstGetIcon("kst_gfx_label"));
    _createLabelAct->setShortcut(QString("F3"));
    _createLabelAct->setStatusTip(tr("Create a label for the current view (%1)").arg(_createLabelAct->shortcut().toString()));
    _createLabelAct->setToolTip(tr("Create label (%1)").arg(_createLabelAct->shortcut().toString()));
    _createLabelAct->setCheckable(true);
    connect(_createLabelAct, SIGNAL(triggered()), this, SLOT(createLabel()));

    _createBoxAct = new QAction(tr("&Box"), this);
    _createBoxAct->setIcon(KstGetIcon("kst_gfx_rectangle"));
    _createBoxAct->setShortcut(QString("F4"));
    _createBoxAct->setStatusTip(tr("Create a box for the current view (%1)").arg(_createBoxAct->shortcut().toString()));
    _createBoxAct->setToolTip(tr("Create box (%1)").arg(_createBoxAct->shortcut().toString()));
    _createBoxAct->setCheckable(true);
    connect(_createBoxAct, SIGNAL(triggered()), this, SLOT(createBox()));

    _createCircleAct = new QAction(tr("&Circle"), this);
    _createCircleAct->setIcon(KstGetIcon("kst_gfx_circle"));
    _createCircleAct->setShortcut(QString("F5"));
    _createCircleAct->setStatusTip(tr("Create a circle for the current view (%1)").arg(_createCircleAct->shortcut().toString()));
    _createCircleAct->setToolTip(tr("Create circle (%1)").arg(_createCircleAct->shortcut().toString()));
    _createCircleAct->setCheckable(true);
    connect(_createCircleAct, SIGNAL(triggered()), this, SLOT(createCircle()));

    _createEllipseAct = new QAction(tr("&Ellipse"), this);
    _createEllipseAct->setIcon(KstGetIcon("kst_gfx_ellipse"));
    _createEllipseAct->setShortcut(QString("F6"));
    _createEllipseAct->setStatusTip(tr("Create an ellipse for the current view (%1)").arg(_createEllipseAct->shortcut().toString()));
    _createEllipseAct->setToolTip(tr("Create ellipse (%1)").arg(_createEllipseAct->shortcut().toString()));
    _createEllipseAct->setCheckable(true);
    connect(_createEllipseAct, SIGNAL(triggered()), this, SLOT(createEllipse()));

    _createLineAct = new QAction(tr("&Line"), this);
    _createLineAct->setIcon(KstGetIcon("kst_gfx_line"));
    _createLineAct->setShortcut(QString("F7"));
    _createLineAct->setStatusTip(tr("Create a line for the current view (%1)").arg(_createLineAct->shortcut().toString()));
    _createLineAct->setToolTip(tr("Create line (%1)").arg(_createLineAct->shortcut().toString()));
    _createLineAct->setCheckable(true);
    connect(_createLineAct, SIGNAL(triggered()), this, SLOT(createLine()));

    _createArrowAct = new QAction(tr("&Arrow"), this);
    _createArrowAct->setIcon(KstGetIcon("kst_gfx_arrow"));
    _createArrowAct->setShortcut(QString("F8"));
    _createArrowAct->setStatusTip(tr("Create a arrow for the current view (%1)").arg(_createArrowAct->shortcut().toString()));
    _createArrowAct->setToolTip(tr("Create arrow (%1)").arg(_createArrowAct->shortcut().toString()));
    _createArrowAct->setCheckable(true);
    connect(_createArrowAct, SIGNAL(triggered()), this, SLOT(createArrow()));

    _createPictureAct = new QAction(tr("&Picture"), this);
    _createPictureAct->setIcon(KstGetIcon("insert-image"));
    _createPictureAct->setShortcut(QString("F9"));
    _createPictureAct->setStatusTip(tr("Create a picture for the current view (%1)").arg(_createPictureAct->shortcut().toString()));
    _createPictureAct->setToolTip(tr("Create picture (%1)").arg(_createPictureAct->shortcut().toString()));
    _createPictureAct->setCheckable(true);
    connect(_createPictureAct, SIGNAL(triggered()), this, SLOT(createPicture()));

#ifndef KST_NO_SVG
    _createSvgAct = new QAction(tr("&Svg"), this);
    _createSvgAct->setIcon(KstGetIcon("draw-bezier-curves"));
    _createSvgAct->setShortcut(QString("F10"));
    _createSvgAct->setStatusTip(tr("Create a svg for the current view (%1)").arg(_createSvgAct->shortcut().toString()));
    _createSvgAct->setToolTip(tr("Create embedded svg image (%1)").arg(_createSvgAct->shortcut().toString()));
    _createSvgAct->setCheckable(true);
    connect(_createSvgAct, SIGNAL(triggered()), this, SLOT(createSvg()));
#endif

    _createSharedAxisBoxAct = new QAction(tr("Shared Axis &Box"), this);
    _createSharedAxisBoxAct->setStatusTip(tr("Create a shared axis box for the current item"));
    _createSharedAxisBoxAct->setIcon(KstGetIcon("kst_gfx_sharedaxisbox"));
    _createSharedAxisBoxAct->setCheckable(true);
    connect(_createSharedAxisBoxAct, SIGNAL(triggered()), this, SLOT(createSharedAxisBox()));

    // ************************** Mode Actions ******************************* //
    // First, general options
    _tiedZoomAct = new QAction(tr("&Toggle Tied Zoom"), this);
    _tiedZoomAct->setIcon(KstGetIcon("tied-zoom"));
    _tiedZoomAct->setCheckable(false);
    _tiedZoomAct->setShortcut(Qt::Key_O);
    _tiedZoomAct->setStatusTip(tr("Toggle the current view's tied zoom (%1)").arg(_tiedZoomAct->shortcut().toString()));
    _tiedZoomAct->setToolTip(tr("Toggle tied zoom (%1)").arg(_tiedZoomAct->shortcut().toString()));
    connect(_tiedZoomAct, SIGNAL(triggered()), this, SLOT(toggleTiedZoom()));

    _tabTiedAct = new QAction(tr("Tie &Across All Tabs"), this);
    _tabTiedAct->setStatusTip(tr("Tied zoom applies between tabs"));
    //_tiedZoomAct->setIcon(KstGetIcon("tied-zoom"));
    _tabTiedAct->setCheckable(true);
    //connect(_tiedZoomAct, SIGNAL(triggered()), this, SLOT(toggleTiedZoom()));


    _highlightPointAct = new QAction(tr("&Highlight Data Points"), this);
    _highlightPointAct->setShortcut(QString("h"));
    _highlightPointAct->setStatusTip(tr("Highlight closest data point (%1)").arg(_highlightPointAct->shortcut().toString()));
    _highlightPointAct->setToolTip(tr("Highlight closest data point (%1)").arg(_highlightPointAct->shortcut().toString()));
    _highlightPointAct->setIcon(KstGetIcon("kst_datamode"));
    _highlightPointAct->setCheckable(true);
    connect(_highlightPointAct, SIGNAL(toggled(bool)), this, SLOT(setHighlightPoint(bool)));


    // Then, exclusive interaction modes
    QActionGroup* _interactionModeGroup = new QActionGroup(this);

    _standardZoomAct = _interactionModeGroup->addAction(tr("X-Y Zoom/Scroll"));
    _standardZoomAct->setStatusTip(tr("Zoom arbitrarily in X- or Y-direction"));
    _standardZoomAct->setShortcut(Qt::Key_K);
    _standardZoomAct->setToolTip(tr("X-Y Zoom mode (%1)").arg(_standardZoomAct->shortcut().toString()));
    _standardZoomAct->setCheckable(true);
    _standardZoomAct->setData(View::ZoomOnlyDisabled);
    _standardZoomAct->setIcon(KstGetIcon("xy-zoom"));

    _xOnlyZoomAct = _interactionModeGroup->addAction(tr("&X-only Zoom"));
    _xOnlyZoomAct->setStatusTip(tr("Zoom only in X direction"));
    _xOnlyZoomAct->setShortcut(Qt::CTRL + Qt::Key_K);
    _xOnlyZoomAct->setToolTip(tr("X Zoom mode (%1)").arg(_xOnlyZoomAct->shortcut().toString()));
    _xOnlyZoomAct->setCheckable(true);
    _xOnlyZoomAct->setData(View::ZoomOnlyX);
    _xOnlyZoomAct->setIcon(KstGetIcon("x-zoom"));

    _yOnlyZoomAct = _interactionModeGroup->addAction(tr("&Y-only Zoom"));
    _yOnlyZoomAct->setStatusTip(tr("Zoom only in Y direction"));
    _yOnlyZoomAct->setShortcut(Qt::SHIFT + Qt::Key_K);
    _yOnlyZoomAct->setToolTip(tr("Y Zoom mode (%1)").arg(_yOnlyZoomAct->shortcut().toString()));
    _yOnlyZoomAct->setData(View::ZoomOnlyY);
    _yOnlyZoomAct->setCheckable(true);
    _yOnlyZoomAct->setIcon(KstGetIcon("y-zoom"));

    _layoutModeAct = _interactionModeGroup->addAction(tr("&Layout Mode"));
    _layoutModeAct->setIcon(KstGetIcon("transform-move"));
    _layoutModeAct->setCheckable(true);
    _layoutModeAct->setShortcut(QString("F2"));
    _layoutModeAct->setStatusTip(tr("Toggle the current view's layout mode (%1)").arg(_layoutModeAct->shortcut().toString()));
    _layoutModeAct->setToolTip(tr("Layout mode (%1)").arg(_layoutModeAct->shortcut().toString()));
    connect(_layoutModeAct, SIGNAL(toggled(bool)), this, SLOT(setLayoutMode(bool)));

    _interactionModeGroup->setExclusive(true);
    connect(_interactionModeGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeZoomOnlyMode(QAction*)));

    // *********************** Tools actions ************************************** //
    _dataManagerAct = new QAction(tr("Data Manager"), this);
    _dataManagerAct->setIcon(KstGetIcon("data-manager"));
    _dataManagerAct->setShortcut(QString("d"));
    _dataManagerAct->setStatusTip(tr("Show Kst's data manager window (%1)").arg(_dataManagerAct->shortcut().toString()));
    _dataManagerAct->setToolTip(tr("Show Data Manager (%1)").arg(_dataManagerAct->shortcut().toString()));
    connect(_dataManagerAct, SIGNAL(triggered()), this, SLOT(showDataManager()));

    _dataWizardAct = new QAction(tr("&Data Wizard"), this);
    _dataWizardAct->setIcon(KstGetIcon("tools-wizard"));
    _dataWizardAct->setShortcut(QString("w"));
    _dataWizardAct->setStatusTip(tr("Show Kst's Data Wizard (%1)").arg(_dataWizardAct->shortcut().toString()));
    _dataWizardAct->setToolTip(tr("Show Data Wizard (%1)").arg(_dataWizardAct->shortcut().toString()));
    connect(_dataWizardAct, SIGNAL(triggered()), this, SLOT(showDataWizard()));

    _changeFileDialogAct = new QAction(tr("Change Data &File"), this);
    _changeFileDialogAct->setIcon(KstGetIcon("change-data-file"));
    _changeFileDialogAct->setStatusTip(tr("Show Kst's Change Data File Dialog (%1)").arg(_changeFileDialogAct->shortcut().toString()));
    _changeFileDialogAct->setShortcut(QString("f"));
    _changeFileDialogAct->setToolTip(tr("Show Change Data File Dialog (%1)").arg(_changeFileDialogAct->shortcut().toString()));
    connect(_changeFileDialogAct, SIGNAL(triggered()), this, SLOT(showChangeFileDialog()));

    _chooseColorDialogAct = new QAction(tr("Assign Curve &Color per File"), this);
    _chooseColorDialogAct->setStatusTip(tr("Show Kst's Choose Color Dialog"));
    _chooseColorDialogAct->setIcon(KstGetIcon("code-class"));
    connect(_chooseColorDialogAct, SIGNAL(triggered()), this, SLOT(showChooseColorDialog()));

    _differentiateCurvesDialogAct = new QAction(tr("D&ifferentiate Curves"), this);
    _differentiateCurvesDialogAct->setStatusTip(tr("Show Kst's Differentiate Curves Dialog"));
    _differentiateCurvesDialogAct->setIcon(KstGetIcon("kst_differentiatecurves"));
    connect(_differentiateCurvesDialogAct, SIGNAL(triggered()), this, SLOT(showDifferentiateCurvesDialog()));

    _filterMultipleDialogAct = new QAction(tr("&Filter Vectors"), this);
    _filterMultipleDialogAct->setStatusTip(tr("Filter multiple vectors"));
    _filterMultipleDialogAct->setIcon(KstGetIcon("kst_filter_vectors"));
    connect(_filterMultipleDialogAct, SIGNAL(triggered()), this, SLOT(filterMultipleDialog()));

    // *********************** Settings actions ************************************** //
    _settingsDialogAct = new QAction(tr("&Configure Kst"), this);
    _settingsDialogAct->setStatusTip(tr("Show Kst's Configuration Dialog"));
    _settingsDialogAct->setIcon(KstGetIcon("configure"));
    connect(_settingsDialogAct, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));

    _themeDialogAct = new QAction(tr("&Theme"), this);
    _themeDialogAct->setStatusTip(tr("Show Kst's Theme Dialog"));
    _themeDialogAct->setIcon(KstGetIcon("themes"));
    connect(_themeDialogAct, SIGNAL(triggered()), this, SLOT(showThemeDialog()));

    _clearUISettings = new QAction(tr("Clear settings and defaults"), this);
    _clearUISettings->setStatusTip(tr("Clear sticky defaults and all settings in all dialogs."));
    //_clearUISettings->setIcon(KstGetIcon("configure"));  // FIXME: pick an icon (broom?)
    connect(_clearUISettings, SIGNAL(triggered()), this, SLOT(clearDefaults()));

    _clearDataSourceSettings = new QAction(tr("Clear datasource settings"), this);
    _clearDataSourceSettings->setStatusTip(tr("Clear datasource settings"));
    //_clearDataSourceSettings->setIcon(KstGetIcon("configure"));   // FIXME: pick an icon (broom?)
    connect(_clearDataSourceSettings, SIGNAL(triggered()), this, SLOT(clearDataSourceSettings()));

    _toggleStatusBar = new QAction(tr("Show status bar"), this);
    _toggleStatusBar->setCheckable(true);
    _toggleStatusBar->setChecked(_settings.value("showstatusbar", true).toBool());
    connect(_toggleStatusBar, SIGNAL(triggered()), this, SLOT(toggleStatusBar()));

    // *********************** Help actions ************************************** //
    _debugDialogAct = new QAction(tr("&Debug Dialog"), this);
    _debugDialogAct->setStatusTip(tr("Show the Kst debugging dialog"));
    _debugDialogAct->setIcon(KstGetIcon("text-x-log"));
    connect(_debugDialogAct, SIGNAL(triggered()), this, SLOT(showDebugDialog()));

    _bugReportWizardAct = new QAction(tr("&Bug Report Wizard"), this);
    _bugReportWizardAct->setStatusTip(tr("Show Kst's Bug Report Wizard"));
    _bugReportWizardAct->setIcon(KstGetIcon("kbugbuster"));
    connect(_bugReportWizardAct, SIGNAL(triggered()), this, SLOT(showBugReportWizard()));

    _aboutAct = new QAction(tr("&About"), this);
    _aboutAct->setStatusTip(tr("Show Kst's About box"));
    _aboutAct->setIcon(KstGetIcon("dialog-information"));
    connect(_aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    _showShortcutAct = new QAction(tr("Keyboard Shortcuts"), this);
    _showShortcutAct->setStatusTip(tr("Show a list of keyboard shortcuts"));
    connect(_showShortcutAct, SIGNAL(triggered()), this, SLOT(showShortcutDialog()));


}