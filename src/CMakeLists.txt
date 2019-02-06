# add libraries to be built
add_library(arbitrage STATIC
    RClient.cpp
    WSClient.cpp
    BinanceClient.cpp
    KuCoinClient.cpp
    Detector.cpp
)

add_library(trading STATIC
    Exchange.cpp
    OrderBook.cpp
    TwofoldArbiStrat.cpp
)

add_library(utils STATIC
    RClient.cpp
    WSClient.cpp
)
