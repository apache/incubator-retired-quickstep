// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

package tmb.voltdb;

import org.voltdb.*;

public class CancelMessages extends VoltProcedure {
  public final SQLStmt getCancellableMessagesSQL = new SQLStmt(
      "SELECT message_id FROM cancellable_message WHERE message_id IN ?;");
  public final SQLStmt deleteCancellableMessageSQL = new SQLStmt(
      "DELETE FROM cancellable_message WHERE message_id = ?;");
  public final SQLStmt deleteQueuedMessagesSQL = new SQLStmt(
      "DELETE FROM queued_message WHERE message_id = ?;");

  public long run(long[] message_ids) throws VoltAbortException {
    voltQueueSQL(getCancellableMessagesSQL, (Object)message_ids);
    VoltTable[] cancellable_result = voltExecuteSQL();
    if (cancellable_result[0].getRowCount() == 0) {
      return 0;
    }

    VoltTableRow receiver_row = cancellable_result[0].fetchRow(0);
    receiver_row.resetRowPosition();
    while (receiver_row.advanceRow()) {
      voltQueueSQL(deleteCancellableMessageSQL, receiver_row.getLong(0));
      voltQueueSQL(deleteQueuedMessagesSQL, receiver_row.getLong(0));
    }

    voltExecuteSQL();
    return 0;
  }
}
