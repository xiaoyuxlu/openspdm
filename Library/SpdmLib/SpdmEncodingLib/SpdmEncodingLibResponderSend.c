/** @file
  SPDM transport library.
  It follows the SPDM Specification.

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpdmEncodingLibInternal.h"

RETURN_STATUS
SpdmEncryptResponse (
  IN     VOID                 *SpdmContext,
  IN     UINT32               SessionId,
  IN     UINTN                ResponseSize,
  IN     VOID                 *Response,
  IN OUT UINTN                *EncResponseSize,
     OUT VOID                 *EncResponse
  )
{
  UINT8                          *WrappedResponse;
  UINTN                          WrappedResponseSize;
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
  AEAD_ENCRYPT                   AeadEncFunc;
  BOOLEAN                        Result;
  VOID                           *Key;
  UINT8                          Salt[MAX_AEAD_IV_SIZE];
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
    Key = SessionInfo->HandshakeSecret.ResponseHandshakeEncryptionKey;
    CopyMem (Salt, SessionInfo->HandshakeSecret.ResponseHandshakeSalt, SessionInfo->AeadIvSize);
    *(UINT64 *)Salt = *(UINT64 *)Salt ^ SessionInfo->HandshakeSecret.ResponseHandshakeSequenceNumber;
    SessionInfo->HandshakeSecret.ResponseHandshakeSequenceNumber ++;
    break;
  case SpdmStateEstablished:
    Key = SessionInfo->ApplicationSecret.ResponseDataEncryptionKey;
    CopyMem (Salt, SessionInfo->ApplicationSecret.ResponseDataSalt, SessionInfo->AeadIvSize);
    *(UINT64 *)Salt = *(UINT64 *)Salt ^ SessionInfo->ApplicationSecret.ResponseDataSequenceNumber;
    SessionInfo->ApplicationSecret.ResponseDataSequenceNumber ++;
    break;
  default:
    ASSERT(FALSE);
    return RETURN_UNSUPPORTED;
    break;
  }

  AeadEncFunc = GetSpdmAeadEncFunc (SpdmContext);
  AeadBlockSize = GetSpdmAeadBlockSize (SpdmContext);
  AeadTagSize = GetSpdmAeadTagSize (SpdmContext);
  PlainTextSize = sizeof(MCTP_MESSAGE_CIPHERTEXT_HEADER) + ResponseSize;
  CipherTextSize = (PlainTextSize + AeadBlockSize - 1) / AeadBlockSize * AeadBlockSize;
  WrappedResponseSize = sizeof(MCTP_MESSAGE_PLAINTEXT_HEADER) + CipherTextSize + AeadTagSize;
  if ((Alignment > 1) && 
      ((WrappedResponseSize & (Alignment - 1)) != 0)) {
    WrappedResponseSize = (WrappedResponseSize + (Alignment - 1)) & ~(Alignment - 1);
  }

  ASSERT (*EncResponseSize >= WrappedResponseSize);
  if (*EncResponseSize < WrappedResponseSize) {
    *EncResponseSize = WrappedResponseSize;
    return RETURN_BUFFER_TOO_SMALL;
  }
  *EncResponseSize = WrappedResponseSize;
  WrappedResponse = EncResponse;
  RecordHeader = (VOID *)WrappedResponse;
  RecordHeader->SessionId = SessionId;
  RecordHeader->Length = (UINT16)WrappedResponseSize;
  EncMsgHeader = (VOID *)(RecordHeader + 1);
  EncMsgHeader->TrueLength = (UINT16)ResponseSize;
  EncMsgHeader->EncapsulatedMessageType.MessageType = MCTP_MESSAGE_TYPE_SPDM;
  CopyMem (EncMsgHeader + 1, Response, ResponseSize);
  AData = (UINT8 *)RecordHeader;
  EncMsg = (UINT8 *)EncMsgHeader;
  DecMsg = (UINT8 *)EncMsgHeader;
  Tag = WrappedResponse + sizeof(MCTP_MESSAGE_PLAINTEXT_HEADER) + CipherTextSize;
  Result = AeadEncFunc (
             Key,
             SessionInfo->AeadKeySize,
             Salt,
             SessionInfo->AeadIvSize,
             (UINT8 *)AData,
             sizeof(MCTP_MESSAGE_PLAINTEXT_HEADER),
             DecMsg,
             CipherTextSize,
             Tag,
             AeadTagSize,
             EncMsg,
             &CipherTextSize
             );
  if (!Result) {
    return RETURN_OUT_OF_RESOURCES;
  }
  return RETURN_SUCCESS;
}

RETURN_STATUS
SpdmEncodeResponseSession (
  IN     VOID                 *SpdmContext,
  IN     UINT32               SessionId,
  IN     UINTN                ResponseSize,
  IN     VOID                 *Response,
  IN OUT UINTN                *MessageSize,
     OUT VOID                 *Message
  )
{
  RETURN_STATUS                      Status;
  SPDM_SESSION_INFO                  *SessionInfo;

  SessionInfo = SpdmGetSessionInfoViaSessionId (SpdmContext, SessionId);
  if (SessionInfo == NULL) {
    ASSERT (FALSE);
    return RETURN_UNSUPPORTED;
  }

  switch (SessionInfo->SessionState) {
  case SpdmStateHandshaking:
  case SpdmStateEstablished:
    ZeroMem (Message, *MessageSize);
    Status = SpdmEncryptResponse (SpdmContext, SessionId, ResponseSize, Response, MessageSize, Message);
    if (RETURN_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "SpdmEncSendResponse - %p\n", Status));
    }
    return Status;
  default:
    ASSERT (FALSE);
    return RETURN_OUT_OF_RESOURCES;
  }
}

RETURN_STATUS
EFIAPI
SpdmEncodeResponse (
  IN     VOID                 *SpdmContext,
  IN     UINT32               *SessionId,
  IN     UINTN                ResponseSize,
  IN     VOID                 *Response,
  IN OUT UINTN                *MessageSize,
     OUT VOID                 *Message
  )
{
  UINTN                          CalcResponseSize;
  UINT32                         Alignment;

  Alignment = SpdmGetAlignment (SpdmContext);

  if (SessionId != NULL) {
    return SpdmEncodeResponseSession (SpdmContext, *SessionId, ResponseSize, Response, MessageSize, Message);
  }

  if (Alignment > 1) {
    CalcResponseSize = (ResponseSize + (Alignment - 1)) & ~(Alignment - 1);
  } else {
    CalcResponseSize = ResponseSize;
  }

  ASSERT (*MessageSize >= CalcResponseSize);
  if (*MessageSize < CalcResponseSize) {
    *MessageSize = CalcResponseSize;
    return RETURN_BUFFER_TOO_SMALL;
  }
  *MessageSize = CalcResponseSize;
  CopyMem (Message, Response, ResponseSize);
  ZeroMem ((UINT8 *)Message + ResponseSize, CalcResponseSize - ResponseSize);

  return RETURN_SUCCESS;
}

