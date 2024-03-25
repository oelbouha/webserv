#!//usr/local/bin/python3

from cgi.session import Session
from cgi.session import new_session
from cgi.cookie import parse_cookie
import config
import os
import fileinput
import urllib.parse
import sys


def home_get():
    cookies = parse_cookie(os.getenv("HTTP_COOKIE") or "")
    print(cookies, file=sys.stderr)
    print(cookies.keys(), file=sys.stderr)
    if "session_id" in cookies.keys():
        print("it is there", file=sys.stderr)
        session = Session(cookies["session_id"], config.get("SESSION_DIR"))
        name = session.get("name")

        print("Status: 200 Ok\r")
        print("Content-Type: text/html\r")
        print("\r")

        print('''
<!DOCTYPE html>
<html>
    <head>
        <title>cgi test - parset.py </title>
        <link rel="stylesheet" href="public/style.css" />
    </head>
    <body>
        <h1>Hello %s</h1>
    </body>
</html>
        ''' % name)
    else:
        print("Status: 200 Ok\r")
        print("Content-Type: text/html\r")
        print("\r")

        print('''
<!DOCTYPE html>
<html>
    <head>
        <title>cgi test - parset.py </title>
        <link rel="stylesheet" href="public/style.css" />
    </head>
    <body>
        <h1>Set Your Name</h1>
        <form method="POST" action="home.py">
            <input type="text" name="name" placeholder="Type Your Name...">
            <button>submit</button>
        </form>
    </body>
</html>
        ''')

def home_post():
    cookies = parse_cookie(os.getenv("HTTP_COOKIE") or "")
    input = ""
    for line in fileinput.input():
        input += line
    form_data = urllib.parse.parse_qs(urllib.parse.unquote(input))

    print("Status: 200 Created\r")
    print("content-type: text/html\r")
    user_session=""
    if "session_id" in cookies.keys():
        user_session = Session(cookies["session_id"], config.get("SESSION_DIR"))
    else:
        user_session_id = new_session(config.get("SESSION_DIR"))
        user_session = Session(user_session_id, config.get("SESSION_DIR"))
        set_session = user_session.get_session_header()
        print(set_session)

    user_session.set("name", form_data["name"][0])
    print("\r")
    print('''
<!DOCTYPE html>
<html>
    <head>
        <title>cgi test - parset.py </title>
        <link rel="stylesheet" href="public/style.css" />
    </head>
    <body>
        <h1>Hello %s</h1>
    </body>
</html>
        ''' % user_session.get("name"))
    user_session.save()
        


if (os.getenv("REQUEST_METHOD") == "GET"):
    home_get()
elif (os.getenv("REQUEST_METHOD") == "POST"):
    home_post()
    