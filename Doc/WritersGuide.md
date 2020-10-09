# SPDM requester and responder writers guide

## SPDM requester writers guide

1. Initialize SPDM context

   1.1, allocate buffer for the SpdmContext and initialize it.

   ```
   SpdmContext = (VOID *)malloc (SpdmGetContextSize());
   SpdmInitContext (SpdmContext);
   ```

   1.2, register the device io functions and transport layer functions.
   The openspdm provides the default transport layer MCTP encode/decode function.
   The SPDM device driver need provide device send/receive function.

   ```
   SpdmRegisterDeviceIoFunc (SpdmContext, SpdmDeviceSendMessage, SpdmDeviceReceiveMessage);
   SpdmRegisterTransportLayerFunc (SpdmContext, SpdmTransportMctpEncodeMessage, SpdmTransportMctpDecodeMessage);
   ```

   1.3, if responder verification is required, deploy the peer public root hash.
   ```
   Parameter.Location = SpdmDataLocationLocal;
   SpdmSetData (SpdmContext, SpdmDataPeerPublicRootCertHash, &Parameter, PeerRootCertHash, PeerRootCertHashSize);
   ```

   1.4, if mutual authentication is supported, deploy slot number, public certificate chain and measurement, then register signing function.
   ```
   Parameter.Location = SpdmDataLocationLocal;
   SpdmSetData (SpdmContext, SpdmDataSlotCount, &Parameter, &SlotNumber, sizeof(SlotNumber));

   Parameter.AdditionalData[0] = SlotIndex;
   SpdmSetData (SpdmContext, SpdmDataPublicCertChains, &Parameter, MyPublicCertChains, MyPublicCertChainsSize);

   Parameter.AdditionalData[0] = MeasurementCount;
   SpdmSetData (SpdmContext, SpdmDataMeasurementRecord, &Parameter, Measurement, MeasurementSize);

   SpdmRegisterDataSignFunc (SpdmContext, SpdmDataSignFunc);
   ```

   1.5, set capabilities and choose algorithms, based upon need.
   ```
   Parameter.Location = SpdmDataLocationLocal;
   SpdmSetData (SpdmContext, SpdmDataCapabilityCTExponent, &Parameter, &CTExponent, sizeof(CTExponent));
   SpdmSetData (SpdmContext, SpdmDataCapabilityFlags, &Parameter, &CapFlags, sizeof(CapFlags));

   SpdmSetData (SpdmContext, SpdmDataBaseAsymAlgo, &Parameter, &BaseAsymAlgo, sizeof(BaseAsymAlgo));
   SpdmSetData (SpdmContext, SpdmDataBaseHashAlgo, &Parameter, &BaseHashAlgo, sizeof(BaseHashAlgo));
   SpdmSetData (SpdmContext, SpdmDataDHENamedGroup, &Parameter, &DHENamedGroup, sizeof(DHENamedGroup));
   SpdmSetData (SpdmContext, SpdmDataAEADCipherSuite, &Parameter, &AEADCipherSuite, sizeof(AEADCipherSuite));
   SpdmSetData (SpdmContext, SpdmDataReqBaseAsymAlg, &Parameter, &ReqBaseAsymAlg, sizeof(ReqBaseAsymAlg));
   SpdmSetData (SpdmContext, SpdmDataKeySchedule, &Parameter, &KeySchedule, sizeof(KeySchedule));
   ```

   1.6, if PSK is required, deploy PSK data.
   ```
   SpdmSetData (SpdmContext, SpdmDataPsk, NULL, PskData, PskDataSize);
   SpdmSetData (SpdmContext, SpdmDataPskHint, NULL, PskHint, PskHintSize);
   ```

2. Create connection with the responder

   Send GET_VERSION, GET_CAPABILITIES and NEGOTIATE_ALGORITHM.
   ```
   SpdmInitConnection (SpdmContext);
   ```

3. Authentication the responder

   Send GET_DIGESTES, GET_CERTIFICATES and CHALLENGE.
   ```
   SpdmGetDigest (SpdmContext, SlotMask, TotalDigestBuffer);
   SpdmGetCertificate (SpdmContext, SlotNum, CertChainSize, CertChain);
   SpdmChallenge (SpdmContext, SlotNum, MeasurementHashType, MeasurementHash);
   ```

4. Get the measurement from the responder

   4.1, Send GET_MEASUREMENT to query the total number of measurements available.
   ```
   SpdmGetMeasurement (
       SpdmContext,
       SPDM_GET_MEASUREMENTS_REQUEST_ATTRIBUTES_GENERATE_SIGNATURE,
       SPDM_GET_MEASUREMENTS_REQUEST_MEASUREMENT_OPERATION_TOTOAL_NUMBER_OF_MEASUREMENTS,
       SlotNum,
       &NumberOfBlocks,
       NULL,
       NULL
       );
   ```

   4.2, Send GET_MEASUREMENT to get measurement one by one.
   ```
   for (Index = 1; Index <= NumberOfBlocks; Index++) {
     SpdmGetMeasurement (
       SpdmContext,
       SPDM_GET_MEASUREMENTS_REQUEST_ATTRIBUTES_GENERATE_SIGNATURE,
       Index,
       SlotNum,
       &NumberOfBlock,
       &MeasurementRecordLength,
       MeasurementRecord
       );
   }
   ```

5. Use an SPDM session

   5.1, Without PSK, send KEY_EXCHANGE/FINISH to create a session.
   ```
   SpdmStartSession (
       SpdmContext,
       FALSE, // KeyExchange
       SPDM_CHALLENGE_REQUEST_TCB_COMPONENT_MEASUREMENT_HASH,
       SlotNum,
       &SessionId,
       &HeartbeatPeriod,
       MeasurementHash
       );
   ```

   Or with PSK, send PSK_EXCHANGE/PSK_FINISH to create a session.
   ```
   SpdmStartSession (
       SpdmContext,
       TRUE, // KeyExchange
       SPDM_CHALLENGE_REQUEST_TCB_COMPONENT_MEASUREMENT_HASH,
       SlotNum,
       &SessionId,
       &HeartbeatPeriod,
       MeasurementHash
       );
   ```

   5.2, Use the SPDM session to communicate with the responder.
   ```
   SpdmSendReceiveData (SpdmContext, &SessionId, &Request, RequestSize, &Response, &ResponseSize);
   ```

   5.3, Send END_SESSION to close the session.
   ```
   SpdmStopSession (SpdmContext, SessionId, EndSessionAttributes);
   ```

   5.4, Send HEARTBEAT, when it is required.
   ```
   SpdmHeartbeat (SpdmContext, SessionId);
   ```

   5.5, Send KEY_UPDATE, when it is required.
   ```
   SpdmKeyUpdate (SpdmContext, SessionId, SingleDirection);
   ```

## SPDM responder writers guide

1. Initialize SPDM context (similar to SPDM requester)

   1.1, allocate buffer for the SpdmContext and initialize it.

   ```
   SpdmContext = (VOID *)malloc (SpdmGetContextSize());
   SpdmInitContext (SpdmContext);
   ```

   1.2, register the device io functions and transport layer functions.
   The openspdm provides the default transport layer MCTP encode/decode function.
   The SPDM device driver need provide device send/receive function.

   ```
   SpdmRegisterDeviceIoFunc (SpdmContext, SpdmDeviceSendMessage, SpdmDeviceReceiveMessage);
   SpdmRegisterTransportLayerFunc (SpdmContext, SpdmTransportMctpEncodeMessage, SpdmTransportMctpDecodeMessage);
   ```

   1.3, if mutual authentication (requester verification) is required, deploy the peer public root hash.
   ```
   Parameter.Location = SpdmDataLocationLocal;
   SpdmSetData (SpdmContext, SpdmDataPeerPublicRootCertHash, &Parameter, PeerRootCertHash, PeerRootCertHashSize);
   ```

   1.4, deploy slot number, public certificate chain and measurement, then register signing function.
   ```
   Parameter.Location = SpdmDataLocationLocal;
   SpdmSetData (SpdmContext, SpdmDataSlotCount, &Parameter, &SlotNumber, sizeof(SlotNumber));

   Parameter.AdditionalData[0] = SlotIndex;
   SpdmSetData (SpdmContext, SpdmDataPublicCertChains, &Parameter, MyPublicCertChains, MyPublicCertChainsSize);

   Parameter.AdditionalData[0] = MeasurementCount;
   SpdmSetData (SpdmContext, SpdmDataMeasurementRecord, &Parameter, Measurement, MeasurementSize);

   SpdmRegisterDataSignFunc (SpdmContext, SpdmDataSignFunc);
   ```

   1.5, set capabilities and choose algorithms, based upon need.
   ```
   Parameter.Location = SpdmDataLocationLocal;
   SpdmSetData (SpdmContext, SpdmDataCapabilityCTExponent, &Parameter, &CTExponent, sizeof(CTExponent));
   SpdmSetData (SpdmContext, SpdmDataCapabilityFlags, &Parameter, &CapFlags, sizeof(CapFlags));

   SpdmSetData (SpdmContext, SpdmDataBaseAsymAlgo, &Parameter, &BaseAsymAlgo, sizeof(BaseAsymAlgo));
   SpdmSetData (SpdmContext, SpdmDataBaseHashAlgo, &Parameter, &BaseHashAlgo, sizeof(BaseHashAlgo));
   SpdmSetData (SpdmContext, SpdmDataDHENamedGroup, &Parameter, &DHENamedGroup, sizeof(DHENamedGroup));
   SpdmSetData (SpdmContext, SpdmDataAEADCipherSuite, &Parameter, &AEADCipherSuite, sizeof(AEADCipherSuite));
   SpdmSetData (SpdmContext, SpdmDataReqBaseAsymAlg, &Parameter, &ReqBaseAsymAlg, sizeof(ReqBaseAsymAlg));
   SpdmSetData (SpdmContext, SpdmDataKeySchedule, &Parameter, &KeySchedule, sizeof(KeySchedule));
   ```

   1.6, if PSK is required, deploy PSK data.
   ```
   SpdmSetData (SpdmContext, SpdmDataPsk, NULL, PskData, PskDataSize);
   SpdmSetData (SpdmContext, SpdmDataPskHint, NULL, PskHint, PskHintSize);
   ```

2. Register vendor defined request response.

   ```
   SpdmRegisterGetResponseFunc (SpdmContext, SpdmGetResponseVendorDefinedRequest);
   ```

3. Dispatch SPDM messages.

   ```
   while (TRUE) {
     Status = SpdmResponderDispatchMessage (mSpdmContext);
     if (Status != RETURN_UNSUPPORTED) {
       continue;
     }
     // handle non SPDM message
     ......
   }
   ```