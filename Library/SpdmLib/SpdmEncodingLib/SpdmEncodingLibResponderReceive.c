/** @file
  SPDM transport library.
  It follows the SPDM Specification.

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpdmEncodingLibInternal.h"

RETURN_STATUS
SpdmDecryptRequest (
  IN     VOID                 *SpdmContext,
  IN     UINT32               SessionId,
  IN     UINTN                RequestSize,
  IN     VOID                 *Request,
  IN OUT UINTN                *DecRequestSize,
     OUT VOID                 *DecRequest
  )
{
  UINTN                          PlainTextSize;
  UINTN                          CipherTextSize;
  UINTN                          AeadBlockSize;
  UINTN                          AeadTagSize;
  UINT8                          *AData;
  UINT8                          *EncMsg;
  UINT8                          *DecMsg;
  UINT8                          *Tag;
  MCTP_MESSAGE_PLAINTEXT_HEADER  *RecordHeader;
  MCTP_MESSAGE_CIPHERTEXT_HEADER *EncMsgHeader;
  AEAD_DECRYPT                   AeadDecFunc;
  BOOLEAN                        Result;
  VOID                           *Key;
  UINT8                          Salt[MAX_AEAD_IV_SIZE];
  SPDM_SESSION_INFO              *SessionInfo;

  SessionInfo = SpdmGetSessionInfoViaSessionId (SpdmContext, SessionId);
  if (SessionInfo == NULL) {
    ASSERT (FALSE);
    return RETURN_UNSUPPORTED;
  }

  switch (SessionInfo->SessionState) {
  case SpdmStateHandshaking:
    Key = SessionInfo->HandshakeSecret.RequestHandshakeEncryptionKey;
    CopyMem (Salt, SessionInfo->HandshakeSecret.RequestHandshakeSalt, SessionInfo->AeadIvSize);
    *(UINT64 *)Salt = *(UINT64 *)Salt ^ SessionInfo->HandshakeSecret.RequestHandshakeSequenceNumber;
    SessionInfo->HandshakeSecret.RequestHandshakeSequenceNumber ++;
    break;
  case SpdmStateEstablished:
    Key = SessionInfo->ApplicationSecret.RequestDataEncryptionKey;
    CopyMem (Salt, SessionInfo->ApplicationSecret.RequestDataSalt, SessionInfo->AeadIvSize);
    *(UINT64 *)Salt = *(UINT64 *)Salt ^ SessionInfo->ApplicationSecret.RequestDataSequenceNumber;
    SessionInfo->ApplicationSecret.RequestDataSequenceNumber ++;
    break;
  default:
    ASSERT(FALSE);
    return RETURN_UNSUPPORTED;
    break;
  }
  
  AeadDecFunc = GetSpdmAeadDecFunc (SpdmContext);
  AeadBlockSize = GetSpdmAeadBlockSize (SpdmContext);
  AeadTagSize = GetSpdmAeadTagSize (SpdmContext);

  if (RequestSize < sizeof(MCTP_MESSAGE_PLAINTEXT_HEADER) + AeadBlockSize + AeadTagSize) {
    return RETURN_DEVICE_ERROR;
  }
  RecordHeader = (VOID *)Request;
  if (RecordHeader->SessionId != SessionId) {
    return RETURN_DEVICE_ERROR;
  }
  if (RecordHeader->Length != RequestSize) {
    return RETURN_DEVICE_ERROR;
  }
  CipherTextSize = (RequestSize - sizeof(MCTP_MESSAGE_PLAINTEXT_HEADER) - AeadTagSize) / AeadBlockSize * AeadBlockSize;
  RequestSize = CipherTextSize + sizeof(MCTP_MESSAGE_PLAINTEXT_HEADER) + AeadTagSize;
  EncMsgHeader = (VOID *)(RecordHeader + 1);
  AData = (UINT8 *)RecordHeader;
  EncMsg = (UINT8 *)EncMsgHeader;
  DecMsg = (UINT8 *)EncMsgHeader;
  Tag = (UINT8 *)Request + RequestSize - AeadTagSize;
  Result = AeadDecFunc (
             Key,
             SessionInfo->AeadKeySize,
             Salt,
             SessionInfo->AeadIvSize,
             (UINT8 *)AData,
             sizeof(MCTP_MESSAGE_PLAINTEXT_HEADER),
             EncMsg,
             CipherTextSize,
             Tag,
             AeadTagSize,
             DecMsg,
             &CipherTextSize
             );
  if (!Result) {
    return RETURN_DEVICE_ERROR;
  }
  PlainTextSize = EncMsgHeader->TrueLength;
  if (PlainTextSize > CipherTextSize) {
    return RETURN_DEVICE_ERROR;      
  }
  if (EncMsgHeader->EncapsulatedMessageType.MessageType != MCTP_MESSAGE_TYPE_SPDM) {
    return RETURN_DEVICE_ERROR;
  }

  ASSERT (*DecRequestSize >= PlainTextSize);
  if (*DecRequestSize < PlainTextSize) {
    *DecRequestSize = PlainTextSize;
    return RETURN_BUFFER_TOO_SMALL;
  }
  *DecRequestSize = PlainTextSize;
  CopyMem (DecRequest, EncMsgHeader + 1, PlainTextSize);

  return RETURN_SUCCESS;
}

RETURN_STATUS
SpdmDecodeRequestSession (
  IN     VOID                 *SpdmContext,
  IN     UINT32               SessionId,
  IN     UINTN                MessageSize,
  IN     VOID                 *Message,
  IN OUT UINTN                *RequestSize,
     OUT VOID                 *Request
  )
{
  RETURN_STATUS                  Status;
  SPDM_SESSION_INFO              *SessionInfo;
  UINT32                         Alignment;

  SessionInfo = SpdmGetSessionInfoViaSessionId (SpdmContext, SessionId);
  if (SessionInfo == NULL) {
    ASSERT (FALSE);
    return RETURN_UNSUPPORTED;
  }

  Alignment = SpdmGetAlignment (SpdmContext);

  switch (SessionInfo->SessionState) {
  case SpdmStateHandshaking:
  case SpdmStateEstablished:
    if (Alignment > 1) {
      ASSERT ((MessageSize & (Alignment - 1)) == 0);
    }

    Status = SpdmDecryptRequest (SpdmContext, SessionId, MessageSize, Message, RequestSize, Request);
    if (RETURN_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "SpdmDecReceiveRequest - %p\n", Status));
    }
    return Status;
  default:
    ASSERT (FALSE);
    return RETURN_OUT_OF_RESOURCES;
  }
}

RETURN_STATUS
EFIAPI
SpdmDecodeRequest (
  IN     VOID                 *SpdmContext,
  IN     UINT32               *SessionId,
  IN     UINTN                MessageSize,
  IN     VOID                 *Message,
  IN OUT UINTN                *RequestSize,
     OUT VOID                 *Request
  )
{
  UINT32                         Alignment;

  Alignment = SpdmGetAlignment (SpdmContext);

  if (SessionId != NULL) {
    return SpdmDecodeRequestSession (SpdmContext, *SessionId, MessageSize, Message, RequestSize, Request);
  }

  if (Alignment > 1) {
    ASSERT ((MessageSize & (Alignment - 1)) == 0);
  }

  ASSERT (*RequestSize >= MessageSize);
  if (*RequestSize < MessageSize) {
    *RequestSize = MessageSize;
    return RETURN_BUFFER_TOO_SMALL;
  }
  *RequestSize = MessageSize;
  CopyMem (Request, Message, MessageSize);

  return RETURN_SUCCESS;
}

