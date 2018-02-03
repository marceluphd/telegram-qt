/*
   Copyright (C) 2018 

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

#ifndef TELEGRAM_CLIENT_RPC_STICKERS_LAYER_HPP
#define TELEGRAM_CLIENT_RPC_STICKERS_LAYER_HPP

#include "ClientRpcLayerExtension.hpp"
#include "TLTypes.hpp"

namespace Telegram {

class PendingRpcOperation;

namespace Client {

class StickersRpcLayer : public BaseRpcLayerExtension
{
    Q_OBJECT
public:
    explicit StickersRpcLayer(QObject *parent = nullptr);

    // Generated Telegram API declarations
    PendingRpcOperation *addStickerToSet(const TLInputStickerSet &stickerset, const TLInputStickerSetItem &sticker);
    PendingRpcOperation *changeStickerPosition(const TLInputDocument &sticker, quint32 position);
    PendingRpcOperation *createStickerSet(quint32 flags, const TLInputUser &userId, const QString &title, const QString &shortName, const TLVector<TLInputStickerSetItem> &stickers);
    PendingRpcOperation *removeStickerFromSet(const TLInputDocument &sticker);
    // End of generated Telegram API declarations
};

} // Client namespace

} // Telegram namespace

#endif // TELEGRAM_CLIENT_RPC_STICKERS_LAYER_HPP
