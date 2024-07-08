-- Gets average energy of all songs by Drake

-- Joining two tables
SELECT AVG(energy)
FROM songs
JOIN artists
ON songs.artist_id = artists.id
WHERE artists.name = "Drake";

-- Using subqueries
SELECT AVG(energy)
FROM songs
WHERE artist_id = (
    SELECT id
    FROM artists
    WHERE name = "Drake"
);
