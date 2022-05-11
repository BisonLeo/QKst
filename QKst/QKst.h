#pragma once

#include <QtWidgets/QMainWindow>
#include <QKeySequence>

#include "ui_QKst.h"

class QUndoGroup;

namespace Kst {
    class TabWidget;
    class View;
    class Document;
    class ScriptServer;
    class DataManager;

    class QKst : public QMainWindow
    {
        Q_OBJECT

    public:
        QKst(QWidget* parent = Q_NULLPTR);
        bool initFromCommandLine();

        void createActions();
        void insertPlot();
        void clearDrawingMarker();
        bool isHighlightPoint() { return _highlightPoint; }

        bool isTiedTabs();  // called in plotitemmanager
        void setStatusMessage(QString message, int timeout = 0, bool delayed = false);
        void updateStatusMessage();
        QString statusMessage();

        void newDoc(bool force);
        bool promptSaveDone();
        void save();
        void saveAs();

        void updateRecentKstFiles(const QString& newfilename = QString());

        void setKstWindowTitle();
        QString scriptServerName();
        bool scriptServerNameSet();
        void setScriptServerName(QString server_name);
        
        TabWidget* tabWidget() const;
        QUndoGroup* undoGroup() const;
        Document* document() const;
    public Q_SLOTS:
        void openFile(const QString& file);
        void updateRecentDataFiles(const QString& newfilename = QString());

    private Q_SLOTS:
        void cleanup();

    private:
        void updateRecentFiles(const QString& key, QMenu* menu, QList<QAction*>& actions, QMenu* submenu, const QString& newfilename, const char* openslot);

        Ui::QKstClass ui;

        Document* _doc = nullptr;
        ScriptServer* _scriptServer;
        DataManager* _dataManager;

        TabWidget* _tabWidget;
        QUndoGroup* _undoGroup;
        bool _highlightPoint;

        QString _statusBarMessage;
        int _statusBarTimeout;

        QString _sessionFileName;

        QAction* _openAct;
        QAction* _backAct;
        QAction* _forwardAct;
        QAction* _readFromEndAct;
        QAction* _readToEndAct;
        QAction* _pauseAct;
        friend class ScriptServer;
    };
}