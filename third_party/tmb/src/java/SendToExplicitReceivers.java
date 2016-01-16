// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

package tmb.voltdb;

import org.voltdb.*;

public class SendToExplicitReceivers extends VoltProcedure {
  public final SQLStmt checkConnectedSQL = new SQLStmt(
      "SELECT client_id FROM client WHERE client_id = ? "
                                 + "AND disconnect_time IS NULL;");
  public final SQLStmt checkSendableSQL = new SQLStmt(
      "SELECT client_id FROM sendable WHERE client_id = ? "
                                   + "AND type_id = ?;");
  public final SQLStmt getConnectedSQL = new SQLStmt(
      "SELECT client_id FROM client WHERE client_id IN ? "
                                 + "AND disconnect_time IS NULL;");
  public final SQLStmt countReceivableSQL = new SQLStmt(
      "SELECT COUNT(*) FROM receivable WHERE client_id IN ? "
                                    + "AND type_id = ?;");
  public final SQLStmt insertQueuedMessage = new SQLStmt(
      "INSERT INTO queued_message VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?);");
  public final SQLStmt insertCancellableMessage = new SQLStmt(
      "INSERT INTO cancellable_message VALUES(?);");

  private static final org.voltdb.types.TimestampType MAX_TIMESTAMP
      = new org.voltdb.types.TimestampType(Long.MAX_VALUE);

  public VoltTable run(int sender_id,
                       int[] receiver_ids,
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
    voltQueueSQL(getConnectedSQL, (Object)receiver_ids);
    voltQueueSQL(countReceivableSQL, (Object)receiver_ids, type_id);
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
    long receivers_registered = check_result[3].asScalarLong();
    if (receivers_connected == 0) {
      retval.addRow(1, -1);
      return retval;  // kNoReceivers
    }
    if (receivers_registered < receivers_connected) {
      retval.addRow(4, -1);
      return retval;  // kReceiverNotRegisteredForMessageType
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
