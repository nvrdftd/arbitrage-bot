## Arbitrage Bot in Crypto

## Future Improvements
* Exception handling: The bot may encounter a few issues, one of which is **disconnection to exchanges**. While fetching the exchanges' data or executing trade, it is possible that one of the connections to exchanges is lost. The bot should reconnect to the exchange before the trade execution.
* Strategy creation: It is better off with a domain-specific language for implementing a trading strategy so as to avoid modifying the existing codebase.
* Exchange creation: It may be possible to insert a new API configuration to connect to an exchange.

## Extra Code for Production
* Robust system: The arbitrage bot could be one of the components. The system can provide a client portal to add API configurations and trading strategies.
* Crypto pair pool: It allows for more pairs of crypto to trade, realizing more trading strategies to be implemented.
* Account management: We shall be able to add a new account and execute trade with the account. If the account does not have enough crypto, we could either stop trading or buy more crypto with USD.
* Activity Logging: It is needed to monitor the trading activity and results. It could give out warnings based on the rules we set up. For example, if the time duration of completing trade is greater than a bound, then a warning should be shown.

## Potential Risk and Avoidance
* API version update: The APIs provided by crypto exchanges may constantly change given that it is an emerging market. In order to resolve this issue, we have to modify our code accordingly. However, this could cause our system downtime, losing the chance of earning profits. We will need a general mechanism to add in a new API to retrieve the crypto exchange data.
* Trading inconsistency: Though we may successfully execute trade on one exchange but fail on another exchange, which increases the risk of losing total profits. In this case, the system should ensure that these exchanges are well connected.
