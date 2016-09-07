-- execute this query as user "hca"

SET TIME ZONE 'UTC';

DROP TABLE IF EXISTS roomclients;
DROP TABLE IF EXISTS rooms;
DROP TABLE IF EXISTS worlds;
DROP TABLE IF EXISTS clients;

CREATE TABLE clients(
id SERIAL PRIMARY KEY,
uuid VARCHAR(40) UNIQUE NOT NULL,
name VARCHAR(255),
description text,
privilege SMALLINT,
status SMALLINT,
avatar VARCHAR(255)
);
INSERT INTO clients(uuid, name, description, privilege, status, avatar)
 VALUES('test-key', 'unnamed', 'description text', 0, 0, '1f42a');

CREATE TABLE worlds(
id SERIAL PRIMARY KEY,
name VARCHAR(255),
description text
);

INSERT INTO worlds(id, name, description)
 VALUES(0, 'Hca', 'The great Hca World!');

CREATE TABLE rooms(
id SERIAL PRIMARY KEY,
name VARCHAR(255),
description text,
motd text,
worldid INTEGER REFERENCES worlds(id),
ownerid INTEGER REFERENCES clients(id),
avatar text
);

INSERT INTO rooms(id, name, description, motd, worldid, avatar)
 VALUES(0, 'Lounge', 'The public Hca Lounge!', 'Welcome and enjoy', 0, '1f3db');

CREATE TABLE roomclients(
id SERIAL PRIMARY KEY,
roomid INTEGER REFERENCES rooms(id),
clientid INTEGER REFERENCES clients(id),
lastseen TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW()
);
CREATE UNIQUE INDEX rooms_clients_unique ON roomclients (roomId, clientId);

