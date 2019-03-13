# Arbitrage Bot in Crypto

# Part I

## Future Improvements
* Exception handling: The bot may encounter a few issues, one of which is **disconnection to exchanges**. While fetching the exchanges' data or executing trade, it is possible that one of the connections to exchanges is lost. The bot should reconnect to the exchange before the trade execution.
* Strategy creation: It is better off with a domain-specific language for implementing a trading strategy so as to avoid modifying the existing codebase.
* Exchange creation: It may be possible to insert a new API configuration to connect to an exchange.

## Extra Code for Production
* Robust system: The arbitrage bot could be one of the components. The system can provide a client portal to add API configurations and trading strategies.
* Crypto pair pool: It allows for more pairs of crypto to trade, realizing more trading strategies to be implemented. For each pair, we can have one dedicated machine fetch real-time data from a specific number of exchanges. We do not distribute to another machine the workload in that we execute time-sensitive trade.
* Account management: We shall be able to add a new account and execute trade with the account. If the account does not have enough crypto, we could either stop trading or buy more crypto with USD to trade.
* Activity Logging: It is needed to monitor the trading activity and results. It could give out warnings based on the rules we set up. For example, if the time duration of completing trade is greater than a bound, then a warning should be shown.

## Potential Risk and Avoidance
* API version update: The APIs provided by crypto exchanges may constantly change given that it is an emerging market. In order to resolve this issue, we have to modify our code accordingly. However, this could cause our system downtime, losing the chance of earning profits. We will need a general mechanism to add in a new API configuration to retrieve the crypto exchange data and trade on crypto.
* Trading inconsistency: Though we may successfully execute trade on one exchange but fail on another exchange, which increases the risk of losing total profits. In this case, the system should ensure that these exchanges are well connected before each trade execution.
* Time sensitivity: Provided that the time taken by any exchange to return results is different, we may be able to measure the time difference to successfully execute trade on each exchange. In turn, we could adjust the internal execution moment for each exchange to maintain the simultaneity of trade execution.

# Part II

## Trading Inconsistency Mitigation
If trading inconsistency occurs, it cannot be undone and the total profits will have been decreased. However, we can have an inventory that keeps track of the losses the system has caused due to the failure. Before each trade execution, the bot will decide how many crypto it will buy in given the inventory. That is, if it is not needed to buy, then the bot can directly sell these crypto from the inventory to mitigate the previous losses. The interesting thing is that it may be more profitable to sell at this moment than that if the previous trade execution was wholly successful.

## Monitoring of all Crypto Pairs Between KuCoin and Binance
Each crypto pair can be monitored by a dedicated cluster, including a primary machine, two backup machines, and two arbiter machines. The primary is to receiving and updating with two exchanges. The backups are continuously in sync with data from the primary. The arbiter is to keep asking if the primary and backups are running. If the primary is down, then one of the two arbiters promotes one of the backups as a primary. However, two arbiters may promote a different backup as a primary at the same time. To resolve this issue, whenever each arbiter detects a primary crash, it will ask the other arbiter if a new primary is assigned before it promotes a backup.

## Database Design for Tracking Trade history
Suppose that our trade execution is processed on the primary in the aforementioned cluster for each pair. After each execution, it sends out the information to a cluster of database servers which will be used for client applications (trader apps). In this cluster, we will have 2n+1 database servers to store the trade history. The reason that we have that number of servers is that we actually use a voting mechanism to eliminate undetermination whenever they choose which is the next primary. Moreover, since the data size for each trade execution, which is associated with a dedicated account mentioned below, is limited, we could use NoSQL servers to store the history, which gives the trader faster access to the trade history compared to the performance of SQL JOIN operations.

## In the Presence of API Request Limitation
We may encounter the situation where a limit of API requests is reached. One solution is to own a specific number of accounts. Every account is for each monitored pair to avoid the limitation. Plus, we may need a cluster of database servers to keep these account information.

# Part III

## 1)
Since the buy order is undoable, other than saving it into the inventory, we could also involve other exchanges to fill the sell order that was not possible to execute previously. Using the following mechanism to keep track of time difference, we collect the price and amount of crypto pair from each exchange.
## 2)
To lower the chance of the problem, we can consider the following time taken.
- The time (Sa) to send a request to exchange A, Sb to exchange B
- The time (Ra) to receive a response from exchange A, Rb from exchange B
- The round-trip time (Sa + Ra) for exchange A, (Sb + Rb) for exchange B
- The time (I) to swap and access to the pair price in the bot (We may ignore this factor under the assumption that our data structures and algorithms are efficiently implemented.)

Now that Ta = Sa + Ra + Sa for exchange A, and Tb = Sb + Rb + Sb for exchange B to successfully update pair price and execute trade, the bot can constantly check as follows.
- If Ta > Tb and n >= 0 where n = (Ta - Sb) / (Sb + Rb), we make sure that the bot updates the information from exchange B n times.
- If Ta = Tb, then it is almost perfect but not really because of uncontrolled factors from the outside world.
- If Ta < Tb, then do the opposite to the above to minimize the loss.

In reality, these decision factors may be mainly based on the history of network traffic.
