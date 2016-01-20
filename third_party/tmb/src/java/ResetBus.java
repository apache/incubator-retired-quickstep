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

public class ResetBus extends VoltProcedure {
  public final SQLStmt deleteClientsSQL = new SQLStmt("DELETE FROM client;");
  public final SQLStmt deleteSendableSQL = new SQLStmt(
      "DELETE FROM sendable;");
  public final SQLStmt deleteReceivableSQL = new SQLStmt(
      "DELETE FROM receivable;");
  public final SQLStmt deleteCancellableMessagesSQL = new SQLStmt(
      "DELETE FROM cancellable_message;");
  public final SQLStmt deleteQueuedMessagesSQL = new SQLStmt(
      "DELETE FROM queued_message;");

  public long run() throws VoltAbortException {
    voltQueueSQL(deleteClientsSQL);
    voltQueueSQL(deleteSendableSQL);
    voltQueueSQL(deleteReceivableSQL);
    voltQueueSQL(deleteCancellableMessagesSQL);
    voltQueueSQL(deleteQueuedMessagesSQL);
    voltExecuteSQL();

    return 0;
  }
}
