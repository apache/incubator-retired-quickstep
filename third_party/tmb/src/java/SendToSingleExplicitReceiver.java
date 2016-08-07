/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

package tmb.voltdb;

import org.voltdb.*;

public class SendToSingleExplicitReceiver extends VoltProcedure {
  public final SQLStmt checkConnectedSQL = new SQLStmt(
      "SELECT client_id FROM client WHERE client_id = ? "
                                 + "AND disconnect_time IS NULL;");
  public final SQLStmt checkSendableSQL = new SQLStmt(
      "SELECT client_id FROM sendable WHERE client_id = ? "
                                   + "AND type_id = ?;");
  public final SQLStmt checkReceivableSQL = new SQLStmt(
      "SELECT client_id FROM receivable WHERE client_id = ? "
                                     + "AND type_id = ?;");
  public final SQLStmt insertQueuedMessage = new SQLStmt(
      "INSERT INTO queued_message VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?);");

  private static final org.voltdb.types.TimestampType MAX_TIMESTAMP
      = new org.voltdb.types.TimestampType(Long.MAX_VALUE);

  public long run(int sender_id,
                  int receiver_id,
                  org.voltdb.types.TimestampType expiration_time,
                  short priority,
                  int type_id,
                  byte[] payload) throws VoltAbortException {
    voltQueueSQL(checkConnectedSQL, sender_id);
    voltQueueSQL(checkSendableSQL, sender_id, type_id);
    voltQueueSQL(checkConnectedSQL, receiver_id);
    voltQueueSQL(checkReceivableSQL, receiver_id, type_id);
    VoltTable[] check_result = voltExecuteSQL();

    if (check_result[0].getRowCount() != 1) {
      return 2;  // kSenderNotConnected
    }
    if (check_result[1].getRowCount() != 1) {
      return 3;  // kSenderNotRegisteredForMessageType
    }
    if (check_result[2].getRowCount() != 1) {
      return 1;  // kNoReceivers
    }
    if (check_result[3].getRowCount() != 1) {
      return 4;  // kReceiverNotRegisteredForMessageType
    }

    org.voltdb.types.TimestampType actual_expiration_time
        = (expiration_time == null
           || expiration_time.getTime() == VoltType.NULL_BIGINT)
               ? MAX_TIMESTAMP : expiration_time;

    voltQueueSQL(insertQueuedMessage,
                 receiver_id,
                 getUniqueId(),
                 sender_id,
                 getTransactionTime(),
                 actual_expiration_time,
                 priority,
                 type_id,
                 payload,
                 payload.length);
    voltExecuteSQL();

    return 0;
  }
}
