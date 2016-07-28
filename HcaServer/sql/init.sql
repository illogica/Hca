﻿-- execute this query as user "hca"

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
status SMALLINT
);
INSERT INTO clients(uuid, name, description, privilege, status)
 VALUES('test-key', 'unnamed', 'description text', 0, 0);

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
ownerid INTEGER REFERENCES clients(id)
);

INSERT INTO rooms(id, name, description, worldid)
 VALUES(0, 'Lounge', 'The public Hca Lounge!', 0);

CREATE TABLE roomclients(
id SERIAL PRIMARY KEY,
roomid INTEGER REFERENCES rooms(id),
clientid INTEGER REFERENCES clients(id)
);
