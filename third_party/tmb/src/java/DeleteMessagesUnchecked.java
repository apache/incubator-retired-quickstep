// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

package tmb.voltdb;

import org.voltdb.*;

public class DeleteMessagesUnchecked extends VoltProcedure {
  public final SQLStmt deleteQueuedMessageSQL = new SQLStmt(
      "DELETE FROM queued_message WHERE receiver_id = ? AND message_id = ?");

  public long run(int receiver_id,
                  long[] message_ids) throws VoltAbortException {
    for (long message_id : message_ids) {
      voltQueueSQL(deleteQueuedMessageSQL, receiver_id, message_id);
    }
    voltExecuteSQL();
    return 0;
  }
}
