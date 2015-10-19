-- MOVIE
-- Primary key must be unique
-- ERROR 1062 (23000): Duplicate entry '2' for key 'PRIMARY'
INSERT INTO Movie(id, title, year, rating, company)
VALUES (2, 'Movie', '2015', 'R', 'Company');
-- CHECK: Titles must be nonempty strings
INSERT INTO Movie(id, title, year, rating, company)
VALUES (500001, '', '2015', 'R', 'Company');
-- Titles must be not null
-- ERROR 1048 (23000): Column 'title' cannot be null
INSERT INTO Movie(id, title, year, rating, company)
VALUES (500001, NULL, '2015', 'R', 'Company');

-- ACTOR
-- dob not null
-- ERROR 1048 (23000): Column 'dob' cannot be null
INSERT INTO Actor(id, last, first, sex, dob, dod)
VALUES(500001, 'Last', 'First', 'Female', NULL, NULL);
-- Primary key must be unique
-- ERROR 1062 (23000): Duplicate entry '1' for key 'PRIMARY'
INSERT INTO Actor(id, last, first, sex, dob, dod)
VALUES(1, 'Last', 'First', 'Female', '2000-01-01', NULL);
-- CHECK: Date of death must be after birth
INSERT INTO Actor(id, last, first, sex, dob, dod)
VALUES(500001, 'Last', 'First', 'Female', '2000-01-01', '1999-01-01');

-- DIRECTOR
-- dob not null
-- ERROR 1048 (23000): Column 'dob' cannot be null
INSERT INTO Director(id, last, first, dob, dod)
VALUES(500001, 'Last', 'First', NULL, NULL);
-- Primary key must be unique
-- ERROR 1062 (23000): Duplicate entry '16' for key 'PRIMARY'
INSERT INTO Director(id, last, first, dob, dod)
VALUES(16, 'Last', 'First', '1960-01-01', NULL);
-- CHECK: Date of death must be after birth
INSERT INTO Director(id, last, first, dob, dod)
VALUES(500001, 'Last', 'First', '1960-01-01', '1900-01-01');

-- MOVIE GENRE
-- Movie id and genre must be unique
-- ERROR 1062 (23000): Duplicate entry '4725-Sci-Fi' for key 'PRIMARY'
INSERT INTO MovieGenre(mid, genre)
VALUES(4725, 'Sci-Fi');
-- Movie ID must be an exisiting movie
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`MovieGenre`, CONSTRAINT `MovieGenre_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO MovieGenre(mid, genre)
VALUES(500001, 'Sci-Fi');

-- MOVIE DIRECTOR
-- Movie and director id must be unique
-- ERROR 1062 (23000): Duplicate entry '2832-68623' for key 'PRIMARY'
INSERT INTO MovieDirector(mid, did)
VALUES(2832, 68623);
-- Must reference exisiting movie
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`MovieDirector`, CONSTRAINT `MovieDirector_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO MovieDirector(mid, did)
VALUES(500001, 16);
-- Must reference exisiting director
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`MovieDirector`, CONSTRAINT `MovieDirector_ibfk_2` FOREIGN KEY (`did`) REFERENCES `Director` (`id`))
INSERT INTO MovieDirector(mid, did)
VALUES(2, 500001);

-- MOVIE ACTOR
-- Movie and actor id and role must be unique
-- ERROR 1062 (23000): Duplicate entry '1526-68622-Himself' for key 'PRIMARY'
INSERT INTO MovieActor(mid, aid, role)
VALUES(1526, 68622, 'Himself');
-- Must reference exisiting movie
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`MovieActor`, CONSTRAINT `MovieActor_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO MovieActor(mid, aid, role)
VALUES(500001, 1, 'Actor');
-- Must reference exisiting actor
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`MovieActor`, CONSTRAINT `MovieActor_ibfk_2` FOREIGN KEY (`aid`) REFERENCES `Actor` (`id`))
INSERT INTO MovieActor(mid, aid, role)
VALUES(2, 500001, 'Actor');

-- REVIEWS
-- name and movie id must be unique (one review per person)
-- ERROR 1062 (23000): Duplicate entry 'Alice-2' for key 'PRIMARY'
INSERT INTO Review(name, time, mid, rating, comment)
VALUES ('Alice', 1, 2, 4, 'Good'),
  ('Alice', 1, 2, 5, 'Good movie');
-- Must reference existing movie
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`Review`, CONSTRAINT `Review_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO Review(name, time, mid, rating, comment)
VALUES('Name', 1, 500001, 1, 'Comment');
-- CHECK: Rating must be between 0 and 5
INSERT INTO Review(name, time, mid, rating, comment)
VALUES('Name', 1, 2, 6, 'Comment');
