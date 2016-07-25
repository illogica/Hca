DROP TABLE IF EXISTS worldrooms;
DROP TABLE IF EXISTS roomclients;
DROP TABLE IF EXISTS worlds;
DROP TABLE IF EXISTS rooms;
DROP TABLE IF EXISTS clients;

CREATE TABLE clients(
uuid VARCHAR(50) primary key,
name VARCHAR(255),
description text
);

CREATE TABLE worlds(
id SERIAL PRIMARY KEY,
name VARCHAR(255),
description text
);

CREATE TABLE rooms(
id SERIAL PRIMARY KEY,
name VARCHAR(255),
description text,
owner varchar(50) REFERENCES clients(uuid)
);

CREATE TABLE worldrooms(
world INTEGER REFERENCES worlds(id),
room INTEGER REFERENCES rooms(id)
);

CREATE TABLE roomclients(
room INTEGER REFERENCES rooms(id),
clientuuid VARCHAR(50) REFERENCES clients(uuid)
);