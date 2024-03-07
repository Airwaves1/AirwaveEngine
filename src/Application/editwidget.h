#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLabel;
class QSpinBox;
class QDoubleSpinBox;
class QGroupBox;

class EditWidget : public QWidget
{
    Q_OBJECT
public:
    EditWidget(QWidget* parent = nullptr);
    ~EditWidget();

private:
    void initUI();
    void setStyle();
    void initConnect();


public slots:
    void onHandleLightPosChanged();

signals:
    void lightPosChanged(const QVector3D& lightPos);


private:
    QVBoxLayout* m_layout = nullptr;

    QGroupBox* m_groupBox_lightPos = nullptr;
    QHBoxLayout* m_layout_lightPos = nullptr;

    QLabel* m_label_lightPosX = nullptr;
    QDoubleSpinBox* m_spinBox_lightPosX = nullptr;
    QLabel* m_label_lightPosY = nullptr;
    QDoubleSpinBox* m_spinBox_lightPosY = nullptr;
    QLabel* m_label_lightPosZ = nullptr;
    QDoubleSpinBox* m_spinBox_lightPosZ = nullptr;





};

#endif // EDITWIDGET_H
