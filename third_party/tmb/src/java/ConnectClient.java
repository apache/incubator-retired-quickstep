// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
