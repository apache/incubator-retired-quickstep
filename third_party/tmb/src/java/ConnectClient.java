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

public class ConnectClient extends VoltProcedure {
  public final SQLStmt insertClientSQL = new SQLStmt(
      "INSERT INTO client VALUES (?, NOW, NULL);");

  public long run() throws VoltAbortException {
    // Generate a probable unique client ID by XORing the high and low parts of
    // the transaction ID together.
    long tx_id = getUniqueId();
    int client_id = (int)tx_id ^ (int)(tx_id >>> 32);

    voltQueueSQL(insertClientSQL, client_id);
    voltExecuteSQL();

    return client_id;
  }
}
