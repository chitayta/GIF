#ifndef GIFDECODER_H
#define GIFDECODER_H

#include <QObject>
#include <QThread>
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QMap>
#include <QTimer>
#include <QSize>
#include <QString>

#define NUMBER_FRAME_BUFFER         2
#define DEFAULT_SCALED_WIDTH        300
#define DEFAULT_SCALED_HEIGHT       200

class GIFDisplayer;
class QElapsedTimer;
class GIFDecoder : public QObject
{
    Q_OBJECT

    QString             fileName;
    GIFDisplayer*       displayer;
    quint64             imageCountValue;
    QImageReader        reader;
    QSize               frameSize;
    QElapsedTimer*      decodeTimer;

public:
    QPixmap             firstFramePixmap;
    quint64             frameNumber;
    QMap <int, QPixmap> pixmapBuffer;
    QMap <int, quint64> delayTimeBuffer;

    GIFDecoder(GIFDisplayer* displayer, QString fileName, QSize size);
    quint64             imageCount();
    void                resetDecoder();

public slots:
    void                onTimerDelayTimeOut();
};

#endif // GIFDECODER_H
