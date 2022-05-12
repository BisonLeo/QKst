#pragma once

#include <QtWidgets/QMainWindow>
#include <QKeySequence>

#include "ui_QKst.h"

class QUndoGroup;
class QSettings;

namespace Kst {
    class TabWidget;
    class View;
    class Document;
    class ScriptServer;
    class DataManager;
    class DebugDialog;

    class QKst : public QMainWindow
    {
        Q_OBJECT

    public:
        QKst(QWidget* parent = Q_NULLPTR);
        TabWidget* tabWidget() const;
        QUndoGroup* undoGroup() const;
        Document* document() const;

        bool initFromCommandLine();

        bool isHighlightPoint() { return _highlightPoint; }

        bool isTiedTabs();  // called in plotitemmanager
        void setStatusMessage(QString message, int timeout = 0, bool delayed = false);
        void updateStatusMessage();
        QString statusMessage();

        void updateRecentKstFiles(const QString& newfilename = QString());

        void setKstWindowTitle();
        QString scriptServerName();
        bool scriptServerNameSet();
        void setScriptServerName(QString server_name);
        

    public Q_SLOTS:
        void openFile(const QString& file);
        void updateRecentDataFiles(const QString& newfilename = QString());
        void clearDrawingMarker();
        void newDoc(bool force = false);

        void save();
        void saveAs();
    private Q_SLOTS:
        void cleanup();
        void insertPlot();
        void createPlot();
    private:
        void updateRecentFiles(const QString& key, QMenu* menu, QList<QAction*>& actions, QMenu* submenu, const QString& newfilename, const char* openslot);
        void createActions();
        void createMenus();
        bool promptSaveDone();
        Ui::QKstClass ui;

        Document* _doc = nullptr;
        ScriptServer* _scriptServer;
        DataManager* _dataManager;
        DebugDialog* _debugDialog;
        QSettings& _settings;

        TabWidget* _tabWidget;
        QUndoGroup* _undoGroup;
        bool _highlightPoint;

        QMenu* _fileMenu;
        QMenu* _editMenu;
        QMenu* _viewMenu;
        QMenu* _createMenu;
        QMenu* _modeMenu;
        QMenu* _rangeMenu;
        QMenu* _toolsMenu;
        QMenu* _settingsMenu;

        QString _statusBarMessage;
        int _statusBarTimeout;

        QString _sessionFileName;

        QAction* _undoAct;
        QAction* _redoAct;
        QAction* _copyTabAct;

        // FIXME: move these into each object, along with the creation slot?
        QAction* _createLabelAct;
        QAction* _createBoxAct;
        QAction* _createCircleAct;
        QAction* _createEllipseAct;
        QAction* _createLineAct;
        QAction* _createArrowAct;
        QAction* _createPictureAct;
        QAction* _createPlotAct;
        QAction* _insertPlotAct;
#ifndef KST_NO_SVG
        QAction* _createSvgAct;
#endif
        QAction* _createSharedAxisBoxAct;

        QAction* _aboutAct;
        QAction* _showShortcutAct;
        QAction* _dataManagerAct;
        QAction* _debugDialogAct;
        QAction* _video1Act;
        QAction* _video2Act;
        QAction* _video3Act;
        QAction* _video4Act;
        QAction* _video5Act;
        QAction* _video6Act;
        QAction* _video7Act;
        QAction* _exitAct;
        QAction* _exportGraphicsAct;
        QAction* _exportVectorsAct;
        QAction* _logAct;
        QAction* _openAct;
        QAction* _saveAct;
        QAction* _saveAsAct;
        QAction* _closeAct;
        QAction* _printAct;
        QAction* _newTabAct;
        QAction* _closeTabAct;
        QAction* _scalarEditorAct;
        QAction* _vectorEditorAct;
        QAction* _matrixEditorAct;
        QAction* _stringEditorAct;

        QAction* _layoutModeAct;
        QAction* _tiedZoomAct;
        QAction* _tabTiedAct;
        QAction* _highlightPointAct;
        QAction* _standardZoomAct;
        QAction* _xOnlyZoomAct;
        QAction* _yOnlyZoomAct;

        QAction* _settingsDialogAct;
        QAction* _themeDialogAct;

        QAction* _clearUISettings;
        QAction* _clearDataSourceSettings;
        QAction* _toggleStatusBar;

        QAction* _differentiateCurvesDialogAct;
        QAction* _filterMultipleDialogAct;
        QAction* _chooseColorDialogAct;
        QAction* _changeDataSampleDialogAct;
        QAction* _changeFileDialogAct;
        QAction* _dataWizardAct;
        QAction* _bugReportWizardAct;

        QAction* _readFromEndAct;
        QAction* _readToEndAct;
        QAction* _pauseAct;
        QAction* _backAct;
        QAction* _forwardAct;
        QAction* _backSmallAct;
        QAction* _forwardSmallAct;
        QAction* _reloadAct;

        QAction* _newScalarAct;
        QAction* _newVectorAct;
        QAction* _newMatrixAct;
        QAction* _newStringAct;
        QAction* _newCurveAct;
        QAction* _newEquationAct;
        QAction* _newPSDAct;
        QAction* _newHistogramAct;
        QAction* _newImageAct;
        QAction* _newSpectrogramAct;
        QAction* _newEventMonitorAct;

        QMenu* _recentKstFilesMenu;
        QMenu* _recentDataFilesMenu;
        QList<QAction*> _bottomRecentKstActions;
        QList<QAction*> _bottomRecentDataActions;
        friend class ScriptServer;
    };
}