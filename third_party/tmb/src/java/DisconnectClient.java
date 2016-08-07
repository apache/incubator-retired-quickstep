/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

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
