
#ifndef CHATMESSAGEITEM_H
#define CHATMESSAGEITEM_H
#include <QListWidgetItem>
#include <QListWidget>
#include <QWidget>

class ChatMessageItem : public QListWidgetItem {
public:
    ChatMessageItem(const QString& message, const QString& username, QListWidget* parent = nullptr)
        : QListWidgetItem(parent)
        , m_username(username)
    {
        setTextAlignment(m_username == message.left(message.indexOf(':')).trimmed() ? Qt::AlignRight : Qt::AlignLeft);

        (*listWidget).setWordWrap(true);
        setText(message);

        QFontMetrics metrics(font());
        QRect rect = metrics.boundingRect(QRect(0, 0, 200, 0), Qt::TextWordWrap, text());

        if (rect.height() > 100) {
            setFlags(flags() | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
            setSizeHint(QSize(100, rect.height()));
        } else {
            setFlags(flags() | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
            setSizeHint(QSize(100, 100));
        }
    }

private:
    QString m_username;
};

#endif // CHATMESSAGEITEM_H
