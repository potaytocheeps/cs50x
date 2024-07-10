-- Lists all movies released in 2010 and their ratings, in descending order by rating.
-- Movies with the same rating are ordered alphabetically
SELECT movies.title, rating
FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE movies.year = 2010 AND rating IS NOT NULL
ORDER BY rating DESC, movies.title;
