import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Get user's form submissions
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure user submitted inputs
        if not symbol:
            return apology("must submit stock's symbol")
        elif not shares:
            return apology("must submit amount of shares")

        # Get stock's current price
        quote = lookup(symbol)

        # Ensure symbol is valid
        if not quote:
            return apology("there is no stock associated with that symbol")

        # Convert shares to integer
        try:
            shares = int(shares)
        except ValueError:
            return apology("must submit an integer for amount of shares")

        # Ensure shares is a positive integer
        if shares < 1:
            return apology("must submit a positive integer for amount of shares")

        # Get user's current cash amount by querying the users table in the database
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session.get("user_id"))

        # Ensure that a list was correctly returned from querying the database
        if not cash:
            return apology("there was an error retrieving cash")

        # Retrieve cash value from list of dictionaries and convert to float for calculations
        cash = float(cash[0].get("cash"))

        # Ensure user has enough cash to buy shares
        if cash < (quote.get("price") * shares):
            return apology("insufficient cash")

        # Add transaction to database
        db.execute("INSERT INTO purchases " +
                   "(user_id, stock_symbol, shares_amount, stock_price, transaction_date) " +
                   "VALUES(?, ?, ?, ?, datetime('now'))",
                   session.get("user_id"), symbol, shares, quote.get("price"))

        # Subtract total shares price from user's account
        cash -= (quote.get("price") * shares)

        # Update users table to reflect user's new balance after buying their shares
        db.execute("UPDATE users " +
                   "SET cash = ? " +
                   "WHERE id = ?",
                   cash, session.get("user_id"))

        # Once having bought their shares, redirect user to homepage ("/")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Get the stock's symbol submitted by the user
        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must submit stock's symbol")

        # Get dictionary containing the stock's symbol and price
        quote = lookup(symbol)

        # Ensure stock exists
        if not quote:
            return apology("there is no stock associated with that symbol")

        # Display stock's symbol and its price
        return render_template("quoted.html", quote=quote)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Display form for user to get a quote for a stock
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password confirmation was submitted
        if not request.form.get("confirmation"):
            return apology("must provide password confirmation", 403)

        # Ensure that password and its confirmation match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password and confirmation do not match", 403)

        # Try to insert new user into users table, display apology if username is already in database
        try:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)",
                       request.form.get("username"),
                       generate_password_hash(request.form.get("password")))
        except ValueError:
            return apology("username is already taken, try a new one", 403)

        # Redirect to login page
        return redirect("/login")


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")
