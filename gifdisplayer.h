#ifndef GIFDISPLAYER_H
#define GIFDISPLAYER_H

#include <QWidget>
#include <QString>
#include <QEvent>
#include <QThread>

class QTimer;
class QLabel;
class GIFDecoder;
class QVBoxLayout;
class QElapsedTimer;
class GIFDisplayer : public QWidget
{
    Q_OBJECT

    QThread         decodeThread;
    QVBoxLayout*    mainLayout;
    GIFDecoder*     decoder;
    QSize           size;
    QLabel*         image;
    QString         fileName;
    quint64         nextTimeDelay;
    QPixmap         firstFramePixmap;
    QPixmap         currentPixmap;
    QTimer*         timerDelay;
    quint64         currentFrameNumber;

public:
    explicit GIFDisplayer(QString fileName, QSize size = QSize(), QWidget *parent = 0);
    ~GIFDisplayer();

protected:
    void start();
    void stop();
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);

public slots:
    void replyTimeOut();
};

#endif // GIFDISPLAYER_H
