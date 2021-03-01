/*
 * Copyright (c) 2020 Seagate Technology LLC and/or its Affiliates
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For any questions about this software or licensing,
 * please email opensource@seagate.com or cortx-questions@seagate.com.
 *
 */

#include <cstring>

#include "s3_url_encode.h"

void escape_char(char ch, std::string& destination) {
  char buf[16];
  snprintf(buf, sizeof(buf), "%%%.2X", (int)(unsigned char)ch);
  destination.append(buf);
}

bool char_needs_url_encoding(char c) {
  if (c <= 0x20 || c >= 0x7f) return true;

  switch (c) {
    case 0x22:
    case 0x23:
    case 0x25:
    case 0x26:
    case 0x2B:
    case 0x2C:
    case 0x2F:
    case 0x3A:
    case 0x3B:
    case 0x3C:
    case 0x3E:
    case 0x3D:
    case 0x3F:
    case 0x40:
    case 0x5B:
    case 0x5D:
    case 0x5C:
    case 0x5E:
    case 0x60:
    case 0x7B:
    case 0x7D:
      return true;
  }
  return false;
}

std::string url_encode(const char* src) {
  if (src == NULL) {
    return "";
  }
  std::string encoded_string = "";
  size_t len = strlen(src);
  for (size_t i = 0; i < len; i++, src++) {
    if (char_needs_url_encoding(*src)) {
      escape_char(*src, encoded_string);
    } else {
      encoded_string.append(src, 1);
    }
  }
  return encoded_string;
}

void remove_escape_char(const char* str, std::string& destination) {
  char buf[3] = "\0";
  unsigned int code = 0;
  snprintf(buf, sizeof(buf), "%s", str + 1);
  sscanf(buf, "%x", &code);
  destination.append(1, (char)code);
  /*
  if (strcmp(buf, "2F") == 0) {
    destination.append(1, '/');
  } else if (strcmp(buf, "3A") == 0) {
    destination.append(1, ':');
  } else {
    destination.append(1, '-');
  }
  */
}

bool is_url_encoded_char(char c) {
  if (c == '%') {
    return true;
  }
  return false;
}

std::string url_decode(const char* src) {
  if (src == NULL) {
    return "";
  }
  std::string decoded_string = "";
  size_t len = strlen(src);
  size_t decoded_len = 0;
  for (size_t i = 0; i < len; i++, src++) {
    if (is_url_encoded_char(*src)) {
      remove_escape_char(src, decoded_string);
      src += 2;
      i += 2;
    } else {
      decoded_string.append(src, 1);
    }
    decoded_len++;
  }
  decoded_string[decoded_len] = '\0';
  return decoded_string;
}
