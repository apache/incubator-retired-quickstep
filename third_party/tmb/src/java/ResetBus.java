// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
