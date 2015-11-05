CREATE TABLE Movie(
  id INT,
  title VARCHAR(100) NOT NULL,
  year INT,
  rating VARCHAR(10),
  company VARCHAR(50),
  -- Every Movie must have a unique ID
  PRIMARY KEY(id),
  -- Movie titles should be nonempty
  CHECK(LENGTH(title) > 0))
ENGINE = INNODB;

CREATE TABLE Actor(
  id INT,
  last VARCHAR(20),
  first VARCHAR(20),
  sex VARCHAR(6),
  dob DATE NOT NULL,
  dod DATE,
  -- Every Actor must have a unique ID
  PRIMARY KEY(id),
  -- Date of death must be after date of birth
  CHECK(dod IS NULL OR dod > dob))
ENGINE = INNODB;

CREATE TABLE Director(
  id INT,
  last VARCHAR(20),
  first VARCHAR(20),
  dob DATE NOT NULL,
  dod DATE,
  -- Every Directory must have a unique ID
  PRIMARY KEY(id),
  -- Date of death must be after date of birth
  CHECK(dod IS NULL OR dod > dob))
ENGINE = INNODB;

CREATE TABLE MovieGenre(
  mid INT,
  genre VARCHAR(20),
  -- No duplicate entries for MovieGenre
  PRIMARY KEY (mid, genre),
  -- A MovieGenre must references an existing Movie
  FOREIGN KEY (mid) references Movie(id))
ENGINE = INNODB;

CREATE TABLE MovieDirector(
  mid INT,
  did INT,
  -- No duplicate entries for MovieDirector
  PRIMARY KEY (mid, did),
  -- A MovieDirector must reference an existing Movie
  FOREIGN KEY(mid) references Movie(id),
  -- A MovieDirector must reference an existing Director
  FOREIGN KEY(did) references Director(id))
ENGINE = INNODB;

CREATE TABLE MovieActor(
  mid INT,
  aid INT,
  role VARCHAR(50),
  -- No duplicate entries for MovieActor
  PRIMARY KEY (mid, aid, role),
  -- A MovieActor must reference an existing Movie
  FOREIGN KEY(mid) references Movie(id),
  -- A MovieActor must reference an existing Actor
  FOREIGN KEY(aid) references Actor(id))
ENGINE = INNODB;

CREATE TABLE Review(
  name VARCHAR(20),
  time timestamp,
  mid INT,
  rating INT,
  comment VARCHAR(500),
  -- One review per person per movie
  PRIMARY KEY (name, mid),
  -- A Review must reference an existing Movie
  FOREIGN KEY(mid) references Movie(id),
  -- Rating must be between 0 and 5
  CHECK(rating >= 0 AND rating <= 5))
ENGINE = INNODB;

CREATE TABLE MaxPersonID(id INT NOT NULL) ENGINE = INNODB;

CREATE TABLE MaxMovieID(id INT NOT NULL) ENGINE = INNODB;
