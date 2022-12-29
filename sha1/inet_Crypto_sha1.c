//
// Copyright (c) 2008 Tridium, Inc.
// Licensed under the Academic Free License version 3.0
//
// History:
//   06 Mar 08  Brian Frank  Creation
//

#include "inet_sha1.h"

//
// This implementation of Crypt.sha1 uses the code straight
// from RFC 3174:
//
//    http://tools.ietf.org/html/rfc3174
//
// static void sha1(byte[] input, int inputOff, int len, byte[] output, int outputOff)
//
void sedona_sha1(char* input, int inputOff, int len, char* output, int outputOff)
{
  SHA1Context cx;

  input  = input + inputOff;
  output = output + outputOff;

  SHA1Reset(&cx);
  SHA1Input(&cx, input, len);
  SHA1Result(&cx, output);

//  return nullCell;
}
