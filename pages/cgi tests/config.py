
config = {
    "SESSION_DIR": "/Users/ysalmi/code/in_progress/webserv/pages/cgi tests/sessions/"
}

def get(key) -> str:
    return config[key]