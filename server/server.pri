
SOURCES += $$PWD/DefaultAuthorizationProvider.cpp
SOURCES += $$PWD/LocalCluster.cpp
SOURCES += $$PWD/MediaService.cpp
SOURCES += $$PWD/MessageService.cpp
SOURCES += $$PWD/ServerDhLayer.cpp
SOURCES += $$PWD/ServerMessageData.cpp
SOURCES += $$PWD/ServerRpcLayer.cpp
SOURCES += $$PWD/ServerRpcOperation.cpp
SOURCES += $$PWD/ServerUtils.cpp
SOURCES += $$PWD/Session.cpp
SOURCES += $$PWD/RpcOperationFactory.cpp
SOURCES += $$PWD/RpcProcessingContext.cpp
SOURCES += $$PWD/TelegramServer.cpp
SOURCES += $$PWD/TelegramServerConfig.cpp
SOURCES += $$PWD/TelegramServerUser.cpp
SOURCES += $$PWD/CServerTcpTransport.cpp
SOURCES += $$PWD/RemoteClientConnection.cpp
SOURCES += $$PWD/RemoteClientConnectionHelper.cpp
SOURCES += $$PWD/RemoteServerConnection.cpp
SOURCES += $$PWD/FunctionStreamOperators.cpp

HEADERS += $$PWD/AuthorizationProvider.hpp
HEADERS += $$PWD/DefaultAuthorizationProvider.hpp
HEADERS += $$PWD/IMediaService.hpp
HEADERS += $$PWD/LocalCluster.hpp
HEADERS += $$PWD/MediaService.hpp
HEADERS += $$PWD/MessageService.hpp
HEADERS += $$PWD/ServerApi.hpp
HEADERS += $$PWD/ServerDhLayer.hpp
HEADERS += $$PWD/ServerNamespace.hpp
HEADERS += $$PWD/ServerMessageData.hpp
HEADERS += $$PWD/ServerRpcLayer.hpp
HEADERS += $$PWD/ServerRpcOperation.hpp
HEADERS += $$PWD/ServerUtils.hpp
HEADERS += $$PWD/Session.hpp
HEADERS += $$PWD/RpcOperationFactory.hpp
HEADERS += $$PWD/RpcOperationFactory_p.hpp
HEADERS += $$PWD/RpcProcessingContext.hpp
HEADERS += $$PWD/TelegramServer.hpp
HEADERS += $$PWD/TelegramServerConfig.hpp
HEADERS += $$PWD/TelegramServerUser.hpp
HEADERS += $$PWD/CServerTcpTransport.hpp
HEADERS += $$PWD/RemoteClientConnection.hpp
HEADERS += $$PWD/RemoteClientConnectionHelper.hpp
HEADERS += $$PWD/RemoteServerConnection.hpp
HEADERS += $$PWD/FunctionStreamOperators.hpp

include(RpcOperations/operations.pri)
