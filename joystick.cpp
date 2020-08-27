#include "joystick.h"
#include <QPaintEvent>
#include <QPainter>
#include <QRadialGradient>
#include <QtDebug>
#include <cmath>

#define SPACE 20
#define PI 3.1415926

JoyStick::JoyStick(QWidget *parent): QLabel(parent)
{

}

QPoint JoyStick::getPos()
{
    return lastPos;
}

void JoyStick::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect();
    // 绘制底圆
    QPen pen;
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawEllipse(rect.center(), baseRadius, baseRadius);

    // 绘制摇杆
    pen.setWidth(2);
    painter.setPen(pen);
    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::Dense7Pattern);
    painter.setBrush(brush);
    painter.drawEllipse(joyPos, joyRadius, joyRadius);

    // 绘制摇杆帽光影
    QRadialGradient gradient(joyPos, edgeLength / 2, joyPos);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, QColor(50, 50, 50, 100));
    painter.setBrush(QBrush(gradient));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(joyPos, joyRadius, joyRadius);

}

QSize JoyStick::sizeHint() const
{
    return QSize(120, 120);
}

void JoyStick::mouseMoveEvent(QMouseEvent *event)
{
    setValue(event->pos());
    QLabel::mouseMoveEvent(event);
}

void JoyStick::mouseReleaseEvent(QMouseEvent *event)
{
    setValue(rect().center());
    QLabel::mouseReleaseEvent(event);
}

void JoyStick::showEvent(QShowEvent *event)
{
    QRect rect = this->rect();
    setMask(QRegion(rect.marginsAdded(QMargins(2, 2, 2, 2)), QRegion::Ellipse));

    edgeLength = qMin(rect.width(), rect.height());
    baseRadius = (edgeLength - SPACE * 2) / 2;
    joyRadius = baseRadius * 2 / 3;
    int capWidth = joyRadius * 2 / 3;
    capWidth = qMin(capWidth, SPACE);
    stickRadius = joyRadius - capWidth;
    joyPos = rect.center();

    QLabel::showEvent(event);
}

void JoyStick::mousePressEvent(QMouseEvent *event)
{
    setValue(event->pos());
    QLabel::mousePressEvent(event);
}

double getDistance(QPoint p1, QPoint p2)
{
    int x = p1.x() - p2.x();
    int y = p1.y() - p2.y();
    return sqrt(x * x + y * y);
}

void JoyStick::setValue(QPoint pos)
{
    auto center = this->rect().center();

    double distance = getDistance(pos, rect().center());
    double availableR = baseRadius - stickRadius;
    QPoint offset;
    offset.setX(pos.x() - center.x());
    offset.setY(pos.y() - center.y());
    if(distance < availableR) {
        joyPos = pos;
    } else {
        double cos = offset.x() / distance;
        double sin = offset.y() / distance;

        offset.setX(static_cast<int>(availableR * cos));
        offset.setY(static_cast<int>(availableR * sin));

        joyPos = center + offset;
    }

    offset.setX(static_cast<int>(offset.x() * 100 / availableR));
    offset.setY(-static_cast<int>(offset.y() * 100 / availableR));
    if(offset != lastPos) {
        emit posChanged(offset);
        update();
        lastPos = offset;
    }

}

