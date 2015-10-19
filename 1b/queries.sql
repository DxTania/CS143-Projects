-- Select first and last names of all actors who were in the movie
-- Die Another Day. Joins MovieActor and Actor tables and selects
-- names of Actors who are also in MovieActor table with the movie
-- id of the movie "Die Another Day"
SELECT CONCAT(first, ' ', last)
FROM MovieActor,Actor
WHERE mid=(SELECT id FROM Movie WHERE title="Die Another Day")
  AND MovieActor.aid=Actor.id;

-- Count number of actors who acted in multiple movies by grouping
-- MovieActor table by Actor ID then filtering by those Actor IDs
-- who have more than 1 Movie ID associated with them.
SELECT COUNT(*)
FROM (SELECT aid
      FROM MovieActor
      GROUP BY aid
      HAVING COUNT(mid) > 1) as mult;

-- Find number of actors who have acted in at least 5 Sci-Fi films
-- by joining MovieActor and MovieGenre, selecting SciFi films, then
-- counting the number of movies per actor by grouping on actor ID.
SELECT COUNT(*)
FROM (SELECT aid
      FROM MovieActor, MovieGenre
      WHERE MovieActor.mid=MovieGenre.mid AND genre='Sci-Fi'
      GROUP BY aid
      HAVING COUNT(MovieActor.mid) >= 5) as scifi;
