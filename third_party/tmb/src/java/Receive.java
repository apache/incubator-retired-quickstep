// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

package tmb.voltdb;

import org.voltdb.*;

public class Receive extends VoltProcedure {
  public final SQLStmt checkConnectedSQL = new SQLStmt(
      "SELECT client_id FROM client WHERE client_id = ? "
                                 + "AND disconnect_time IS NULL;");
  // TODO(chasseur): The ORDER BY clause in these queries necessitates a sort,
  // even though tuples should come out of the index in the correct order.
  // Figure out a way to elide the sort (thus far omitting the ORDER BY clause
  // causes priority order to be reversed, regardless of whether ASC or DESC is
  // specified for the priority order in the schema).
  public final SQLStmt getMessagesSQL = new SQLStmt(
      "SELECT message_id, sender_id, send_time, type_id, payload, "
               + "payload_bytes "
           + "FROM queued_message "
           + "WHERE receiver_id = ? AND priority >= ? "
                 + "AND NOW < expiration_time "
           + "ORDER BY priority DESC, expiration_time ASC, send_time ASC, "
                    + "message_id ASC, receiver_id;");
  public final SQLStmt getMessagesWithLimitSQL = new SQLStmt(
      "SELECT message_id, sender_id, send_time, type_id, payload, "
               + "payload_bytes "
           + "FROM queued_message "
           + "WHERE receiver_id = ? AND priority >= ? "
                 + "AND NOW < expiration_time "
           + "ORDER BY priority DESC, expiration_time ASC, send_time ASC, "
                    + "message_id ASC, receiver_id "
           + "LIMIT ?;");

  public VoltTable run(int client_id,
                       short minimum_priority,
                       long max_messages) throws VoltAbortException {
    voltQueueSQL(checkConnectedSQL, EXPECT_ONE_ROW, client_id);
    if (max_messages == 0) {
      voltQueueSQL(getMessagesSQL, client_id, minimum_priority);
    } else {
      voltQueueSQL(getMessagesWithLimitSQL, client_id,
                                            minimum_priority,
                                            max_messages);
    }

    VoltTable[] results = voltExecuteSQL();
    return results[1];
  }
}
