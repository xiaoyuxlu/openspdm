Transition:
Get_version
1.	A Requester shall not issue other requests until it receives a successful VERSION response and identifies a common version that both sides support. 
Successfully finish version changes, cur_phase to be POST_VERSION;
GET_CAPABILITIES can only be executed in POST_VERSION state;
POST_VERSIONS are not conditions of other functions
**done**
2.	A Responder shall not respond to the GET_VERSION request message with ErrorCode=ResponseNotReady .
ERROR and RESPONSE_IF_READY created and connected
With LTL properties checked
**done**
3.	A Requester may issue a GET_VERSION request message to a Responder at any time, when a Requester must restart the protocol due to an internal error or reset.
Reset and reset_resp created and connected
**to be implemented**
4.	After receiving a GET_VERSION request, the Responder shall cancel all previous requests from the same Requester. Additionally, this message shall clear or reset the previously Negotiated State, if any, in both the Requester and its corresponding Responder.
Clear state done. How to cancel previous requests?
**to be implemented**

Get_capabilities
1.	A Responder shall not respond to GET_CAPABILITIES request message with ErrorCode=ResponseNotReady .
**done**

Negotiate_algorithms
1.	A Requester shall not issue a NEGOTIATE_ALGORITHMS request message until it receives a successful CAPABILITIES response message.
**done**
2.	A Requester shall not issue any other SPDM requests, with the exception of GET_VERSION until it receives a successful ALGORITHMS response message
**done**
3.	A Responder shall not respond to NEGOTIATE_ALGORITHMS request message with ErrorCode=ResponseNotReady .
**done**
4.	A Responder shall not select both a SPDM-enumerated asymmetric key signature algorithm and an extended asymmetric key signature algorithm. A Responder shall not select both a SPDM-enumerated hashing algorithm and an extended hashing algorithm.
**to be implemented**
5.	both endpoints may issue NEGOTIATE_ALGORITHMS request messages independently of each other
**normal case** **done**

Get_digests
**done**

Get_certificate
1.	The Requester sends one or more GET_CERTIFICATE requests to retrieve Responder's certificate chain.
**seems done**

Challenge
1. Page 60
The possible request orderings after reset leading up to and including CHALLENGE are:
• GET_VERSION , GET_CAPABILITIES , NEGOTIATE_ALGORITHMS , GET_DIGESTS , GET_CERTIFICATE , CHALLENGE
(A1, B1, C1)
• GET_VERSION , GET_CAPABILITIES , NEGOTIATE_ALGORITHMS , GET_DIGESTS , CHALLENGE (A1, B3, C1)
• GET_VERSION , GET_CAPABILITIES , NEGOTIATE_ALGORITHMS , CHALLENGE (A1, B2, C1)
• GET_DIGESTS , GET_CERTIFICATE , CHALLENGE (A2, B1, C1)
• GET_DIGESTS , CHALLENGE (A2, B3, C1)
• CHALLENGE (A2, B2, C1)
The possible request orderings after reset without CHALLENGE are:
• GET_DIGESTS (A2, B3, C2)
• NULL (A2, B2, C2)
**to be implemented**
**property: only check the first ordering, need to implement reset**


Get_measurements

1. All request messages in this clause shall be supported by an endpoint that returns MEAS_CAP=01b or MEAS_CAP=10b in CAPABILITIES response.
**done**
**property done**
2. If MEAS_FRESH_CAP bit in the CAPABILITIES response message returns 0, and the Requester requires fresh measurements, the Responder must be reset before GET_MEASUREMENTS is resent. The mechanisms employed for resetting the Responder are outside the scope of this specification
**done**
**property done**
3. A Requester should not send this message until it has received at least one successful CHALLENGE_AUTH response message from the responder. The successful CHALLENGE_AUTH response may have been received before the last reset.
**done** (what about key exchange?)
**property done (without the things mentioned in the brackets)**
4. Any communication between Requester and Responder other than a GET_MEASUREMENTS request or response resets L1/L2 computation to null.
**to be implemented**

Error
1.	For a SPDM operation that results in an error, the Responder shall send an ERROR response message to the Requester.
**equivalently done**
**assertions added**

Respond_if_ready
1. This request message shall ask for the response to the original request upon receipt of ResponseNotReady error code.
**directly implemented** 
**assertions added**
2. If the response to the original request is ready, the Responder shall return that response message. 
**equivalent assertions done**
3. If the response to the original request is not ready, the Responder shall return the ERROR response message, set ErrorCode = ResponseNotReady and return the same token as the previous ResponseNotReady response message.
**for all**
**now cannot check**

VENDOR_DEFINED_REQUEST
1. A Requester intending to define a unique request to meet its need can use this request message. 
**done**
2. The Requester should send this request message only after sending GET_VERSION , GET_CAPABILITIES and NEGOTIATE_ALGORITHMS request sequence.
**to be implemented**
**property done**

Key_exchange
1.  two independent sets of root session keys can be coexisting at the same time.
**to be implemented**
2. a Responder may generate an ErrorCode=Busy response to second KEY_EXCHANGE request message until first FINISH response message is complete in order to simplify implementation
**to be implemented**
**currently will not be able to process the second one while processing the first one**

Finish
1. This request message shall complete the handshake between Requester and Responder initiated by a KEY_EXCHANGE request. 
**done graph**
2. The purpose of the FINISH request and FINISH_RSP response messages is to provide key confirmation, bind each party's identity to the exchanged keys and protect the entire handshake against manipulation by an active attacker. 
**done graph**
3. When multiple session keys are being established between the same Requester and Responder pair, Signature over Transcript HASH during FINISH request is computed using only the corresponding KEY_EXCHANGE, KEY_EXCHANGE_RSP and FINISH request parameters.
**to be implemented**

Psk_exchange
1. provides an option for a Requester and a Responder to perform mutual authentication and session key establishment with symmetric-key cryptography. This option is especially useful for endpoints that do not support asymmetric-key cryptography or certificate processing. 
**to be implemented**
2. This option requires the Requester and the Responder to have prior knowledge of a common PSK before the handshake
**to be implemented**
3.  A pair of Requester and Responder may be provisioned with one or more PSKs.
**partially implemented graph**
**not checked in property**
4. A Requester may be paired with multiple Responders. Likewise, a Responder may be paired with multiple Requesters.
**to be implemented**
5. An endpoint may act as a Requester to one device and simultaneously a Responder to another device.
**to be implemented**
6. The PSK may be provisioned in a trusted environment, for example, during the secure manufacturing process. In an untrusted environment, the PSK may be agreed upon between the two endpoints using a secure protocol
**to be implemented**
7. During PSK provisioning, an endpoint's capabilities and supported algorithms may be communicated to the peer. SPDM commands GET_CAPABILITIES and NEGOTIATE_ALGORITHMS are not required during session key establishment with the PSK option
**not implememt**
**property partially implemented**
The PSK_EXCHANGE message carries three responsibilities:
*1. Prompts the Responder to retrieve the specific PSK.
*2. Exchanges contexts between the Requester and the Responder.
*3. Proves to the Requester that the Responder knows the correct PSK and has derived the correct session keys.
8. The field OpaquePSKData is optional (absent if P is set to 0). It is introduced to address two scenarios:
• The Responder is provisioned with multiple PSKs and stores them in secure storage. The Requester uses
OpaquePSKData as an ID to specify which PSK will be used in this session.
• The Responder does not store the value of the PSK, but can derive the PSK using OpaquePSKData. For example, if the Responder has an immutable UDS (Unique Device Secret) in fuses, then during provisioning, a PSK may be derived from the UDS or its derivative and a non-secret salt provided by the Requester. During session key establishment, the same salt is sent to the Responder in OpaquePSKData of PSK_EXCHANGE. This mechanism allows the Responder to support any number of PSKs, without consuming secure storage.
**may not implement?**
9. Upon receiving PSK_EXCHANGE request, the Responder:
*1. Acquires PSK from OpaquePSKData, if necessary.
*2. Generates ResponderContext, if supported.
*3. Derives the Responder's finished_key by following Key Schedule.
*4. Constructs PSK_EXCHANGE_RSP response message and sends to the Requester
10. If a nonce is not present in ResponderContext, then the Responder is not challenging the Requester for real-time knowledge of PSK. Such a session is subject to replay attacks - a man-in-the-middle attacker could record and replay prior PSK_EXCHANGE and PSK_FINISH messages and set up a session with the Responder. But the bogus session would not leak secrets, so long as the PSK or session keys of the prior replayed session are not compromised.
**to be implemented**
11. If ResponderContext is present in the response (i.e., PSK_CAP in Responder's CAPABILITIES is 10b ), then the Requester must send PSK_FINISH with requester_verify_data to prove that it has derived correct session keys. However, if ResponderContext is absent (i.e., PSK_CAP in Responder's CAPABILITIES is 01b ), then the Requester is not required to send PSK_FINISH, as the session keys are solely determined by the Requester. In other words, if the Responder demands session key verification, then it must use ResponderContext, even if a nonce is not included, to signal the Requester to send PSK_FINISH request.
**partially implemented graph**
**assertions added**
12. Upon receiving PSK_EXCHANGE_RSP, the Requester:
*1. Derives the Responder's finish key by following Key Schedule.
*2. Verify ResponderVerifyData by calculating the MAC in the same manner as the Responder. If
verification fails, the Requester aborts the session.
**every phase should **
*3. If the Responder contributes to session key derivation ( PSK_CAP in Responder's CAPABILITIES is 10b ), construct PSK_FINISH request and send to the Responder.
**to be implemented**
**last has assertions added**

Psk_finish
1. The PSK_FINISH request proves to the Responder that the Requester knows the PSK and has derived the correct session keys.
**to be implemented**
2. Upon receiving PSK_FINISH request, the Responder derives the Requester's finished_key and calculates the MAC independently in the same manner and verifies the result matches RequesterVerifyData. If verified, then the Responder constructs PSK_FINISH_RSP response and sends to the Requester. Otherwise, the Responder sends ERROR response message to the Requester
**to be implemented**
**last has assertions corresponded**

Heartbeat
1. The HEARTBEAT request shall be sent periodically as indicated in HeartbeatPeriod in either KEY_EXCHANGE_RSP or PSK_EXCHANGE_RSP response messages.
**to be implemented**
2.  The Responder shall terminate the session if a HEARTBEAT request is not received in twice HeartbeatPeriod
**to be implemented**
3. the Requester shall terminate the session if a HEARTBEAT_ACK response or ERROR response is not received in twice HeartbeatPeriod
**to be implemented**
4.  If an Error with ErrorCode=InvalidSessionID Response is received, the Requester shall terminate the session
**partially implemented graph**
**END_SESSION now will not go to GET_VERSION**
**assetion added**
5. The Requester may retry HEARTBEAT requests. The Requester shall wait ST1 time for the response before retrying
**to be implemented**
6. The timer for the Heartbeat period shall start at the transmission, for Responders, or reception, for Requester, of either the FINISH_RSP or PSK_FINISH_RSP response messages. 
**to be implemented**
7. When determining the value of HeartbeatPeriod, the Responder should ensure this value is sufficiently greater than RTT .
**to be implemented**
8. The transport layer may allow the HEARTBEAT request to be sent from the Responder to the Requester. This is recommended for transports capable of asynchronous bidirectional communication.
**may not implemented?**


/*only read transitions*/

Key_update
1. The KEY_UPDATE request can be issued by the Responder as well using the GET_ENCAPSULATED_REQUEST mechanism. 
**to be implemented**
2. A KEY_UPDATE request shall update session keys in the direction of the request only. Because the Responder can also send this request, it is possible that two simultaneous key updates, one for each direction, can occur. However, only one KEY_UPDATE request for a single direction shall occur. 
**to be implmemented**
3. Until the session key update synchronization successfully completes, subsequent KEY_UPDATE request for the same direction shall be considered a retry of the original KEY_UPDATE request
**to be implemented**
4. The Session Key Update Synchronization process shall start with the responsibility on the sender to quiesce all application data traffic to the receiver. (do it by itself internally) If UpdateAllKeys is the selected operation, the receiver shall also quiesce all application data traffic to the sender. If UpdateKey is the selected operation, the receiver may also quiesce application traffic to the sender but this is unnecessary. 
**to be implemented**
5. Once the sender has quiesced the transportation of application data to the receiver, the sender shall, then, send a KEY_UPDATE request with UpdateKey or UpdateAllKeys operation.
**to be implmemented**
6. If the sender has not received KEY_UPDATE_ACK , the sender may retry or end the session. The sender shall not proceed to the next step until successfully receiving the corresponding KEY_UPDATE_ACK
**to be implmemented**
**assertions added**
7. Upon the successful reception of the KEY_UPDATE_ACK , the sender shall have ST1 time to transmit a KEY_UPDATE request with VerifyNewKey operation using the new session keys. The sender may retry until the corresponding KEY_UPDATE_ACK response is received. However, the sender shall be prohibited, at this point, from restarting this process or going back to a previous step
**partially implemented graph**
****
8. After the sender successfully receives the corresponding KEY_UPDATE_ACK,transportation of application data may resume. 
**to be implemented**
9.  the receiver may reply with an ERROR message with ErrorCode=Busy. The sender should retry the request after a reasonable period of time with a reasonable amount of retries to prevent premature session termination.
**partially implemented graph**

Get_encapsulated_request
1. The response for this message encapsulates an SPDM request message as if the Responder was a Requester.
**partially implemented graph**
2. The Responder shall use the same SPDM version the Requester used
**to be implemented**
3. Except for this request and DELIVER_ENCAPSULATED_RESPONSE , the Requester shall not send any other SPDM request message until successfully fulfilling the Responder's request. If a Responder receives a request other than DELIVER_ENCAPSULATED_RESPONSE or GET_ENCAPSULATED_REQUEST after the Responder already has provided a request to the Requester to which it has not received a response, the Responder shall respond with ErrorCode=RequestInFlight .
**partially implemented graph**
****
**partially implementations added**

Deliver_encapsulated_response
1. This request delivers the response to the Responder's request which was encapsulated in the previous ENCAPSULATED_REQUEST response message.
**partially implemented graph**
2. if there are additional requests from the Responder, the Responder shall provide the next request in the ENCAPSULATED_RESPONSE_ACK response message
**partially implemented graph**
3. the Requester shall not send any other requests with the exception of DELIVER_ENCAPSULATED_RESPONSE until successfully delivering the response to the current request from the Responder. If a Responder receives a request other than DELIVER_ENCAPSULATED_RESPONSE after the Responder already has provided a request to the Requester to which it has not received a response, the Responder shall respond with ErrorCode=RequestInFlight.
**partially implemented graph**
**partially implementations added, similar to the previous one?**
**shall not proceed to the next, which is equivalent to the KEY_UPDATE 6?**
****
4. Using a unique request ID is highly recommended to avoid confusion between a retry and a new request of the DELIVER_ENCAPSULATED_RESPONSE request
**to be implemented**
5.  the response timing for ENCAPSULATED_RESP_ACK shall be subject to the same timing constraints as the original request.
**to be implemented**

End_session
1. Further communication between the Requester and Responder using the same session ID shall be prohibited. 
**to be implemented**
2. The Responder shall return ErrorCode=InvalidSession after session termination.
**partially implemented graph**
**(but still can initiate new one?)**

