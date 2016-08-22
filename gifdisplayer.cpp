#include "gifdisplayer.h"
#include <QDebug>
#include <QImage>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>
#include <QElapsedTimer>
#include "gifdecoder.h"

GIFDisplayer::GIFDisplayer(QString fileName, QSize size ,QWidget *parent) : QWidget(parent), fileName(fileName), size(size)
{
    decoder = new GIFDecoder(this, fileName, size);
    decoder->moveToThread(&decodeThread);
    decodeThread.start();

    firstFramePixmap = QPixmap(decoder->firstFramePixmap);
    image = new QLabel;
    image->setPixmap(firstFramePixmap);
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(image);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    timerDelay = new QTimer;
    QObject::connect(timerDelay, SIGNAL(timeout()), this, SLOT(replyTimeOut()));
    QObject::connect(timerDelay, SIGNAL(timeout()), decoder, SLOT(onTimerDelayTimeOut()));
}

void GIFDisplayer::start()
{
    decoder->resetDecoder();
    currentFrameNumber = 0;
    nextTimeDelay = decoder->delayTimeBuffer.take(currentFrameNumber);
    currentFrameNumber = currentFrameNumber + 1;
    timerDelay->start(nextTimeDelay);
}

void GIFDisplayer::stop()
{
    timerDelay->stop();
    image->setPixmap(firstFramePixmap);
}

void GIFDisplayer::replyTimeOut()
{
    while (decoder->pixmapBuffer.count() == 0);
    timerDelay->start(decoder->delayTimeBuffer.take(currentFrameNumber));
    currentPixmap = QPixmap(decoder->pixmapBuffer.take(currentFrameNumber));
    image->setPixmap(currentPixmap);

    currentFrameNumber = currentFrameNumber + 1;
    if (currentFrameNumber == decoder->imageCount())
    {
        currentFrameNumber = 0;
    }

}

void GIFDisplayer::enterEvent(QEvent*)
{
    this->start();
}

void GIFDisplayer::leaveEvent(QEvent*)
{
    this->stop();
}

GIFDisplayer::~GIFDisplayer()
{
}
