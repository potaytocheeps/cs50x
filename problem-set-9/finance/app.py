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

    # Get stocks and number of shares per stock owned
    portfolio = db.execute("SELECT stock_symbol, shares_amount as shares " +
                           "FROM user_portfolios " +
                           "WHERE user_id = ? ",
                           session.get("user_id"))

    # Declare variable to hold user's total balance
    total_balance = 0

    # Iterate over each stock in portfolio
    for stock in portfolio:
        # Get the current price of each stock owned
        stock["current_price"] = lookup(stock.get("stock_symbol")).get("price")

        # Calculate total value of each holding
        stock["total_value"] = stock.get("shares") * stock.get("current_price")

        # Add total value of each holding to user's total balance
        total_balance += stock.get("total_value")

    # Get user's current cash amount by querying the users table in the database
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session.get("user_id"))

    # Ensure that a list was correctly returned from querying the database
    if not cash:
        return apology("there was an error retrieving cash")

    # Retrieve cash value from list of dictionaries
    cash = cash[0].get("cash")

    # Add user's current amount of cash to their total balance
    total_balance += cash

    return render_template("index.html", portfolio=portfolio, cash=cash, total=total_balance)


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
        db.execute("INSERT INTO transactions " +
                   "(user_id, stock_symbol, shares_amount, stock_price, transaction_type, transaction_date) " +
                   "VALUES(?, ?, ?, ?, ?, datetime('now'))",
                   session.get("user_id"), symbol.upper(), shares, quote.get("price"), "buy")

        # Update user_portfolios table in database to reflect this transaction
        db.execute("INSERT INTO user_portfolios (user_id, stock_symbol, shares_amount) " +
                   "VALUES(?, ?, ?) " +
                   "ON CONFLICT(user_id, stock_symbol) DO UPDATE SET " +
                   "shares_amount = shares_amount + ? ",
                   session.get("user_id"), symbol.upper(), shares, shares)

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

    # Get list of transactions from the transactions table
    transactions = db.execute("SELECT stock_symbol, shares_amount, stock_price, transaction_type, transaction_date " +
                              "FROM transactions " +
                              "WHERE user_id = ?",
                              session.get("user_id"))

    # Iterate over each transaction
    for transaction in transactions:
        # Calculate total value for each transaction
        transaction["total_value"] = usd(transaction.get("stock_price") * transaction.get("shares_amount"))

        # Format each transaction to display whether the user bought or sold shares for that transaction
        if transaction.get("transaction_type") == "buy":
            transaction["total_value"] = "-" + transaction["total_value"]
            transaction["shares_amount"] = "+" + str(transaction.get("shares_amount"))
        else:
            transaction["total_value"] = "+" + transaction["total_value"]
            transaction["shares_amount"] = "-" + str(transaction.get("shares_amount"))

    # Display table showing history of user's transactions
    return render_template("history.html", transactions=transactions)


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


@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    """Change password"""

    # User reached this route via POST
    if request.method == "POST":
        # Ensure user submitted inputs
        if not request.form.get("old_password"):
            return apology("must submit old password")
        elif not request.form.get("new_password"):
            return apology("must submit new password")

        # Get user's data from the users table
        rows = db.execute("SELECT hash " +
                          "FROM users " +
                          "WHERE id = ?",
                          session.get("user_id"))

        # Confirm that the old password matches the password in the database
        if not check_password_hash(rows[0].get("hash"), request.form.get("old_password")):
            return apology("old password is invalid")

        # Ensure old password and new password are different
        if check_password_hash(rows[0].get("hash"), request.form.get("new_password")):
            return apology("new password must be different from old password")

        # Update users table to change the old password to the new password
        db.execute("UPDATE users " +
                   "SET hash = ? " +
                   "WHERE id = ?",
                   generate_password_hash(request.form.get("new_password")), session.get("user_id"))

        # Return user to homepage
        return redirect("/")

    # User reached this route via GET
    else:
        return render_template("password.html")


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
            return apology("must provide username")

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password")

        # Ensure password confirmation was submitted
        if not request.form.get("confirmation"):
            return apology("must provide password confirmation")

        # Ensure that password and its confirmation match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password and confirmation do not match")

        # Try to insert new user into users table, display apology if username is already in database
        try:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)",
                       request.form.get("username"),
                       generate_password_hash(request.form.get("password")))
        except ValueError:
            return apology("username is already taken, try a new one")

        # Redirect to login page
        return redirect("/login")


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached this route via POST
    if request.method == "POST":
        # Get user's form submissions
        symbol = request.form.get("symbol")
        shares_to_sell = request.form.get("shares")

        # Get list of stocks owned by user
        stocks_owned = db.execute("SELECT stock_symbol " +
                                  "FROM user_portfolios " +
                                  "WHERE user_id = ?",
                                  session.get("user_id"))

        # Create a list of the different values in the list of dictionaries
        stocks_owned = [stock['stock_symbol'] for stock in stocks_owned]

        # Ensure user submitted inputs, and that the stock submitted is a stock that the user owns
        if not symbol:
            return apology("must submit stock's symbol")
        elif not symbol in stocks_owned:
            return apology("you do not own any shares of that stock")
        elif not shares_to_sell:
            return apology("must submit amount of shares")

        # Get stock's current price
        quote = lookup(symbol)

        # Ensure symbol is valid
        if not quote:
            return apology("there is no stock associated with that symbol")

        # Convert shares_to_sell to integer
        try:
            shares_to_sell = int(shares_to_sell)
        except ValueError:
            return apology("must submit an integer for amount of shares")

        # Ensure shares_to_sell is a positive integer
        if shares_to_sell < 1:
            return apology("must submit a positive integer for amount of shares")

        # Get amount of shares owned for stock submitted
        shares_owned = db.execute("SELECT shares_amount " +
                                  "FROM user_portfolios " +
                                  "WHERE stock_symbol = ? AND user_id = ?",
                                  symbol, session.get("user_id"))

        # Ensure that a list of shares_owned was correctly returned from querying the database
        if not shares_owned:
            return apology("there was an error retrieving amount of shares owned")

        # Extract integer from result of querying the database
        shares_owned = shares_owned[0].get("shares_amount")

        # Ensure user has enough shares to sell
        if shares_owned < shares_to_sell:
            return apology("insufficient shares owned")

        # Get user's current cash amount by querying the users table in the database
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session.get("user_id"))

        # Ensure that a list was correctly returned from querying the database
        if not cash:
            return apology("there was an error retrieving cash")

        # Retrieve cash value from list of dictionaries and convert to float for calculations
        cash = float(cash[0].get("cash"))

        # Add transaction to database
        db.execute("INSERT INTO transactions " +
                   "(user_id, stock_symbol, shares_amount, stock_price, transaction_type, transaction_date) " +
                   "VALUES(?, ?, ?, ?, ?, datetime('now'))",
                   session.get("user_id"), symbol.upper(), shares_to_sell, quote.get("price"), "sell")

        # Update user_portfolios table in database to reflect this transaction
        db.execute("UPDATE user_portfolios " +
                   "SET shares_amount = shares_amount - ? " +
                   "WHERE user_id = ? AND stock_symbol = ?",
                    shares_to_sell, session.get("user_id"), symbol.upper())

        # Remove sold stock from user's portfolio if they have 0 shares left
        if shares_owned - shares_to_sell == 0:
            db.execute("DELETE FROM user_portfolios " +
                       "WHERE user_id = ? AND stock_symbol = ? AND shares_amount = 0",
                       session.get("user_id"), symbol)

        # Add total price of shares sold to user's cash amount
        cash += (quote.get("price") * shares_to_sell)

        # Update users table to reflect user's new balance after selling their shares
        db.execute("UPDATE users " +
                   "SET cash = ? " +
                   "WHERE id = ?",
                   cash, session.get("user_id"))

        # Once having sold their shares, redirect user to homepage ("/")
        return redirect("/")

    # User reached this route via GET
    else:
        # Get list of stocks owned by user
        stocks_owned = db.execute("SELECT stock_symbol " +
                                  "FROM user_portfolios " +
                                  "WHERE user_id = ?",
                                  session.get("user_id"))

        # Create a list of the different values in the list of dictionaries
        stocks_owned = [stock['stock_symbol'] for stock in stocks_owned]

        # Pass list of stocks owned by user to template
        return render_template("sell.html", stocks=stocks_owned)
