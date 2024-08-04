import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # Get the user's inputs from the form and validate them
        name = request.form.get("name")
        if not name:
            return redirect("/")

        month = request.form.get("month")
        if not month:
            return redirect("/")
        try:
            month = int(month)
        except ValueError:
            return redirect("/")
        if month < 1 or month > 12:
            return redirect("/")

        day = request.form.get("day")
        if not day:
            return redirect("/")
        try:
            day = int(day)
        except ValueError:
            return redirect("/")
        if day < 1 or day > 31:
            return redirect("/")

        # Add the user's entry into the database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)

        return redirect("/")

    else:

        # Get the birthdays from the database
        birthdays = db.execute("SELECT * FROM birthdays")

        # Display the entries in the database on index.html
        return render_template("index.html", birthdays=birthdays)


# Deletes a birthday from the database
@app.route("/delete", methods=["POST"])
def delete():

    # Get id of birthday to delete from database
    id = request.form.get("id")

    # Delete birthday from database
    db.execute("DELETE FROM birthdays WHERE id = ?", id)

    # Return to index page to show the table
    return redirect("/")


# Edits a birthday in the database
@app.route("/edit", methods=["POST"])
def edit():

    # Get id of birthday to edit in database
    id = request.form.get("id")

    # Get the birthday data to edit from the database
    birthday = db.execute("SELECT * FROM birthdays WHERE id = ?", id)

    # Get the user's inputs from the form and validate them
    name = request.form.get("name")
    if not name:
        return render_template("edit.html", birthday=birthday[0])

    month = request.form.get("month")
    if not month:
        return render_template("edit.html", birthday=birthday[0])
    try:
        month = int(month)
    except ValueError:
        return render_template("edit.html", birthday=birthday[0])
    if month < 1 or month > 12:
        return render_template("edit.html", birthday=birthday[0])

    day = request.form.get("day")
    if not day:
        return render_template("edit.html", birthday=birthday[0])
    try:
        day = int(day)
    except ValueError:
        return render_template("edit.html", birthday=birthday[0])
    if day < 1 or day > 31:
        return render_template("edit.html", birthday=birthday[0])

    # Edit birthday in database with updated data
    db.execute("UPDATE birthdays SET (name, month, day) = (?, ?, ?) WHERE id = ?", name, month, day, id)

    # Return to index page to show the updated table
    return redirect("/")

