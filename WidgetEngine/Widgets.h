#include <qnamespace.h>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QEasingCurve>
#include <QApplication>
#include <QPushButton>
#include <QMouseEvent>
#include <QWidget>
#include <QWindow>
#include <QLayout>
#include <QObject>
#include <QFrame>
#include <QLabel>
#include <QTimer>

#pragma once

namespace WidgetEngine {
  class Button : public QPushButton {
      Q_OBJECT

    public:
      using QPushButton::QPushButton;

      ~Button() {}
      Button(QWidget* parent = nullptr) : QPushButton(parent) {
        setAttribute(Qt::WA_Hover);
      }

    signals:
      void Clicked(Qt::MouseButton btn);
      void HoverEnter();
      void HoverLeave();

    protected:
      void mousePressEvent(QMouseEvent* e) override {
        QPushButton::mousePressEvent(e);
        emit Clicked(e->button());
      }

      bool event(QEvent* e) override {
        switch (e->type()) {
          case QEvent::HoverEnter:
            emit HoverEnter();
            break;
          case QEvent::HoverLeave:
            emit HoverLeave();
            break;
          default:
            break;
        }
        return QPushButton::event(e);
      }
  };

  class Label : public QLabel {
    Q_OBJECT

    public:
      using QLabel::QLabel;

      Label(QWidget* parent = nullptr) : QLabel(parent) {}
      ~Label() {}
  };
};
