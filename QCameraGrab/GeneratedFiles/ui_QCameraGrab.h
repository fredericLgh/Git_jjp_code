/********************************************************************************
** Form generated from reading UI file 'QCameraGrab.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCAMERAGRAB_H
#define UI_QCAMERAGRAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QCameraGrabClass
{
public:
    QFrame *frame;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *m_pLabelLeftLed;
    QPushButton *m_pBtnSetLeft;
    QLineEdit *m_pLineEditNameLeft;
    QLineEdit *m_pLineEditExpLeft;
    QLineEdit *m_pLineEditGainLeft;
    QLabel *label_15;
    QFrame *frame_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *m_pLineEditImageHeight;
    QLineEdit *m_pLineEditImageWidth;
    QFrame *frame_3;
    QLabel *m_pLabelRightLed;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QPushButton *m_pBtnSetRight;
    QLineEdit *m_pLineEditNameRight;
    QLineEdit *m_pLineEditExpRight;
    QLabel *label_16;
    QLineEdit *m_pLineEditGainRight;
    QPushButton *m_pBtnHide;
    QLabel *label_19;
    QFrame *line;
    QFrame *line_2;
    QPushButton *m_pBtnExit;

    void setupUi(QWidget *QCameraGrabClass)
    {
        if (QCameraGrabClass->objectName().isEmpty())
            QCameraGrabClass->setObjectName(QStringLiteral("QCameraGrabClass"));
        QCameraGrabClass->resize(480, 365);
        QFont font;
        font.setPointSize(9);
        QCameraGrabClass->setFont(font);
        QCameraGrabClass->setContextMenuPolicy(Qt::NoContextMenu);
        QIcon icon;
        icon.addFile(QStringLiteral(":/QCameraGrab/CameraICON"), QSize(), QIcon::Normal, QIcon::Off);
        QCameraGrabClass->setWindowIcon(icon);
        frame = new QFrame(QCameraGrabClass);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 129, 201, 221));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 57, 41, 21));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 99, 41, 21));
        label_2->setFont(font1);
        label_5 = new QLabel(frame);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 140, 41, 21));
        label_5->setFont(font1);
        m_pLabelLeftLed = new QLabel(frame);
        m_pLabelLeftLed->setObjectName(QStringLiteral("m_pLabelLeftLed"));
        m_pLabelLeftLed->setGeometry(QRect(30, 10, 101, 31));
        m_pLabelLeftLed->setFont(font1);
        m_pLabelLeftLed->setFrameShape(QFrame::Box);
        m_pLabelLeftLed->setAlignment(Qt::AlignCenter);
        m_pBtnSetLeft = new QPushButton(frame);
        m_pBtnSetLeft->setObjectName(QStringLiteral("m_pBtnSetLeft"));
        m_pBtnSetLeft->setGeometry(QRect(30, 180, 81, 31));
        m_pBtnSetLeft->setFont(font1);
        m_pLineEditNameLeft = new QLineEdit(frame);
        m_pLineEditNameLeft->setObjectName(QStringLiteral("m_pLineEditNameLeft"));
        m_pLineEditNameLeft->setGeometry(QRect(60, 60, 113, 21));
        QFont font2;
        font2.setPointSize(9);
        font2.setBold(true);
        font2.setWeight(75);
        m_pLineEditNameLeft->setFont(font2);
        m_pLineEditNameLeft->setReadOnly(true);
        m_pLineEditExpLeft = new QLineEdit(frame);
        m_pLineEditExpLeft->setObjectName(QStringLiteral("m_pLineEditExpLeft"));
        m_pLineEditExpLeft->setGeometry(QRect(60, 100, 71, 21));
        m_pLineEditExpLeft->setFont(font2);
        m_pLineEditGainLeft = new QLineEdit(frame);
        m_pLineEditGainLeft->setObjectName(QStringLiteral("m_pLineEditGainLeft"));
        m_pLineEditGainLeft->setGeometry(QRect(60, 140, 71, 21));
        m_pLineEditGainLeft->setFont(font2);
        label_15 = new QLabel(frame);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(137, 101, 21, 16));
        label_15->setFont(font1);
        frame_2 = new QFrame(QCameraGrabClass);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(10, 41, 271, 80));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 49, 54, 21));
        label_3->setFont(font1);
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 16, 54, 21));
        label_4->setFont(font1);
        m_pLineEditImageHeight = new QLineEdit(frame_2);
        m_pLineEditImageHeight->setObjectName(QStringLiteral("m_pLineEditImageHeight"));
        m_pLineEditImageHeight->setGeometry(QRect(60, 52, 71, 20));
        m_pLineEditImageHeight->setFont(font2);
        m_pLineEditImageHeight->setReadOnly(true);
        m_pLineEditImageWidth = new QLineEdit(frame_2);
        m_pLineEditImageWidth->setObjectName(QStringLiteral("m_pLineEditImageWidth"));
        m_pLineEditImageWidth->setGeometry(QRect(60, 16, 71, 20));
        m_pLineEditImageWidth->setFont(font2);
        m_pLineEditImageWidth->setReadOnly(true);
        frame_3 = new QFrame(QCameraGrabClass);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(260, 129, 201, 221));
        frame_3->setFrameShape(QFrame::Panel);
        frame_3->setFrameShadow(QFrame::Raised);
        m_pLabelRightLed = new QLabel(frame_3);
        m_pLabelRightLed->setObjectName(QStringLiteral("m_pLabelRightLed"));
        m_pLabelRightLed->setGeometry(QRect(50, 10, 91, 31));
        m_pLabelRightLed->setFont(font1);
        m_pLabelRightLed->setFrameShape(QFrame::Box);
        m_pLabelRightLed->setAlignment(Qt::AlignCenter);
        label_12 = new QLabel(frame_3);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(20, 60, 41, 21));
        label_12->setFont(font1);
        label_13 = new QLabel(frame_3);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(20, 100, 41, 21));
        label_13->setFont(font1);
        label_14 = new QLabel(frame_3);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(20, 140, 41, 21));
        label_14->setFont(font1);
        m_pBtnSetRight = new QPushButton(frame_3);
        m_pBtnSetRight->setObjectName(QStringLiteral("m_pBtnSetRight"));
        m_pBtnSetRight->setGeometry(QRect(60, 180, 81, 31));
        m_pBtnSetRight->setFont(font1);
        m_pLineEditNameRight = new QLineEdit(frame_3);
        m_pLineEditNameRight->setObjectName(QStringLiteral("m_pLineEditNameRight"));
        m_pLineEditNameRight->setGeometry(QRect(70, 60, 113, 20));
        m_pLineEditNameRight->setFont(font2);
        m_pLineEditNameRight->setReadOnly(true);
        m_pLineEditExpRight = new QLineEdit(frame_3);
        m_pLineEditExpRight->setObjectName(QStringLiteral("m_pLineEditExpRight"));
        m_pLineEditExpRight->setGeometry(QRect(70, 100, 71, 20));
        m_pLineEditExpRight->setFont(font2);
        label_16 = new QLabel(frame_3);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(150, 102, 31, 16));
        label_16->setFont(font1);
        m_pLineEditGainRight = new QLineEdit(frame_3);
        m_pLineEditGainRight->setObjectName(QStringLiteral("m_pLineEditGainRight"));
        m_pLineEditGainRight->setGeometry(QRect(70, 140, 71, 20));
        m_pLineEditGainRight->setFont(font2);
        m_pBtnHide = new QPushButton(QCameraGrabClass);
        m_pBtnHide->setObjectName(QStringLiteral("m_pBtnHide"));
        m_pBtnHide->setGeometry(QRect(399, 0, 41, 41));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/QCameraGrab/Min"), QSize(), QIcon::Normal, QIcon::Off);
        m_pBtnHide->setIcon(icon1);
        m_pBtnHide->setIconSize(QSize(32, 32));
        m_pBtnHide->setFlat(true);
        label_19 = new QLabel(QCameraGrabClass);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(173, 8, 81, 21));
        label_19->setFont(font1);
        line = new QFrame(QCameraGrabClass);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(33, 20, 118, 3));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(QCameraGrabClass);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(252, 19, 118, 3));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        m_pBtnExit = new QPushButton(QCameraGrabClass);
        m_pBtnExit->setObjectName(QStringLiteral("m_pBtnExit"));
        m_pBtnExit->setGeometry(QRect(437, -1, 41, 41));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/QCameraGrab/Close"), QSize(), QIcon::Normal, QIcon::Off);
        m_pBtnExit->setIcon(icon2);
        m_pBtnExit->setIconSize(QSize(32, 32));
        m_pBtnExit->setFlat(true);

        retranslateUi(QCameraGrabClass);

        QMetaObject::connectSlotsByName(QCameraGrabClass);
    } // setupUi

    void retranslateUi(QWidget *QCameraGrabClass)
    {
        QCameraGrabClass->setWindowTitle(QApplication::translate("QCameraGrabClass", "\347\233\270\346\234\272\351\207\207\351\233\206", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        QCameraGrabClass->setToolTip(QApplication::translate("QCameraGrabClass", "\345\205\263\351\227\255\347\250\213\345\272\217", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("QCameraGrabClass", "\345\220\215\347\247\260:", Q_NULLPTR));
        label_2->setText(QApplication::translate("QCameraGrabClass", "\346\233\235\345\205\211:", Q_NULLPTR));
        label_5->setText(QApplication::translate("QCameraGrabClass", "\345\242\236\347\233\212:", Q_NULLPTR));
        m_pLabelLeftLed->setText(QApplication::translate("QCameraGrabClass", "\347\212\266\346\200\201", Q_NULLPTR));
        m_pBtnSetLeft->setText(QApplication::translate("QCameraGrabClass", "\350\256\276\347\275\256", Q_NULLPTR));
        label_15->setText(QApplication::translate("QCameraGrabClass", "us", Q_NULLPTR));
        label_3->setText(QApplication::translate("QCameraGrabClass", "\351\253\230\345\272\246\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("QCameraGrabClass", "\345\256\275\345\272\246\357\274\232", Q_NULLPTR));
        m_pLabelRightLed->setText(QApplication::translate("QCameraGrabClass", "\347\212\266\346\200\201", Q_NULLPTR));
        label_12->setText(QApplication::translate("QCameraGrabClass", "\345\220\215\347\247\260:", Q_NULLPTR));
        label_13->setText(QApplication::translate("QCameraGrabClass", "\346\233\235\345\205\211:", Q_NULLPTR));
        label_14->setText(QApplication::translate("QCameraGrabClass", "\345\242\236\347\233\212:", Q_NULLPTR));
        m_pBtnSetRight->setText(QApplication::translate("QCameraGrabClass", "\350\256\276\347\275\256", Q_NULLPTR));
        label_16->setText(QApplication::translate("QCameraGrabClass", "us", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        m_pBtnHide->setToolTip(QApplication::translate("QCameraGrabClass", "\351\232\220\350\227\217\347\252\227\345\217\243", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        m_pBtnHide->setText(QString());
        label_19->setText(QApplication::translate("QCameraGrabClass", "\347\233\270\346\234\272\351\207\207\351\233\206", Q_NULLPTR));
        m_pBtnExit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QCameraGrabClass: public Ui_QCameraGrabClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCAMERAGRAB_H
