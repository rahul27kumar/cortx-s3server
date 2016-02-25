/*
 * COPYRIGHT 2016 SEAGATE LLC
 *
 * THIS DRAWING/DOCUMENT, ITS SPECIFICATIONS, AND THE DATA CONTAINED
 * HEREIN, ARE THE EXCLUSIVE PROPERTY OF SEAGATE TECHNOLOGY
 * LIMITED, ISSUED IN STRICT CONFIDENCE AND SHALL NOT, WITHOUT
 * THE PRIOR WRITTEN PERMISSION OF SEAGATE TECHNOLOGY LIMITED,
 * BE REPRODUCED, COPIED, OR DISCLOSED TO A THIRD PARTY, OR
 * USED FOR ANY PURPOSE WHATSOEVER, OR STORED IN A RETRIEVAL SYSTEM
 * EXCEPT AS ALLOWED BY THE TERMS OF SEAGATE LICENSES AND AGREEMENTS.
 *
 * YOU SHOULD HAVE RECEIVED A COPY OF SEAGATE'S LICENSE ALONG WITH
 * THIS RELEASE. IF NOT PLEASE CONTACT A SEAGATE REPRESENTATIVE
 * http://www.seagate.com/contact
 *
 * Original author:  Kaustubh Deorukhkar   <kaustubh.deorukhkar@seagate.com>
 * Original creation date: 3-Feb-2016
 */

#pragma once

#ifndef __MERO_FE_S3_SERVER_S3_DELETE_MULTIPLE_OBJECTS_ACTION_H__
#define __MERO_FE_S3_SERVER_S3_DELETE_MULTIPLE_OBJECTS_ACTION_H__

#include <memory>
#include <map>

#include "s3_action_base.h"
#include "s3_bucket_metadata.h"
#include "s3_object_metadata.h"
#include "s3_clovis_writer.h"
#include "s3_clovis_kvs_writer.h"
#include "s3_clovis_kvs_reader.h"
#include "s3_delete_multiple_objects_body.h"
#include "s3_delete_multiple_objects_response_body.h"

class S3DeleteMultipleObjectsAction : public S3Action {
  std::shared_ptr<S3BucketMetadata> bucket_metadata;
  std::vector<std::shared_ptr<S3ObjectMetadata>> objects_metadata;
  std::shared_ptr<S3ClovisWriter> clovis_writer;
  std::shared_ptr<S3ClovisKVSReader> clovis_kv_reader;
  std::shared_ptr<S3ClovisKVSWriter> clovis_kv_writer;

  // index within delete object list
  bool is_request_content_corrupt;
  bool is_request_too_large;
  S3DeleteMultipleObjectsBody delete_request;
  int delete_index;

  S3DeleteMultipleObjectsResponseBody delete_objects_response;

  std::string get_bucket_index_name() {
    return "BUCKET/" + request->get_bucket_name();
  }

public:
  S3DeleteMultipleObjectsAction(std::shared_ptr<S3RequestObject> req);

  // Helpers
  void setup_steps();

  void validate_request();
  void validate_request_body(std::string content);
  void consume_incoming_content();
  void validate_request_body();

  void fetch_bucket_info();
  void fetch_bucket_info_failed();
  void fetch_objects_info();
  void fetch_objects_info_failed();

  void delete_objects();
  void delete_objects_successful();
  void delete_objects_failed();

  void delete_objects_metadata();
  void delete_objects_metadata_successful();
  void delete_objects_metadata_failed();

  void send_response_to_s3_client();
};

#endif