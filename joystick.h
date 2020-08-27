#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QLabel>


class JoyStick : public QLabel
{
    Q_OBJECT
public:
    JoyStick(QWidget *parent = nullptr);
    QPoint getPos();

signals:
    void posChanged(QPoint);

private:
    void paintEvent(QPaintEvent *) override;
    QSize sizeHint() const override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void showEvent(QShowEvent *) override;
    void mousePressEvent(QMouseEvent *ev) override;

    void setValue(QPoint);

    QPoint lastPos;
    QPoint joyPos;
    int edgeLength;
    int baseRadius;
    int joyRadius;
    int stickRadius;
};

#endif // JOYSTICK_H
