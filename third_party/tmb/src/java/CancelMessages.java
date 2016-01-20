//   Copyright 2014-2015 Quickstep Technologies LLC.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

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
