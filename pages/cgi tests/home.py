#!/usr/local/bin/python3

from chameleon import PageTemplateLoader
from cgi.session import Session
from cgi.response import Response
from cgi.session import new_session
from cgi.cookie import parse_cookie
from cgi.forms import parse_urlencoded_form_data
import config
import os

path = os.path.dirname(__file__)
templates = PageTemplateLoader(os.path.join(path, "templates"))
template = templates['home.pt']

response = Response()
data = {
    "links": [
        "home.php",
        "server_redir.py",
        "client_redir.py",
        "client_redir_body.py",
        "instant_exit.py",
        "infinite_loop.py",
        "not_executable.py"
    ]
}

def home_get():
    cookies = parse_cookie(os.getenv("HTTP_COOKIE") or "")
    if "session_id" in cookies.keys():
        session = Session(cookies["session_id"], config.get("SESSION_DIR"))
        data["name"] = session.get("name")
        data["logged"] = True

        response.setHeader("Status", "200 Ok")
        response.setHeader("Content-Type", "text/html")
        response.setBody(template(data=data))
        response.send()
    else:
        data["logged"] = False
        response.setHeader("Status", "200 Ok")
        response.setHeader("Content-Type", "text/html")
        response.setBody(template(data=data))
        response.send()

def home_post():
    cookies = parse_cookie(os.getenv("HTTP_COOKIE") or "")
    form_data = parse_urlencoded_form_data()

    data["logged"] = True
    if "session_id" in cookies.keys():
        user_session = Session(cookies["session_id"], config.get("SESSION_DIR"))
        user_session.set("name", form_data["name"][0])
        data["name"] = user_session.get("name")
    else:
        user_session_id = new_session(config.get("SESSION_DIR"))
        user_session = Session(user_session_id, config.get("SESSION_DIR"))
        user_session.set("name", form_data["name"][0])
        session_cookie = user_session.get_session_cookie()
        data["name"] = user_session.get("name")
        response.setHeader("Set-Cookie", session_cookie)

    response.setHeader("Status", "200 Ok")
    response.setHeader("Content-Type", "text/html")
    response.setBody(template(data=data))
    user_session.save()
    response.send()
        


if (os.getenv("REQUEST_METHOD") == "GET"):
    home_get()
elif (os.getenv("REQUEST_METHOD") == "POST"):
    home_post()
else:
    template = templates["error.pt"]
    response.setHeader("Status", "405 Not Allowed")
    response.setHeader("Content-Type", "text/html")
    response.setBody(template(message = "This method is not allowed here"))
    response.send()
    