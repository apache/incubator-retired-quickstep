// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

package tmb.voltdb;

import org.voltdb.*;

public class RegisterReceiver extends VoltProcedure {
  public final SQLStmt checkConnectedSQL = new SQLStmt(
      "SELECT client_id FROM client WHERE client_id = ? "
                                 + "AND disconnect_time IS NULL");
  public final SQLStmt insertReceivableSQL = new SQLStmt(
      "INSERT INTO receivable VALUES(?, ?);");

  public long run(int client_id,
                  int message_type_id) throws VoltAbortException {
    voltQueueSQL(checkConnectedSQL, EXPECT_ONE_ROW, client_id);
    voltQueueSQL(insertReceivableSQL, client_id, message_type_id);
    voltExecuteSQL();

    return 0;
  }
}
