1. anything (rule) directly mentioned "attacks"
2. checking with signature (verification)
3. definitions of cleaning/canceling/not doing previous states
4. define the value range of the messages 
4.1. more security-oriented
5. both party involves checking
6. rule checking


go through md to GET_MEASUREMENTS
check questions  to .c pdf and imp to .c; if not enough, directly read .c when implmementing

can both of reque and respon be malicious or only one upon a time@
later


Security_Validation

Property: &
interesting to discuss: $$
categories: <>
question: Que?; now no question: @
important rules to implement: imp
then first level rules to implement: first
second level rules to implement: sec
not added: XXX


GET_VERSION:
1. The Requester shall begin the discovery process by sending a GET_VERSION request message with major version 0x1. The Requester shall consult the VERSION response to select a common supported version, which is typically the latest supported common version. The Requester shall use the selected version in all future communication of other requests.
-> version>=1, yes now only 1 and 1.1
&
ltl px {[] (version>=1)};
<4>
imp

2. A Requester may issue a GET_VERSION request message to a Responder at any time, which is as an exception to Requirements for Requesters for the case where a Requester must restart the protocol due to an internal error or reset. After receiving a GET_VERSION request, the Responder shall cancel all previous requests from the same Requester. Additionally, this message shall clear or reset the previously Negotiated State, if any, in both the Requester and its corresponding Responder.
ltl px {[]! (signal==VERSION && Responder_cache!=0)};
<3>
what is internal error or reset@
signature not created, verification fail -> critical 
random number generator not generated, reset
SPDM context reset to zero
(bitmap is one field of them)
sec

3. All Responders that have completed a firmware update shall respond with ErrorCode=RequestResynch to any request until a GET_VERSION request is received.
sec

MajorVersion: Version of the specification with changes that are incompatible with one or more functions in earlier major versions of the specification; MinorVersion: Version of the specification with changes that are compatible with functions in earlier minor versions of this major version specification.
major version, minor version.
major version
minor 0->1


GET_CAPABILITIES:
version in p33 still V1.0=0x10@, 1.0->1.1

CTExponent in both reque and respon not the same@ do not has communication function or not
own timing requriement

MEAS_CAP: The corresponding bits of the Responder flag fields definitions indicate MEASUREMENT response capabilities. These bits shall be set to 00b


4. 
English spec:   If MEAS_FRESH_CAP bit in the CAPABILITIES response message returns 0, and the Requester requires fresh measurements, the Responder must be reset before GET_MEASUREMENTS is resent. The mechanisms employed for resetting the Responder are outside the scope of this specification.
explanations:   MEASUREMENTS related messages must be cleaned up before sending back the response message.
                If not cleaning up as the MEAS_FRESH_CAP indicates, the measurements may contain previous data (may not be fully replaced by this time's measurement if the length is not the same), 
                in which case correctness is not guaranteed and previous caching information may be leaked (may have side channel issues)
LTL (and possible C) assertions (I happen to also treat it as a rule in state translation rules):   
                right after the possible clean up states and before GET_MEASUREMENTS are prepared: 
                    assert(!(MEAS_FRESH_CAP==0 && measurements_match==1));
                    (cannot be the case that pre-states require cleaning up while the related states such as measurements_match is still not cleaned up now)
&
<3>
imp

p35 MUT_AUTH_CAP is used nowhere@
in the following CHALLENGE

5. ENCAP_CAP: If set, Requester supports GET_ENCAPSULATED_REQUEST , ENCAPSULATED_REQUEST, DELIVER_ENCAPSULATED_RESPONSE and ENCAPSULATED_RESPONSE_ACK messages. If mutual authentication is also supported by the Requester, this field shall be set also.
enable when either one of the two is supported
&
ltl px {[]! (ENCAP_CAP==1&&(4 signals are not supported)||(mutual authentication is not supported))))};
<4>
sec

6. 
English spec:   If CACHE_CAP is set, the Responder supports the ability to cache the Negotiated State across a reset. The Responder shall cache the selected cryptographic algorithms as one of the parameters of the Negotiated State. If the Requester chooses to skip issuing these requests after the reset, the Requester shall also cache the same selected cryptographic algorithms.
explanations:   this is a assertions related to two parties or cross-party checking. 
                The two cached results from the Requester and Responder shall be the same.
&
ltl px {[]! ((CACHE_CAP != 0) && (Requester_Cache!=Responder_Cache))};
<5>
imp

NEGOTIATE_ALGORITHMS
7. 
English spec:   A Requester shall not issue a NEGOTIATE_ALGORITHMS request message until it receives a successful CAPABILITIES response message.
explanations:   I currently treat it not only a state-transition related rules but also possibly rules related to security validation.
                Image the NEGOTIATE_ALGORITHMS is done before GET_CAPABILITIES, the related fields indicating the capabilities should be default set to 0 (or some random value related to the actual implementation). 0 still has its meaning of capabilities and most of 0s to the capabilities means the corresponding function is not supported including measuremnts, secure session. In this case, it may result in a mis-judging of the capabilities of responder, leading to misfunction or function-not-enabled communication channel.
LTL prop (I happend to also treat it as a rule in state translation rules):   
                right after the possible clean up states and before GET_MEASUREMENTS are prepared: 
                    ltl p35 {[]! ((capabilities_checked == 0) && (response_signal==NEGOTIATE_ALGORITHMS))};
C assertions:   inside NEGOTIATE_ALGORITHMS function: 
                    assert(capabilities_checked==1)
&
<3>
imp


length implementation: should be the total message fields excluding the missing part@ 
right, seems to be considered

8. 
English spec:   MeasurementSpecificationSel is a bit mask. The Responder shall select one of the    measurement specifications  supported by the Requester. **Thus, no more than one bit shall be set**. The MeasurementSpecification field of the Measurement block format table defines the values in this field.
explanations:   whenever you receive the ALGORITHMS response, you check MeasurementSpecificationSel value should be power of 2 (less than 256) or 0.
                Imaging if more than 1 bit is set (e.g., 00100010) and actually the 7th bit (calculated fomr the left) is the right bit to set:
                If in implementation, checking is done from MSB to LSB and find the 3th bit (calculated fomr the left) is set, wrong measurements are extracted and derived, which may lead to error. On the other hand, if 3th is the correct bit to set while 7th is not and checking is still done from left to right, 7th measurement is secretly translated from responder to the requester and may be illegally used further
&
ltl p35 {[]! ((MeasurementSpecificationSel!=0x00000000)&&(MeasurementSpecificationSel!=0x00000001)&&\
                (MeasurementSpecificationSel!=0x00000010)&&(MeasurementSpecificationSel!=0x00000100)&&\
                (MeasurementSpecificationSel!=0x00001000)&&(MeasurementSpecificationSel!=0x00010000)&&\
                (MeasurementSpecificationSel!=0x00100000)&&(MeasurementSpecificationSel!=0x01000000)&&\
                (MeasurementSpecificationSel!=0x10000000))};
<4.1>
imp

9. A Responder shall not select both a SPDM-enumerated asymmetric key signature algorithm and an extended asymmetric key signature algorithm. A Responder shall not select both a SPDM-enumerated hashing algorithm and an extended hashing algorithm.
&
ltl px {[]! ((BaseAsymSel != 0) && (ExtAsymSel !=0))}; 
ltl px {[]! ((BaseHashSel != 0) && (ExtHashSel !=0))};
<6>
imp

GET_DIGESTS
10. The cryptographic hash values of each of the certificate chains stored on an endpoint is returned with the DIGESTS response message, such that the Requester can cache the previously retrieved certificate chain hash values to detect any change to the certificate chains stored on the device before issuing the GET_CERTIFICATE request message.
&& hash values
ltl px {[]! ((prev_hash_value!=cur_hash_value))};
implicit<2>
integrity
imp

how to deal with usage of algorithms

11. For the runtime challenge-response flow, the signature field in the CHALLENGE_AUTH response message payload shall be signed by using the device private key over the hash of the message transcript. This ensures cryptographic binding between a specific request message from a specific Requester and a specific response message from a specific Responder and enables the Requester to detect the presence of an active adversary attempting to downgrade cryptographic algorithms or SPDM versions.
&
ltl px {[]! (!(key(hash(message))))};
<2>
imp

12. a Requester-generated nonce protects the challenge-response from replay attacks, whereas a Responder-generated nonce prevents the Responder from signing over arbitrary data that the Requester dictates.
&
ltl px {[]! (Request(nonce)!=Verify(PK, Hash(M2), Signature))};
<1>
imp

13. Certificate chains are stored in locations called slots.Each slot shall either be empty or contain one complete certificate chain. A device shall not contain more than eight slots. Slot 0 is populated by default.
&
ltl px {[]! (len(slot[i])>0 && len(slot[i])<LEN)};
<4>
sec
already hardcodedgit 

Is the slot like caching to the requester/responder@ Right

14. DIGESTS Param2 Slot mask. The bit in position K of this byte shall be set to 1b if and only if slot number K contains a certificate chain for the protocol version in the SPDMVersion field. The number of digests returned shall be equal to the number of bits set in this byte. The digests shall be returned in order of increasing slot number.
&
ltl px {!<> (Param2[k]==1&&Digest[k]==0)};
ltl px {!<> (count_1(Param2)!=count_not0(Digest))};
<6>
imp

15. n in p51 < 8@
&
ltl px {[]! (n>=8)};
<4>
fir

GET_CERTIFICATE
16. The Requester should, at a minimum, save the public key of the leaf certificate and associate it with each of the digests returned by DIGESTS message response. The Requester sends one or more GET_CERTIFICATE requests to retrieve Responder's certificate chain.
sec

17. Param1 1 Slot number of the target certificate chain to read from. The value in this field shall be between 0 and 7 inclusive.
&
ltl px {[]! (Param1<0 && Param1>7)};
<4>
fir

18. Offset in bytes from the start of the certificate chain to where the read request message begins. The Responder should send its certificate chain starting from this offset. For the first GET_CERTIFICATE request, the Requester must set this field to 0. For non-first requests, Offset is the sum of PortionLength values in all previous GET_CERTIFICATE responses.
&
ltl px {[]! (Offset!=sum(PortionLength))};
<4>
sec

For endpoint devices to verify the certificate, the following required fields must be present.
Is this required in modeling SPDM protocol@
fir

We assume the length sent through the request is agreed on with the responder (to have the same length) by the previous NEGOTIATE_ALGORITHMS??

XXX: mutual authentication and leaf certificate

CHALLENGE
19. 
English spec:   In CHALLENGE_AUTH and related phases, typically to check and verify the signature such as:
                Signature = Sign(SK, Hash(M1));
                Verify(PK, Hash(M2), Signature);
&
ltl px {[]! (Verify(PK, Hash(M2), Signature)==False)};
<2>
imp

20. If a response contains ErrorCode=ResponseNotReady : Concatenation function is performed on the contents of both the original request and the response received during RESPOND_IF_READY. If a response contains an ErrorCode other than ResponseNotReady: No concatenation function is performed on the contents of both the original request and response.
<6>
Remained question with the ERROR's return
Other ERROR's request will be the original request
What is the request of ERRORs other than RESPOND_IF_READY@: no request
imp

in-discussion
invalid request: caller wrongly send, resend correct one
busy: send again

possible for MeasurementSummaryHash in CHALLENGE (p56)@ see the contradiction (missing) of this part with phases in p59.
Known measurement or do not care


CHALLENGE and before is verified for integrity, measurement is verified for integrity, but no where verifies the sending-measurement endpoint is the previous CHALLENGE-verified endpoint?
$$
EMAIL
imp

GET_MEASUREMENTS

21. Any communication between Requester and Responder other than a GET_MEASUREMENTS request or response resets L1/L2 computation to null.
&
ltl px {[]! (prev_signal==GET_MEASUREMENTS && signal!=GET_MEASUREMENTS && signal!=ERROR && (L1!=0 ||L2!=0))};
But that won't influence the correct verification since both L1 and L2 are reset?
<3>
imp
busy, not ready
integrity check, buffer flow check, basic package


vendor

KEY_EXCHANGE

22. The specification does not require a specific temporal relationship between the second KEY_EXCHANGE request message and first FINISH response message. However a Responder may generate an ErrorCode=Busy response to second KEY_EXCHANGE request message until first FINISH response message is complete in order to simplify implementation.
sec

<canceled>23. DHE_Named_Group: This field is a duplicate of that found in the NEGOTIATE_ALGORITHMS/ALGORITHMS commands. This is included for early error detection and must be the same algorithm as selected in NEGOTIATE_ALGORITHMS/ALGORITHMS
&
When defining DHE_Named_Group
assert(DHE_Named_Group==ALGORITHMS(error_detection));
<6>

24. ReqSessionID Two-byte Requester contribution to allow construction of a unique four-byte session ID between a Requester-Responder pair. The final session ID = Concatenate (ReqSessionID, RspSessionID).
Otherwise they collide to the same session
One session should be possible to connect with more than 1 key pair?
does every new construction of the session (ID) requires end_session of previous session? If so, didn't see the requirement or in the diagram; if not, then end_session does not bind with ID, but reque-respon pair?
also questions from the last part of the file
&
ltl px {[]! (session_ID=ID_list[-i])}; (1<i<5)
<6>
assert(ID == Concatenate (ReqSessionID, RspSessionID));
imp

MutAuthRequested: Bit 0 – If set, Responder is requesting a Mutual Authentication flow. Requester shall initiate a GET_ENCAPSULATED_REQUEST request; Bit 1 - If set, Responder is requesting a Mutual Authentication flow with implicit GET_DIGESTS request. Requester shall initiate a DELIVER_ENCAPSULATED_RESPONSE request which encapsulates DIGESTS response.
without initiate FINISH first@ when? right after KEY_EXCHANGE?
<6>
fir
next request
flag 3 types
digest, bit 0 1, ->encapsulated, 


FINISH:

25. Param1: Bit 0 – If set, the Signature field is included. This bit shall be set when mutual authentication occurs.
&
ltl px {[]! (Param1[0]==0 && signal==FINISH_RSP)};
<6>
fir

Transcript Hash for KEY_EXCHANGE_RSP signature: why there are some fields of KEY_EXCHANGE_RSP not included (p95)@ Because the corresponding field hashes the previous results
7,8 has overlap?

When multiple session keys are being established between the same Requester and Responder pair, Signature over Transcript HASH during FINISH request is computed using only the corresponding KEY_EXCHANGE, KEY_EXCHANGE_RSP and FINISH request parameters.
How can they know they are the same requester and responder?
$$
imp (multiple session keys scenarios)
first sesssion all calculated
spdm_context is based on connection
(implementation specific)

26. This request message shall complete the handshake between Requester and Responder initiated by a KEY_EXCHANGE request. The purpose of the FINISH request and FINISH_RSP response messages is to provide key confirmation, bind each party's identity to the exchanged keys and protect the entire handshake against manipulation by an active attacker. 
<1>
ltl px {[]! (ResponderVerifyData != caculated_result)};
imp

PSK_EXCHANGE

where is the key exchanged@ they exchange what key (crypto) to use and then self-develop them

27. The RequesterContext is the Requester's contribution to session key derivation. It must contain a random nonce to make sure the derived session keys are ephemeral for this session only to mitigate against replay attacks. It may also contain other information from the Requester.
<1>
imp

28. It should be noted that the nonce in ResponderContext is critical for anti-replay. If a nouce is not present in ResponderContext, then the Responder is not challenging the Requester for real-time knowledge of PSK. Such a session is subject to replay attacks - a man-in-the-middle attacker could record and replay prior PSK_EXCHANGE and PSK_FINISH messages and set up a session with the Responder. But the bogus session would not leak secrets, so long as the PSK or session keys of the prior replayed session are not compromised.
But the information requester derived might be wrong although he thinks it is correct (for the last sentence)@ couldn't be
<1>
sec (same as previous one)

PSK_FINISH
29. If ResponderContext is absent, such as when PSK_CAP in the CAPABILITIES of the Responder is 01b , the Requester shall not send PSK_FINISH , because the session keys are solely determined by the Requester and the Session immediately enters the Application Phase. If and only the ResponderContext is present in the response, such as when PSK_CAP in the CAPABILITIES of the Responder is 10b , the Requester shall send PSK_FINISH with RequesterVerifyData to prove that it has derived correct session keys.
&
<6>
in PSK_FINISH
assert(!session_key_verification||(session_key_verification&&ResponderContext!=0))
imp

HEARTBEAT
30. This request shall keep a session alive if HEARTBEAT is supported by both the Requester and Responder
Param1 in PSK_EXCHANGE_RSP checks responder(p87)
<6>

31. Terminate the session: The Responder shall terminate the session if a HEARTBEAT request is not received in twice HeartbeatPeriod . Likewise, the Requester shall terminate the session if a HEARTBEAT_ACK response or ERROR response is not received in twice HeartbeatPeriod . If an Error with ErrorCode=InvalidSessionID Response is received, the Requester shall terminate the session.
&
<4>
if (heartbeat_count>=2) assert(request_signal==END_SESSION)
imp


KEY_UPDATE
To update session keys when the per-record nonce will soon reach its maximum value and rollover?
sequence number
session id, tag
(document 276)
sessionlimit (implmementation choice)
self-define 
(check implmentation: only has sequence number, -> when reaching limit, send key_update)

32. Until the session key update synchronization successfully completes, subsequent KEY_UPDATE request for the same direction shall be considered a retry of the original KEY_UPDATE request.
ID tag
<6>
imp

<canceled>33. The Session Key Update Synchronization process shall start with the responsibility on the sender to quiesce all application data traffic to the receiver. If UpdateAllKeys is the selected operation, the receiver shall also quiesce all application data traffic to the sender. Once the sender has quiesced the transportation of application data to the receiver, the sender shall, then, send a KEY_UPDATE request with UpdateKey or UpdateAllKeys operation.
&
how about UpdateAllKeys and when should receiver to quiesce@
<3>
ltl px {[]! (request_signal==KEY_UPDATE && Requester_Cache!=0)};
ltl px {[]! (request_signal==KEY_UPDATE && reqiest_signal_operations==UpdateAllKeys && Responder_Cache!=0)};
no requirement for that now?
EMAIL

33. If the sender has not received KEY_UPDATE_ACK , the sender may retry or end the session. The sender shall not proceed to the next step until successfully receiving the corresponding KEY_UPDATE_ACK .
imp


34. After the sender successfully receives the corresponding KEY_UPDATE_ACK , transportation of application data may resume. Also, at this point, the transportation of the application data shall now use the new session keys accordingly.
$$
no where to authenticate the original responder? I can just "help" responder to update the key to violate the integrity?
(time limit, 64bit sequenece -> 30 days -> send key_update)
encrypted messages
&
<6>
imp

35. the transport may allow or disallow the KEY_UPDATE to be sent asynchronously without using the GET_ENCAPSULATED_REQUEST mechanism.
$$ deadlock...
It bears repeating that a key update in one direction can happen simultaneously with a key update in the opposite direction. Still, the aforementioned synchronization process still works and occurs independently but simultaneously for each direction.
How can it be simultaneous?
calculated, mostly requester sent
<6>
sec


GET_ENCAPSULATED_REQUEST
36. If a Responder receives a request other than DELIVER_ENCAPSULATED_RESPONSE or GET_ENCAPSULATED_REQUEST after the Responder already has provided a request to the Requester to which it has not received a response, the Responder shall respond with ErrorCode=RequestInFlight .
&
after GET_ENCAPSULATED_REQUEST
if (request_signal!=GET_ENCAPSULATED_REQUEST && request_signal!=DELIVER_ENCAPSULATED_RESPONSE) assert(signal==ERROR&&error_code==RequestInFlight)
<6>
fir

Normally when will the ENCAPSULATED_REQUEST be raised@ For example, for responder to update key@ Just saw one chance in Mut_Auth_Requested in KEY_EXCHANGE_RSP
the transport and/or physical layer is responsible for defining an alerting mechanism for the Requester. Upon receiving the alert, the Requester shall issue a GET_ENCAPSULATED_REQUEST to the Responder.

37. Param1 of response, Request ID. This field should be unique to help the Responder match response to request.
&
has matched ID from requester side@
DELIVER_ENCAPSULATED_RESPONSE, Param1 Request ID. The Requester shall use the same Request ID as provided by the Responder.
<5>
in DELIVER_ENCAPSULATED_RESPONSE;
assert(DELIVER_ENCAPSULATED_REQUEST.Param1==DELIVER_ENCAPSULATED_RESPONSE.Param1)
fir


ENCAPSULATED_RESPONSE
38. If a Responder receives a request other than DELIVER_ENCAPSULATED_RESPONSE after the Responder already has provided a request to the Requester to which it has not received a response, the Responder shall respond with ErrorCode=RequestInFlight.
&
after DELIVER_ENCAPSULATED_REQUEST
if (request_signal!=DELIVER_ENCAPSULATED_RESPONSE) assert(signal==ERROR&&error_code==RequestInFlight)
<6>
fir


$$ length implementation problems@
remind the length

END_SESSION
39. Further communication between the Requester and Responder using the same session ID shall be prohibited. The Responder shall return ErrorCode=InvalidSession after session termination.
&
after DELIVER_ENCAPSULATED_REQUEST
if (request_signal) assert(signal==ERROR&&error_code==InvalidSession)
<6>
fir

40. If the Responder supports Negotiated State caching ( CAP_CACHE==1 ), the Responder shall preserve the Negotiated State.
&
Does the negotiate state need to be protected in anyway?
not need to be protected
challenge_auth
in NEGOTIATE_ALGORITHMS:
assert(CAP_CACHE!=1||(CAP_CACHE==1&&Negotiate_State!=0))
<3>
imp

handshake
41. If an error occurs in this phase with ErrorCode = DecryptError , the session shall immediately terminate and proceed to session termination.
&
in KEY_EXCHANGE or PSK_EXCHANGE
if (error) assert(request_signal==END_SESSION)
<6>
fir

session termination phase
42. there are no explicit SPDM messages sent or received. When a session terminates, both Requester and Responder shall destroy or clean up all session keys such as derived session secrets, DHE secrets and encryption keys. Requester and Responder may have other internal data tied to this session that they may want to also clean up.
&
after END_SESSION:
assert(states.clear())
<3>
imp

43. the maximum number of simultaneous active sessions shall be a minimum of one. If the KEY_EXCHANGE or PSK_EXCHANGE request will exceed the Responder's maximum number of simultaneous active session, the Responder shall respond with an Errorcode = SessionLimitExceeded .
&
maximum number: implementation specific
ltl px {[]! (active_session_num>1)};
check through every phase:
if (active_session_num>1) assert(signal==ERROR&&error_code==SessionLimitExceeded)
<6>
imp
The sessions are established based on different reque-respon pair or different key_exchange, psk_exchange pair?
If it belongs to the later one, contradict with examples in p88?

What is the associated_data? p131
not 274, 
276: AEAD tag

How the TH1, TH2 hashing messages are chosen?
p126?
If the Requester and Responder used KEY_EXCHANGE/KEY_EXCHANGE_RSP to exchange initial keying information, then
TH1 shall be the output of applying the negotiated hash function to the concatenation of the following:

If the Requester and Responder used KEY_EXCHANGE/KEY_EXCHANGE_RSP to exchange initial keying information, then
TH2 shall be the output of applying the negotiated hash function to the concatenation of the following: