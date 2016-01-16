// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
