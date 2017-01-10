-- Licensed to the Apache Software Foundation (ASF) under one
-- or more contributor license agreements.  See the NOTICE file
-- distributed with this work for additional information
-- regarding copyright ownership.  The ASF licenses this file
-- to you under the Apache License, Version 2.0 (the
-- "License"); you may not use this file except in compliance
-- with the License.  You may obtain a copy of the License at
--
--   http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing,
-- software distributed under the License is distributed on an
-- "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
-- KIND, either express or implied.  See the License for the
-- specific language governing permissions and limitations
-- under the License.

CREATE TABLE client(
    client_id INTEGER NOT NULL,
    connect_time TIMESTAMP NOT NULL,
    disconnect_time TIMESTAMP,
    CONSTRAINT client_pk_hash PRIMARY KEY(client_id)
);

CREATE TABLE sendable(
    client_id INTEGER NOT NULL,
    type_id INTEGER NOT NULL,
    CONSTRAINT sendable_pk_hash PRIMARY KEY(client_id, type_id));
CREATE INDEX sendable_idx_client_id_hash ON sendable(client_id);

CREATE TABLE receivable(
    client_id INTEGER NOT NULL,
    type_id INTEGER NOT NULL,
    CONSTRAINT receivable_pk_hash PRIMARY KEY(client_id, type_id));
CREATE INDEX receivable_idx_client_id_hash ON receivable(client_id);
CREATE INDEX receivable_idx_type_id_hash ON receivable(type_id);

CREATE TABLE cancellable_message(
    message_id BIGINT NOT NULL,
    CONSTRAINT message_pk_hash PRIMARY KEY(message_id));

-- All message data is denormalized into this table, which is partitioned on
-- receiver_id.
CREATE TABLE queued_message(
    receiver_id INTEGER NOT NULL,
    message_id BIGINT NOT NULL,
    sender_id INTEGER NOT NULL,
    send_time TIMESTAMP NOT NULL,
    -- Instead of using NULL for no expiration time, we use the farthest point
    -- in the future that can be represented. This is because VoltDB stores
    -- NULL values as the minimum possible signed integer value, causing them
    -- to be ordered first in indexes and sorts, which is the opposite of the
    -- behavior we desire. This also allows us to simplify predicates that
    -- check if a message has expired to be simply 'NOW < expiration_time'
    -- without a secondary NULL check.
    expiration_time TIMESTAMP NOT NULL,
    priority SMALLINT NOT NULL,
    type_id INTEGER NOT NULL,
    payload VARBINARY(1048576) NOT NULL,  -- VoltDB limits VARBINARY to 1MB.
    payload_bytes INTEGER NOT NULL,
    CONSTRAINT queued_message_pk_hash PRIMARY KEY(receiver_id, message_id));
PARTITION TABLE queued_message ON COLUMN receiver_id;
-- TODO(chasseur): Previously this index also included 'expiration_time',
-- 'send_time', and 'message_id' because a predicate on 'expiration_time' is
-- part of the WHERE clause that selects messages, and all 3 columns are part
-- of the ORDER BY clause which sorts messages. However, even with these
-- columns in the index in correct order, the VoltDB optimizer still produces
-- a plan which includes a filter on 'expiration_time' and a sort to implement
-- ORDER BY. Therefore, the index has been reduced to only the columns which
-- are actually used by the query plan. Investigate further to see whether we
-- can coax VoltDB to use an index on the 5 relevant columns to omit a sort.
CREATE INDEX queue_index ON queued_message(receiver_id, priority DESC);
CREATE INDEX queued_message_idx_message_id_hash ON queued_message(message_id);
CREATE INDEX queued_message_idx_expiration_time
       ON queued_message(expiration_time);

CREATE PROCEDURE RegisterReceiverUnchecked AS
    INSERT INTO receivable VALUES(?, ?);
CREATE PROCEDURE RegisterSenderUnchecked AS
    INSERT INTO sendable VALUES(?, ?);

CREATE PROCEDURE CountQueuedMessagesForClient AS
    SELECT COUNT(*) FROM queued_message WHERE receiver_id = ?;
PARTITION PROCEDURE CountQueuedMessagesForClient
    ON TABLE queued_message COLUMN receiver_id
    PARAMETER 0;

CREATE PROCEDURE FROM CLASS tmb.voltdb.CancelMessages;
CREATE PROCEDURE FROM CLASS tmb.voltdb.ConnectClient;
CREATE PROCEDURE FROM CLASS tmb.voltdb.DeleteMessages;
PARTITION PROCEDURE DeleteMessages
    ON TABLE queued_message COLUMN receiver_id
    PARAMETER 0;
CREATE PROCEDURE FROM CLASS tmb.voltdb.DeleteMessagesUnchecked;
PARTITION PROCEDURE DeleteMessagesUnchecked
    ON TABLE queued_message COLUMN receiver_id
    PARAMETER 0;
CREATE PROCEDURE FROM CLASS tmb.voltdb.DisconnectClient;
CREATE PROCEDURE FROM CLASS tmb.voltdb.LoadState;
CREATE PROCEDURE FROM CLASS tmb.voltdb.Receive;
PARTITION PROCEDURE Receive
    ON TABLE queued_message COLUMN receiver_id
    PARAMETER 0;
CREATE PROCEDURE FROM CLASS tmb.voltdb.ReceiveAndDelete;
PARTITION PROCEDURE ReceiveAndDelete
    ON TABLE queued_message COLUMN receiver_id
    PARAMETER 0;
CREATE PROCEDURE FROM CLASS tmb.voltdb.RegisterReceiver;
CREATE PROCEDURE FROM CLASS tmb.voltdb.RegisterSender;
CREATE PROCEDURE FROM CLASS tmb.voltdb.ResetBus;
CREATE PROCEDURE FROM CLASS tmb.voltdb.SendToAny;
CREATE PROCEDURE FROM CLASS tmb.voltdb.SendToExplicitReceivers;
CREATE PROCEDURE FROM CLASS tmb.voltdb.SendToExplicitReceiversUnchecked;
CREATE PROCEDURE FROM CLASS tmb.voltdb.SendToSingleExplicitReceiver;
PARTITION PROCEDURE SendToSingleExplicitReceiver
    ON TABLE queued_message COLUMN receiver_id
    PARAMETER 1;
CREATE PROCEDURE FROM CLASS tmb.voltdb.SendToSingleExplicitReceiverUnchecked;
PARTITION PROCEDURE SendToSingleExplicitReceiverUnchecked
    ON TABLE queued_message COLUMN receiver_id
    PARAMETER 1;
