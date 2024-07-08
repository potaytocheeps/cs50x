-- Gets names of songs by Post Malone

-- Joining two tables
SELECT songs.name
FROM songs
JOIN artists
ON songs.artist_id = artists.id
WHERE artists.name = "Post Malone";

-- Using subqueries
SELECT name
FROM songs
WHERE artist_id = (
    SELECT id
    FROM artists
    WHERE name = 'Post Malone'
);
