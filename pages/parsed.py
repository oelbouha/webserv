#!//usr/local/bin/python3
import os
import sys
import fileinput
import urllib.parse

input = ""
for line in fileinput.input():
    input += line
body_temp = '''<!DOCTYPE html>
    <html>
        <head>
            <title>cgi test - parset.py </title>
            <style>
                * { margin: 0; padding 0; }
                body {
                    background: %s; 
                    padding: 10px; 
                    display: flex;
                    flex-direction: column;
                    justify-content: center;
                    align-items: center;
                    color: white;
                    width: 1080px;
                    max-width: 100%% ;
                    margin: 0 auto;
                }
                ul {
                    max-width: 100%% ;
                    list-style-type: none;
                }
                img {
                    max-width: 100%% ;
                }
            </style>
        </head>
        <body>
            <h1>This is a title</h1>
            <ul>
                <li>this is working</li>
                <li>i guess</li>
                %s
            </ul>
                <img src="/file.jpeg">
        </body>
    </html>'''

method = os.getenv("REQUEST_METHOD")
cookie = os.getenv("HTTP_COOKIE")

if (method == "GET"):
    bg_color = "#282828"
    if len(cookie) > 0:
        res = urllib.parse.parse_qs(cookie)
        bg_color = res["bg_color"][0]
    body = body_temp % (bg_color, "")

    print("Status: 200 Ok\r")
    print("content-type: text/html\r")
    print("content-length: ", end="")
    print(len(body), end="\r\n")

    print("\r")
    print(body)

elif (method == "POST"):
    decoded = urllib.parse.unquote(input)
    res = urllib.parse.parse_qs(decoded)
    print(res["bg_color"][0], file=sys.stderr)
    body = body_temp % (res["bg_color"][0], decoded)

    setcookie = "Set-Cookie: bg_color=%s\r" % res["bg_color"][0]

    print("Status: 201 Created\r")
    print("content-type: text/html\r")
    print("content-length: ", end="")
    print(len(body), end="\r\n")
    print(setcookie)

    print("\r")
    print(body)
