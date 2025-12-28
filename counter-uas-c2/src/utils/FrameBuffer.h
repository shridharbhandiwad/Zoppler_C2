#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <QObject>
#include <QImage>
#include <QMutex>
#include <QQueue>

namespace CounterUAS {

struct BufferedFrame {
    QImage image;
    qint64 timestamp;
    int frameNumber;
};

class FrameBuffer : public QObject {
    Q_OBJECT
    
public:
    explicit FrameBuffer(int capacity = 30, QObject* parent = nullptr);
    
    void push(const QImage& frame, qint64 timestamp);
    BufferedFrame pop();
    BufferedFrame peek() const;
    
    bool isEmpty() const;
    int count() const;
    int capacity() const { return m_capacity; }
    void setCapacity(int capacity);
    void clear();
    
signals:
    void frameAvailable();
    void bufferFull();
    
private:
    mutable QMutex m_mutex;
    QQueue<BufferedFrame> m_buffer;
    int m_capacity;
    int m_frameCounter = 0;
};

} // namespace CounterUAS

#endif // FRAMEBUFFER_H
