###Protocol
####Typical behaviour 
Client wakes up periodically, sends its status to associated server. 
Then client starts to listening for servers response for 

<pre>
  0          8                             72      
 +----------+------------------------------+
 | MSG_TYPE |           TOKEN              |
 -----------+-------------------------------
 |               PAYLOAD                   |
 +-----------------------------------------+
 </pre>
