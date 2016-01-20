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

// TODO(chasseur): This loads the entire bus state in one go, which could
// potentially overrun some VoltDB limits on result set size. Look into
// piecewise loading as an alternative.
public class LoadState extends VoltProcedure {
  public final SQLStmt getClientsSQL = new SQLStmt(
      "SELECT client_id FROM client WHERE disconnect_time IS NULL;");
  public final SQLStmt getSendableSQL = new SQLStmt(
      "SELECT client_id, type_id FROM sendable;");
  public final SQLStmt getReceivableSQL = new SQLStmt(
      "SELECT client_id, type_id FROM receivable;");
  public final SQLStmt getCancellableMessagesSQL = new SQLStmt(
      "SELECT message_id FROM cancellable_message;");
  public final SQLStmt getQueuedMessagesSQL = new SQLStmt(
      "SELECT * FROM queued_message;");

  public VoltTable[] run() throws VoltAbortException {
    voltQueueSQL(getClientsSQL);
    voltQueueSQL(getSendableSQL);
    voltQueueSQL(getReceivableSQL);
    voltQueueSQL(getCancellableMessagesSQL);
    voltQueueSQL(getQueuedMessagesSQL);

    return voltExecuteSQL();
  }
}
