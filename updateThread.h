
#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H
#include <QThread>
#include <QTimer>

class UpdateThread : public QThread {

Q_OBJECT

public:
    explicit UpdateThread(QObject *parent = nullptr)
        :QThread(parent), m_timer(nullptr)
    {
    }

    ~UpdateThread() override {
        if (m_timer != nullptr) {
            m_timer->stop();
            delete m_timer;
        }
    }

signals:
    void updateUsersList();
    void updateGroupList();
    void updateChannelList();
    void updateCurrentChatMessages();

protected:
    void run() override {
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &UpdateThread::updateAll);
        m_timer->start(30000);
        exec();
    }

private slots:
    void updateAll() {
        emit updateUsersList();
        emit updateGroupList();
        emit updateChannelList();
        emit updateCurrentChatMessages();
    }


private:
    QTimer *m_timer;
};

#endif // UPDATETHREAD_H
