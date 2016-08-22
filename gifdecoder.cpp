#include "gifdecoder.h"
#include "gifdisplayer.h"
#include <QDebug>
#include <QElapsedTimer>

GIFDecoder::GIFDecoder(GIFDisplayer* displayer, QString fileName, QSize size) : displayer(displayer), fileName(fileName), frameSize(size)
{
    decodeTimer = new QElapsedTimer;
    reader.setFileName(fileName);
    imageCountValue = reader.imageCount();
    if (frameSize.isValid())
    {
        reader.setScaledSize(QSize(frameSize.width(), frameSize.height()));
    }
    firstFramePixmap = QPixmap(QPixmap::fromImage(reader.read()));
}

void GIFDecoder::resetDecoder()
{
    frameNumber = 0;
    pixmapBuffer.clear();
    delayTimeBuffer.clear();
    reader.setFileName(fileName);
    delayTimeBuffer.insert(frameNumber, reader.nextImageDelay());
    if (frameSize.isValid())
    {
        reader.setScaledSize(QSize(frameSize.width(), frameSize.height()));
    }
    firstFramePixmap = QPixmap(QPixmap::fromImage(reader.read()));
    for(int index = 0; index < NUMBER_FRAME_BUFFER; index++)
    {
        frameNumber = frameNumber + 1;
        pixmapBuffer.insert(frameNumber, QPixmap(QPixmap::fromImage(reader.read())));
        delayTimeBuffer.insert(frameNumber, reader.nextImageDelay());
    }
}

void GIFDecoder::onTimerDelayTimeOut()
{
    frameNumber = frameNumber + 1;
    if (frameNumber < imageCountValue)
    {
        pixmapBuffer.insert(frameNumber, QPixmap(QPixmap::fromImage(reader.read())));
        delayTimeBuffer.insert(frameNumber, reader.nextImageDelay());
    }
    else
    {
        reader.setFileName(fileName);
        frameNumber = 0;
        pixmapBuffer.insert(frameNumber, QPixmap(QPixmap::fromImage(reader.read())));
        delayTimeBuffer.insert(frameNumber, reader.nextImageDelay());
    }
}

quint64 GIFDecoder::imageCount()
{
    return this->imageCountValue;
}
