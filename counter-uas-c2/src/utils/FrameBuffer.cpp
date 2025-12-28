#include "utils/FrameBuffer.h"
#include <QMutexLocker>

namespace CounterUAS {

FrameBuffer::FrameBuffer(int capacity, QObject* parent)
    : QObject(parent), m_capacity(capacity) {}

void FrameBuffer::push(const QImage& frame, qint64 timestamp) {
    QMutexLocker locker(&m_mutex);
    
    BufferedFrame bf;
    bf.image = frame;
    bf.timestamp = timestamp;
    bf.frameNumber = m_frameCounter++;
    
    m_buffer.enqueue(bf);
    
    while (m_buffer.size() > m_capacity) {
        m_buffer.dequeue();
    }
    
    locker.unlock();
    emit frameAvailable();
    
    if (m_buffer.size() >= m_capacity) {
        emit bufferFull();
    }
}

BufferedFrame FrameBuffer::pop() {
    QMutexLocker locker(&m_mutex);
    if (m_buffer.isEmpty()) return BufferedFrame();
    return m_buffer.dequeue();
}

BufferedFrame FrameBuffer::peek() const {
    QMutexLocker locker(&m_mutex);
    if (m_buffer.isEmpty()) return BufferedFrame();
    return m_buffer.head();
}

bool FrameBuffer::isEmpty() const {
    QMutexLocker locker(&m_mutex);
    return m_buffer.isEmpty();
}

int FrameBuffer::count() const {
    QMutexLocker locker(&m_mutex);
    return m_buffer.size();
}

void FrameBuffer::setCapacity(int capacity) {
    QMutexLocker locker(&m_mutex);
    m_capacity = capacity;
    while (m_buffer.size() > m_capacity) {
        m_buffer.dequeue();
    }
}

void FrameBuffer::clear() {
    QMutexLocker locker(&m_mutex);
    m_buffer.clear();
}

} // namespace CounterUAS
