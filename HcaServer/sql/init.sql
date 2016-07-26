-- execute this query as user "hca"

DROP TABLE IF EXISTS worldrooms;
DROP TABLE IF EXISTS roomclients;
DROP TABLE IF EXISTS worlds;
DROP TABLE IF EXISTS rooms;
DROP TABLE IF EXISTS clients;

CREATE TABLE clients(
uuid VARCHAR(40) primary key,
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

CREATE TABLE rooms(
id SERIAL PRIMARY KEY,
name VARCHAR(255),
description text,
owner varchar(40) REFERENCES clients(uuid)
);

CREATE TABLE worldrooms(
world INTEGER REFERENCES worlds(id),
room INTEGER REFERENCES rooms(id)
);

CREATE TABLE roomclients(
room INTEGER REFERENCES rooms(id),
clientuuid VARCHAR(40) REFERENCES clients(uuid)
);
