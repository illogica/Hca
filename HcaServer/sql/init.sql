-- execute this query as user "hca"

DROP TABLE IF EXISTS roomclients;
DROP TABLE IF EXISTS rooms;
DROP TABLE IF EXISTS worlds;
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

INSERT INTO worlds(id, name, description)
 VALUES(0, 'Hca', 'The great Hca World!');

CREATE TABLE rooms(
id SERIAL PRIMARY KEY,
name VARCHAR(255),
description text,
world INTEGER REFERENCES worlds(id),
owner varchar(40) REFERENCES clients(uuid)
);

INSERT INTO rooms(id, name, description, world)
 VALUES(0, 'Lounge', 'The public Hca Lounge!', 0);

CREATE TABLE roomclients(
id SERIAL PRIMARY KEY,
room INTEGER REFERENCES rooms(id),
client VARCHAR(40) REFERENCES clients(uuid)
);
