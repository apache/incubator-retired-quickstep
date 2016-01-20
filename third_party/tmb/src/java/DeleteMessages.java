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
