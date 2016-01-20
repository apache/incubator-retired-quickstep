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
