// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

package tmb.voltdb;

import org.voltdb.*;

public class DisconnectClient extends VoltProcedure {
  public final SQLStmt checkConnectedSQL = new SQLStmt(
      "SELECT client_id FROM client WHERE client_id = ? "
                                 + "AND disconnect_time IS NULL");
  public final SQLStmt disconnectClientSQL = new SQLStmt(
      "UPDATE client SET disconnect_time = NOW WHERE client_id = ?;");
  public final SQLStmt removeSenderEntriesSQL = new SQLStmt(
      "DELETE FROM sendable WHERE client_id = ?;");
  public final SQLStmt removeReceiverEntriesSQL = new SQLStmt(
      "DELETE FROM receivable WHERE client_id = ?;");
  public final SQLStmt removeQueuedMessagesSQL = new SQLStmt(
      "DELETE FROM queued_message WHERE receiver_id = ?;");

  public long run(int client_id) throws VoltAbortException {
    voltQueueSQL(checkConnectedSQL, EXPECT_ONE_ROW, client_id);
    voltQueueSQL(disconnectClientSQL, client_id);
    voltQueueSQL(removeSenderEntriesSQL, client_id);
    voltQueueSQL(removeReceiverEntriesSQL, client_id);
    voltQueueSQL(removeQueuedMessagesSQL, client_id);
    voltExecuteSQL();

    return 0;
  }
}
