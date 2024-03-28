import fileinput
import urllib.parse

def parse_urlencoded_form_data():
    input = ""
    for line in fileinput.input():
        input += line
    form_data = urllib.parse.parse_qs(urllib.parse.unquote(input))
    return form_data