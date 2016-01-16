// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

package tmb.voltdb;

import org.voltdb.*;

public class ReceiveAndDelete extends VoltProcedure {
  public final SQLStmt checkConnectedSQL = new SQLStmt(
      "SELECT client_id FROM client WHERE client_id = ? "
                                 + "AND disconnect_time IS NULL;");
  public final SQLStmt deleteExpiredMessagesSQL = new SQLStmt(
      "DELETE FROM queued_message WHERE NOW > expiration_time;");
  // TODO(chasseur): Once the ordering issue mentioned in Receive.java is
  // fixed, update these queries as well.
  public final SQLStmt getMessagesSQL = new SQLStmt(
      "SELECT message_id, sender_id, send_time, type_id, payload, "
               + "payload_bytes "
           + "FROM queued_message "
           + "WHERE receiver_id = ? AND priority >= ? "
           + "ORDER BY priority DESC, expiration_time ASC, send_time ASC, "
                    + "message_id ASC, receiver_id;");
  public final SQLStmt getMessagesWithLimitSQL = new SQLStmt(
      "SELECT message_id, sender_id, send_time, type_id, payload, "
               + "payload_bytes "
           + "FROM queued_message "
           + "WHERE receiver_id = ? AND priority >= ? "
           + "ORDER BY priority DESC, expiration_time ASC, send_time ASC, "
                    + "message_id ASC, receiver_id "
           + "LIMIT ?;");
  public final SQLStmt deleteMessagesSQL = new SQLStmt(
      "DELETE FROM queued_message "
           + "WHERE receiver_id = ? AND priority >= ?;");
  public final SQLStmt deleteQueuedMessageSQL = new SQLStmt(
      "DELETE FROM queued_message WHERE receiver_id = ? AND message_id = ?");

  public VoltTable run(int client_id,
                       short minimum_priority,
                       long max_messages) throws VoltAbortException {
    voltQueueSQL(checkConnectedSQL, EXPECT_ONE_ROW, client_id);
    voltQueueSQL(deleteExpiredMessagesSQL);
    if (max_messages == 0) {
      voltQueueSQL(getMessagesSQL, client_id, minimum_priority);
      voltQueueSQL(deleteMessagesSQL, client_id, minimum_priority);
    } else {
      voltQueueSQL(getMessagesWithLimitSQL, client_id,
                                            minimum_priority,
                                            max_messages);
    }
    VoltTable[] results = voltExecuteSQL();

    if ((max_messages != 0) && (results[2].getRowCount() > 0)) {
      while (results[2].advanceRow()) {
        voltQueueSQL(deleteQueuedMessageSQL, client_id, results[2].getLong(0));
      }
      voltExecuteSQL();
    }

    return results[2];
  }
}
