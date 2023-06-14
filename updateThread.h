#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <QThread>
#include <QTimer>
#include <QMutex>

class UpdateThread : public QThread {
    Q_OBJECT

public:
    explicit UpdateThread(QObject *parent = nullptr) :
        QThread(parent),
        m_timer(new QTimer(this)),
        m_mutex(new QMutex())
    {
        connect(m_timer, &QTimer::timeout, this, &UpdateThread::updateUI);
        m_timer->start(10000);
    }

    ~UpdateThread() override {
        if (m_timer != nullptr) {
            m_timer->stop();
            delete m_timer;
        }
        if (m_mutex != nullptr) {
            delete m_mutex;
        }
    }

signals:
    void updateUsersList();
    void updateGroupList();
    void updateChannelList();
    void updateCurrentChatMessages();

protected:
    void run() override {
        exec();
    }

private slots:
    void updateUI() {
        // Lock the mutex to prevent concurrent access to the shared data
        m_mutex->lock();

        // Update the user/group/channel lists
        emit updateUsersList();
        emit updateGroupList();
        emit updateChannelList();

        // Unlock the mutex to allow access to the shared data by other threads
        m_mutex->unlock();

        // Reload the messages in the currently selected chat
        emit updateCurrentChatMessages();
    }

private:
    QTimer *m_timer;
    QMutex *m_mutex;
};

#endif // UPDATETHREAD_H
