// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

package tmb.voltdb;

import org.voltdb.*;

public class SendToSingleExplicitReceiverUnchecked extends VoltProcedure {
  public final SQLStmt insertQueuedMessage = new SQLStmt(
      "INSERT INTO queued_message VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?);");

  private static final org.voltdb.types.TimestampType MAX_TIMESTAMP
      = new org.voltdb.types.TimestampType(Long.MAX_VALUE);

  public long run(int sender_id,
                  int receiver_id,
                  org.voltdb.types.TimestampType send_time,
                  org.voltdb.types.TimestampType expiration_time,
                  short priority,
                  int type_id,
                  byte[] payload) throws VoltAbortException {
    org.voltdb.types.TimestampType actual_expiration_time
        = (expiration_time == null
           || expiration_time.getTime() == VoltType.NULL_BIGINT)
               ? MAX_TIMESTAMP : expiration_time;

    voltQueueSQL(insertQueuedMessage,
                 receiver_id,
                 getUniqueId(),
                 sender_id,
                 send_time,
                 actual_expiration_time,
                 priority,
                 type_id,
                 payload,
                 payload.length);
    voltExecuteSQL();

    return getUniqueId();
  }
}
