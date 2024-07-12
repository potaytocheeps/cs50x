-- Keep a log of any SQL queries you execute as you solve the mystery.

-- 
-- All you know is that the theft took place on July 28, 2023 and that it took place on Humphrey Street.
-- 



-- I will first look at the schema to see the layout of the database and the tables
.schema

-- I will now check how each of the tables looks in terms of the data they hold
-- This will help me to have a better understanding of how the tables are formatted and
-- how all of the data looks and connects to one another
SELECT * FROM crime_scene_reports LIMIT 10;
SELECT * FROM interviews LIMIT 10;

-- I will reduce the Limit to 3 since it isn't necessary to see 10 lines of it, especially
-- since the previous query resulted in a bit of a mess with all of the descriptions
SELECT * FROM atm_transactions LIMIT 3;
SELECT * FROM bank_accounts LIMIT 3;
SELECT * FROM airports LIMIT 3;
SELECT * FROM flights LIMIT 3;
SELECT * FROM passengers LIMIT 3;
SELECT * FROM phone_calls LIMIT 3;
SELECT * FROM people LIMIT 3;
SELECT * FROM bakery_security_logs LIMIT 3;

-- Now having a better idea of the layout of each table and the way that each of the data
-- is represented, I can start to narrow down my search to the specific date of the crime
-- and the street where it happened. This should allow me to discover what step to take next.
-- I will be using the crime_scene_reports table and get only the crime scenes that happened on
-- July 28th. I was first thinking about selecting the street as opposed to the date, but this
-- could potentially show other dates that aren't relevant to the case at hand. Instead, selecting
-- the date should show any crime that happened that day, which may all be relevant to the case
-- of the missing duck.
SELECT *
  FROM crime_scene_reports
 WHERE year = 2023 AND month = 7 AND day = 28;

-- The previous query yielded relevant information for the case, where the id is 295, on July
-- 28th, 2023 on Humphrey Street, just as we knew from the beginning. The description of the
-- crime report states that: Theft of the CS50 duck took place at 10:15am at the Humphrey
-- Street bakery. Interviews were conducted today with three witnesses who were present at the
-- time – each of their interview transcripts mentions the bakery.

-- Given this information about the exact location of the crime: Humphrey Street Bakery, and
-- that there were three witnesses present at the time of the crime, I will review the
-- interviews of the witnesses to see what they had to say about the crime so that I can
-- further get some information about what to investigate next.

-- The interviews table also contains columns for the year, month, and day, so I will use a
-- similar query as before to only get the interviews for that particular day.
SELECT *
  FROM interviews
 WHERE year = 2023 AND month = 7 AND day = 28;

-- The previous query gave me the accounts of the three witnesses that were there at the time
-- of the crime of the CS50 duck's abduction. The following are the three pieces of information
-- extracted with the previous query:

-- | 161 | Ruth    | 2023 | 7     | 28  | Sometime within ten minutes of the theft, I saw the
-- thief get into a car in the bakery parking lot and drive away. If you have security footage
-- from the bakery parking lot, you might want to look for cars that left the parking lot in
-- that time frame.

-- | 162 | Eugene  | 2023 | 7     | 28  | I don't know the thief's name, but it was someone I
-- recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the
-- ATM on Leggett Street and saw the thief there withdrawing some money.

-- | 163 | Raymond | 2023 | 7     | 28  | As the thief was leaving the bakery, they called
-- someone who talked to them for less than a minute. In the call, I heard the thief say that
-- they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then
-- asked the person on the other end of the phone to purchase the flight ticket.

-- I can be sure that these three interviews of witnesses belong to the ones relevant to this
-- case because they each make mention of a bakery, as was previously mentioned in the crime
-- scene report. As such, the transcripts of the interviews will give me a clue of where to
-- investigate next.

-- The first witness interview, id 161, named Ruth, mentions that the thief got into a car
-- sometime within ten minutes of the theft. The crime scene report states that the theft
-- happened at 10:15am. This means that the thief got into a car sometime between 10:15am
-- and 10:25am. I have available some bakery security logs which appear to contain information
-- about the entry and exit time and date of vehicles at the bakery parking lot, as well as
-- their license plate numbers. These logs will be a good place to continue my search once
-- I'm done going through the rest of the interview transcripts.

-- Eugene, id 162, mentioned that earlier that morning, that is, before 10:15am, they saw
-- the thief withdrawing money from the ATM on Leggett Street. The bakery is run by someone
-- named Emma, in case that may be relevant later with the case.
-- I can check the atm_transactions table at Leggett Street on July 28th to find information
-- about whether the thief deposited or withdrew money from their account, and the amount
-- that they deposited or withdrew. As well as their account_number, which will be particularly
-- useful later on for the case.
-- The account_number can be used in conjunction with the bank_accounts table to find that
-- account_number's person_id, which can then be used with the people table to find a name
-- and phone number that I can use to narrow down the search of the thief and their accomplice

-- Raymond, id 163, has a good amount of relevant information to share. For starters, it
-- appears that the thief called someone on the phone just as they were leaving the bakery,
-- which will have been soon after the time of the crime at 10:15am. They only talked on
-- the phone for less than a minute though.
-- Apparently the thief was planning on taking the earliest flight out of Fiftyville the next
-- day. That means that the thief wanted to take a flight on July 29th, and not just any flight
-- on that day, but the first one that happened going out of Fiftyville on July 29th.
-- Finally, the thief didn't buy their own ticket, but asked their accomplice on the other side
-- of the phone to do so.
-- With this information, I can check the phone_calls table to see the records for any phone
-- call that happened on July 28th of 2023 and that lasted less than 60 seconds, that is, less
-- than a minute.
-- Next, I can check the airports table to see what the id of the airport is for Fiftyville.
-- I can use the result of that information in conjunction with the flights table to see all
-- the flights that happened on July 29th, 2023, where the origin_airport_id is that of
-- Fiftyville acquired from the airports table previously. This will let me know what the
-- exact flight was that the thief took to leave Fiftyville on July 29th 2023.
-- Once having the information for the exact flight that the thief took, I can use the id for
-- that flight in conjunction with the passengers table to acquire the passport_number of
-- all the passengers in that particular flight. This passport_number list will not allow me
-- to straight up find the thief, but I can use this information which is also part of the
-- people table, which itself contains information for each person's passport_number AND
-- license_plate, which I will have acquired through the bakery_security_logs earlier.
-- That means that I can narrow down the name of the passenger by combining the information
-- in the people table, the passengers table, and the bakery_security_logs table, to see
-- which one person was part of the flight and also has a license_plate matching the one
-- seen in the bakery_security_log. But since the accomplice bought the ticket for the 
-- thief, it's likely that the name that is found through these queries will give me the
-- name of the accomplice, and not the name of the thief just yet.


-- I am now unsure as to whether the passport_number will guide me to the thief or the
-- accomplice of the thief. But either way, it should lead me to one of them, and I can
-- use the information acquired through the atm_transactions table to corroborate the
-- identity of at least one of them, namely the thief. I can then use their phone number
-- along with the phone_calls to see when the caller, found through the atm_transactions
-- table and the people table through the account_number, to confirm the identity of the
-- accomplice by matching the phone number of the receiver of the call with that found
-- in the people table.




-- With all of this information and plans acquired through the accounts of the the three
-- witnesses, I want to first start by checking the atm_transactions to be able to get
-- closer to finding who the thief is. So I will get all of the information from the
-- atm_transactions table that happened on July 28th 2023 on Leggett Street.
SELECT *
FROM atm_transactions
WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street';

-- I failed to realize that the atm_transactions table doesn't have any time logs, meaning
-- that the information acquired through the previous query wasn't as useful in narrowing
-- down the thief as I was expecting it to be. This query lead me to a dead end on the
-- case for the time being, since it is impossible to deduce the account_number of the
-- thief based on the current information at hand.

-- I will instead proceed with the investigation with the account of the first witness's
-- interview transcript, Ruth, and search through the bakery_security_logs for the
-- license_plate of a car leaving the premises on July 28th, 2023 between 10:15am and
-- 10:25am, inclusive.
SELECT *
FROM bakery_security_logs
WHERE year = 2023 AND month = 7 AND day = 28
AND hour = 10 AND minute BETWEEN 15 AND 25;

-- I am now beginning to realize this case is a bit more difficult than I had anticipated,
-- since this previous query, too, has lead to a dead end for the time being since there
-- are many cars that left the bakery within 10 minutes of the time of the crime. So this
-- information alone doesn't help to narrow down to one specific license_plate for the
-- thief's car.

-- The current information gotten through the first two witnesses' transcripts have lead
-- me to dead ends with the information at hand, so I will now proceed with the
-- investigation with the account of the third witness, Raymond.

-- For starters, I will check the records in the phone_calls table to see... Looking at
-- the way the data is organized in that table, I can now see that this query will also
-- result in a dead end since there are no logs for the time at which the phone calls
-- occurred but only the length of each phone call, meaning that a fairly long list will
-- likely be returned. That said, I still need to know this information for later, so
-- I will check phone_calls for any calls that happened on July 28th, 2023 that lasted
-- less than 60 seconds.
SELECT *
FROM phone_calls
WHERE year = 2023 AND month = 7 AND day = 28
AND duration < 60;

-- As epected, the previous query didn't yield information that will help me right now
-- but it will be useful later on.
-- So next, I need to find all the flights that left Fiftyville on July 29th, 2023.
-- In order to find this, I need to get the id of the Fiftyville airport and use the
-- result of that query in a query on the flights table to find all flights leaving
-- Fiftyville on July 29th, 2023 and get the earliest flight from the resulting list.
SELECT *
  FROM flights
 WHERE origin_airport_id = (
       SELECT id
         FROM airports
        WHERE year = 2023 AND month = 7 AND day = 29
 );

--  I left the assignment for a couple of hours while I went to eat something, and
-- upon returning again, I finished the query above and when I ran it, nothing showed
-- up. I realized soon after that my predicate was wrong since that information is not
-- in the airports table, and it needed to be outside of the subquery. So I will fix
-- the query and get the correct result now:
SELECT *
  FROM flights
 WHERE origin_airport_id = (
       SELECT id
         FROM airports
        WHERE city = 'Fiftyville'
 )
   AND year = 2023 AND month = 7 AND day = 29;

-- The table returned as a result of the query is correct now, but for the purpose of
-- making the information I need easier to find, I need to ORDER it by the date, so I
-- will revise the query once again.
SELECT *
FROM flights
WHERE origin_airport_id = (
    SELECT id
    FROM airports
    WHERE city = 'Fiftyville'
)
AND year = 2023 AND month = 7 AND day = 29
ORDER BY hour, minute;

-- The table resulting from the previous query shows that the earliest flight out of
-- Fiftyville on July 29th, 2023 is the flight of id 36, from origin_airport_id 8,
-- which is Fiftyville, to destination_airport_id 4 (which I've yet to find out what
-- city it is) at 8:20am.
-- Since I don't know what airport and city has an id of 4, I will now query that
-- by getting the name of the city from the airports table where the airport's id is 4.
SELECT *
FROM airports
WHERE id = 4;

-- With the previous query, I was able to find out that the thief escaped to New York
-- City the day after committing the crime.
-- As per my plan laid out previously, I can now use the id of the specific flight
-- the thief took to escape, which has an id of 36, to see the passengers that took
-- that flight, and from that list of passengers, use their passport_number in
-- conjunction with the list of license_plate numbers seen leaving the bakery soon
-- after the time of the crime, to find out what person from the people table has
-- a passport_number and license_plate that matches a number from the passengers and
-- bakery_security_logs tables.
-- So, all in all, I need to get from the people table, the person's row of data
-- whose passport_number was in the flight to New York and whose license_plate was
-- seen leaving the bakery soon after the scene of the crime. In reality, I need only
-- to combine my previous queries into one, which should give me one person, who should
-- be the thief who stole the CS50 Duck.
SELECT *
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
)
AND license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28
    AND hour = 10 AND minute BETWEEN 15 AND 25
);

-- Running the previous query resulted in a table containing 4 possible suspects to
-- be the culprit of the crime of stealing the CS50 Duck. I was thinking that this
-- would result in only one person being returned, but since that isn't the case,
-- it means that I need to further narrow down this list, which could be done
-- in two different ways (or perhaps both ways will be needed in the end): first,
-- by comparing the phone_number of each suspect to those from the phone_calls table
-- who made a call on July 28th, 2023 that lasted less than a minute; second, by
-- comparing the id of each person from the people table to those whose account_number
-- appears in the list of people who made a transaction from the ATM at Leggett Street.
-- For the second method, a conjunction of the people, atm_transactions, and bank_accounts
-- tables will need to be made in order to connect each person's account_number to their id.

-- With that said, I'm curious as to what going with the first method of using the phone number
-- will yield, and whether this alone (though I doubt this will be the case) might be enough
-- to find the thief. So I will compare the phone numbers of the list of suspect to those who
-- made a call on the day of the crime that lasted less than a minute. In order to do so,
-- I will make use of the previous query and add to it so that I can make use of the list of
-- suspects to further narrow things down.
SELECT *
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
)
AND license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28
    AND hour = 10 AND minute BETWEEN 15 AND 25
)
AND phone_number = (
    SELECT caller
    FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28
    AND duration < 60
);

-- The previous query did in fact return one result, meaning that the record of that person
-- must be the one of the thief. However, as I cannot yet be sure that I made the correct
-- choices and assumptions, I need to corroborate this information by using the second method
-- previously laid out of using the bank account_number to be able to be entirely certain
-- that it will yield the same result.
-- As it stands, it appears that the thief is:
-- Sofia, with an id of 398010, phone number of (130) 555-0289, passport number of 1695452385,
-- and license plate number of G412CB7.

-- In order to be certain that this is in fact the thief, I need to get the person who's id
-- matches an account_number of the list of people who made a transaction on the day of the
-- crime.
SELECT *
FROM people
WHERE id IN (
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2023 AND month = 7 AND day = 28
              AND atm_location = 'Leggett Street'
    )
);

-- I made a mistake and forgot to use the list of the four suspects from earlier, so I
-- will modify the query to fix that mistake and use the list of four suspects to compare
-- their id with those of the people who made a transaction on the day of the crime.
SELECT *
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
)
AND license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28
    AND hour = 10 AND minute BETWEEN 15 AND 25
)
AND id IN (
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2023 AND month = 7 AND day = 28
              AND atm_location = 'Leggett Street'
    )
);

-- This doesn't make any sense. The previous query yielded a table of two suspects,
-- neither of which matches the suspect that I previously thought was the thief. I
-- will need to retrace my steps a bit and figure out why the two pieces of information
-- aren't matching one another.

-- Coming back to this assignment after a couple of hours, I realized just now that in
-- the previous query, where I was trying to narrow down the four suspects through the
-- phone numbers, I used an = sign in my WHERE argument, instead of using IN. This made
-- it so that only the first phone number returned from the table would match the
-- condition, and so that's why only one suspect was returned earlier, instead of a
-- list of possible suspects as it should have been. Which means that the person I
-- previously thought was the culprit was not so after all.

-- I'll be running a fixed version of that query next to see the correct results
-- for any matching phone numbers of people who were in the first flight out of
-- Fiftyville and who were seen leaving the bakery soon after the crime happened.
SELECT *
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
)
AND license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28
    AND hour = 10 AND minute BETWEEN 15 AND 25
)
AND phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28
    AND duration < 60
);

-- The previous fixed query now returned a list of three suspects, and the only suspect
-- that is present in both lists is Bruce, which means that he must be the thief.
-- To make my query result easier to understand, I will combine all of the information
-- that I know about the suspect into one query which should result in returning the culprit,
-- as opposed to having two separate queries resulting in two lists of possible suspects.
SELECT *
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
)
AND license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28
    AND hour = 10 AND minute BETWEEN 15 AND 25
)
AND phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28
    AND duration < 60
)
AND id IN (
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2023 AND month = 7 AND day = 28
              AND atm_location = 'Leggett Street'
    )
);

-- The previous query works and does in fact return only one suspect which must be
-- the thief who stole the CS50 Duck. Only one thing remains to be discovered, which
-- is the thief's accomplice. The only way to find out who it is is to check the
-- phone_calls on the day of the crime that lasted less than a minute and specifically
-- check for the phone call that occurred where the caller matches Bruce's phone_number.
-- This information should allow me to match the phone_number of the receiver of the
-- call logged in phone_calls to its owner from the people table.

-- In order to create the query to find the thief's accomplice, I will make use of the
-- query that I used to find the thief, to get their phone_number and use it to check
-- the phone_calls table and find the owner of the phone_number that received a call from
-- the thief, Bruce, on the day of the crime.
SELECT *
FROM people
WHERE phone_number IN (
    SELECT receiver
    FROM phone_calls
    WHERE caller = (
        SELECT phone_number
        FROM people
        WHERE passport_number IN (
            SELECT passport_number
            FROM passengers
            WHERE flight_id = 36
        )
        AND license_plate IN (
            SELECT license_plate
            FROM bakery_security_logs
            WHERE year = 2023 AND month = 7 AND day = 28
            AND hour = 10 AND minute BETWEEN 15 AND 25
        )
        AND phone_number IN (
            SELECT caller
            FROM phone_calls
            WHERE year = 2023 AND month = 7 AND day = 28
            AND duration < 60
        )
        AND id IN (
            SELECT person_id
            FROM bank_accounts
            WHERE account_number IN (
                SELECT account_number
                FROM atm_transactions
                WHERE year = 2023 AND month = 7 AND day = 28
                    AND atm_location = 'Leggett Street'
            )
        )
    )
);

-- Running the above query resulted in a long list of people being returned. Upon further
-- inspection, I realized that I forgot to specify that the phone call had to have happened
-- on the day of the crime and lasted less than a minute. Making those corrections results
-- in the following query:
SELECT *
FROM people
WHERE phone_number IN (
    SELECT receiver
    FROM phone_calls
    WHERE caller = (
        SELECT phone_number
        FROM people
        WHERE passport_number IN (
            SELECT passport_number
            FROM passengers
            WHERE flight_id = 36
        )
        AND license_plate IN (
            SELECT license_plate
            FROM bakery_security_logs
            WHERE year = 2023 AND month = 7 AND day = 28
            AND hour = 10 AND minute BETWEEN 15 AND 25
        )
        AND phone_number IN (
            SELECT caller
            FROM phone_calls
            WHERE year = 2023 AND month = 7 AND day = 28
            AND duration < 60
        )
        AND id IN (
            SELECT person_id
            FROM bank_accounts
            WHERE account_number IN (
                SELECT account_number
                FROM atm_transactions
                WHERE year = 2023 AND month = 7 AND day = 28
                    AND atm_location = 'Leggett Street'
            )
        )
    )
    AND year = 2023 AND month = 7 AND day = 28 AND duration < 60
);

-- The fixed query worked correctly now and helped to identify the thief's accomplice
-- as having been someone named Robin.

-- In conclusion, through my investigation of the crime that happened on July 28th, 2023
-- at 10:15am in the bakery at Humphrey Street, it is evident that the theft of the
-- CS50 Duck was committed by Bruce, who escaped from Fiftyville to New York City the
-- following morning, on July 29th, 2023, and whose accomplice was Robin, as they
-- helped Bruce to secure the tickets for the flight out of Fiftyville.
