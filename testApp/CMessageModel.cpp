/*
   Copyright (C) 2014-2015 Alexandr Akulich <akulichalexander@gmail.com>

   This file is a part of TelegramQt library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

 */

#include "CMessageModel.hpp"
#include "CContactModel.hpp"

#include "CTelegramCore.hpp"

#include <QDateTime>

QString messageDeliveryStatusStr(CMessageModel::SMessage::Status status)
{
    switch (status) {
    case CMessageModel::SMessage::StatusUnknown:
        return QLatin1String("Unknown");
    case CMessageModel::SMessage::StatusSent:
        return QLatin1String("Sent");
    case CMessageModel::SMessage::StatusReceived:
        return QLatin1String("Received");
    case CMessageModel::SMessage::StatusRead:
        return QLatin1String("Read");
    case CMessageModel::SMessage::StatusDeleted:
        return QLatin1String("Deleted");
    default:
        return QString();
    }
}

CMessageModel::CMessageModel(CTelegramCore *backend, QObject *parent) :
    QAbstractTableModel(parent),
    m_backend(backend)
{
    connect(m_backend, SIGNAL(sentMessageIdReceived(quint64,quint32)),
            SLOT(setResolvedMessageId(quint64,quint32)));

    connect(m_backend, SIGNAL(messageReadInbox(TelegramNamespace::Peer,quint32)),
            SLOT(setMessageInboxRead(TelegramNamespace::Peer,quint32)));
    connect(m_backend, SIGNAL(messageReadOutbox(TelegramNamespace::Peer,quint32)),
            SLOT(setMessageOutboxRead(TelegramNamespace::Peer,quint32)));
}

QVariant CMessageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    switch (section) {
    case Peer:
        return tr("Peer");
    case Contact:
        return tr("Phone");
    case Direction:
        return tr("Direction");
    case Timestamp:
        return tr("Timestamp");
        break;
    case MessageId:
        return tr("Message Id");
    case Message:
        return tr("Message text");
    case Status:
        return tr("Status");
    case ForwardFromContact:
        return tr("Forward from");
    case ForwardTimestamp:
        return tr("Forward timestamp");
    default:
        break;
    }

    return QVariant();
}

QVariant CMessageModel::data(const QModelIndex &index, int role) const
{
    int section = index.column();
    int messageIndex = index.row();

    if (messageIndex >= rowCount()) {
        return QVariant();
    }

    if ((role == Qt::DecorationRole) && (section == Message)) {
        if (m_messages.at(messageIndex).mediaData.isValid()) {
            return m_messages.at(messageIndex).mediaData;
        }

        return QVariant();
    }

    if ((role != Qt::DisplayRole) && (role != Qt::EditRole)) {
        return QVariant();
    }

    return rowData(messageIndex, section);
}

QVariant CMessageModel::rowData(quint32 messageIndex, int column) const
{
    if (int(messageIndex) >= m_messages.count()) {
        return QVariant();
    }

    switch (column) {
    case Peer:
        return QVariant::fromValue(m_messages.at(messageIndex).peer());
    case Contact:
        return m_messages.at(messageIndex).userId;
    case Direction:
        return (m_messages.at(messageIndex).flags & TelegramNamespace::MessageFlagOut) ? tr("out") : tr("in");
    case Timestamp:
        return QDateTime::fromMSecsSinceEpoch(quint64(m_messages.at(messageIndex).timestamp) * 1000);
    case MessageId:
        return m_messages.at(messageIndex).id ? m_messages.at(messageIndex).id : m_messages.at(messageIndex).id64;
    case Message:
        return m_messages.at(messageIndex).text;
    case Status:
        return messageDeliveryStatusStr(m_messages.at(messageIndex).status);
    case ForwardFromContact:
        return m_messages.at(messageIndex).forwardContactId;
    case ForwardTimestamp:
        if (m_messages.at(messageIndex).fwdTimestamp) {
            return QDateTime::fromMSecsSinceEpoch(quint64(m_messages.at(messageIndex).fwdTimestamp) * 1000);
        }
        break;
    default:
        break;
    }

    return QVariant();
}

const CMessageModel::SMessage *CMessageModel::messageAt(quint32 messageIndex) const
{
    if (int(messageIndex) >= m_messages.count()) {
        return 0;
    }

    return &m_messages[messageIndex];
}

int CMessageModel::messageIndex(quint64 messageId) const
{
    for (int i = 0; i < m_messages.count(); ++i) {
        if (m_messages.at(i).id == messageId) {
            return i;
        }
        if (m_messages.at(i).id64 == messageId) {
            return i;
        }
    }

    return -1;
}

void CMessageModel::addMessage(const SMessage &message)
{
    for (int i = 0; i < m_messages.count(); ++i) {
        if ((m_messages.at(i).id64 && (m_messages.at(i).id64 == message.id64))
                || (!m_messages.at(i).id64 && (m_messages.at(i).id == message.id))) {
            m_messages.replace(i, message);

            emit dataChanged(index(i, 0), index(i, ColumnsCount - 1));
            return;
        }
    }
    beginInsertRows(QModelIndex(), m_messages.count(), m_messages.count());
    m_messages.append(message);
    if (!m_messages.last().timestamp) {
        m_messages.last().timestamp = QDateTime::currentMSecsSinceEpoch() / 1000;
    }
    endInsertRows();
}

int CMessageModel::setMessageMediaData(quint64 messageId, const QVariant &data)
{
    int i = messageIndex(messageId);
    if (i < 0) {
        return i;
    }

    m_messages[i].mediaData = data;
#if QT_VERSION < 0x050000
    emit dataChanged(index(i, Message), index(i, Message));
#else
    emit dataChanged(index(i, Message), index(i, Message), QVector<int>() << Qt::DecorationRole);
#endif
    return i;
}

void CMessageModel::setMessageRead(TelegramNamespace::Peer peer, quint32 messageId, bool out)
{
    int from = -1;
    for (int i = 0; i < m_messages.count(); ++i) {
        SMessage &message = m_messages[i];

        const bool hasFlagOut = message.flags & TelegramNamespace::MessageFlagOut;
        const bool hasRightDirection = hasFlagOut == out;
        const bool idIsFitInRange = message.id <= messageId;
        const bool isNotAlreadyRead = message.status != CMessageModel::SMessage::StatusRead;
        const bool haveTargetPeer = message.peer() == peer;

        if (hasRightDirection && idIsFitInRange && isNotAlreadyRead && haveTargetPeer) {
            if (from < 0) {
                from = i;
            }

            message.status = CMessageModel::SMessage::StatusRead;
        } else {
            if (from >= 0) {
                QModelIndex firstIndex = index(from, Status);
                QModelIndex lastIndex = index(i - 1, Status);
                emit dataChanged(firstIndex, lastIndex);
                from = -1;
            }
        }
    }

    if (from >= 0) {
        QModelIndex firstIndex = index(from, Status);
        QModelIndex lastIndex = index(m_messages.count() - 1, Status);
        emit dataChanged(firstIndex, lastIndex);
    }
}

void CMessageModel::setMessageInboxRead(TelegramNamespace::Peer peer, quint32 messageId)
{
    setMessageRead(peer, messageId, /* out */ false);
}

void CMessageModel::setMessageOutboxRead(TelegramNamespace::Peer peer, quint32 messageId)
{
    setMessageRead(peer, messageId, /* out */ true);
}

void CMessageModel::setResolvedMessageId(quint64 randomId, quint32 resolvedId)
{
    for (int i = 0; i < m_messages.count(); ++i) {
        SMessage &message = m_messages[i];
        if (message.id64 == randomId) {
            message.id = resolvedId;
            message.status = CMessageModel::SMessage::StatusSent;

            QModelIndex firstIndex = index(i, MessageId);
            QModelIndex lastIndex = index(i, Status);
            emit dataChanged(firstIndex, lastIndex);
            break;
        }
    }
}