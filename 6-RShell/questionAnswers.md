1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_my client uses a loop that repeatedly calls recv() until it detects a designated end of transmission character. After each recv it checks whether the last byte equals that EOF marker. If it does, the client knows the full output has been received. a technique for handling partial reads could be aggregating data from multiple recv() calls until the special marker is seen_

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_TCP is stream oriented and does not preserve message boundaries, a networked shell protocol must define its own delimiters or length headers. Without a clear delimiter or length field, messages can be split or concatenated across packets, causing parsing issues._

3. Describe the general differences between stateful and stateless protocols.

_Stateful protocols maintain information about past interactions across multiple messages. Stateless protocols treat each request as an independent transaction with no memory of previous requests._

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_UDP has lower latency and overhead. it is good in cases where occasional packet loss is acceptable._

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_<sys/socket.h> and <arpa/inet.h> are provided by the operating system and allow us to use functions such as socket(), connect(), send(), and recv()_