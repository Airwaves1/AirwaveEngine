#include "mainwindow.h"

#include <QFile>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QStackedWidget>
#include <QDockWidget>


#include <QThread>


#include "openglworker.h"
#include "Utils/logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1200, 800);
    initUI();
    setStyle();


    m_openGLWidget = new OpenGLWidget();
    setCentralWidget(m_openGLWidget);

    initThread();

    initConnect();


    Logger::info("MainWindow::MainWindow");
}

MainWindow::~MainWindow() {}

void MainWindow::initThread()
{
    // 创建线程
    t1 = new QThread();
    woker = new OpenGLWoker(m_openGLWidget);

    woker->moveToThread(t1);
    connect(t1, &QThread::started, woker, &OpenGLWoker::renderWork);

    t1->start();

    connect(this, &MainWindow::destroyed, this, [=]() {
        t1->quit();
        t1->wait();

        t1->deleteLater();
        woker->deleteLater();
    });

}

void MainWindow::initUI()
{

    /* 创建菜单栏 */
    m_menuBar = new QMenuBar(this);
    setMenuBar(m_menuBar);

    m_menuFile = new QMenu("File", m_menuBar);
    m_menuBar->addMenu(m_menuFile);
    m_menuEdit = new QMenu("Edit", m_menuBar);
    m_menuBar->addMenu(m_menuEdit);


    /* 创建工具栏 */
    m_toolBar = new QToolBar(this);
    addToolBar(m_toolBar);

    m_actionOpen = new QAction("Open", this);
    m_actionSave = new QAction("Save", this);
    m_actionExit = new QAction("Exit", this);

    m_menuFile->addAction(m_actionOpen);
    m_menuFile->addAction(m_actionSave);
    m_menuFile->addAction(m_actionExit);

    m_toolBar->addAction(m_actionOpen);
    m_toolBar->addAction(m_actionSave);
    m_toolBar->addAction(m_actionExit);


    /* 创建编辑界面 */
    m_dockWidget_edit = new QDockWidget("Edit", this);
    m_editWidget = new EditWidget();
    m_dockWidget_edit->setWidget(m_editWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_dockWidget_edit);

    Logger::info("MainWindow::initUI");


}

void MainWindow::setStyle()
{
    QFile styleFile("./assets/qss/dark.qss");  // 使用资源文件中的样式表
    styleFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream styleStream(&styleFile);
    QString styleString = styleStream.readAll();
    setStyleSheet(styleString);
    styleFile.close();
}

void MainWindow::initConnect()
{

    initConnect_editWidget();

}

void MainWindow::initConnect_editWidget()
{
    connect(m_editWidget, &EditWidget::lightPosChanged, m_openGLWidget, &OpenGLWidget::setLightPos);
}

