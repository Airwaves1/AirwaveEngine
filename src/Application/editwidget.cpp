#include "editwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QFile>
#include <QVector3D>

#include "Utils/logger.h"

EditWidget::EditWidget(QWidget *parent)
{
    initUI();
    setStyle();


    initConnect();

}

EditWidget::~EditWidget()
{

}

void EditWidget::initUI()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(2, 2, 2, 2);
    m_layout->setSpacing(2);

    m_groupBox_lightPos = new QGroupBox("光源位置");
    m_layout->addWidget(m_groupBox_lightPos, 1);  // 使用拉伸因子 1

    m_layout_lightPos = new QHBoxLayout(m_groupBox_lightPos);
    m_layout_lightPos->setContentsMargins(5, 8, 5, 5);
    m_layout_lightPos->setSpacing(5);

    m_label_lightPosX = new QLabel("X");
    m_label_lightPosX->setStyleSheet("QLabel {"
                                     "  color: #ffffff;"  // 文本颜色
                                     "  border-radius: 3px;"  // 圆角边框
                                     "}");
    m_spinBox_lightPosX = new QDoubleSpinBox();
    m_spinBox_lightPosX->setRange(-100, 100);
    m_spinBox_lightPosX->setSingleStep(0.1);
    m_spinBox_lightPosX->setValue(0.5f);

    m_label_lightPosY = new QLabel("Y");
    m_label_lightPosY->setStyleSheet("QLabel {"
                                     "  color: #ffffff;"  // 文本颜色
                                     "  border-radius: 3px;"  // 圆角边框
                                     "}");
    m_spinBox_lightPosY = new QDoubleSpinBox();
    m_spinBox_lightPosY->setRange(-100, 100);
    m_spinBox_lightPosY->setSingleStep(0.1);
    m_spinBox_lightPosY->setValue(1.0f);

    m_label_lightPosZ = new QLabel("Z");
    m_label_lightPosZ->setStyleSheet("QLabel {"
                                     "  color: #ffffff;"  // 文本颜色
                                     "  border-radius: 3px;"  // 圆角边框
                                     "}");
    m_spinBox_lightPosZ = new QDoubleSpinBox();
    m_spinBox_lightPosZ->setRange(-100, 100);
    m_spinBox_lightPosZ->setSingleStep(0.1);
    m_spinBox_lightPosZ->setValue(1.0f);

    m_layout_lightPos->addWidget(m_label_lightPosX);
    m_layout_lightPos->addWidget(m_spinBox_lightPosX);
    m_layout_lightPos->addWidget(m_label_lightPosY);
    m_layout_lightPos->addWidget(m_spinBox_lightPosY);
    m_layout_lightPos->addWidget(m_label_lightPosZ);
    m_layout_lightPos->addWidget(m_spinBox_lightPosZ, 1);  // 使用拉伸因子 1

    m_layout_lightPos->addStretch();

    m_groupBox_lightPos->setLayout(m_layout_lightPos);

    m_layout->addStretch(1);  // 使用拉伸因子 1

    setLayout(m_layout);

}

void EditWidget::setStyle()
{
    QFile styleFile("./assets/qss/editwidget.qss");  // 使用资源文件中的样式表
    styleFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream styleStream(&styleFile);
    QString styleString = styleStream.readAll();
    setStyleSheet(styleString);
    styleFile.close();

}

void EditWidget::initConnect()
{
    connect(m_spinBox_lightPosX, &QDoubleSpinBox::valueChanged, this, &EditWidget::onHandleLightPosChanged);
    connect(m_spinBox_lightPosY, &QDoubleSpinBox::valueChanged, this, &EditWidget::onHandleLightPosChanged);
    connect(m_spinBox_lightPosZ, &QDoubleSpinBox::valueChanged, this, &EditWidget::onHandleLightPosChanged);
}

void EditWidget::onHandleLightPosChanged()
{
    QVector3D lightPos(m_spinBox_lightPosX->value(), m_spinBox_lightPosY->value(), m_spinBox_lightPosZ->value());
    Logger::debug("emit: x:%1,y:%2,z:%3", m_spinBox_lightPosX->value(), m_spinBox_lightPosY->value(), m_spinBox_lightPosZ->value());
    emit lightPosChanged(lightPos);
}
