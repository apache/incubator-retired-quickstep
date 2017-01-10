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

public class SendToAny extends VoltProcedure {
  public final SQLStmt checkConnectedSQL = new SQLStmt(
      "SELECT client_id FROM client WHERE client_id = ? "
                                 + "AND disconnect_time IS NULL;");
  public final SQLStmt checkSendableSQL = new SQLStmt(
      "SELECT client_id FROM sendable WHERE client_id = ? "
                                   + "AND type_id = ?;");
  public final SQLStmt getReceivableSQL = new SQLStmt(
      "SELECT client_id FROM receivable WHERE type_id = ? "
                                     + "ORDER BY client_id;");
  public final SQLStmt insertQueuedMessage = new SQLStmt(
      "INSERT INTO queued_message VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?);");
  public final SQLStmt insertCancellableMessage = new SQLStmt(
      "INSERT INTO cancellable_message VALUES(?);");

  private static final org.voltdb.types.TimestampType MAX_TIMESTAMP
      = new org.voltdb.types.TimestampType(Long.MAX_VALUE);

  public VoltTable run(int sender_id,
                       org.voltdb.types.TimestampType expiration_time,
                       short priority,
                       int type_id,
                       byte[] payload,
                       byte broadcast,
                       byte cancellable) throws VoltAbortException {
    VoltTable retval = new VoltTable(
        new VoltTable.ColumnInfo("SendStatus", VoltType.BIGINT),
        new VoltTable.ColumnInfo("message_id", VoltType.BIGINT));

    voltQueueSQL(checkConnectedSQL, sender_id);
    voltQueueSQL(checkSendableSQL, sender_id, type_id);
    voltQueueSQL(getReceivableSQL, type_id);
    VoltTable[] check_result = voltExecuteSQL();

    if (check_result[0].getRowCount() != 1) {
      retval.addRow(2, -1);
      return retval;  // kSenderNotConnected
    }
    if (check_result[1].getRowCount() != 1) {
      retval.addRow(3, -1);
      return retval;  // kSenderNotRegisteredForMessageType
    }

    long receivers_connected = check_result[2].getRowCount();
    if (receivers_connected == 0) {
      retval.addRow(1, -1);
      return retval;  // kNoReceivers
    }

    org.voltdb.types.TimestampType actual_expiration_time
        = (expiration_time == null
           || expiration_time.getTime() == VoltType.NULL_BIGINT)
               ? MAX_TIMESTAMP : expiration_time;

    if (broadcast != 0) {
      VoltTableRow receiver_row = check_result[2].fetchRow(0);
      receiver_row.resetRowPosition();
      while (receiver_row.advanceRow()) {
        voltQueueSQL(insertQueuedMessage,
                     receiver_row.getLong(0),
                     getUniqueId(),
                     sender_id,
                     getTransactionTime(),
                     actual_expiration_time,
                     priority,
                     type_id,
                     payload,
                     payload.length);
      }
    } else {
      // Select one receiver at random.
      int receiver_pos = getSeededRandomNumberGenerator().nextInt(
          check_result[2].getRowCount());
      VoltTableRow receiver_row = check_result[2].fetchRow(receiver_pos);
      voltQueueSQL(insertQueuedMessage,
                   receiver_row.getLong(0),
                   getUniqueId(),
                   sender_id,
                   getTransactionTime(),
                   actual_expiration_time,
                   priority,
                   type_id,
                   payload,
                   payload.length);
    }

    if (cancellable != 0) {
      voltQueueSQL(insertCancellableMessage, getUniqueId());
    }

    voltExecuteSQL();

    retval.addRow(0, (cancellable != 0) ? getUniqueId() : -1);
    return retval;
  }
}
