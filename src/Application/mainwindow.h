#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "OpenGL/openglwidget.h"
#include "editwidget.h"

class QStackedWidget;
class QDockWidget;
class QMenuBar;
class QMenu;
class QToolBar;
class QAction;
class QThread;
class OpenGLWoker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void initThread();
    void initUI();
    void setStyle();

    void initConnect();
    void initConnect_editWidget();

private:
    //OpenGL相关
    OpenGLWidget* m_openGLWidget = nullptr;
    QThread* t1 = nullptr;
    OpenGLWoker* woker = nullptr;


    //UI
    QMenuBar* m_menuBar = nullptr;
    QMenu* m_menuFile = nullptr;
    QMenu* m_menuEdit = nullptr;

    QToolBar* m_toolBar = nullptr;
    QAction* m_actionOpen = nullptr;
    QAction* m_actionSave = nullptr;
    QAction* m_actionExit = nullptr;


    //DockWidget_Edit
    QDockWidget* m_dockWidget_edit = nullptr;
    EditWidget* m_editWidget = nullptr;

};
#endif // MAINWINDOW_H
