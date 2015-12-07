/******************************************************************************
Copyright (c) 2010, Google Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the <ORGANIZATION> nor the names of its contributors 
    may be used to endorse or promote products derived from this software 
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#pragma once
class DataChunk;
class SocketInfo;
class TrackSockets;

typedef enum {
  SSL_IN,
  SSL_OUT
} SSL_DATA_DIRECTION;

class SSLStream {
public:
  SSLStream(TrackSockets &sockets, SocketInfo *socket_info, SSL_DATA_DIRECTION direction);
  ~SSLStream();
  void Append(const DataChunk& chunk);
  CStringA          client_random_;       // 32-byte client random in hex format (includes the time)
  unsigned __int16  cipher_suite_;
  __int8            compression_;

private:
  void OutputDebugStringA(CStringA message);
  void PrecessMessage();
  CStringA GetCipherSuiteName(unsigned __int16 cipher_suite);

  // Different SSL message types
  void ProcessHandshake();
  void ProcessApplicationData();
  void ProcessChangeCipherSpec();
  void ProcessAlert();

  // SSL Handshake processing
  void HandshakeHelloRequest();
  void HandshakeClientHello();
  void HandshakeServerHello();
  void HandshakeCertificate();
  void HandshakeServerKeyExchange();
  void HandshakeCertificateRequest();
  void HandshakeServerDone();
  void HandshakeCertificateVerify();
  void HandshakeClientKeyExchange();
  void HandshakeFinished();

  unsigned char message_[65540]; // 65535 max TLS message length + 5 byte header
  __int32 message_size_;         // Total size of the current message once filled (-1 for not set)
  __int32 message_len_;          // Current size of accumulated message
  SSL_DATA_DIRECTION direction_;
  TrackSockets &sockets_;
  SocketInfo * socket_info_;
};
