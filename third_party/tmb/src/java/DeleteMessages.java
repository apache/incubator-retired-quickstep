// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

package tmb.voltdb;

import org.voltdb.*;

public class DeleteMessages extends VoltProcedure {
  public final SQLStmt deleteQueuedMessageSQL = new SQLStmt(
      "DELETE FROM queued_message WHERE receiver_id = ? AND message_id = ?");
  public final SQLStmt deleteExpiredMessagesSQL = new SQLStmt(
      "DELETE FROM queued_message WHERE NOW > expiration_time;");

  public long run(int receiver_id,
                  long[] message_ids) throws VoltAbortException {
    for (long message_id : message_ids) {
      voltQueueSQL(deleteQueuedMessageSQL, receiver_id, message_id);
    }
    // Delete all expired messages in this partition while we're at it.
    voltQueueSQL(deleteExpiredMessagesSQL);
    voltExecuteSQL();
    return 0;
  }
}
